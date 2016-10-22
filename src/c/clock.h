#pragma once
#include <pebble.h>
#include "config.h"

#define CLOCK_POS_X 0
#define CLOCK_POS_Y 0
#define SEC_HAND_LENGTH 72
#define SEC_HAND_TAIL_LENGTH 28
#define SEC_HAND_TIP_LENGTH 16
#define MIN_HAND_LENGTH 64
#define HOUR_HAND_LENGTH 42
#define SEC_HAND_WIDTH 1
#define MIN_HAND_WIDTH 9
#define HOUR_HAND_WIDTH 13
#define CENTER_DOT_RADIUS 15

#define COLOUR_DOT              PBL_IF_COLOR_ELSE( GColorWhite, GColorWhite )
#define COLOUR_DOT_OUTLINE      PBL_IF_COLOR_ELSE( GColorBlack, GColorBlack )
#define COLOUR_HANDS_OUTLINE    PBL_IF_COLOR_ELSE( GColorBlack, GColorBlack )
#define COLOUR_HOUR_HAND        PBL_IF_COLOR_ELSE( GColorJaegerGreen /* GColorIslamicGreen */, GColorWhite )
#define COLOUR_MIN_HAND         PBL_IF_COLOR_ELSE( GColorChromeYellow /* GColorOrange */, GColorWhite )
#define COLOUR_SEC_HAND         PBL_IF_COLOR_ELSE( GColorWhite, GColorWhite )
#define COLOUR_SEC_HAND_TIP     PBL_IF_COLOR_ELSE( GColorDarkCandyAppleRed, GColorWhite )

struct ANALOG_LAYER_DATA {
  bool show_seconds;
};

struct HAND_DRAW_PARAMS {
  GContext *ctx;
  GPoint center_pt;
  GPoint from_pt;
  GPoint to_pt;
  uint8_t hand_width;
  GColor hand_color;
  GColor hand_outline_color;
  uint16_t dot_radius;
  GColor dot_color;
  GColor dot_outline_color;
};

bool is_X_in_range( int a, int b, int x );
void draw_clock( void );
void clock_init( Window* window );
void clock_deinit( void );
