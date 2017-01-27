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

static const GLYPH g_mist = { 'I', PBL_IF_COLOR_ELSE( LightGray, White ) };
static const GLYPH g_sun = { 'T', PBL_IF_COLOR_ELSE( ChromeYellow, White ) };
static const GLYPH g_moon = { 'N', PBL_IF_COLOR_ELSE( White, White ) };
static const GLYPH g_wind = { 'V', PBL_IF_COLOR_ELSE( BabyBlueEyes, White ) };
static const GLYPH g_cloud = { 'G', PBL_IF_COLOR_ELSE( LightGray, White ) };
static const GLYPH g_lightning = { 'U', PBL_IF_COLOR_ELSE( Yellow, White ) };
static const GLYPH g_cloud_open = { 'F', PBL_IF_COLOR_ELSE( LightGray, White ) };
static const GLYPH g_moon_peek = { 'A', PBL_IF_COLOR_ELSE( White, White ) };
static const GLYPH g_sun_peek = { 'B', PBL_IF_COLOR_ELSE( ChromeYellow, White ) };
static const GLYPH g_cloud_windy_snow = { 'J', PBL_IF_COLOR_ELSE( LightGray, White ) };
static const GLYPH g_cloud_windy_rain = { 'R', PBL_IF_COLOR_ELSE( LightGray, White ) };
static const GLYPH g_snow_small = { 'D', PBL_IF_COLOR_ELSE( VividCerulean, White ) };
static const GLYPH g_snow = { 'L', PBL_IF_COLOR_ELSE( VividCerulean, White ) };
static const GLYPH g_frost = { 'C', PBL_IF_COLOR_ELSE( VividCerulean, White ) };
static const GLYPH g_sleet = { 'M', PBL_IF_COLOR_ELSE( VividCerulean, White ) };
static const GLYPH g_hail = { 'P', PBL_IF_COLOR_ELSE( VividCerulean, White ) };
static const GLYPH g_raining_small = { 'O', PBL_IF_COLOR_ELSE( Blue, White ) };
static const GLYPH g_drizzle = { 'K', PBL_IF_COLOR_ELSE( Blue, White ) };
static const GLYPH g_rain = { 'H', PBL_IF_COLOR_ELSE( Blue, White ) };
static const GLYPH g_shower = { 'E', PBL_IF_COLOR_ELSE( Blue, White ) };
static const GLYPH g_cloud_closed = { 'W', PBL_IF_COLOR_ELSE( LightGray, White ) };
static const GLYPH g_dust = { 'X', PBL_IF_COLOR_ELSE( ArmyGreen, White ) };

void draw_icon( GContext *ctx, GRect bounds, int32_t icon_id, bool is_day_not_night );
void weather_icons_init( void );
void weather_icons_deinit( void );

#endif