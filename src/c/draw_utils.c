#include <pebble.h>
#include "draw_utils.h"
#include "global.h"

static GPath *s_gmt_hand = 0;
static GPath *s_gmt_inlay = 0;
static GPath *s_gs_hour_hand = 0;
static GPath *s_gs_hour_hand_highlight = 0;
static GPath *s_gs_minute_hand = 0;
static GPath *s_gs_minute_hand_highlight = 0;
static GPath *s_sbge001_hour_hand = 0;
static GPath *s_sbge001_hour_hand_highlight = 0;
static GPath *s_sbge001_minute_hand = 0;
static GPath *s_sbge001_minute_hand_highlight = 0;

static void draw_seconds( DRAW_CLOCK_PARAMS *pCP );

static void draw_clock_hand( HAND_DRAW_PARAMS *pDP ) {
  // dot outline
  graphics_context_set_stroke_color( pDP->ctx, pDP->dot_outline_color );
  graphics_context_set_stroke_width( pDP->ctx, 1 );
  graphics_draw_circle( pDP->ctx, pDP->center_pt, pDP->dot_radius );
  // hand outline
  graphics_context_set_stroke_color( pDP->ctx, pDP->hand_outline_color );
  graphics_context_set_stroke_width( pDP->ctx, pDP->hand_width + 2);
  graphics_draw_line( pDP->ctx, pDP->from_pt, pDP->to_pt );
  // hand
  graphics_context_set_stroke_color( pDP->ctx, pDP->hand_color );
  graphics_context_set_stroke_width( pDP->ctx, pDP->hand_width );
  graphics_draw_line( pDP->ctx, pDP->from_pt, pDP->to_pt );
  // dot
  graphics_context_set_fill_color( pDP->ctx, pDP->dot_color );
  graphics_fill_circle( pDP->ctx, pDP->center_pt, pDP->dot_radius - 1 );
  /*
  // dot outline
  graphics_context_set_stroke_color( pDP->ctx, GColorDarkGray );
  graphics_context_set_stroke_width( pDP->ctx, 1 );
  graphics_draw_circle( pDP->ctx, pDP->center_pt, pDP->dot_radius - 2 );
  */
}

static void draw_gpath_hands( GPATH_HANDS_PARAMS *pGP ) {
  // for hour and minute hands
  graphics_context_set_stroke_width( pGP->ctx, 1 );

  // hour hand
  gpath_rotate_to( pGP->s_hour_hand, pGP->hour_angle );
  gpath_rotate_to( pGP->s_hour_hand_highlight, pGP->hour_angle );
  gpath_move_to( pGP->s_hour_hand, pGP->center_pt );
  gpath_move_to( pGP->s_hour_hand_highlight, pGP->center_pt );

  graphics_context_set_fill_color( pGP->ctx, pGP->hour_hand_colour );
  gpath_draw_filled( pGP->ctx, pGP->s_hour_hand );
  graphics_context_set_fill_color( pGP->ctx, pGP->hour_hand_highlight_colour );
  gpath_draw_filled( pGP->ctx, pGP->s_hour_hand_highlight );
  graphics_context_set_stroke_color( pGP->ctx, pGP->hand_outline_color );
  gpath_draw_outline( pGP->ctx, pGP->s_hour_hand );

  // min hand
  gpath_rotate_to( pGP->s_min_hand, pGP->min_angle );
  gpath_rotate_to( pGP->s_min_hand_highlight, pGP->min_angle );
  gpath_move_to( pGP->s_min_hand, pGP->center_pt );
  gpath_move_to( pGP->s_min_hand_highlight, pGP->center_pt );

  graphics_context_set_fill_color( pGP->ctx, pGP->min_hand_colour );
  gpath_draw_filled( pGP->ctx, pGP->s_min_hand );
  graphics_context_set_fill_color( pGP->ctx, pGP->min_hand_highlight_colour );
  gpath_draw_filled( pGP->ctx, pGP->s_min_hand_highlight );
  graphics_context_set_fill_color( pGP->ctx, pGP->hand_outline_color );
  gpath_draw_outline( pGP->ctx, pGP->s_min_hand );

  if ( ! ( pGP->show_seconds ) ) {
    graphics_context_set_fill_color( pGP->ctx, GColorBlack );
    graphics_fill_circle( pGP->ctx, pGP->center_pt, 2 );
  } 
}

void draw_spiffy_gs_clock_hands( DRAW_CLOCK_PARAMS *pCP ) {
  GPATH_HANDS_PARAMS gpath_params = (GPATH_HANDS_PARAMS) {
    .ctx = pCP->ctx,
    .center_pt = pCP->center_pt,
    .hour_angle = pCP->hour_angle,
    .min_angle = pCP->min_angle,
    .s_hour_hand = s_gs_hour_hand,
    .s_hour_hand_highlight = s_gs_hour_hand_highlight,
    .s_min_hand = s_gs_minute_hand,
    .s_min_hand_highlight = s_gs_minute_hand_highlight,
    .hour_hand_colour = GColorLightGray,
    .hour_hand_highlight_colour = GColorWhite,
    .min_hand_colour = GColorLightGray,
    .min_hand_highlight_colour = GColorWhite,
    .hand_outline_color = COLOUR_BG_BITMAP_BG,
    .show_seconds = pCP->show_seconds
  };
  draw_gpath_hands( &gpath_params );
  
  draw_seconds( pCP );
}

void draw_sbge001_clock_hands( DRAW_CLOCK_PARAMS *pCP ) {
  // gmt hands
  gpath_rotate_to( s_gmt_hand, pCP->gmt_angle );
  gpath_move_to( s_gmt_hand, pCP->center_pt );
  graphics_context_set_fill_color( pCP->ctx, PBL_IF_COLOR_ELSE( GColorDarkCandyAppleRed, GColorWhite ) );
  gpath_draw_filled( pCP->ctx, s_gmt_hand );
  graphics_context_set_stroke_color( pCP->ctx, PBL_IF_COLOR_ELSE( GColorRed, GColorWhite ) );
  gpath_draw_outline( pCP->ctx, s_gmt_hand );
  // gmt inlay
  gpath_rotate_to( s_gmt_inlay, pCP->gmt_angle );
  gpath_move_to( s_gmt_inlay, pCP->center_pt );
  graphics_context_set_fill_color( pCP->ctx, GColorWhite );
  gpath_draw_filled( pCP->ctx, s_gmt_inlay );
  // graphics_context_set_stroke_color( pCP->ctx, GColorWhite );
  // gpath_draw_outline( pCP->ctx, s_gmt_inlay );

  GPATH_HANDS_PARAMS gpath_params = (GPATH_HANDS_PARAMS) {
    .ctx = pCP->ctx,
    .center_pt = pCP->center_pt,
    .hour_angle = pCP->hour_angle,
    .min_angle = pCP->min_angle,
    .s_hour_hand = s_sbge001_hour_hand,
    .s_hour_hand_highlight = s_sbge001_hour_hand_highlight,
    .s_min_hand = s_sbge001_minute_hand,
    .s_min_hand_highlight = s_sbge001_minute_hand_highlight,
    .hour_hand_colour = GColorDarkGray,
    .hour_hand_highlight_colour = GColorWhite,
    .min_hand_colour = GColorDarkGray,
    .min_hand_highlight_colour = GColorWhite,
    .hand_outline_color = COLOUR_BG_BITMAP_BG,
    .show_seconds = pCP->show_seconds
  };
  draw_gpath_hands( &gpath_params );
  
  draw_seconds( pCP );
}

void draw_cont_clock_hands( DRAW_CLOCK_PARAMS *pCP ) {
  HAND_DRAW_PARAMS hand_params;
  
  GPoint hour_hand = (GPoint) {
    .x = ( sin_lookup( pCP->hour_angle ) * HOUR_HAND_LENGTH / TRIG_MAX_RATIO ) + pCP->center_pt.x,
    .y = ( -cos_lookup( pCP->hour_angle ) * HOUR_HAND_LENGTH / TRIG_MAX_RATIO ) + pCP->center_pt.y
  };

  // hour hand
  hand_params = (HAND_DRAW_PARAMS) {
    .ctx = pCP->ctx,
    .center_pt = pCP->center_pt,
    .from_pt = pCP->center_pt,
    .to_pt = hour_hand,
    .hand_width = HOUR_HAND_WIDTH,
    .hand_color = COLOUR_HOUR_HAND,
    .hand_outline_color = COLOUR_BG_BITMAP_BG,
    .dot_radius = HOUR_CENTER_DOT_RADIUS,
    .dot_color = COLOUR_HOUR_HAND, // COLOUR_DOT,
    .dot_outline_color = COLOUR_DOT_OUTLINE
  };
  draw_clock_hand( &hand_params );

  GPoint min_hand = (GPoint) {
    .x = ( sin_lookup( pCP->min_angle ) * MIN_HAND_LENGTH / TRIG_MAX_RATIO ) + pCP->center_pt.x,
    .y = ( -cos_lookup( pCP->min_angle ) * MIN_HAND_LENGTH / TRIG_MAX_RATIO ) + pCP->center_pt.y
  };

  // minute hand
  hand_params = (HAND_DRAW_PARAMS) {
    .ctx = pCP->ctx,
    .center_pt = pCP->center_pt,
    .from_pt = pCP->center_pt,
    .to_pt = min_hand,
    .hand_width = MIN_HAND_WIDTH,
    .hand_color = COLOUR_MIN_HAND,
    .hand_outline_color = COLOUR_BG_BITMAP_BG,
    .dot_radius = MIN_CENTER_DOT_RADIUS,
    .dot_color = COLOUR_MIN_HAND, // COLOUR_DOT,
    .dot_outline_color = COLOUR_DOT_OUTLINE
  };
  draw_clock_hand( &hand_params );
  
  draw_seconds( pCP );
}

static void draw_seconds( DRAW_CLOCK_PARAMS *pCP ) {
  if ( ! pCP->show_seconds ) return;
  
  GPoint sec_hand = (GPoint) {
    .x = ( sin_lookup( pCP->sec_angle ) * SEC_HAND_LENGTH / TRIG_MAX_RATIO ) + pCP->center_pt.x,
    .y = ( -cos_lookup( pCP->sec_angle ) * SEC_HAND_LENGTH / TRIG_MAX_RATIO ) + pCP->center_pt.y
  };    
  GPoint sec_hand_tail = (GPoint) {
    .x = ( sin_lookup( pCP->sec_tail_angle ) * SEC_HAND_TAIL_LENGTH / TRIG_MAX_RATIO ) + pCP->center_pt.x,
    .y = ( -cos_lookup( pCP->sec_tail_angle ) * SEC_HAND_TAIL_LENGTH / TRIG_MAX_RATIO ) + pCP->center_pt.y
  };  

  // second hand
  HAND_DRAW_PARAMS hand_params = (HAND_DRAW_PARAMS) {
    .ctx = pCP->ctx,
    .center_pt = pCP->center_pt,
    .from_pt = sec_hand,
    .to_pt = sec_hand_tail,
    .hand_width = SEC_HAND_WIDTH,
    .hand_color = COLOUR_SEC_HAND,
    .hand_outline_color = COLOUR_BG_BITMAP_BG,
    .dot_radius = SEC_CENTER_DOT_RADIUS,
    .dot_color = COLOUR_SEC_HAND, // COLOUR_DOT,
    .dot_outline_color = COLOUR_DOT_OUTLINE
  };
  draw_clock_hand( &hand_params );

  #if defined( PBL_COLOR ) // second hand tip
  GPoint sec_hand_tip = (GPoint) {
    .x = ( sin_lookup( pCP->sec_angle ) * ( SEC_HAND_LENGTH - SEC_HAND_TIP_LENGTH ) / TRIG_MAX_RATIO ) + pCP->center_pt.x,
    .y = ( -cos_lookup( pCP->sec_angle ) * ( SEC_HAND_LENGTH - SEC_HAND_TIP_LENGTH ) / TRIG_MAX_RATIO ) + pCP->center_pt.y
  };
  graphics_context_set_stroke_width( pCP->ctx, SEC_HAND_WIDTH );
  graphics_context_set_stroke_color( pCP->ctx, COLOUR_SEC_HAND_TIP );
  graphics_draw_line( pCP->ctx, sec_hand, sec_hand_tip );
  #endif
}

void gpaths_init( void ) {
  s_gmt_hand = gpath_create( &GMT_HAND );
  s_gmt_inlay = gpath_create( &GMT_HAND_INLAY );
  //
  s_gs_minute_hand = gpath_create( &MINUTE_HAND_SPIFFY_GS_POINTS );
  s_gs_minute_hand_highlight = gpath_create( &MINUTE_HAND_SPIFFY_GS_POINTS_HIGHLIGHT );
  s_gs_hour_hand = gpath_create( &HOUR_HAND_SPIFFY_GS_POINTS );
  s_gs_hour_hand_highlight = gpath_create( &HOUR_HAND_SPIFFY_GS_POINTS_HIGHLIGHT );
  //
  s_sbge001_minute_hand = gpath_create( &MINUTE_HAND_SBGE001_POINTS );
  s_sbge001_minute_hand_highlight = gpath_create( &MINUTE_HAND_SBGE001_POINTS_HIGHLIGHT );
  s_sbge001_hour_hand = gpath_create( &HOUR_HAND_SBGE001_POINTS );
  s_sbge001_hour_hand_highlight = gpath_create( &HOUR_HAND_SBGE001_POINTS_HIGHLIGHT );
}

void gpaths_deinit( void ) {
  gpath_destroy( s_sbge001_minute_hand );
  gpath_destroy( s_sbge001_minute_hand_highlight );
  gpath_destroy( s_sbge001_hour_hand );
  gpath_destroy( s_sbge001_hour_hand_highlight );
  gpath_destroy( s_gs_minute_hand );
  gpath_destroy( s_gs_minute_hand_highlight );
  gpath_destroy( s_gs_hour_hand );
  gpath_destroy( s_gs_hour_hand_highlight );
  gpath_destroy( s_gmt_inlay );
  gpath_destroy( s_gmt_hand );
}