#include <pebble.h>
#include "draw_utils.h"

void draw_battery_hand( BATTERY_HAND_DRAW_PARAMS *pDP ) {
  gpath_rotate_to( pDP->s_hand, DEG_TO_TRIGANGLE( pDP->batt_angle ) );
  gpath_move_to( pDP->s_hand, pDP->center_pt );
  
  graphics_context_set_fill_color( pDP->ctx, pDP->hand_colour );
  gpath_draw_filled( pDP->ctx, pDP->s_hand );
  graphics_context_set_stroke_color( pDP->ctx, pDP->hand_outline_colour );
  gpath_draw_outline( pDP->ctx, pDP->s_hand );
  
  graphics_context_set_fill_color( pDP->ctx, pDP->charge_state.is_charging ? GColorKellyGreen : 
                                  pDP->charge_state.charge_percent < 16 ? GColorDarkCandyAppleRed : GColorDarkGray );
  graphics_context_set_stroke_color( pDP->ctx, pDP->hand_outline_colour );
  graphics_context_set_stroke_width( pDP->ctx, 1 );
  graphics_fill_circle( pDP->ctx, pDP->center_pt, pDP->dot_radius - 1 );	
  graphics_draw_circle( pDP->ctx, pDP->center_pt, pDP->dot_radius );
}

void draw_clock_hand( HAND_DRAW_PARAMS *pDP ) {
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

void draw_gpath_hands( GPATH_HANDS_PARAMS *pGP ) {
  // for hour and minute hands
  graphics_context_set_stroke_width( pGP->ctx, 1 );

  // hour hand
  gpath_rotate_to( pGP->s_hour_hand, pGP->hour_angle );
  gpath_rotate_to( pGP->s_hour_hand_highlight, pGP->hour_angle );
  gpath_move_to( pGP->s_hour_hand, pGP->center_pt );
  gpath_move_to( pGP->s_hour_hand_highlight, pGP->center_pt );

  graphics_context_set_fill_color( pGP->ctx, GColorLightGray );
  gpath_draw_filled( pGP->ctx, pGP->s_hour_hand );
  graphics_context_set_fill_color( pGP->ctx, GColorWhite );
  gpath_draw_filled( pGP->ctx, pGP->s_hour_hand_highlight );
  graphics_context_set_fill_color( pGP->ctx, pGP->hour_hand_colour );
  graphics_context_set_stroke_color( pGP->ctx, pGP->hand_outline_color );
  gpath_draw_outline( pGP->ctx, pGP->s_hour_hand );

  // min hand
  gpath_rotate_to( pGP->s_min_hand, pGP->min_angle );
  gpath_rotate_to( pGP->s_min_hand_highlight, pGP->min_angle );
  gpath_move_to( pGP->s_min_hand, pGP->center_pt );
  gpath_move_to( pGP->s_min_hand_highlight, pGP->center_pt );

  graphics_context_set_fill_color( pGP->ctx, GColorLightGray );
  gpath_draw_filled( pGP->ctx, pGP->s_min_hand );
  graphics_context_set_fill_color( pGP->ctx, GColorWhite );
  gpath_draw_filled( pGP->ctx, pGP->s_min_hand_highlight );
  graphics_context_set_fill_color( pGP->ctx, pGP->hour_hand_colour );
  graphics_context_set_stroke_color( pGP->ctx, pGP->hand_outline_color );
  gpath_draw_outline( pGP->ctx, pGP->s_min_hand );

  if ( ! ( pGP->show_seconds ) ) {
    graphics_context_set_fill_color( pGP->ctx, GColorBlack );
    graphics_fill_circle( pGP->ctx, pGP->center_pt, 2 );
  } 
}
