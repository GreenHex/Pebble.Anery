#include <pebble.h>
#include "global.h"
#include "weather_icons.h"
// Weather icons from Forecast Font <http://forecastfont.iconvau.lt>.

#ifdef INCLUDE_WEATHER

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

static GFont icon_font;

void draw_glyph( GContext *ctx, GRect bounds, GLYPH glyph ) {
  graphics_context_set_text_color( ctx, GColorFromHEX( glyph.colour ) );
  graphics_draw_text( ctx, &(glyph.glyph), icon_font, bounds, GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL );
}

void draw_icon( GContext *ctx, GRect bounds, WEATHER_ICON icon ) {
  APP_LOG( APP_LOG_LEVEL_INFO, "%d %c", drizzle.num_glyphs, drizzle.glyphs[0].glyph );
  for( int i = 0; i < icon.num_glyphs; i++ ) {
    APP_LOG( APP_LOG_LEVEL_INFO, "%d %c", i, drizzle.glyphs[i].glyph );
    draw_glyph( ctx, bounds, drizzle.glyphs[i] );
  }
}

void weather_icons_init( void ) {
  mist = (WEATHER_ICON) { 
    .num_glyphs = 1,
    .glyphs = (GLYPH []) { g_mist }
  };
  sunny = (WEATHER_ICON) {
    .num_glyphs = 1,
    .glyphs = (GLYPH []) { g_sun } 
  };
  moon = (WEATHER_ICON) {
    .num_glyphs = 1,
    .glyphs = (GLYPH []) { g_moon } 
  };
  cloudy = (WEATHER_ICON) {
    .num_glyphs = 1,
    .glyphs = (GLYPH []) { g_cloud } 
  };
  drizzle = (WEATHER_ICON) {
    .num_glyphs = 2,
    .glyphs = (GLYPH []) { g_cloud, g_drizzle } 
  };
  raining = (WEATHER_ICON) {
    .num_glyphs = 2,
    .glyphs = (GLYPH []) { g_cloud, g_rain } 
  };
  snowing = (WEATHER_ICON) {
    .num_glyphs = 2,
    .glyphs = (GLYPH []) { g_cloud, g_snow } 
  };
  showers = (WEATHER_ICON) {
    .num_glyphs = 2,
    .glyphs = (GLYPH []) { g_cloud, g_shower } 
  };
  thunder = (WEATHER_ICON) {
    .num_glyphs = 2,
    .glyphs = (GLYPH []) { g_cloud, g_lightning } 
  };
  frosty = (WEATHER_ICON) {
    .num_glyphs = 2,
    .glyphs = (GLYPH []) { g_cloud, g_frost } 
  };
  hail = (WEATHER_ICON) {
    .num_glyphs = 2,
    .glyphs = (GLYPH []) { g_cloud, g_hail } 
  };
  sleet = (WEATHER_ICON) {
    .num_glyphs = 2,
    .glyphs = (GLYPH []) { g_cloud, g_sleet } 
  };
  windy = (WEATHER_ICON) {
    .num_glyphs = 2,
    .glyphs = (GLYPH []) { g_cloud, g_wind } 
  };
  windy_raining = (WEATHER_ICON) {
    .num_glyphs = 2,
    .glyphs = (GLYPH []) { g_cloud_windy_rain, g_raining_small } 
  };
  windy_snowing = (WEATHER_ICON) {
    .num_glyphs = 2,
    .glyphs = (GLYPH []) { g_cloud_windy_snow, g_snow_small } 
  };
  
  icon_font = fonts_load_custom_font( resource_get_handle( RESOURCE_ID_FONT_FORECASTFONT_32 ) );
}

void weather_icons_deinit( void ) {
  fonts_unload_custom_font( icon_font );
}
  
#endif
