//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#pragma once
#include <pebble.h>

#ifdef INCLUDE_WEATHER

#define WEATHER_ICON_SIZE_W 32
#define WEATHER_ICON_SIZE_H 32
#define WEATHER_ICON_OUTLINE_THK 1
#define WEATHER_ICON_VERT_ADJ 0
//
#define WEATHER_TEXT_SIZE_W 58
#define WEATHER_TEXT_SIZE_H 18
#define WEATHER_TEXT_OUTLINE_THK 1
#define WEATHER_TEXT_VERT_ADJ 4
//
#if PBL_DISPLAY_WIDTH == 200
#define WEATHER_WINDOW_POS_X 60
#define WEATHER_WINDOW_POS_Y 80
#else
#define WEATHER_WINDOW_POS_X 60
#define WEATHER_WINDOW_POS_Y 90
#endif
#define WEATHER_WINDOW_SIZE_W ( ( WEATHER_TEXT_SIZE_W > WEATHER_ICON_SIZE_W ? WEATHER_TEXT_SIZE_W : WEATHER_ICON_SIZE_W ) + ( WEATHER_TEXT_OUTLINE_THK * 2 ) + ( ( WEATHER_TEXT_SIZE_W > WEATHER_ICON_SIZE_W ? WEATHER_TEXT_OUTLINE_THK : WEATHER_ICON_OUTLINE_THK )  * 2 ) ) 
#define WEATHER_WINDOW_SIZE_H ( WEATHER_ICON_SIZE_H + ( WEATHER_ICON_OUTLINE_THK * 2 ) + WEATHER_TEXT_SIZE_H + ( WEATHER_TEXT_OUTLINE_THK * 2 ) )
#define WEATHER_WINDOW_OUTLINE_THK 1
//

typedef struct {
  char temp_str[8];
  uint32_t icon_id;
} WEATHER_DATA;

void clear_weather( void );
void show_weather( Tuple *tuple_ptr_temp, DictionaryIterator *iterator );
void get_weather( struct tm *time, bool ignoreUpdateInterval );
void weather_init( Layer *parent_layer );
void weather_deinit( void );

#endif

