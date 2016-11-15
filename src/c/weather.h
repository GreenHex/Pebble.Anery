//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#pragma once
#include <pebble.h>

#ifdef INCLUDE_WEATHER

#define WEATHER_WINDOW_POS_X 22
#if PBL_DISPLAY_WIDTH == 200
#define WEATHER_WINDOW_POS_Y 174
#else
#define WEATHER_WINDOW_POS_Y 133
#endif
#define WEATHER_WINDOW_SIZE_W 58
#define WEATHER_WINDOW_SIZE_H 20
#define WEATHER_WINDOW_OUTLINE_THK 1
#define WEATHER_TEXT_VERT_ADJ 4
#define WEATHER_ICON_SIZE_W 29
#define WEATHER_ICON_SIZE_H 29
#define WEATHER_ICON_OUTLINE_THK 2

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

