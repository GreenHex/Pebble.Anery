#pragma once
#include <pebble.h>
#include "config.h"

#define CLOCK_POS_X 0
#define CLOCK_POS_Y 0
#define SEC_HAND_LENGTH 73
#define SEC_HAND_TAIL_LENGTH 28
#define SEC_HAND_TIP_LENGTH 16
#define MIN_HAND_LENGTH 64
#define HOUR_HAND_LENGTH 42
#define SEC_HAND_WIDTH 1
#define MIN_HAND_WIDTH 9
#define HOUR_HAND_WIDTH 13
#define CENTER_DOT_RADIUS 13
#define DATE_WINDOW_WIDTH 34
#define DATE_WINDOW_HEIGHT 28
#define DATE_WINDOW_OUTLINE_THK 2

#define COLOUR_DOT              PBL_IF_COLOR_ELSE( GColorWhite, GColorWhite )
#define COLOUR_DOT_OUTLINE      PBL_IF_COLOR_ELSE( GColorBlack, GColorBlack )
#define COLOUR_HANDS_OUTLINE    PBL_IF_COLOR_ELSE( GColorBlack, GColorBlack )
#define COLOUR_HOUR_HAND        PBL_IF_COLOR_ELSE( GColorJaegerGreen /* GColorIslamicGreen */, GColorWhite )
#define COLOUR_MIN_HAND         PBL_IF_COLOR_ELSE( GColorChromeYellow /* GColorOrange */, GColorWhite )
#define COLOUR_SEC_HAND         PBL_IF_COLOR_ELSE( GColorWhite, GColorWhite )
#define COLOUR_SEC_HAND_TIP     PBL_IF_COLOR_ELSE( GColorDarkCandyAppleRed, GColorWhite )

static const GPathInfo HOUR_HAND_SPIFFY_GS_POINTS = {
  4, (GPoint []) {
    { 0, 25 },
    { -9, 0 },
    { 0, -48 },
    { 9, 0 }
  }
};

static const GPathInfo HOUR_HAND_SPIFFY_GS_POINTS_LEFT = {
  3, (GPoint []) {
    { 0, 25 },
    { -9, 0 },
    { 0, -48 }
  }
};

static const GPathInfo MINUTE_HAND_SPIFFY_GS_POINTS = {
  4, (GPoint []) {
    { 0, 25 },
    { -8, 0 },
    { 0, -78 },
    { 8, 0 },
  }
};

static const GPathInfo MINUTE_HAND_SPIFFY_GS_POINTS_LEFT = {
  3, (GPoint []) {
    { 0, 25 },
    { -8, 0 },
    { 0, -78 },
  }
};

static const GPathInfo HOUR_HAND_SBGE001_POINTS = {
  6, (GPoint []) {
    { -3, 20 },
    { -10, 0 },
    { -3, -48 },
    { 3, -48 },
    { 10, 0 },
    { 3, 20 }
  }
};

static const GPathInfo HOUR_HAND_SBGE001_POINTS_LEFT = {
  5, (GPoint []) {
    { 0, 20 },
    { -3, 20 },
    { -10, 0 },
    { -3, -48 },
    { 0, -48 }
  }
};

static const GPathInfo MINUTE_HAND_SBGE001_POINTS = {
  6, (GPoint []) {
    { -3, 20 },
    { -8, 0 },
    { -3, -74 },
    { 3, -74 },
    { 8, 0 },
    { 3, 20 }
  }
};

static const GPathInfo MINUTE_HAND_SBGE001_POINTS_LEFT = {
  5, (GPoint []) {
    { 0, 20 },
    { -3, 20 },
    { -8, 0 },
    { -3, -74 },
    { 0, -74 }
  }
};

static const GPathInfo GMT_HAND = {
  7, (GPoint []) {
    { -4, 0 },
    { -2, -56 },
    { -8, -56 },
    { 0, -76 },
    { 8, -56 },
    { 2, -56 },
    { 4,  0 }
  }
};

static const GPathInfo GMT_HAND_INLAY = {
  3, (GPoint []) {
    { -5, -59 },
    { 0, -73 },
    { 5, -59 },
  }
};

enum ANALOG_HANDS_STYLE {
  STYLE_CONTEMPORARY = 0,
  STYLE_SPIFFY_GS = 1,
  STYLE_SBGE001 = 2
};

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

struct GPATH_HANDS_PARAMS {
  GContext *ctx;
  GPoint center_pt;
  uint32_t hour_angle;
  uint32_t min_angle;
  GPath *s_hour_arrow;
  GPath *s_hour_arrow_left;
  GPath *s_min_arrow;
  GPath *s_min_arrow_left;
  GColor hand_outline_color;
};

bool is_X_in_range( int a, int b, int x );
void draw_clock( void );
void clock_init( Window* window );
void clock_deinit( void );
