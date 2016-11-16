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

static BitmapLayer *weather_bitmap_layer = 0;
static TextLayer *weather_text_layer = 0;
static TextLayer *weather_icon_text_layer = 0;
static WEATHER_DATA weather_data;

void show_weather( Tuple *tuple_ptr_temp, DictionaryIterator *iterator ) {
  Tuple* tuple_ptr_weather_icon_id = ( iterator ) ? dict_find( iterator, MESSAGE_KEY_WEATHER_ICON_ID ) : 0;
  
  if ( tuple_ptr_temp ) {
    if ( tuple_ptr_weather_icon_id ) {
      weather_data.icon_id = 0; // 
    }
    snprintf( weather_data.temp_str, sizeof( weather_data.temp_str ), "%s", tuple_ptr_temp->value->cstring );
  }
  layer_mark_dirty( bitmap_layer_get_layer( weather_bitmap_layer ) );
}

void clear_weather( void ) {
  weather_data.temp_str[0] = 0;
  weather_data.icon_id = 0;
}

void get_weather( struct tm *tick_time, bool ignoreUpdateInterval ) {
  
  if (DEBUG) APP_LOG( APP_LOG_LEVEL_INFO, "weather.c: get_weather(): (%02d:%02d) %lu: %d", tick_time->tm_hour, tick_time->tm_min, MESSAGE_KEY_SHOW_WEATHER, (int) persist_read_bool( MESSAGE_KEY_SHOW_WEATHER ) );
  
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

  send_request( REQUEST_WEATHER );
}

void weather_bitmap_layer_update_proc(  Layer *layer, GContext *ctx ) {
  if( ! persist_read_bool( MESSAGE_KEY_SHOW_WEATHER ) ) return;
  // if ( ! strlen( weather_data.temp_str ) ) return;
  
  GRect weather_window_bounds = layer_get_bounds( layer );
  
  graphics_context_set_fill_color( ctx, GColorWhite );
  graphics_fill_rect( ctx, weather_window_bounds, WEATHER_WINDOW_OUTLINE_THK, GCornersAll );
  weather_window_bounds = grect_inset( weather_window_bounds, GEdgeInsets( WEATHER_WINDOW_OUTLINE_THK ) );
  graphics_context_set_fill_color( ctx, GColorBlue );
  graphics_fill_rect( ctx, weather_window_bounds, WEATHER_WINDOW_OUTLINE_THK, GCornersAll );
}

void weather_text_layer_update_proc( Layer *layer, GContext *ctx ) {
  if( ! persist_read_bool( MESSAGE_KEY_SHOW_WEATHER ) ) return;
  // if ( ! strlen( weather_data.temp_str ) ) return;

  GRect weather_text_layer_bounds = layer_get_bounds( layer );
  graphics_context_set_fill_color( ctx, GColorOrange );
  graphics_fill_rect( ctx, weather_text_layer_bounds, WEATHER_ICON_OUTLINE_THK, GCornersAll );

  weather_text_layer_bounds.origin.y -= WEATHER_TEXT_VERT_ADJ;
  graphics_context_set_text_color( ctx, GColorWhite );
  graphics_draw_text( ctx, weather_data.temp_str, fonts_get_system_font( FONT_KEY_ROBOTO_CONDENSED_21 ), weather_text_layer_bounds,
                     GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL );
}

void weather_icon_text_layer_update_proc( Layer *layer, GContext *ctx ) {
  if( ! persist_read_bool( MESSAGE_KEY_SHOW_WEATHER ) ) return;
  // if( ! weather_data.icon_id ) return;
  
  GRect weather_icon_layer_bounds = layer_get_bounds( layer );
  graphics_context_set_fill_color( ctx, GColorBlack );
  graphics_fill_rect( ctx, weather_icon_layer_bounds, WEATHER_ICON_OUTLINE_THK, GCornersAll );
  draw_icon( ctx, weather_icon_layer_bounds, windy_raining );
}


void weather_init( Layer *parent_layer ) {
  GRect parent_layer_bounds = layer_get_bounds( parent_layer );
  
  GRect weather_bitmap_layer_frame = GRect( parent_layer_bounds.origin.x + WEATHER_WINDOW_POS_X - WEATHER_WINDOW_OUTLINE_THK,
                                           parent_layer_bounds.origin.y + WEATHER_WINDOW_POS_Y - WEATHER_WINDOW_OUTLINE_THK,
                                           WEATHER_WINDOW_SIZE_W + ( WEATHER_WINDOW_OUTLINE_THK * 2 ),
                                           WEATHER_WINDOW_SIZE_H + ( WEATHER_WINDOW_OUTLINE_THK * 2 ) );
  weather_bitmap_layer = bitmap_layer_create( weather_bitmap_layer_frame );
  bitmap_layer_set_compositing_mode( weather_bitmap_layer, GCompOpAssign );
  layer_set_update_proc( bitmap_layer_get_layer( weather_bitmap_layer ), weather_bitmap_layer_update_proc );
  layer_add_child( parent_layer, bitmap_layer_get_layer( weather_bitmap_layer ) );
  
  GRect weather_text_layer_bounds = GRect( WEATHER_WINDOW_SIZE_W / 2 - WEATHER_TEXT_SIZE_W / 2,
                                          WEATHER_WINDOW_OUTLINE_THK + WEATHER_ICON_SIZE_H + ( WEATHER_ICON_OUTLINE_THK * 2 ),
                                          WEATHER_TEXT_SIZE_W + ( WEATHER_TEXT_OUTLINE_THK * 2 ),
                                          WEATHER_TEXT_SIZE_H + ( WEATHER_TEXT_OUTLINE_THK * 2 ) );
  weather_text_layer = text_layer_create( weather_text_layer_bounds );
  layer_set_update_proc( text_layer_get_layer( weather_text_layer ), weather_text_layer_update_proc );
  layer_add_child( bitmap_layer_get_layer( weather_bitmap_layer ), text_layer_get_layer( weather_text_layer ) );
  
  GRect weather_icon_text_layer_frame = GRect( WEATHER_WINDOW_SIZE_W / 2 - WEATHER_ICON_SIZE_W / 2,
                                              WEATHER_WINDOW_OUTLINE_THK,
                                              WEATHER_ICON_SIZE_W + ( WEATHER_ICON_OUTLINE_THK * 2 ), 
                                              WEATHER_ICON_SIZE_H + ( WEATHER_ICON_OUTLINE_THK * 2 ) );
  weather_icon_text_layer = text_layer_create( weather_icon_text_layer_frame );
  layer_set_update_proc( text_layer_get_layer( weather_icon_text_layer ), weather_icon_text_layer_update_proc );
  layer_add_child( bitmap_layer_get_layer( weather_bitmap_layer ), text_layer_get_layer( weather_icon_text_layer ) );
  
  weather_icons_init();
}

void weather_deinit() {
  weather_icons_deinit();
  if ( weather_icon_text_layer ) text_layer_destroy( weather_icon_text_layer );
  if ( weather_text_layer ) text_layer_destroy( weather_text_layer );
  if ( weather_bitmap_layer ) bitmap_layer_destroy( weather_bitmap_layer );
}

#endif
