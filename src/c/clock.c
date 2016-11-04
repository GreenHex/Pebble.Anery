#include <pebble.h>
#include "global.h"
#include "clock.h"
#include "draw_utils.h"
#include "date.h"
#include "battery.h"
#include "chime.h"

static Layer *window_layer = 0;
// analog clock
static BitmapLayer *analog_clock_bitmap_layer = 0;
static Layer *analog_clock_layer = 0;
static GBitmap *analog_clock_bitmap = 0;
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
// misc.
static AppTimer *secs_display_apptimer = 0; 
extern tm tm_time;
extern tm tm_gmt;

static void start_seconds_display( AccelAxisType axis, int32_t direction );

// function is "adjusted"" for whole hours or minutes; "after" 9:00 AM or "upto" 9:00 AM.
// "after" includes the hour, "upto" excludes the hour.
bool is_X_in_range( int a, int b, int x ) { return ( ( b > a ) ? ( ( x >= a ) && ( x < b ) ) : ( ( x >= a ) || ( x < b ) ) ); };

void draw_clock( void ) {
  time_t now = time( NULL );
  tm_time = *localtime( &now ); // copy to global
  tm_gmt = *gmtime( &now ); // copy to global
  if ( persist_read_int( MESSAGE_KEY_ANALOG_SECONDS_DISPLAY_TIMEOUT_SECS ) ) accel_tap_service_subscribe( start_seconds_display );
  layer_mark_dirty( analog_clock_layer );
}

static void handle_clock_tick( struct tm *tick_time, TimeUnits units_changed ) {
  tm_time = *tick_time; // copy to global
  time_t now = time( NULL );
  tm_gmt = *gmtime( &now ); // copy to global
  
  // if (DEBUG) APP_LOG( APP_LOG_LEVEL_INFO, "clock.c: handle_clock_tick(): %d:%d:%d", tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec );

  layer_mark_dirty( analog_clock_layer );
  
  if ( ( units_changed & MINUTE_UNIT ) == MINUTE_UNIT ) do_chime( &tm_time );
}

static void analog_clock_layer_update_proc( Layer *layer, GContext *ctx ) {
  // uses global tm_time

  static HAND_DRAW_PARAMS hand_params;
  static GPATH_HANDS_PARAMS gpath_params;
  GRect layer_bounds = layer_get_bounds( layer );
  GPoint center_pt = grect_center_point( &layer_bounds );
  uint32_t gmt_angle = ( TRIG_MAX_ANGLE * ( ( ( tm_gmt.tm_hour ) * 6 ) + ( tm_gmt.tm_min / 10 ) ) ) / ( 12 * 6 * 2 );
  uint32_t hour_angle = ( TRIG_MAX_ANGLE * ( ( ( tm_time.tm_hour % 12 ) * 6 ) + ( tm_time.tm_min / 10 ) ) ) / ( 12 * 6 );
  uint32_t min_angle = TRIG_MAX_ANGLE * tm_time.tm_min / 60;

  if ( persist_read_int( MESSAGE_KEY_ANALOG_HANDS_STYLE ) == STYLE_SPIFFY_GS ) {
    gpath_params = (GPATH_HANDS_PARAMS) {
      .ctx = ctx,
      .center_pt = center_pt,
      .hour_angle = hour_angle,
      .min_angle = min_angle,
      .s_hour_hand = s_gs_hour_hand,
      .s_hour_hand_highlight = s_gs_hour_hand_highlight,
      .s_min_hand = s_gs_minute_hand,
      .s_min_hand_highlight = s_gs_minute_hand_highlight,
      .hour_hand_colour = GColorLightGray,
      .hour_hand_highlight_colour = GColorWhite,
      .min_hand_colour = GColorLightGray,
      .min_hand_highlight_colour = GColorWhite,
      .hand_outline_color = COLOUR_BG_BITMAP_BG,
      .show_seconds = ( (ANALOG_LAYER_DATA *) layer_get_data( analog_clock_layer ) )->show_seconds
    };
    draw_gpath_hands( &gpath_params );
  } else if( persist_read_int( MESSAGE_KEY_ANALOG_HANDS_STYLE ) == STYLE_SBGE001 ) {
    
    // gmt hands
    gpath_rotate_to( s_gmt_hand, gmt_angle );
    gpath_move_to( s_gmt_hand, center_pt );
    graphics_context_set_fill_color( ctx, PBL_IF_COLOR_ELSE( GColorDarkCandyAppleRed, GColorWhite ) );
    gpath_draw_filled( ctx, s_gmt_hand );
    graphics_context_set_stroke_color( ctx, PBL_IF_COLOR_ELSE( GColorRed, GColorWhite ) );
    gpath_draw_outline( ctx, s_gmt_hand );
    // gmt inlay
    gpath_rotate_to( s_gmt_inlay, gmt_angle );
    gpath_move_to( s_gmt_inlay, center_pt );
    graphics_context_set_fill_color( ctx, GColorWhite );
    gpath_draw_filled( ctx, s_gmt_inlay );
    // graphics_context_set_stroke_color( ctx, GColorWhite );
    // gpath_draw_outline( ctx, s_gmt_inlay );
    
    gpath_params = (GPATH_HANDS_PARAMS) {
      .ctx = ctx,
      .center_pt = center_pt,
      .hour_angle = hour_angle,
      .min_angle = min_angle,
      .s_hour_hand = s_sbge001_hour_hand,
      .s_hour_hand_highlight = s_sbge001_hour_hand_highlight,
      .s_min_hand = s_sbge001_minute_hand,
      .s_min_hand_highlight = s_sbge001_minute_hand_highlight,
      .hour_hand_colour = GColorDarkGray,
      .hour_hand_highlight_colour = GColorLightGray,
      .min_hand_colour = GColorDarkGray,
      .min_hand_highlight_colour = GColorLightGray,
      .hand_outline_color = COLOUR_BG_BITMAP_BG,
      .show_seconds = ( (ANALOG_LAYER_DATA *) layer_get_data( analog_clock_layer ) )->show_seconds
    };
    draw_gpath_hands( &gpath_params );
  } else { // contemporary
    
    GPoint hour_hand = (GPoint) {
      .x = ( sin_lookup( hour_angle ) * HOUR_HAND_LENGTH / TRIG_MAX_RATIO ) + center_pt.x,
      .y = ( -cos_lookup( hour_angle ) * HOUR_HAND_LENGTH / TRIG_MAX_RATIO ) + center_pt.y
    };

    // hour hand
    hand_params = (HAND_DRAW_PARAMS) {
      .ctx = ctx,
      .center_pt = center_pt,
      .from_pt = center_pt,
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
      .x = ( sin_lookup( min_angle ) * MIN_HAND_LENGTH / TRIG_MAX_RATIO ) + center_pt.x,
      .y = ( -cos_lookup( min_angle ) * MIN_HAND_LENGTH / TRIG_MAX_RATIO ) + center_pt.y
    };

    // minute hand
    hand_params = (HAND_DRAW_PARAMS) {
      .ctx = ctx,
      .center_pt = center_pt,
      .from_pt = center_pt,
      .to_pt = min_hand,
      .hand_width = MIN_HAND_WIDTH,
      .hand_color = COLOUR_MIN_HAND,
      .hand_outline_color = COLOUR_BG_BITMAP_BG,
      .dot_radius = MIN_CENTER_DOT_RADIUS,
      .dot_color = COLOUR_MIN_HAND, // COLOUR_DOT,
      .dot_outline_color = COLOUR_DOT_OUTLINE
    };
    draw_clock_hand( &hand_params );
  }

  if ( ( (ANALOG_LAYER_DATA *) layer_get_data( analog_clock_layer ) )->show_seconds ) {
    int32_t sec_angle = TRIG_MAX_ANGLE * tm_time.tm_sec / 60;
    int32_t sec_tail_angle = sec_angle + ( TRIG_MAX_ANGLE / 2 );
    GPoint sec_hand = (GPoint) {
      .x = ( sin_lookup( sec_angle ) * SEC_HAND_LENGTH / TRIG_MAX_RATIO ) + center_pt.x,
      .y = ( -cos_lookup( sec_angle ) * SEC_HAND_LENGTH / TRIG_MAX_RATIO ) + center_pt.y
    };    
    GPoint sec_hand_tail = (GPoint) {
      .x = ( sin_lookup( sec_tail_angle ) * SEC_HAND_TAIL_LENGTH / TRIG_MAX_RATIO ) + center_pt.x,
      .y = ( -cos_lookup( sec_tail_angle ) * SEC_HAND_TAIL_LENGTH / TRIG_MAX_RATIO ) + center_pt.y
    };  

    // second hand
    hand_params = (HAND_DRAW_PARAMS) {
      .ctx = ctx,
      .center_pt = center_pt,
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
      .x = ( sin_lookup( sec_angle ) * ( SEC_HAND_LENGTH - SEC_HAND_TIP_LENGTH ) / TRIG_MAX_RATIO ) + center_pt.x,
      .y = ( -cos_lookup( sec_angle ) * ( SEC_HAND_LENGTH - SEC_HAND_TIP_LENGTH ) / TRIG_MAX_RATIO ) + center_pt.y
    };
    graphics_context_set_stroke_width( ctx, SEC_HAND_WIDTH );
    graphics_context_set_stroke_color( ctx, COLOUR_SEC_HAND_TIP );
    graphics_draw_line( ctx, sec_hand, sec_hand_tip );
    #endif
  }
}
  
static void stop_seconds_display( void* data ) { // after timer elapses
  if ( secs_display_apptimer ) app_timer_cancel( secs_display_apptimer ); // just for fun.
  secs_display_apptimer = 0; // docs don't say if this is set to zero when timer expires. 

  ( (ANALOG_LAYER_DATA *) layer_get_data( analog_clock_layer ) )->show_seconds = false;

  tick_timer_service_subscribe( MINUTE_UNIT, handle_clock_tick );
}

static void start_seconds_display( AccelAxisType axis, int32_t direction ) {
  #ifdef SECONDS_ALWAYS_ON
  return;
  #endif
  
  if ( ! persist_read_int( MESSAGE_KEY_ANALOG_SECONDS_DISPLAY_TIMEOUT_SECS ) ) return;

  tick_timer_service_subscribe( SECOND_UNIT, handle_clock_tick );

  ( (ANALOG_LAYER_DATA *) layer_get_data( analog_clock_layer ) )->show_seconds = true;
  //
  if ( secs_display_apptimer ) {
    app_timer_reschedule( secs_display_apptimer, (uint32_t) persist_read_int( MESSAGE_KEY_ANALOG_SECONDS_DISPLAY_TIMEOUT_SECS ) * 1000 );
  } else {
    secs_display_apptimer = app_timer_register( (uint32_t) persist_read_int( MESSAGE_KEY_ANALOG_SECONDS_DISPLAY_TIMEOUT_SECS ) * 1000,
                                               stop_seconds_display, 0 );
  }
}

void clock_init( Window *window ) {
  window_layer = window_get_root_layer( window );
  GRect window_bounds = layer_get_bounds( window_layer );
  GRect clock_layer_bounds = GRect( window_bounds.origin.x + CLOCK_POS_X, window_bounds.origin.y + CLOCK_POS_Y, 
                                   window_bounds.size.w - CLOCK_POS_X, window_bounds.size.h - CLOCK_POS_Y );
  // background bitmap
  analog_clock_bitmap = gbitmap_create_with_resource( RESOURCE_ID_ANALOG_EMERY_FULL_SLIM );
  analog_clock_bitmap_layer = bitmap_layer_create( clock_layer_bounds );
  bitmap_layer_set_bitmap( analog_clock_bitmap_layer, analog_clock_bitmap );
  layer_add_child( window_layer, bitmap_layer_get_layer( analog_clock_bitmap_layer ) );
  layer_set_hidden( bitmap_layer_get_layer( analog_clock_bitmap_layer ), false );
  // battery
  battery_init( bitmap_layer_get_layer( analog_clock_bitmap_layer ) );
  // date
  date_init( bitmap_layer_get_layer( analog_clock_bitmap_layer ) );
  // clock layer
  analog_clock_layer = layer_create_with_data( layer_get_bounds( bitmap_layer_get_layer( analog_clock_bitmap_layer ) ),
                                              sizeof( ANALOG_LAYER_DATA ) );  
  #ifdef SECONDS_ALWAYS_ON
  ( (ANALOG_LAYER_DATA *) layer_get_data( analog_clock_layer ) )->show_seconds = true;
  #else
  ( (ANALOG_LAYER_DATA *) layer_get_data( analog_clock_layer ) )->show_seconds = false;
  #endif
  layer_add_child( bitmap_layer_get_layer( analog_clock_bitmap_layer ), analog_clock_layer );
  layer_set_update_proc( analog_clock_layer, analog_clock_layer_update_proc ); 
  layer_set_hidden( analog_clock_layer, false );
  
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

  // subscriptions
  #ifdef SECONDS_ALWAYS_ON
  tick_timer_service_subscribe( SECOND_UNIT, handle_clock_tick );
  #else
  tick_timer_service_subscribe( MINUTE_UNIT, handle_clock_tick );
  #endif
  
  // show current time
  draw_clock();
}

void clock_deinit( void ) {
  if ( secs_display_apptimer ) app_timer_cancel( secs_display_apptimer );
  accel_tap_service_unsubscribe(); // are we over-unsubscribing?
  tick_timer_service_unsubscribe();
  layer_destroy( analog_clock_layer );
  date_deinit();
  battery_deinit();
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
  bitmap_layer_destroy( analog_clock_bitmap_layer );
  gbitmap_destroy( analog_clock_bitmap );
}
