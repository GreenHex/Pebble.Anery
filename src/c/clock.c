//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#include <pebble.h>
#include "global.h"
#include "clock.h"
#include "draw_utils.h"
#include "date.h"
#include "battery.h"

#ifdef PBL_HEALTH
#include "health.h"
#ifdef INCLUDE_HR
#include "heart.h"
#endif
#endif
#ifdef INCLUDE_WEATHER
#include "weather.h"
#endif
#include "chime.h"

static Layer *window_layer = 0;
// analog clock
static Layer *dial_layer = 0;
static Layer *digit_layer[ NUM_DIGITS ];
static int gmt_digits[ NUM_DIGITS ] = { 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23 };
static char digit_str[] = "12";
static BitmapLayer *snooze_layer = 0;
static Layer *analog_clock_layer = 0;
static GBitmap *analog_clock_bitmap = 0;
// misc
#ifndef SECONDS_ALWAYS_ON
static AppTimer *secs_display_apptimer = 0;
static GFont digit_font_expanded;
static GFont digit_font_normal;
#endif
extern tm tm_time;
extern tm tm_gmt;

static void start_seconds_display( AccelAxisType axis, int32_t direction );

// function is "adjusted"" for whole hours or minutes; "after" 9:00 AM or "upto" 9:00 AM.
// "after" includes the hour, "upto" excludes the hour.
bool is_X_in_range( int a, int b, int x ) { return ( ( b > a ) ? ( ( x >= a ) && ( x < b ) ) : ( ( x >= a ) || ( x < b ) ) ); };

void draw_clock( void ) {
  time_t now = time( NULL );
  tm_time = *localtime( &now ); // copy to global
  tm_gmt = *gmtime( &now ); // copy to global
  #ifndef SECONDS_ALWAYS_ON
  if ( persist_read_int( MESSAGE_KEY_ANALOG_SECONDS_DISPLAY_TIMEOUT_SECS ) ) {
    accel_tap_service_subscribe( start_seconds_display );
  } else {
    accel_tap_service_unsubscribe();
  }
  #endif
  layer_mark_dirty( analog_clock_layer );
  #ifdef INCLUDE_WEATHER
  get_weather( &tm_time, true );
  #endif
}

static void handle_clock_tick( struct tm *tick_time, TimeUnits units_changed ) {
  tm_time = *tick_time; // copy to global
  time_t now = time( NULL );
  tm_gmt = *gmtime( &now ); // copy to global
  
  #ifdef DEBUG
  APP_LOG( APP_LOG_LEVEL_INFO, "clock.c: handle_clock_tick(): %d:%d:%d", tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec );
  #endif
  
  #ifdef INCLUDE_WEATHER
  if ( units_changed & MINUTE_UNIT ) get_weather( &tm_time, false );
  #endif
  
  layer_mark_dirty( analog_clock_layer );
  
  if ( ( !quiet_time_is_active() ) && ( units_changed & MINUTE_UNIT ) ) do_chime( &tm_time );
}

static void dial_layer_update_proc( Layer *layer, GContext *ctx ) {
  GRect bounds = layer_get_bounds( layer );
  graphics_context_set_antialiased( ctx, true );
  graphics_context_set_fill_color( ctx, BACKGROUND_COLOUR );
  graphics_fill_rect( ctx, bounds, 0, GCornerNone );
  if ( persist_read_bool( MESSAGE_KEY_ANALOG_SHOW_SECONDS_TICKS ) &&
      ( persist_read_int( MESSAGE_KEY_ANALOG_HANDS_STYLE ) != STYLE_SBGE001 ) ) {
    draw_seconds_ticks( & (DRAW_TICKS_PARAMS) { 
      .layer = layer, 
      .ctx = ctx, 
      .p_gpath_info = &PATH_TICK, 
      .increment = 1, 
      .tick_thk = 1, 
      .tick_length = 6,
      #ifdef REVERSE
      .tick_colour = GColorLightGray, 
      #else
      .tick_colour = GColorDarkGray, 
      #endif
      .bg_colour = BACKGROUND_COLOUR
    } );
  }
  draw_seconds_ticks( & (DRAW_TICKS_PARAMS) { 
    .layer = layer, 
    .ctx = ctx, 
    .p_gpath_info = &PATH_TICK, 
    .increment = 5, 
    .tick_thk = 1, 
    .tick_length = 10, 
    .tick_colour = CLOCK_TICK_HOUR_COLOUR, 
    .bg_colour = BACKGROUND_COLOUR
  } );
  draw_seconds_ticks( & (DRAW_TICKS_PARAMS) {
    .layer = layer,
    .ctx = ctx,
    .p_gpath_info = &PATH_TICK,
    .increment = 15,
    .tick_thk = 3,
    .tick_length = 15,
    .tick_colour = CLOCK_TICK_HOUR_COLOUR, 
    .bg_colour = BACKGROUND_COLOUR
  } );
  graphics_context_set_stroke_color( ctx, BACKGROUND_COLOUR );
  graphics_context_set_stroke_width( ctx, CLOCK_TICK_EDGE_OFFSET );
  graphics_draw_round_rect( ctx, grect_inset( bounds, GEdgeInsets( CLOCK_TICK_EDGE_OFFSET / 2 ) ), 0 ); 
  graphics_context_set_stroke_color( ctx, GColorWhite );
  graphics_context_set_stroke_width( ctx, 1 );
  graphics_draw_round_rect( ctx, bounds, 9 ); 
}

#define DIGIT_FONT_EXPANDED RESOURCE_ID_FONT_BIORHYME_EXPANDED_REGULAR_9
#define DIGIT_FONT_NORMAL RESOURCE_ID_FONT_BIORHYME_REGULAR_9

static void digit_layer_update_proc( Layer *layer, GContext *ctx ) {
  if ( persist_read_int( MESSAGE_KEY_ANALOG_HANDS_STYLE ) != STYLE_SBGE001 ) return;
  GRect layer_bounds = layer_get_bounds( layer );
  /*
  graphics_context_set_stroke_width( ctx, 1 );
  graphics_context_set_stroke_color( ctx, PBL_IF_COLOR_ELSE( GColorDarkGray, GColorLightGray ) );
  graphics_draw_round_rect( ctx, layer_bounds, 0 );
  */
  snprintf( digit_str, sizeof( digit_str ), "%u",  ( (DIGIT_LAYER_DATA_GMT *) layer_get_data( layer ) )->digit );
  layer_bounds.origin.y -= DIGIT_TXT_VERT_ADJ;
  GFont font = ( ( (DIGIT_LAYER_DATA_GMT *) layer_get_data( layer ) )->digit < 10 ) ?
    digit_font_expanded : digit_font_normal;
  graphics_context_set_text_color( ctx, ( (DIGIT_LAYER_DATA_GMT *) layer_get_data( layer ) )->colour );
  graphics_draw_text( ctx, digit_str, font /* fonts_get_system_font( FONT_KEY_GOTHIC_14 ) */, layer_bounds,
                     GTextOverflowModeTrailingEllipsis, ( ( DIGIT_LAYER_DATA_GMT *) layer_get_data( layer ) )->text_alignment, NULL );
}

static void snooze_layer_update_proc( Layer *layer, GContext *ctx ) {
  if ( quiet_time_is_active() ) {
    GRect bounds = layer_get_bounds( layer );
    graphics_context_set_fill_color( ctx, GColorBlack );
    graphics_fill_rect( ctx, bounds, 0, GCornerNone );
    graphics_context_set_antialiased( ctx, true );
    graphics_context_set_compositing_mode( ctx, GCompOpSet );
    #ifdef REVERSE
    GBitmap *snooze_bitmap = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_MOUSE_W );
    #else
    GBitmap *snooze_bitmap = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_MOUSE_B );
    #endif
    graphics_draw_bitmap_in_rect( ctx, snooze_bitmap, bounds );
    gbitmap_destroy( snooze_bitmap );
  }
}

static void analog_clock_layer_update_proc( Layer *layer, GContext *ctx ) {
  // uses global tm_time
  GRect layer_bounds = layer_get_bounds( layer );
  
  DRAW_CLOCK_PARAMS draw_clock_params = (DRAW_CLOCK_PARAMS) {
    .ctx = ctx,
    .center_pt = grect_center_point( &layer_bounds ),
    .gmt_angle = ( TRIG_MAX_ANGLE * ( ( ( tm_gmt.tm_hour ) * 6 ) + ( tm_gmt.tm_min / 10 ) ) ) / ( 12 * 6 * 2 ),
    .hour_angle = ( TRIG_MAX_ANGLE * ( ( ( tm_time.tm_hour % 12 ) * 6 ) + ( tm_time.tm_min / 10 ) ) ) / ( 12 * 6 ),
    .min_angle = TRIG_MAX_ANGLE * tm_time.tm_min / 60,
    .sec_angle = TRIG_MAX_ANGLE * tm_time.tm_sec / 60,
    .sec_tail_angle = ( TRIG_MAX_ANGLE * tm_time.tm_sec / 60 ) + ( TRIG_MAX_ANGLE / 2 ),
    .show_seconds = ( (ANALOG_LAYER_DATA *) layer_get_data( analog_clock_layer ) )->show_seconds
  };
  
  if ( persist_read_int( MESSAGE_KEY_ANALOG_HANDS_STYLE ) == STYLE_SPIFFY_GS ) {
    draw_spiffy_gs_clock_hands( &draw_clock_params );
  } else if( persist_read_int( MESSAGE_KEY_ANALOG_HANDS_STYLE ) == STYLE_SBGE001 ) {
    draw_sbge001_clock_hands( &draw_clock_params );
  } else { // contemporary 
    draw_cont_clock_hands( &draw_clock_params );
  }
}

#ifndef SECONDS_ALWAYS_ON
static void stop_seconds_display( void* data ) { // after timer elapses
  secs_display_apptimer = 0; // docs don't say if this is set to zero when timer expires. 

  ( (ANALOG_LAYER_DATA *) layer_get_data( analog_clock_layer ) )->show_seconds = false;

  tick_timer_service_subscribe( MINUTE_UNIT, handle_clock_tick );
}

static void start_seconds_display( AccelAxisType axis, int32_t direction ) {
  if ( ! persist_read_int( MESSAGE_KEY_ANALOG_SECONDS_DISPLAY_TIMEOUT_SECS ) ) return;

  tick_timer_service_subscribe( SECOND_UNIT, handle_clock_tick );

  ( (ANALOG_LAYER_DATA *) layer_get_data( analog_clock_layer ) )->show_seconds = true;
  //
  if ( secs_display_apptimer ) {
    app_timer_reschedule( secs_display_apptimer, (uint32_t) persist_read_int( MESSAGE_KEY_ANALOG_SECONDS_DISPLAY_TIMEOUT_SECS ) * 1000 );
  } else {
    secs_display_apptimer = app_timer_register( (uint32_t) persist_read_int( MESSAGE_KEY_ANALOG_SECONDS_DISPLAY_TIMEOUT_SECS ) * 1000,
                                               stop_seconds_display, 0 );
  }
}
#endif

void clock_init( Window *window ) {
  window_layer = window_get_root_layer( window );
  GRect window_bounds = layer_get_bounds( window_layer );
  GRect clock_layer_frame = GRect( window_bounds.origin.x + CLOCK_POS_X, window_bounds.origin.y + CLOCK_POS_Y, 
                                   window_bounds.size.w - CLOCK_POS_X, window_bounds.size.h - CLOCK_POS_Y );
  // background
  dial_layer = layer_create( clock_layer_frame );
  layer_set_update_proc( dial_layer, dial_layer_update_proc );
  layer_add_child( window_layer, dial_layer );
  for ( int i = 0; i < NUM_DIGITS; i++ ) {
    GRect digit_layer_frame = GRect( DIGIT_LOCATIONS.points[ i ].x, DIGIT_LOCATIONS.points[ i ].y,
                                    DIGIT_RECT_SIZE_W, DIGIT_RECT_SIZE_H ); 
    digit_layer[i] = layer_create_with_data( digit_layer_frame, sizeof( DIGIT_LAYER_DATA_GMT ) );
    *(DIGIT_LAYER_DATA_GMT *) layer_get_data( digit_layer[ i ] ) = (DIGIT_LAYER_DATA_GMT) {
      .digit = gmt_digits[ i ],
      .colour = PBL_IF_COLOR_ELSE( GColorRed, GColorWhite ),
      .text_alignment = GTextAlignmentCenter,
    };
    layer_set_update_proc( digit_layer[ i ], digit_layer_update_proc );
    layer_add_child( dial_layer, digit_layer[ i ] );
  }
  digit_font_expanded = fonts_load_custom_font( resource_get_handle( DIGIT_FONT_EXPANDED ) );
  digit_font_normal = fonts_load_custom_font( resource_get_handle( DIGIT_FONT_NORMAL ) );
  snooze_layer = bitmap_layer_create( SNOOZE_LAYER_FRAME );
  layer_set_update_proc( bitmap_layer_get_layer( snooze_layer ), snooze_layer_update_proc );
  layer_add_child( dial_layer, bitmap_layer_get_layer( snooze_layer ) );
  
  // battery, date, health, weather
  battery_init( dial_layer );
  date_init( dial_layer );
  #ifdef PBL_HEALTH
  health_init( dial_layer );
  #ifdef INCLUDE_HR
  heart_init( dial_layer );
  #endif
  #endif
  #ifdef INCLUDE_WEATHER
  weather_init( dial_layer );
  #endif
  // clock layer
  analog_clock_layer = layer_create_with_data( layer_get_bounds( dial_layer ),
                                              sizeof( ANALOG_LAYER_DATA ) );  
  #ifdef SECONDS_ALWAYS_ON
  ( (ANALOG_LAYER_DATA *) layer_get_data( analog_clock_layer ) )->show_seconds = true;
  #else
  ( (ANALOG_LAYER_DATA *) layer_get_data( analog_clock_layer ) )->show_seconds = false;
  #endif
  layer_add_child( dial_layer, analog_clock_layer );
  layer_set_update_proc( analog_clock_layer, analog_clock_layer_update_proc ); 
  layer_set_hidden( analog_clock_layer, false );
  
  gpaths_init();
  
  // subscriptions
  #ifdef SECONDS_ALWAYS_ON
  tick_timer_service_subscribe( SECOND_UNIT, handle_clock_tick );
  #else
  tick_timer_service_subscribe( MINUTE_UNIT, handle_clock_tick );
  #endif
  
  // show current time
  draw_clock();
}

void clock_deinit( void ) {
  #ifndef SECONDS_ALWAYS_ON
  if ( secs_display_apptimer ) app_timer_cancel( secs_display_apptimer );
  accel_tap_service_unsubscribe(); // are we over-unsubscribing?
  #endif
  tick_timer_service_unsubscribe();
  if ( analog_clock_layer ) layer_destroy( analog_clock_layer );
  #ifdef INCLUDE_WEATHER
  weather_deinit();
  #endif
  #ifdef PBL_HEALTH
  #ifdef INCLUDE_HR
  heart_deinit();
  #endif
  health_deinit();
  #endif
  date_deinit();
  battery_deinit();
  gpaths_deinit();
  if ( snooze_layer ) bitmap_layer_destroy( snooze_layer );
  fonts_unload_custom_font( digit_font_normal );
  fonts_unload_custom_font( digit_font_expanded );  
  for ( int i = 0; i < NUM_DIGITS; i ++ ) {
    if ( digit_layer[i] ) layer_destroy( digit_layer[i] );
  }
  if ( dial_layer ) layer_destroy( dial_layer );
  gbitmap_destroy( analog_clock_bitmap );
}
