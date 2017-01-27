//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#pragma once
#include <pebble.h>

#ifdef INCLUDE_WEATHER

#define WEATHER_ICON_SIZE_W 50
#define WEATHER_ICON_SIZE_H 50

#define WEATHER_TEXT_SIZE_W 56
#define WEATHER_TEXT_SIZE_H 24

#define WEATHER_WINDOW_SIZE_W ( WEATHER_ICON_SIZE_W + WEATHER_TEXT_SIZE_W )
#define WEATHER_WINDOW_SIZE_H ( WEATHER_ICON_SIZE_H > WEATHER_TEXT_SIZE_H ? WEATHER_ICON_SIZE_H : WEATHER_TEXT_SIZE_H )

#define WEATHER_WINDOW_POS_X ( ( PBL_DISPLAY_WIDTH / 2 ) - ( WEATHER_WINDOW_SIZE_W / 2 ) )
#define WEATHER_WINDOW_POS_Y ( ( PBL_DISPLAY_HEIGHT / 2 ) + 16 )

#define WEATHER_WINDOW_FRAME ( GRect( WEATHER_WINDOW_POS_X, WEATHER_WINDOW_POS_Y, WEATHER_WINDOW_SIZE_W, WEATHER_WINDOW_SIZE_H ) )
#define WEATHER_ICON_FRAME ( GRect( 0, 0, WEATHER_ICON_SIZE_W, WEATHER_ICON_SIZE_H ) )
#define WEATHER_TEXT_FRAME ( GRect( WEATHER_ICON_SIZE_W, (WEATHER_WINDOW_SIZE_H/2) - (WEATHER_TEXT_SIZE_H/2), WEATHER_TEXT_SIZE_W, WEATHER_TEXT_SIZE_H ) )

#define WEATHER_ICON_VERT_ADJ 4
#define WEATHER_TEXT_VERT_ADJ 6

void clear_weather( void );
void show_weather( Tuple *tuple_ptr_temp, DictionaryIterator *iterator );
void get_weather( struct tm *time, bool ignoreUpdateInterval );
void weather_init( Layer *parent_layer );
void weather_deinit( void );

#endif

