//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//
// Weather icons from Forecast Font <http://forecastfont.iconvau.lt>.
//

#pragma once
#include <pebble.h>
#include "global.h"
#include "pbl_64_hex_colours.h"

#ifdef INCLUDE_WEATHER

typedef struct {
  char glyph;
  uint32_t colour;
} GLYPH;

typedef struct {
  int num_glyphs;
  GLYPH *glyphs;
} WEATHER_ICON;

static const GLYPH g_mist = { 'I', LightGray };
static const GLYPH g_sun = { 'T', ChromeYellow };
static const GLYPH g_moon = { 'N', White };
static const GLYPH g_wind = { 'V', BabyBlueEyes };
static const GLYPH g_cloud = { 'G', LightGray };
static const GLYPH g_lightning = { 'U', Yellow };
static const GLYPH g_cloud_open = { 'F', LightGray };
static const GLYPH g_moon_peek = { 'A', Brass };
static const GLYPH g_sun_peek = { 'B', ChromeYellow };
static const GLYPH g_cloud_windy_snow = { 'J', LightGray };
static const GLYPH g_cloud_windy_rain = { 'R', LightGray };
static const GLYPH g_snow_small = { 'D', VividCerulean };
static const GLYPH g_snow = { 'L', VividCerulean };
static const GLYPH g_frost = { 'C', VividCerulean };
static const GLYPH g_sleet = { 'M', VividCerulean };
static const GLYPH g_hail = { 'P', VividCerulean };
static const GLYPH g_raining_small = { 'O', Blue };
static const GLYPH g_drizzle = { 'K', Blue };
static const GLYPH g_rain = { 'H', Blue };
static const GLYPH g_shower = { 'E', Blue };
static const GLYPH g_cloud_closed = { 'W', LightGray };
static const GLYPH g_dust = { 'X', ArmyGreen };

void draw_icon( GContext *ctx, GRect bounds, int32_t icon_id, bool is_day_not_night );
void weather_icons_init( void );
void weather_icons_deinit( void );

#endif