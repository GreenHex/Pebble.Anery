#ifdef PBL_HEALTH

#include <pebble.h>
#include "heart.h"
#include "global.h"

static BitmapLayer *hr_bitmap_layer = 0;
static TextLayer *hr_text_layer = 0;
static BitmapLayer *hr_icon_bitmap_layer = 0;
static HealthValue hr_bpm;
static char hr_bpm_str[4] = "71";
static bool hr_available = false;

void hr_bitmap_layer_update_proc(  Layer *layer, GContext *ctx ) {
  if ( ! hr_available ) return;
  if( ! persist_read_bool( MESSAGE_KEY_SHOW_HEALTH ) ) return;
  
  GRect hr_window_bounds = layer_get_bounds( layer );
  
  // graphics_context_set_fill_color( ctx, GColorBlack );
  // graphics_fill_rect( ctx, hr_window_bounds, HR_WINDOW_OUTLINE_THK, GCornersAll );
  hr_window_bounds = grect_inset( hr_window_bounds, GEdgeInsets( HR_WINDOW_OUTLINE_THK ) );
  graphics_context_set_fill_color( ctx, GColorBlack );
  graphics_fill_rect( ctx, hr_window_bounds, HR_WINDOW_OUTLINE_THK, GCornersAll );  
}

void hr_text_layer_update_proc( Layer *layer, GContext *ctx ) {
  if ( ! hr_available ) return;
  if( ! persist_read_bool( MESSAGE_KEY_SHOW_HEALTH ) ) return;
  
  HealthValue hr_bpm = health_service_peek_current_value( HealthMetricHeartRateBPM );
  
  if ( hr_bpm > 0 ) {
    snprintf( hr_bpm_str, sizeof( hr_bpm_str ), "%lu", hr_bpm );
    GRect hr_text_layer_bounds = layer_get_bounds( layer );
    hr_text_layer_bounds.origin.y -= HR_TEXT_VERT_ADJ;
    graphics_context_set_text_color( ctx, GColorLightGray );
    graphics_draw_text( ctx, hr_bpm_str, fonts_get_system_font( FONT_KEY_DROID_SERIF_28_BOLD ), hr_text_layer_bounds,
                       GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL );
  }
}

void hr_icon_bitmap_layer_update_proc( Layer *layer, GContext *ctx ) {
  if ( ! hr_available ) return;
  if( ! persist_read_bool( MESSAGE_KEY_SHOW_HEALTH ) ) return;
  
  GRect hr_icon_layer_bounds = layer_get_bounds( layer );
  // graphics_context_set_fill_color( ctx, GColorWhite );
  // graphics_fill_rect( ctx, hr_icon_layer_bounds, 0, GCornersAll );
  GBitmap *health_icon_bitmap = gbitmap_create_with_resource( RESOURCE_ID_ICON_HEART );
  graphics_draw_bitmap_in_rect( ctx, health_icon_bitmap, hr_icon_layer_bounds );
  gbitmap_destroy( health_icon_bitmap );
}

void heart_init( Layer* parent_layer ) {
  HealthServiceAccessibilityMask hr_mask = health_service_metric_accessible( HealthMetricHeartRateBPM, time_start_of_today(), time( NULL ) );
  if ( ! ( hr_mask & HealthServiceAccessibilityMaskAvailable ) ) {
    hr_available = true;
    return;
  }
  
  GRect parent_layer_bounds = layer_get_bounds( parent_layer );
  
  GRect hr_bitmap_layer_frame = GRect( parent_layer_bounds.origin.x + HR_WINDOW_POS_X + parent_layer_bounds.size.w / 2 - HR_WINDOW_SIZE_W / 2 - HR_WINDOW_OUTLINE_THK,
                                      parent_layer_bounds.origin.y + HR_WINDOW_POS_Y,
                                      HR_WINDOW_SIZE_W + HR_WINDOW_OUTLINE_THK * 2, HR_WINDOW_SIZE_H + HR_WINDOW_OUTLINE_THK * 2 );
  hr_bitmap_layer = bitmap_layer_create( hr_bitmap_layer_frame );
  bitmap_layer_set_compositing_mode( hr_bitmap_layer, GCompOpAssign );
  layer_set_update_proc( bitmap_layer_get_layer( hr_bitmap_layer ), hr_bitmap_layer_update_proc );
  layer_add_child( parent_layer, bitmap_layer_get_layer( hr_bitmap_layer ) );
  
  GRect hr_text_layer_bounds = GRect( HR_WINDOW_OUTLINE_THK, HR_WINDOW_OUTLINE_THK, HR_WINDOW_SIZE_W, HR_WINDOW_SIZE_H );
  hr_text_layer = text_layer_create( hr_text_layer_bounds );
  layer_set_update_proc( text_layer_get_layer( hr_text_layer ), hr_text_layer_update_proc );
  layer_add_child( bitmap_layer_get_layer( hr_bitmap_layer ), text_layer_get_layer( hr_text_layer ) );
  
  GRect hr_icon_bitmap_layer_frame = GRect( hr_bitmap_layer_frame.origin.x + hr_bitmap_layer_frame.size.w / 2 - HR_ICON_SIZE_H / 2,
                                           hr_bitmap_layer_frame.origin.y - HR_ICON_SIZE_H,
                                           HR_ICON_SIZE_W, HR_ICON_SIZE_H );
  hr_icon_bitmap_layer = bitmap_layer_create( hr_icon_bitmap_layer_frame );
  layer_set_update_proc( bitmap_layer_get_layer( hr_icon_bitmap_layer ), hr_icon_bitmap_layer_update_proc );
  layer_add_child( parent_layer, bitmap_layer_get_layer( hr_icon_bitmap_layer ) );
}

void heart_deinit( void ) {
  if ( hr_icon_bitmap_layer ) bitmap_layer_destroy( hr_icon_bitmap_layer );
  if ( hr_text_layer ) text_layer_destroy( hr_text_layer );
  if ( hr_bitmap_layer ) bitmap_layer_destroy( hr_bitmap_layer );
}

#endif