#pragma once
#include <pebble.h>
#include "global.h"

///////
#if PBL_DISPLAY_WIDTH == 200
/////// 200 x 228
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
#define HOUR_CENTER_DOT_RADIUS 13
#define MIN_CENTER_DOT_RADIUS ( HOUR_CENTER_DOT_RADIUS - 4 )
#define SEC_CENTER_DOT_RADIUS ( HOUR_CENTER_DOT_RADIUS - 8 )
///////
#else
/////// 144 x 168
#define CLOCK_POS_X 0
#define CLOCK_POS_Y 0
#define SEC_HAND_LENGTH 55 /* 53 */
#define SEC_HAND_TAIL_LENGTH 20
#define SEC_HAND_TIP_LENGTH 12
#define MIN_HAND_LENGTH 50
#define HOUR_HAND_LENGTH 35
#define SEC_HAND_WIDTH 1
#define MIN_HAND_WIDTH 4
#define HOUR_HAND_WIDTH 6
#define HOUR_CENTER_DOT_RADIUS 10
#define MIN_CENTER_DOT_RADIUS ( HOUR_CENTER_DOT_RADIUS - 3 )
#define SEC_CENTER_DOT_RADIUS ( HOUR_CENTER_DOT_RADIUS - 6 )
///////
#endif
///////

#define COLOUR_BG_BITMAP_BG     GColorBlack

#define COLOUR_DOT              GColorWhite
#define COLOUR_DOT_OUTLINE      GColorBlack
#define COLOUR_HANDS_OUTLINE    GColorBlack
#define COLOUR_HOUR_HAND        PBL_IF_COLOR_ELSE( GColorJaegerGreen /* GColorIslamicGreen */, GColorWhite )
#define COLOUR_MIN_HAND         PBL_IF_COLOR_ELSE( GColorChromeYellow /* GColorOrange */, GColorWhite )
#define COLOUR_SEC_HAND         GColorWhite
#define COLOUR_SEC_HAND_TIP     PBL_IF_COLOR_ELSE( GColorRed /* GColorDarkCandyAppleRed */, GColorWhite )

///////
#if PBL_DISPLAY_WIDTH == 200
/////// 200 x 228

static const GPathInfo HOUR_HAND_SPIFFY_GS_POINTS = {
  4, (GPoint []) {
    { 0, 25 },
    { -9, 0 },
    { 0, -48 },
    { 9, 0 }
  }
};

static const GPathInfo HOUR_HAND_SPIFFY_GS_POINTS_HIGHLIGHT = {
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

static const GPathInfo MINUTE_HAND_SPIFFY_GS_POINTS_HIGHLIGHT = {
  3, (GPoint []) {
    { 0, 25 },
    { 8, 0 },
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

static const GPathInfo HOUR_HAND_SBGE001_POINTS_HIGHLIGHT = {
  6, (GPoint []) {
    { -1, 18 },
    { -8, 0 },
    { -1, -46 },
    { 1, -46 },
    { 8, 0 },
    { 1, 18 }
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

static const GPathInfo MINUTE_HAND_SBGE001_POINTS_HIGHLIGHT = {
  6, (GPoint []) {
    { -1, 18 },
    { -6, 0 },
    { -1, -72 },
    { 1, -72 },
    { 6, 0 },
    { 1, 18 }
  }
};

static const GPathInfo GMT_HAND = {
  7, (GPoint []) {
    { -4, 0 },
    { -2, -64 },
    { -8, -64 },
    { 0, -84 },
    { 8, -64 },
    { 2, -64 },
    { 4,  0 }
  }
};

static const GPathInfo GMT_HAND_INLAY = {
  3, (GPoint []) {
    { -5, -67 },
    { 0, -80 },
    { 5, -67 },
  }
};

///////
#else
/////// 144 x 168

static const GPathInfo HOUR_HAND_SPIFFY_GS_POINTS = {
  4, (GPoint []) {
    { 0, 18 },
    { -8, 0 },
    { 0, -46 },
    { 8, 0 }
  }
};

static const GPathInfo HOUR_HAND_SPIFFY_GS_POINTS_HIGHLIGHT = {
  3, (GPoint []) {
    { 0, 18 },
    { -8, 0 },
    { 0, -46 }
  }
};

static const GPathInfo MINUTE_HAND_SPIFFY_GS_POINTS = {
  4, (GPoint []) {
    { 0, 18 },
    { -7, 0 },
    { 0, -68 },
    { 7, 0 },
  }
};

static const GPathInfo MINUTE_HAND_SPIFFY_GS_POINTS_HIGHLIGHT = {
  3, (GPoint []) {
    { 0, 18 },
    { 7, 0 },
    { 0, -68 },
  }
};

static const GPathInfo HOUR_HAND_SBGE001_POINTS = {
  6, (GPoint []) {
    { -3, 14 },
    { -8, 0 },
    { -2, -38 },
    { 2, -38 },
    { 8, 0 },
    { 3, 14 }
  }
};

static const GPathInfo HOUR_HAND_SBGE001_POINTS_HIGHLIGHT = {
  6, (GPoint []) {
    { -1, 12 },
    { -6, 0 },
    { 0, -36 },
    { 0, -36 },
    { 6, 0 },
    { 1, 12 }
  }
};

static const GPathInfo MINUTE_HAND_SBGE001_POINTS = {
  6, (GPoint []) {
    { -3, 14 },
    { -7, 0 },
    { -2, -60 },
    { 2, -60 },
    { 7, 0 },
    { 3, 14 }
  }
};

static const GPathInfo MINUTE_HAND_SBGE001_POINTS_HIGHLIGHT = {
  6, (GPoint []) {
    { -1, 12 },
    { -5, 0 },
    { -0, -58 },
    { 0, -58 },
    { 5, 0 },
    { 1, 12 }
  }
};

static const GPathInfo GMT_HAND = {
  7, (GPoint []) {
    { -3, 0 },
    { -2, -44 },
    { -6, -44 },
    { 0, -59 },
    { 6, -44 },
    { 2, -44 },
    { 3,  0 }
  }
};

static const GPathInfo GMT_HAND_INLAY = {
  3, (GPoint []) {
    { -4, -47 },
    { 0, -56 },
    { 4, -47 },
  }
};

///////
#endif
///////

typedef struct {
  bool show_seconds;
} ANALOG_LAYER_DATA;

bool is_X_in_range( int a, int b, int x );
void draw_clock( void );
void clock_init( Window* window );
void clock_deinit( void );
