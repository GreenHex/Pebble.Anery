#pragma once
#include <pebble.h>
#include "global.h"

///////
#if PBL_DISPLAY_WIDTH == 200
/////// 200 x 228
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

///////
#if PBL_DISPLAY_WIDTH == 200
/////// 200 x 228

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

typedef struct {
  GContext *ctx;
  uint32_t batt_angle;
  GPoint center_pt;
  GPath *s_hand;
  GColor hand_colour;
  GColor hand_outline_colour;
  uint16_t dot_radius;
  BatteryChargeState charge_state;
} BATTERY_HAND_DRAW_PARAMS;

void battery_init( Layer *parent_layer );
void battery_deinit( void );