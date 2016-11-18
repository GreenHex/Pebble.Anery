//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//
// Weather icons from Forecast Font <http://forecastfont.iconvau.lt>.
//

#include <pebble.h>
#include "global.h"
#include "weather_icons.h"
#include "pbl_64_hex_colours.h"

#ifdef INCLUDE_WEATHER

static GFont icon_font;

static void draw_glyph( GContext *ctx, GRect bounds, GLYPH glyph ) {
  #ifdef DEBUG
  APP_LOG( APP_LOG_LEVEL_INFO, "B-> %c %ld", glyph.glyph, glyph.colour );
  #endif
  graphics_context_set_text_color( ctx, GColorFromHEX( glyph.colour ) );
  graphics_draw_text( ctx, &(glyph.glyph), icon_font, bounds, GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL );
}

void draw_icon( GContext *ctx, GRect bounds, int32_t icon_id, bool is_day_not_night ) {
  
  WEATHER_ICON *p_glyphs = 0;
  WEATHER_ICON *p_glyph_peek = 0;
  
  // This is here because 'C', and "glyphs" array is variable length.
  WEATHER_ICON mist = { .num_glyphs = 1, .glyphs = (GLYPH []) { g_mist } };
  WEATHER_ICON sun = { .num_glyphs = 1, .glyphs = (GLYPH []) { g_sun } };
  WEATHER_ICON moon = { .num_glyphs = 1, .glyphs = (GLYPH []) { g_moon } };
  WEATHER_ICON sun_peek = { .num_glyphs = 1, .glyphs = (GLYPH []) { g_sun_peek } };
  WEATHER_ICON moon_peek = { .num_glyphs = 1, .glyphs = (GLYPH []) { g_moon_peek } };
  WEATHER_ICON cloudy = { .num_glyphs = 1, .glyphs = (GLYPH []) { g_cloud_closed } };
  WEATHER_ICON drizzle = { .num_glyphs = 2, .glyphs = (GLYPH []) { g_cloud_open, g_drizzle } };
  WEATHER_ICON raining = { .num_glyphs = 2, .glyphs = (GLYPH []) { g_cloud_open, g_rain } };
  WEATHER_ICON snowing = { .num_glyphs = 2, .glyphs = (GLYPH []) { g_cloud_open, g_snow } };
  WEATHER_ICON showers = { .num_glyphs = 2, .glyphs = (GLYPH []) { g_cloud_open, g_shower } };
  WEATHER_ICON thunder = { .num_glyphs = 2, .glyphs = (GLYPH []) { g_cloud_open, g_lightning } };
  WEATHER_ICON frosty = { .num_glyphs = 2, .glyphs = (GLYPH []) { g_cloud_open, g_frost } };
  WEATHER_ICON hail = { .num_glyphs = 2, .glyphs = (GLYPH []) { g_cloud_open, g_hail } };
  WEATHER_ICON sleet = { .num_glyphs = 2, .glyphs = (GLYPH []) { g_cloud_open, g_sleet } };
  WEATHER_ICON windy = { .num_glyphs = 2, .glyphs = (GLYPH []) { g_cloud_open, g_wind } };
  WEATHER_ICON windy_raining = { .num_glyphs = 2, .glyphs = (GLYPH []) { g_cloud_windy_rain, g_raining_small } };
  WEATHER_ICON windy_snowing = { .num_glyphs = 2, .glyphs = (GLYPH []) { g_cloud_windy_snow, g_snow_small } };
  WEATHER_ICON dusty = { .num_glyphs = 2, .glyphs = (GLYPH []) { g_mist, g_dust } };

  switch ( icon_id ) {
    case 1: p_glyphs = is_day_not_night ? &sun : &moon; break; // clear sky
    case 2: p_glyphs = &cloudy; p_glyph_peek = is_day_not_night ? &sun_peek : &moon_peek; break; // few clouds
    case 3: p_glyphs = &cloudy; p_glyph_peek = is_day_not_night ? &sun_peek : &moon_peek; break; // scattered clouds
    case 4: p_glyphs = &cloudy; p_glyph_peek = is_day_not_night ? &sun_peek : &moon_peek; break; // broken clouds
    case 9: p_glyphs = &drizzle; p_glyph_peek = is_day_not_night ? &sun_peek : &moon_peek; break; // shower rain
    case 10: p_glyphs = &raining; p_glyph_peek = is_day_not_night ? &sun_peek : &moon_peek; break; // rain
    case 11: p_glyphs = &thunder; p_glyph_peek = is_day_not_night ? &sun_peek : &moon_peek; break; // thunderstorm
    case 13: p_glyphs = &frosty; p_glyph_peek = is_day_not_night ? &sun_peek : &moon_peek; break; // snow
    case 711: // smoke
    case 751: // sand
    case 761: // dust
    case 762: p_glyphs = &dusty; p_glyph_peek = is_day_not_night ? &sun_peek : &moon_peek; break; // volcanic ash
    case 905: p_glyphs = &windy; p_glyph_peek = is_day_not_night ? &sun_peek : &moon_peek; break; // windy
    case 906: p_glyphs = &hail; p_glyph_peek = is_day_not_night ? &sun_peek : &moon_peek; break; // hail
    case 50: p_glyphs = &mist; break; // mist
    default: break;
  }
  
  if ( p_glyphs ) {
    for( int i = 0; i < p_glyphs->num_glyphs; i++ ) {
      draw_glyph( ctx, bounds, p_glyphs->glyphs[i] );
    }
    if ( p_glyph_peek ) {
      draw_glyph( ctx, bounds, p_glyph_peek->glyphs[0] );
    }
  }
}

void weather_icons_init( void ) {  
  icon_font = fonts_load_custom_font( resource_get_handle( RESOURCE_ID_FONT_FORECASTFONT_32 ) );
}

void weather_icons_deinit( void ) {
  fonts_unload_custom_font( icon_font );
}
  
#endif
