//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#include <pebble.h>
#include "battery.h"
#include "draw_utils.h"

static BatteryChargeState charge_state;
// cont battery gauge
static BitmapLayer *cont_batt_gauge_bitmap_layer = 0;
// moser battery gauge
static BitmapLayer *moser_batt_gauge_bitmap_layer = 0;
static GPath *s_moser_batt_gauge_hand = 0;
// sbge001 battry gauge
static BitmapLayer *sbge001_batt_gauge_bitmap_layer = 0;
static GPath *s_sbge001_batt_gauge_hand = 0;

static void batt_gauge_update_proc( BatteryChargeState state ) {
  charge_state = state;
  if ( persist_read_int( MESSAGE_KEY_ANALOG_HANDS_STYLE ) == STYLE_SPIFFY_GS ) {
    layer_mark_dirty( bitmap_layer_get_layer( moser_batt_gauge_bitmap_layer ) );
  } else if ( persist_read_int( MESSAGE_KEY_ANALOG_HANDS_STYLE ) == STYLE_SBGE001 ) {
    // layer_mark_dirty( bitmap_layer_get_layer( sbge001_batt_gauge_bitmap_layer ) );
  } else {
    layer_mark_dirty( bitmap_layer_get_layer( cont_batt_gauge_bitmap_layer ) );
  }
}

static void draw_battery_hand( BATTERY_HAND_DRAW_PARAMS *pDP ) {
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

static void cont_batt_gauge_layer_update_proc( Layer *layer, GContext *ctx ) {
  if( ! persist_read_bool( MESSAGE_KEY_SHOW_BATTERY_GAUGE ) ) return;
  if( persist_read_int( MESSAGE_KEY_ANALOG_HANDS_STYLE ) != STYLE_CONTEMPORARY ) return;
  
  GRect batt_gauge_window_bounds = layer_get_bounds( layer );
  // GPoint center_pt = grect_center_point( &batt_gauge_window_bounds );
  uint32_t batt_angle = TRIG_MAX_ANGLE * charge_state.charge_percent / 100;
 
  GColor batt_indicator_colour = GColorDarkGray;
  #if defined( PBL_COLOR )
  if ( charge_state.charge_percent < 16 ) {
    batt_indicator_colour = COLOUR_BATT_15;
  } else if ( charge_state.charge_percent < 96 ) {
    batt_indicator_colour = COLOUR_BATT_95;
  } else {
    batt_indicator_colour = COLOUR_BATT_100;
  }
  #endif
  
  graphics_context_set_fill_color( ctx, batt_indicator_colour );
  graphics_fill_radial( ctx, batt_gauge_window_bounds, GOvalScaleModeFitCircle, 
                      CONT_BATT_GAUGE_EXT_RADIUS - CONT_BATT_GAUGE_INT_RADIUS, 0, batt_angle );
  /*  
  graphics_context_set_stroke_color( ctx, GColorDarkGray );
  graphics_context_set_stroke_width( ctx, 1 );
  
  graphics_draw_circle( ctx, center_pt, CONT_BATT_GAUGE_INT_RADIUS );
  graphics_draw_circle( ctx, center_pt, CONT_BATT_GAUGE_EXT_RADIUS );
  */
}

static void moser_batt_gauge_layer_update_proc( Layer *layer, GContext *ctx ) {
  if( ! persist_read_bool( MESSAGE_KEY_SHOW_BATTERY_GAUGE ) ) return;
  if( persist_read_int( MESSAGE_KEY_ANALOG_HANDS_STYLE ) != STYLE_SPIFFY_GS ) return;
  
  GRect batt_gauge_window_bounds = layer_get_bounds( layer );
  GPoint center_pt = grect_center_point( &batt_gauge_window_bounds );
  GBitmap *moser_batt_gauge_bitmap = gbitmap_create_with_resource( RESOURCE_ID_MOSER_BATTERY_GAUGE );
  graphics_draw_bitmap_in_rect( ctx, moser_batt_gauge_bitmap, batt_gauge_window_bounds );
  
  center_pt.x = MOSER_BATT_GAUGE_SIZE_W - 6;
 
  // uint32_t batt_angle = (uint32_t) TRIG_MAX_ANGLE * ( ( ( charge_state.charge_percent * 50 ) / 100 ) + 245 );
  uint32_t batt_angle = DEG_TO_TRIGANGLE( (uint32_t) ( ( charge_state.charge_percent * 50 ) / 100 ) + 245 );
  GPoint battery_hand = (GPoint) {
    .x = ( sin_lookup( batt_angle ) * BATT_HAND_LENGTH / TRIG_MAX_RATIO ) + center_pt.x,
    .y = ( -cos_lookup( batt_angle ) * BATT_HAND_LENGTH / TRIG_MAX_RATIO ) + center_pt.y
  };
  
  draw_clock_hand( & (HAND_DRAW_PARAMS) {
    .ctx = ctx,
    .center_pt = center_pt,
    .from_pt = center_pt,
    .to_pt = battery_hand,
    .hand_width = 1,
    .hand_color = GColorDarkGray,
    .hand_outline_color = GColorBlack,
    .dot_radius = 3,
    .dot_color = GColorDarkGray,
    .dot_outline_color = GColorBlack
  } );
  
  graphics_context_set_fill_color( ctx, charge_state.is_charging ? GColorJaegerGreen : 
                                  charge_state.charge_percent < 16 ? GColorFolly : GColorDarkGray );
  graphics_fill_circle( ctx, center_pt, 1 );
  
  
  /*
  BATTERY_HAND_DRAW_PARAMS batt_hand_params = {
    .ctx = ctx,
    .batt_angle = batt_angle,
    .center_pt = center_pt,
    .s_hand = s_moser_batt_gauge_hand,
    .hand_colour = GColorDarkGray,
    .hand_outline_colour = GColorLightGray,
    .dot_radius = MOSER_BATT_GAUGE_DOT_RADIUS,
    .charge_state = charge_state,    
  };
  draw_battery_hand( &batt_hand_params );
  */
  gbitmap_destroy( moser_batt_gauge_bitmap );
}

static void sbge001_batt_gauge_layer_update_proc( Layer *layer, GContext *ctx ) {
  if( ! persist_read_bool( MESSAGE_KEY_SHOW_BATTERY_GAUGE ) ) return;
  if( persist_read_int( MESSAGE_KEY_ANALOG_HANDS_STYLE ) != STYLE_SBGE001 ) return;
  
}

static void sbge001_batt_gauge_layer_update_proc_OLD( Layer *layer, GContext *ctx ) {
  return;
  if( ! persist_read_bool( MESSAGE_KEY_SHOW_BATTERY_GAUGE ) ) return;
  if( persist_read_int( MESSAGE_KEY_ANALOG_HANDS_STYLE ) != STYLE_SBGE001 ) return;

  GRect batt_gauge_window_bounds = layer_get_bounds( layer );
  GPoint center_pt = grect_center_point( &batt_gauge_window_bounds );
  GBitmap *sbge001_batt_gauge_bitmap = gbitmap_create_with_resource( RESOURCE_ID_SBGE001_BATTERY_GAUGE );
  graphics_draw_bitmap_in_rect( ctx, sbge001_batt_gauge_bitmap, batt_gauge_window_bounds );
  
  uint32_t batt_angle = (uint32_t) ( ( charge_state.charge_percent * 105 ) / 100 ) + 225;
  BATTERY_HAND_DRAW_PARAMS batt_hand_params = {
    .ctx = ctx,
    .batt_angle = batt_angle,
    .center_pt = center_pt,
    .s_hand = s_sbge001_batt_gauge_hand,
    .hand_colour = GColorDarkGray,
    .hand_outline_colour = GColorLightGray,
    .dot_radius = SBGE001_BATT_GAUGE_DOT_RADIUS,
    .charge_state = charge_state,   
  };
  draw_battery_hand( &batt_hand_params );
  gbitmap_destroy( sbge001_batt_gauge_bitmap );
}

void battery_init( Layer* parent_layer ) {
  GRect parent_layer_bounds = layer_get_bounds( parent_layer );
  // cont battery
  GRect cont_batt_gauge_bitmap_layer_frame = GRect( parent_layer_bounds.origin.x + ( ( parent_layer_bounds.size.w - CONT_BATT_GAUGE_SIZE ) / 2 ),
                                  parent_layer_bounds.origin.y + ( ( parent_layer_bounds.size.h - CONT_BATT_GAUGE_SIZE ) / 2 ),
                                  CONT_BATT_GAUGE_SIZE, CONT_BATT_GAUGE_SIZE );
  cont_batt_gauge_bitmap_layer = bitmap_layer_create( cont_batt_gauge_bitmap_layer_frame );
  layer_set_update_proc( bitmap_layer_get_layer( cont_batt_gauge_bitmap_layer ), cont_batt_gauge_layer_update_proc );
  layer_add_child( parent_layer, bitmap_layer_get_layer( cont_batt_gauge_bitmap_layer ) );
  // moser battery
  GRect moser_batt_gauge_bitmap_layer_frame = GRect( parent_layer_bounds.origin.x + 3,
                                  parent_layer_bounds.origin.y + ( ( parent_layer_bounds.size.h - MOSER_BATT_GAUGE_SIZE_H ) / 2 ),
                                  MOSER_BATT_GAUGE_SIZE_W, MOSER_BATT_GAUGE_SIZE_H );
  moser_batt_gauge_bitmap_layer = bitmap_layer_create( moser_batt_gauge_bitmap_layer_frame );
  layer_set_update_proc( bitmap_layer_get_layer( moser_batt_gauge_bitmap_layer ), moser_batt_gauge_layer_update_proc );
  layer_add_child( parent_layer, bitmap_layer_get_layer( moser_batt_gauge_bitmap_layer ) );
  // sbge001 battery
  GRect sbge001_batt_gauge_layer_frame = GRect( SBGE001_BATT_GAUGE_LOC_X, SBGE001_BATT_GAUGE_LOC_Y,
                                               SBGE001_BATT_GAUGE_SIZE, SBGE001_BATT_GAUGE_SIZE );
  sbge001_batt_gauge_bitmap_layer = bitmap_layer_create( sbge001_batt_gauge_layer_frame );
  layer_set_update_proc( bitmap_layer_get_layer( sbge001_batt_gauge_bitmap_layer ), sbge001_batt_gauge_layer_update_proc );
  layer_add_child( parent_layer, bitmap_layer_get_layer( sbge001_batt_gauge_bitmap_layer ) );
  
  // hands
  s_moser_batt_gauge_hand = gpath_create( &MOSER_BATT_GAUGE_HAND);
  s_sbge001_batt_gauge_hand = gpath_create( &SBGE001_BATT_GAUGE_HAND );

  // subscriptions
  batt_gauge_update_proc( battery_state_service_peek() );
  battery_state_service_subscribe( batt_gauge_update_proc );
}

void battery_deinit( void ) {
  bitmap_layer_destroy( cont_batt_gauge_bitmap_layer );
  bitmap_layer_destroy( moser_batt_gauge_bitmap_layer );
  bitmap_layer_destroy( sbge001_batt_gauge_bitmap_layer );
  gpath_destroy( s_sbge001_batt_gauge_hand );
  gpath_destroy( s_moser_batt_gauge_hand );
}