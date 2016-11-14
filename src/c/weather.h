//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#pragma once
#include <pebble.h>

#ifdef INCLUDE_WEATHER

#define WEATHER_WINDOW_POS_X 22
#if PBL_DISPLAY_WIDTH == 200
#define WEATHER_WINDOW_POS_Y 154
#else
#define WEATHER_WINDOW_POS_Y 112
#endif
#define WEATHER_WINDOW_SIZE_W 54
#define WEATHER_WINDOW_SIZE_H 28
#define WEATHER_WINDOW_OUTLINE_THK 2
#define WEATHER_TEXT_VERT_ADJ 4
#define WEATHER_ICON_SIZE_W 13
#define WEATHER_ICON_SIZE_H 12

void clear_weather( void );
void show_weather( Tuple *tuple_ptr, DictionaryIterator *iterator );
void get_weather( struct tm *time, bool ignoreUpdateInterval );
void weather_init( Layer *parent_layer );
void weather_deinit( void );

#endif

