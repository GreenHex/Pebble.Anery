#include <pebble.h>
#include "global.h"
#include "weather_icons.h"
// Weather icons from Forecast Font <http://forecastfont.iconvau.lt>.

#ifdef INCLUDE_WEATHER
#define ARRAY_SIZE(array) (sizeof(array)/sizeof(*array))

static GFont icon_font;

static void draw_glyph( GContext *ctx, GRect bounds, GLYPH glyph ) {
  APP_LOG( APP_LOG_LEVEL_INFO, "B-> %c %ld", glyph.glyph, glyph.colour );
  graphics_context_set_text_color( ctx, GColorFromHEX( glyph.colour ) );
  graphics_draw_text( ctx, &(glyph.glyph), icon_font, bounds, GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL );
}

void draw_icon( GContext *ctx, GRect bounds, uint32_t icon_id ) {
  GLYPHS mist1 = { .num_glyphs = 1, .glyphs = (GLYPH []) { g_mist } };
  GLYPHS windy1 = { .num_glyphs = 2, .glyphs = (GLYPH []) { g_cloud_open, g_wind } };
  
  GLYPH mist[] = { g_mist };
  GLYPH sunny[] = { g_sun };
  GLYPH moon[] = { g_moon };
  GLYPH cloudy[] = { g_cloud };
  GLYPH drizzle[] = { g_cloud_open, g_drizzle };
  GLYPH raining[] = { g_cloud_open, g_rain };
  GLYPH snowing[] = { g_cloud_open, g_snow };
  GLYPH showers[] = { g_cloud_open, g_shower };
  GLYPH thunder[] = { g_cloud_open, g_lightning };
  GLYPH frosty[] = { g_cloud_open, g_frost };
  GLYPH hail[] = { g_cloud_open, g_hail };
  GLYPH sleet[] = { g_cloud_open, g_sleet };
  GLYPH windy[] = { g_cloud_open, g_wind };
  GLYPH windy_raining[] = { g_cloud_windy_rain, g_raining_small };
  GLYPH windy_snowing[] = { g_cloud_windy_snow, g_snow_small };
  
  GLYPH *glyph = 0;
  glyph = windy_snowing;
  int num_glyphs = ARRAY_SIZE(glyph);
  
  // APP_LOG( APP_LOG_LEVEL_INFO, "K-> %d %d %d", sizeof(mist), sizeof(*mist), ARRAY_SIZE(mist) );
  // APP_LOG( APP_LOG_LEVEL_INFO, "L-> %d %d %d", sizeof(thunder), sizeof(*thunder), ARRAY_SIZE(thunder) );
  // APP_LOG( APP_LOG_LEVEL_INFO, "M-> %d %d %d", sizeof(glyph), sizeof(*glyph), ARRAY_SIZE(glyph) );
  // for( size_t i = 0; i < 2; i++ ) {
  //  APP_LOG( APP_LOG_LEVEL_INFO, "A-> %d %c %ld", i, glyph[i].glyph, glyph[i].colour );
  //  draw_glyph( ctx, bounds, glyph[i] );
  // }
  APP_LOG( APP_LOG_LEVEL_INFO, "mist1-> %d %d %d", sizeof(mist1), mist1.num_glyphs, sizeof(mist1.glyphs) );
  APP_LOG( APP_LOG_LEVEL_INFO, "windy1-> %d %d %d", sizeof(windy1), windy1.num_glyphs, sizeof(windy1.glyphs) );
  for( int i = 0; i < windy1.num_glyphs; i++ ) {
    APP_LOG( APP_LOG_LEVEL_INFO, "J-> %d %c %ld", i, windy1.glyphs[i].glyph, windy1.glyphs[i].colour );
    draw_glyph( ctx, bounds, windy1.glyphs[i] );
  }
}

void weather_icons_init( void ) {  
  icon_font = fonts_load_custom_font( resource_get_handle( RESOURCE_ID_FONT_FORECASTFONT_32 ) );
}

void weather_icons_deinit( void ) {
  fonts_unload_custom_font( icon_font );
}
  
#endif
