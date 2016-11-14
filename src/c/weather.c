//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#include <pebble.h>
#include "global.h"

#ifdef INCLUDE_WEATHER

#include "weather.h"
#include "clock.h"
#include "app_messaging.h"

static BitmapLayer *weather_bitmap_layer = 0;
static TextLayer *weather_text_layer = 0;
static BitmapLayer *weather_icon_bitmap_layer = 0;

void show_weather( Tuple *tuple_ptr, DictionaryIterator *iterator ) {
  static char buff[8] = "";
  Tuple* weather_icon_id = ( iterator ) ? dict_find( iterator, MESSAGE_KEY_WEATHER_ICON_ID ) : 0;
  
  if ( tuple_ptr ) {
    if ( weather_icon_id ) {
      
    }
    snprintf( buff, sizeof( buff ), "%s", tuple_ptr->value->cstring );
  }
}

void clear_weather( void ) {

}

void get_weather( struct tm *tick_time, bool ignoreUpdateInterval ) {
  if ( ! ( persist_read_int( MESSAGE_KEY_SHOW_WEATHER ) ) ) {
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

  send_request( CMD_WEATHER );
}

void weather_bitmap_layer_update_proc(  Layer *layer, GContext *ctx ) {
  if( ! persist_read_bool( MESSAGE_KEY_SHOW_WEATHER ) ) return;  
}

void weather_text_layer_update_proc( Layer *layer, GContext *ctx ) {
  if( ! persist_read_bool( MESSAGE_KEY_SHOW_WEATHER ) ) return;
}

void weather_icon_bitmap_layer_update_proc( Layer *layer, GContext *ctx ) {
  if( ! persist_read_bool( MESSAGE_KEY_SHOW_WEATHER ) ) return;  
}


void weather_init( Layer *parent_layer ) {
  GRect parent_layer_bounds = layer_get_bounds( parent_layer );
  
  GRect weather_bitmap_layer_frame = GRect( parent_layer_bounds.origin.x + WEATHER_WINDOW_POS_X + parent_layer_bounds.size.w / 2 - WEATHER_WINDOW_SIZE_W / 2 - WEATHER_WINDOW_OUTLINE_THK,
                                      parent_layer_bounds.origin.y + WEATHER_WINDOW_POS_Y,
                                      WEATHER_WINDOW_SIZE_W + WEATHER_WINDOW_OUTLINE_THK * 2, WEATHER_WINDOW_SIZE_H + WEATHER_WINDOW_OUTLINE_THK * 2 );
  weather_bitmap_layer = bitmap_layer_create( weather_bitmap_layer_frame );
  bitmap_layer_set_compositing_mode( weather_bitmap_layer, GCompOpAssign );
  layer_set_update_proc( bitmap_layer_get_layer( weather_bitmap_layer ), weather_bitmap_layer_update_proc );
  layer_add_child( parent_layer, bitmap_layer_get_layer( weather_bitmap_layer ) );
  
  GRect weather_text_layer_bounds = GRect( WEATHER_WINDOW_OUTLINE_THK, WEATHER_WINDOW_OUTLINE_THK, WEATHER_WINDOW_SIZE_W, WEATHER_WINDOW_SIZE_H );
  weather_text_layer = text_layer_create( weather_text_layer_bounds );
  layer_set_update_proc( text_layer_get_layer( weather_text_layer ), weather_text_layer_update_proc );
  layer_add_child( bitmap_layer_get_layer( weather_bitmap_layer ), text_layer_get_layer( weather_text_layer ) );
  
  GRect weather_icon_bitmap_layer_frame = GRect( weather_bitmap_layer_frame.origin.x + weather_bitmap_layer_frame.size.w / 2 - WEATHER_ICON_SIZE_H / 2,
                                           weather_bitmap_layer_frame.origin.y - WEATHER_ICON_SIZE_H,
                                           WEATHER_ICON_SIZE_W, WEATHER_ICON_SIZE_H );
  weather_icon_bitmap_layer = bitmap_layer_create( weather_icon_bitmap_layer_frame );
  layer_set_update_proc( bitmap_layer_get_layer( weather_icon_bitmap_layer ), weather_icon_bitmap_layer_update_proc );
  layer_add_child( parent_layer, bitmap_layer_get_layer( weather_icon_bitmap_layer ) );
}

void weather_deinit() {
  if ( weather_icon_bitmap_layer ) bitmap_layer_destroy( weather_icon_bitmap_layer );
  if ( weather_text_layer ) text_layer_destroy( weather_text_layer );
  if ( weather_bitmap_layer ) bitmap_layer_destroy( weather_bitmap_layer );
}

#endif
