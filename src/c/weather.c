//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#include <pebble.h>
#include "global.h"

#ifdef INCLUDE_WEATHER

#include "weather.h"
#include "weather_icons.h"
#include "clock.h"
#include "app_messaging.h"

bool is_day_not_night;

static BitmapLayer *weather_bitmap_layer = 0;
static TextLayer *weather_text_layer = 0;
static TextLayer *weather_icon_text_layer = 0;
static WEATHER_DATA weather_data;

void show_weather( Tuple *tuple_ptr_temp, DictionaryIterator *iterator ) {
  Tuple* tuple_ptr_weather_icon_id = ( iterator ) ? dict_find( iterator, MESSAGE_KEY_WEATHER_ICON_ID ) : 0;
  
  if ( tuple_ptr_temp ) {
    if ( tuple_ptr_weather_icon_id ) {
      weather_data.icon_id = tuple_ptr_weather_icon_id->value->int32;
      // weather_data.icon_id = 10; // test
    }
    snprintf( weather_data.temp_str, sizeof( weather_data.temp_str ), "%s", tuple_ptr_temp->value->cstring );
    // snprintf( weather_data.temp_str, sizeof( weather_data.temp_str ), "%s", "38 C" ); // test
  }
  layer_mark_dirty( bitmap_layer_get_layer( weather_bitmap_layer ) );
}

void clear_weather( void ) {
  weather_data.temp_str[0] = 0;
  // snprintf( weather_data.temp_str, sizeof( weather_data.temp_str ), "%s", "38 C" );
  weather_data.icon_id = 0;
}

void get_weather( struct tm *tick_time, bool ignoreUpdateInterval ) {
  
  #ifdef DEBUG
  APP_LOG( APP_LOG_LEVEL_INFO, "weather.c: get_weather(): (%02d:%02d) %lu: %d", tick_time->tm_hour, tick_time->tm_min, MESSAGE_KEY_SHOW_WEATHER, (int) persist_read_bool( MESSAGE_KEY_SHOW_WEATHER ) );
  #endif
  
  if ( ! ( persist_read_bool( MESSAGE_KEY_SHOW_WEATHER ) ) ) {
    clear_weather();
    return;
  }
  
  if ( ! is_X_in_range( persist_read_int( MESSAGE_KEY_WEATHER_UPDATE_START_TIME ), 
                       persist_read_int( MESSAGE_KEY_WEATHER_UPDATE_END_TIME ), tick_time->tm_hour ) ) {
    clear_weather();
    return;
  }

  if ( ! persist_read_bool( MESSAGE_KEY_WEATHER_UPDATE_ON_DAYS + tick_time->tm_wday ) ) {
    clear_weather();
    return;
  }

  if ( ( ! ignoreUpdateInterval ) && ( tick_time->tm_min % ( persist_read_int( MESSAGE_KEY_WEATHER_UPDATE_INTERVAL ) ) ) ) return;

  is_day_not_night = ( ( tick_time->tm_hour > 4 ) && ( tick_time->tm_hour < 18 ) ); // quick-and-dirty
  send_request( REQUEST_WEATHER );
}

void weather_bitmap_layer_update_proc(  Layer *layer, GContext *ctx ) {
  if( ! persist_read_bool( MESSAGE_KEY_SHOW_WEATHER ) ) return;
  if ( ! strlen( weather_data.temp_str ) ) return;
  
  GRect weather_window_bounds = layer_get_bounds( layer );
  
  // graphics_context_set_fill_color( ctx, PBL_IF_COLOR_ELSE( GColorDarkGray, GColorLightGray ) );
  // graphics_fill_rect( ctx, weather_window_bounds, 5, GCornersAll );
  // weather_window_bounds = grect_inset( weather_window_bounds, GEdgeInsets( WEATHER_WINDOW_OUTLINE_THK ) );
  // graphics_context_set_fill_color( ctx, GColorBlue );
  // graphics_fill_rect( ctx, weather_window_bounds, WEATHER_WINDOW_OUTLINE_THK, GCornersAll );
  
  // graphics_context_set_stroke_width( ctx, 1 );
  // graphics_context_set_stroke_color( ctx, PBL_IF_COLOR_ELSE( GColorDarkGray, GColorLightGray ) );
  // graphics_draw_round_rect( ctx, weather_window_bounds, 6 );
}

void weather_text_layer_update_proc( Layer *layer, GContext *ctx ) {
  if( ! persist_read_bool( MESSAGE_KEY_SHOW_WEATHER ) ) return;
  if ( ! strlen( weather_data.temp_str ) ) return;

  GRect weather_text_layer_bounds = layer_get_bounds( layer );
  // graphics_context_set_fill_color( ctx, GColorOrange );
  // graphics_fill_rect( ctx, weather_text_layer_bounds, 2, GCornersAll );
  graphics_context_set_antialiased( ctx, true );
  weather_text_layer_bounds.origin.y -= WEATHER_TEXT_VERT_ADJ;
  graphics_context_set_text_color( ctx, GColorLightGray );
  #if defined( PBL_COLOR1 )
  graphics_draw_text( ctx, weather_data.temp_str, fonts_get_system_font( FONT_KEY_GOTHIC_28 /* FONT_KEY_ROBOTO_CONDENSED_21 */ ), weather_text_layer_bounds,
                     GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL );
  #else
    graphics_draw_text( ctx, weather_data.temp_str, fonts_get_system_font( FONT_KEY_ROBOTO_CONDENSED_21 ), weather_text_layer_bounds,
                     GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL );
  #endif
}

void weather_icon_text_layer_update_proc( Layer *layer, GContext *ctx ) {
  if( ! persist_read_bool( MESSAGE_KEY_SHOW_WEATHER ) ) return;
  if( ! weather_data.icon_id ) return;
  
  GRect weather_icon_layer_bounds = layer_get_bounds( layer );
  // graphics_context_set_fill_color( ctx, GColorOrange );
  // graphics_fill_rect( ctx, weather_icon_layer_bounds, 2, GCornersAll );
  graphics_context_set_antialiased( ctx, true );
  weather_icon_layer_bounds.origin.y -= WEATHER_ICON_VERT_ADJ;
  draw_icon( ctx, weather_icon_layer_bounds, weather_data.icon_id, is_day_not_night );
}

void weather_init( Layer *parent_layer ) {
  GRect parent_layer_bounds = layer_get_bounds( parent_layer );
  weather_icons_init();
  
  GRect weather_bitmap_layer_frame = WEATHER_WINDOW_FRAME;
  weather_bitmap_layer = bitmap_layer_create( weather_bitmap_layer_frame );
  bitmap_layer_set_compositing_mode( weather_bitmap_layer, GCompOpAssign );
  layer_set_update_proc( bitmap_layer_get_layer( weather_bitmap_layer ), weather_bitmap_layer_update_proc );
  layer_add_child( parent_layer, bitmap_layer_get_layer( weather_bitmap_layer ) );
  
  GRect weather_text_layer_bounds = WEATHER_TEXT_FRAME;
  weather_text_layer = text_layer_create( weather_text_layer_bounds );
  layer_set_update_proc( text_layer_get_layer( weather_text_layer ), weather_text_layer_update_proc );
  layer_add_child( bitmap_layer_get_layer( weather_bitmap_layer ), text_layer_get_layer( weather_text_layer ) );
  
  GRect weather_icon_text_layer_frame = WEATHER_ICON_FRAME;
  weather_icon_text_layer = text_layer_create( weather_icon_text_layer_frame );
  layer_set_update_proc( text_layer_get_layer( weather_icon_text_layer ), weather_icon_text_layer_update_proc );
  layer_add_child( bitmap_layer_get_layer( weather_bitmap_layer ), text_layer_get_layer( weather_icon_text_layer ) );
}

void weather_deinit() {
  if ( weather_icon_text_layer ) text_layer_destroy( weather_icon_text_layer );
  if ( weather_text_layer ) text_layer_destroy( weather_text_layer );
  if ( weather_bitmap_layer ) bitmap_layer_destroy( weather_bitmap_layer );
  weather_icons_deinit();
}

#endif
