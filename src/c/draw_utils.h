//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#pragma once

///////
#if PBL_DISPLAY_WIDTH == 200
/////// 200 x 228
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
#define BACKGROUND_COLOUR       COLOUR_BG_BITMAP_BG
#define TICKS_COLOUR            GColorBlack

#define COLOUR_DOT              GColorWhite
#define COLOUR_DOT_OUTLINE      GColorBlack
#define COLOUR_HANDS_OUTLINE    GColorBlack
#define COLOUR_HOUR_HAND        PBL_IF_COLOR_ELSE( GColorJaegerGreen /* GColorIslamicGreen */, GColorWhite )
#define COLOUR_MIN_HAND         PBL_IF_COLOR_ELSE( GColorChromeYellow /* GColorOrange */, GColorWhite )
#define COLOUR_SEC_HAND         GColorWhite
#define COLOUR_SEC_HAND_TIP     PBL_IF_COLOR_ELSE( GColorRed /* GColorDarkCandyAppleRed */, GColorWhite )

#define CLOCK_DIAL_SIZE_W       PBL_DISPLAY_WIDTH 
#define CLOCK_DIAL_SIZE_H       PBL_DISPLAY_HEIGHT
#define CLOCK_DIAL_POS_X        0
#define CLOCK_DIAL_POS_Y        0
#define CLOCK_TICK_EDGE_OFFSET  3 /* make it an odd number */
#define CLOCK_TICK_HOUR_COLOUR  GColorWhite
#define CLOCK_TICK_MINUTE_COLOUR GColorDrakGray

static GPathInfo PATH_TICK = {
  2, (GPoint []) {
    { 0, - ( CLOCK_DIAL_SIZE_W > CLOCK_DIAL_SIZE_H ? CLOCK_DIAL_SIZE_W : CLOCK_DIAL_SIZE_H ) },
    { 0, ( CLOCK_DIAL_SIZE_W >  CLOCK_DIAL_SIZE_H ? CLOCK_DIAL_SIZE_W : CLOCK_DIAL_SIZE_H ) }
  }
};

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
    { -9, 0 },
    { 0, -46 },
    { 9, 0 }
  }
};

static const GPathInfo HOUR_HAND_SPIFFY_GS_POINTS_HIGHLIGHT = {
  3, (GPoint []) {
    { 0, 18 },
    { -9, 0 },
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
  Layer *layer;
  GContext *ctx;
  GPathInfo *p_gpath_info;
  int increment;
  int tick_thk;
  int tick_length;
  GColor tick_colour;
  GColor bg_colour;
} DRAW_TICKS_PARAMS;

typedef struct {
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
} HAND_DRAW_PARAMS;

typedef struct {
  GContext *ctx;
  GPoint center_pt;
  uint32_t hour_angle;
  uint32_t min_angle;
  GPath *s_hour_hand;
  GPath *s_hour_hand_highlight;
  GPath *s_min_hand;
  GPath *s_min_hand_highlight;
  GColor hour_hand_colour;
  GColor hour_hand_highlight_colour;
  GColor min_hand_colour;
  GColor min_hand_highlight_colour;
  GColor hand_outline_color;
  bool show_seconds;
} GPATH_HANDS_PARAMS;

typedef struct {
  GContext *ctx;
  GPoint center_pt;
  uint32_t gmt_angle;
  uint32_t hour_angle;
  uint32_t min_angle;
  uint32_t sec_angle;
  uint32_t sec_tail_angle;
  bool show_seconds;
} DRAW_CLOCK_PARAMS;

void draw_seconds_ticks( DRAW_TICKS_PARAMS *pDTP );
void draw_cont_clock_hands( DRAW_CLOCK_PARAMS *pCP );
void draw_spiffy_gs_clock_hands( DRAW_CLOCK_PARAMS *pCP );
void draw_sbge001_clock_hands( DRAW_CLOCK_PARAMS *pCP );
void gpaths_init( void );
void gpaths_deinit( void );
