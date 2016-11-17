//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#pragma once
#include <pebble.h>

#define DEBUG 0
// #define SECONDS_ALWAYS_ON
// #define INCLUDE_HR
#define INCLUDE_WEATHER
#define GARNISH_HOLIDAYS

enum ANALOG_HANDS_STYLE {
  STYLE_CONTEMPORARY = 0,
  STYLE_SPIFFY_GS = 1,
  STYLE_SBGE001 = 2
};

typedef struct {
  char temp_str[8];
  uint32_t icon_id;
} WEATHER_DATA;

tm tm_time;
tm tm_gmt;
