#pragma once
#include <pebble.h>
// Weather icons from Forecast Font <http://forecastfont.iconvau.lt>.

#ifdef INCLUDE_WEATHER

typedef struct {
  char str[2];
  uint32_t colour;
} GLYPH;

typedef struct {
  int num_glyphs;
  GLYPH *glyphs;
} WEATHER_ICON;

static const GLYPH g_mist = { "I", 0xAAAAAA };
static const GLYPH g_sun = { "T", 0xFFAA00 };
static const GLYPH g_moon = { "N", 0xFFAA00 };
static const GLYPH g_wind = { "V", 0x00AAFF };
static const GLYPH g_cloud = { "G", 0xAAAAAA };
static const GLYPH g_lightning = { "U", 0xFFAA00 };
static const GLYPH g_cloud_open = { "F", 0xAAAAAA };
static const GLYPH g_moon_peek = { "A", 0xFFAA00 };
static const GLYPH g_sun_peek = { "B", 0xFFAA00 };
static const GLYPH g_cloud_windy_snow = { "J", 0xAAAAAA };
static const GLYPH g_cloud_windy_rain = { "R", 0xAAAAAA };
static const GLYPH g_snow_small = { "D", 0x00AAFF };
static const GLYPH g_snow = { "L", 0x00AAFF };
static const GLYPH g_frost = { "C", 0x00AAFF };
static const GLYPH g_sleet = { "M", 0x00AAFF };
static const GLYPH g_hail = { "P", 0xAAAAAA };
static const GLYPH g_raining_small = { "O", 0x00AAFF };
static const GLYPH g_drizzle = { "K", 0x00AAFF };
static const GLYPH g_rain = { "H", 0x00AAFF };
static const GLYPH g_shower = { "E", 0x00AAFF };
//
static WEATHER_ICON mist;
static WEATHER_ICON sunny;
static WEATHER_ICON moon;
static WEATHER_ICON cloudy;
static WEATHER_ICON drizzle;
static WEATHER_ICON raining;
static WEATHER_ICON snowing;
static WEATHER_ICON showers;
static WEATHER_ICON thunder;
static WEATHER_ICON frosty;
static WEATHER_ICON hail;
static WEATHER_ICON sleet;
static WEATHER_ICON windy;
static WEATHER_ICON windy_raining;
static WEATHER_ICON windy_snowing;

void draw_icon( GContext *ctx, GRect bounds, WEATHER_ICON icon );
void weather_icons_init( void );
void weather_icons_deinit( void );

#endif