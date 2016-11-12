#ifdef PBL_HEALTH

#include <pebble.h>
#include "health.h"
#include "global.h"

static BitmapLayer *health_bitmap_layer = 0;
static Layer *health_digit_layer[ HEALTH_NUMBER_OF_DIGITS ];
static TextLayer *health_unit_text_layer = 0;
static HealthValue dist_walked_today;
static char digits[ HEALTH_NUMBER_OF_DIGITS + 1 ];
static MeasurementSystem units = MeasurementSystemMetric;
static char unit_str[3] = "m";

static void health_text_unit_layer_update_proc( Layer *layer, GContext *ctx ) {
  if( ! persist_read_bool( MESSAGE_KEY_SHOW_HEALTH ) ) return;
  
  GRect health_unit_text_layer_bounds = layer_get_bounds( layer );
  graphics_context_set_fill_color( ctx, GColorDarkGray );
  graphics_fill_rect( ctx, health_unit_text_layer_bounds, HEALTH_WINDOW_OUTLINE_THK, GCornersRight );
  
  graphics_context_set_text_color( ctx, GColorWhite );
  health_unit_text_layer_bounds.origin.y -= HEALTH_UNIT_TXT_VERT_ADJ;
  graphics_draw_text( ctx, unit_str, fonts_get_system_font( FONT_KEY_GOTHIC_14 ), health_unit_text_layer_bounds,
                     GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL );  
}

static void health_bitmap_layer_update_proc( Layer *layer, GContext *ctx ) {
  if( ! persist_read_bool( MESSAGE_KEY_SHOW_HEALTH ) ) return;
  
  static int dist = 0;
  
  GRect health_window_bounds = layer_get_bounds( layer );
  graphics_context_set_fill_color( ctx, PBL_IF_COLOR_ELSE( GColorDarkGray, GColorBlack ) );
  graphics_fill_rect( ctx, health_window_bounds, HEALTH_WINDOW_OUTLINE_THK, GCornersLeft | GCornerBottomRight );
  health_window_bounds = grect_inset( health_window_bounds, GEdgeInsets( HEALTH_WINDOW_OUTLINE_THK ) );
  graphics_context_set_fill_color( ctx, GColorLightGray );
  graphics_fill_rect( ctx, health_window_bounds, 0, GCornerNone );
  
  dist_walked_today = health_service_sum_today( HealthMetricWalkedDistanceMeters );
  units = health_service_get_measurement_system_for_display( HealthMetricWalkedDistanceMeters );

  if ( units == MeasurementSystemImperial ) {
    dist = (int) ( (float) dist_walked_today * 3.28F );
    snprintf( unit_str, sizeof( unit_str ), "%s", "ft" );
  } else {
    dist = (int) dist_walked_today;
    snprintf( unit_str, sizeof( unit_str ), "%s", "m" );
  }
  snprintf( digits, sizeof( digits ), "%06d", dist );
}

static void health_digit_layer_update_proc( Layer *layer, GContext *ctx ) {
  if( ! persist_read_bool( MESSAGE_KEY_SHOW_HEALTH ) ) return;
  
  GRect health_digit_layer_bounds = layer_get_bounds( layer );
  graphics_context_set_fill_color( ctx, GColorBlack );
  graphics_fill_rect( ctx, health_digit_layer_bounds, 0, GCornerNone );
  
  graphics_context_set_text_color( ctx, GColorWhite );
  health_digit_layer_bounds.origin.y -= HEALTH_TXT_VERT_ADJ;
  char digit = digits[ ( (DIGIT_LAYER_DATA *) layer_get_data( layer ) )->idx ];
  graphics_draw_text( ctx, &digit, fonts_get_system_font( FONT_KEY_ROBOTO_CONDENSED_21 ), health_digit_layer_bounds,
                     GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL );
}

void health_init( Layer *parent_layer ) {
  GRect parent_layer_bounds = layer_get_bounds( parent_layer );
  // health bitmap layer
  GRect health_window_frame = GRect( parent_layer_bounds.origin.x + HEALTH_WINDOW_POS_X +
                                    parent_layer_bounds.size.w / 2 - HEALTH_WINDOW_SIZE_W / 2 - HEALTH_WINDOW_OUTLINE_THK,
                                    parent_layer_bounds.origin.y + HEALTH_WINDOW_POS_Y,
                                    HEALTH_WINDOW_SIZE_W + ( HEALTH_WINDOW_OUTLINE_THK * 2 ), 
                                    HEALTH_DIGIT_SIZE_H + ( HEALTH_WINDOW_OUTLINE_THK * 2 ) );
  health_bitmap_layer = bitmap_layer_create( health_window_frame );
  bitmap_layer_set_compositing_mode( health_bitmap_layer, GCompOpAssign );
  layer_set_update_proc( bitmap_layer_get_layer( health_bitmap_layer ), health_bitmap_layer_update_proc );
  layer_add_child( parent_layer, bitmap_layer_get_layer( health_bitmap_layer ) );
  
  for ( int i = 0 ; i < HEALTH_NUMBER_OF_DIGITS ; i++ ) {
    GRect health_digit_frame = GRect( ( HEALTH_DIGIT_SIZE_W + HEALTH_DIGIT_GAP ) * i + HEALTH_WINDOW_OUTLINE_THK,
                                     HEALTH_WINDOW_OUTLINE_THK, HEALTH_DIGIT_SIZE_W, HEALTH_DIGIT_SIZE_H );
    health_digit_layer[i] = layer_create_with_data( health_digit_frame, sizeof( DIGIT_LAYER_DATA ) );
    ( (DIGIT_LAYER_DATA *) layer_get_data( health_digit_layer[i] ) )->idx = i;
    layer_set_update_proc( health_digit_layer[i], health_digit_layer_update_proc );
    layer_add_child( bitmap_layer_get_layer( health_bitmap_layer ), health_digit_layer[i] );    
  }
  
  GRect health_unit_text_layer_frame = GRect( health_window_frame.origin.x + health_window_frame.size.w + HEALTH_UNIT_POS_X,
                                             health_window_frame.origin.y + HEALTH_UNIT_POS_Y,
                                             HEALTH_UNIT_SIZE_W, HEALTH_UNIT_SIZE_H );
  health_unit_text_layer = text_layer_create( health_unit_text_layer_frame );
  layer_set_update_proc( text_layer_get_layer( health_unit_text_layer ), health_text_unit_layer_update_proc );
  layer_add_child( parent_layer, text_layer_get_layer( health_unit_text_layer ) );
}

void health_deinit( void ) {
  text_layer_destroy( health_unit_text_layer );
  bitmap_layer_destroy( health_bitmap_layer );
  for ( int i = 0 ; i < HEALTH_NUMBER_OF_DIGITS ; i++ ) {
    layer_destroy( health_digit_layer[i] );
  }
}

#endif