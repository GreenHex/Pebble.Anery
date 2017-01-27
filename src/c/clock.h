//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#pragma once
#include <pebble.h>
#include "global.h"

#if PBL_DISPLAY_WIDTH == 200

#define CLOCK_POS_X 0
#define CLOCK_POS_Y 0

#else

#define CLOCK_POS_X 0
#define CLOCK_POS_Y 0

#endif

// #define SNOOZE_LAYER_FRAME ( GRect( 89, ( PBL_DISPLAY_HEIGHT / 2 ) - ( 20 / 2 ), 20, 20 ) )
#define SNOOZE_LAYER_FRAME ( GRect( ( PBL_DISPLAY_WIDTH / 2 ) - ( 20 / 2 ), 2, 20, 20 ) )

typedef struct {
  bool show_seconds;
} ANALOG_LAYER_DATA;

bool is_X_in_range( int a, int b, int x );
void draw_clock( void );
void clock_init( Window* window );
void clock_deinit( void );
