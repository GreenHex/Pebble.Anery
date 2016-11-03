#pragma once
#include <pebble.h>
#include "config.h"

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
#define DATE_WINDOW_WIDTH 34
#define DATE_WINDOW_HEIGHT 28
#define DATE_LEFT_GAP 2
#define DATE_WINDOW_OUTLINE_THK 2
#define CONT_BATT_GAUGE_INT_RADIUS 16
#define CONT_BATT_GAUGE_EXT_RADIUS 20
#define CONT_BATT_GAUGE_SIZE ( CONT_BATT_GAUGE_EXT_RADIUS * 2 + 2 )
#define SBGE001_BATT_GAUGE_LOC_X 33
#define SBGE001_BATT_GAUGE_LOC_Y 132
#define SBGE001_BATT_GAUGE_SIZE 71
#define SBGE001_BATT_GAUGE_DOT_RADIUS 4
#define MOSER_BATT_GAUGE_SIZE_W 55
#define MOSER_BATT_GAUGE_SIZE_H 45
#define MOSER_BATT_GAUGE_DOT_RADIUS 3
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
#define DATE_WINDOW_WIDTH 34
#define DATE_WINDOW_HEIGHT 28
#define DATE_LEFT_GAP 2
#define DATE_WINDOW_OUTLINE_THK 2
#define CONT_BATT_GAUGE_INT_RADIUS 12
#define CONT_BATT_GAUGE_EXT_RADIUS 15
#define CONT_BATT_GAUGE_SIZE ( CONT_BATT_GAUGE_EXT_RADIUS * 2 + 2 )
#define SBGE001_BATT_GAUGE_LOC_X 24
#define SBGE001_BATT_GAUGE_LOC_Y 95
#define SBGE001_BATT_GAUGE_SIZE 51
#define SBGE001_BATT_GAUGE_DOT_RADIUS 3
#define MOSER_BATT_GAUGE_SIZE_W 40
#define MOSER_BATT_GAUGE_SIZE_H 32
#define MOSER_BATT_GAUGE_DOT_RADIUS 2
///////
#endif
///////

#define COLOUR_BATT_15          GColorRed
#define COLOUR_BATT_95          GColorDarkGreen
#define COLOUR_BATT_100         GColorMayGreen

#define COLOUR_DOT              PBL_IF_COLOR_ELSE( GColorWhite, GColorWhite )
#define COLOUR_DOT_OUTLINE      PBL_IF_COLOR_ELSE( GColorBlack, GColorBlack )
#define COLOUR_HANDS_OUTLINE    PBL_IF_COLOR_ELSE( GColorBlack, GColorBlack )
#define COLOUR_HOUR_HAND        PBL_IF_COLOR_ELSE( GColorJaegerGreen /* GColorIslamicGreen */, GColorWhite )
#define COLOUR_MIN_HAND         PBL_IF_COLOR_ELSE( GColorChromeYellow /* GColorOrange */, GColorWhite )
#define COLOUR_SEC_HAND         PBL_IF_COLOR_ELSE( GColorWhite, GColorWhite )
#define COLOUR_SEC_HAND_TIP     PBL_IF_COLOR_ELSE( GColorDarkCandyAppleRed, GColorWhite )

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
    { -2, -60 },
    { -8, -60 },
    { 0, -80 },
    { 8, -60 },
    { 2, -60 },
    { 4,  0 }
  }
};

static const GPathInfo GMT_HAND_INLAY = {
  3, (GPoint []) {
    { -5, -63 },
    { 0, -76 },
    { 5, -63 },
  }
};

static const GPathInfo MOSER_BATT_GAUGE_HAND = {
  5, (GPoint []) {
    { -1, 0 },
    { -2, -19 },
    { 0, -38 },
    { 2, -19 },
    { 1, 0 }
  }
};

static const GPathInfo SBGE001_BATT_GAUGE_HAND = {
  3, (GPoint []) {
    { -3, 0 },
    { 0, -24 },
    { 3, 0 }
  }
};

///////
#else
///////

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
    { -2, -39 },
    { -6, -39 },
    { 0, -54 },
    { 6, -39 },
    { 2, -39 },
    { 3,  0 }
  }
};

static const GPathInfo GMT_HAND_INLAY = {
  3, (GPoint []) {
    { -4, -41 },
    { 0, -51 },
    { 4, -41 },
  }
};

static const GPathInfo MOSER_BATT_GAUGE_HAND = {
  5, (GPoint []) {
    { -1, 0 },
    { -2, -13 },
    { 0, -27 },
    { 2, -13 },
    { 1, 0 }
  }
};

static const GPathInfo SBGE001_BATT_GAUGE_HAND = {
  3, (GPoint []) {
    { -2, 0 },
    { 0, -17 },
    { 2, 0 }
  }
};

///////
#endif
///////

enum ANALOG_HANDS_STYLE {
  STYLE_CONTEMPORARY = 0,
  STYLE_SPIFFY_GS = 1,
  STYLE_SBGE001 = 2
};

typedef struct {
  bool show_seconds;
} ANALOG_LAYER_DATA;

#ifdef GARNISH_HOLIDAYS
typedef struct {
  int date;
  int month; // use normal month, not unix month
  uint32_t iconID;
} HOLIDAY;
#endif

bool is_X_in_range( int a, int b, int x );
void draw_clock( void );
void clock_init( Window* window );
void clock_deinit( void );
