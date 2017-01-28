//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#pragma once
#include <pebble.h>
#include "global.h"

#define CLOCK_POS_X 0
#define CLOCK_POS_Y 0

#define DIGIT_RECT_SIZE_W 11
#define DIGIT_RECT_SIZE_H 11

#define DIGIT_INCR_X1 22
#define DIGIT_INCR_Y1 67
#define DIGIT_INCR_Y2 19

#define DIGIT_TOP_BOTTOM_GAP 0
#define DIGIT_LEFT_RIGHT_GAP 0

#define DIGIT_TXT_VERT_ADJ 0

#define NUM_DIGITS 12

static const GPathInfo DIGIT_LOCATIONS = {
  NUM_DIGITS, (GPoint []) {
    /* 1 */ { PBL_DISPLAY_WIDTH/2 + DIGIT_INCR_X1 - DIGIT_RECT_SIZE_W/2, DIGIT_TOP_BOTTOM_GAP },
    /* 3 */ { PBL_DISPLAY_WIDTH - DIGIT_RECT_SIZE_W - DIGIT_LEFT_RIGHT_GAP, PBL_DISPLAY_HEIGHT/2 - DIGIT_INCR_Y1 - DIGIT_RECT_SIZE_H/2 },
    /* 5 */ { PBL_DISPLAY_WIDTH - DIGIT_RECT_SIZE_W - DIGIT_LEFT_RIGHT_GAP, PBL_DISPLAY_HEIGHT/2 - DIGIT_INCR_Y2 - DIGIT_RECT_SIZE_H/2 },
    /* 7 */ { PBL_DISPLAY_WIDTH - DIGIT_RECT_SIZE_W - DIGIT_LEFT_RIGHT_GAP, PBL_DISPLAY_HEIGHT/2 + DIGIT_INCR_Y2 - DIGIT_RECT_SIZE_H/2 },
    /* 9 */ { PBL_DISPLAY_WIDTH - DIGIT_RECT_SIZE_W - DIGIT_LEFT_RIGHT_GAP, PBL_DISPLAY_HEIGHT/2 + DIGIT_INCR_Y1 - DIGIT_RECT_SIZE_H/2 },
    /* 11 */ { PBL_DISPLAY_WIDTH/2 + DIGIT_INCR_X1 - DIGIT_RECT_SIZE_W/2, PBL_DISPLAY_HEIGHT - DIGIT_RECT_SIZE_H - DIGIT_TOP_BOTTOM_GAP },
    /* 13 */ { PBL_DISPLAY_WIDTH/2 - DIGIT_INCR_X1 - DIGIT_RECT_SIZE_W/2, PBL_DISPLAY_HEIGHT - DIGIT_RECT_SIZE_H - DIGIT_TOP_BOTTOM_GAP },
    /* 15 */ { DIGIT_LEFT_RIGHT_GAP, PBL_DISPLAY_HEIGHT/2 + DIGIT_INCR_Y1 - DIGIT_RECT_SIZE_H/2 },
    /* 17 */ { DIGIT_LEFT_RIGHT_GAP, PBL_DISPLAY_HEIGHT/2 + DIGIT_INCR_Y2 - DIGIT_RECT_SIZE_H/2 },
    /* 19 */ { DIGIT_LEFT_RIGHT_GAP, PBL_DISPLAY_HEIGHT/2 - DIGIT_INCR_Y2 - DIGIT_RECT_SIZE_H/2 },
    /* 21 */ { DIGIT_LEFT_RIGHT_GAP, PBL_DISPLAY_HEIGHT/2 - DIGIT_INCR_Y1 - DIGIT_RECT_SIZE_H/2 },
    /* 23 */ { PBL_DISPLAY_WIDTH/2 - DIGIT_INCR_X1 - DIGIT_RECT_SIZE_W/2, DIGIT_TOP_BOTTOM_GAP }
  }
};

// #define SNOOZE_LAYER_FRAME ( GRect( 89, ( PBL_DISPLAY_HEIGHT / 2 ) - ( 20 / 2 ), 20, 20 ) )
#define SNOOZE_LAYER_FRAME ( GRect( ( PBL_DISPLAY_WIDTH / 2 ) - ( 20 / 2 ), 2, 20, 20 ) )

typedef struct {
  bool show_seconds;
} ANALOG_LAYER_DATA;

typedef struct {
  uint8_t digit;
  GColor colour;
  GTextAlignment text_alignment;
} DIGIT_LAYER_DATA_GMT;

bool is_X_in_range( int a, int b, int x );
void draw_clock( void );
void clock_init( Window* window );
void clock_deinit( void );
