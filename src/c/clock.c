#include <pebble.h>
#include "global.h"
#include "clock.h"
#include "draw_utils.h"
#include "date.h"
#include "battery.h"
#include "health.h"
#include "chime.h"

static Layer *window_layer = 0;
// analog clock
static BitmapLayer *analog_clock_bitmap_layer = 0;
static Layer *analog_clock_layer = 0;
static GBitmap *analog_clock_bitmap = 0;
// misc
#ifndef SECONDS_ALWAYS_ON
static AppTimer *secs_display_apptimer = 0;
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
}

static void handle_clock_tick( struct tm *tick_time, TimeUnits units_changed ) {
  tm_time = *tick_time; // copy to global
  time_t now = time( NULL );
  tm_gmt = *gmtime( &now ); // copy to global
  
  // if (DEBUG) APP_LOG( APP_LOG_LEVEL_INFO, "clock.c: handle_clock_tick(): %d:%d:%d", tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec );

  layer_mark_dirty( analog_clock_layer );
  
  if ( ( units_changed & MINUTE_UNIT ) == MINUTE_UNIT ) do_chime( &tm_time );
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
    .sec_tail_angle = draw_clock_params.sec_angle + ( TRIG_MAX_ANGLE / 2 ),
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
  if ( secs_display_apptimer ) app_timer_cancel( secs_display_apptimer ); // just for fun.
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
  // background bitmap
  analog_clock_bitmap = gbitmap_create_with_resource( RESOURCE_ID_ANALOG_EMERY_FULL_SLIM );
  analog_clock_bitmap_layer = bitmap_layer_create( clock_layer_frame );
  bitmap_layer_set_bitmap( analog_clock_bitmap_layer, analog_clock_bitmap );
  layer_add_child( window_layer, bitmap_layer_get_layer( analog_clock_bitmap_layer ) );
  layer_set_hidden( bitmap_layer_get_layer( analog_clock_bitmap_layer ), false );
  // battery, date, health
  battery_init( bitmap_layer_get_layer( analog_clock_bitmap_layer ) );
  date_init( bitmap_layer_get_layer( analog_clock_bitmap_layer ) );
  health_init( bitmap_layer_get_layer( analog_clock_bitmap_layer ) );
  // clock layer
  analog_clock_layer = layer_create_with_data( layer_get_bounds( bitmap_layer_get_layer( analog_clock_bitmap_layer ) ),
                                              sizeof( ANALOG_LAYER_DATA ) );  
  #ifdef SECONDS_ALWAYS_ON
  ( (ANALOG_LAYER_DATA *) layer_get_data( analog_clock_layer ) )->show_seconds = true;
  #else
  ( (ANALOG_LAYER_DATA *) layer_get_data( analog_clock_layer ) )->show_seconds = false;
  #endif
  layer_add_child( bitmap_layer_get_layer( analog_clock_bitmap_layer ), analog_clock_layer );
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
  layer_destroy( analog_clock_layer );
  health_deinit();
  date_deinit();
  battery_deinit();
  gpaths_deinit();
  bitmap_layer_destroy( analog_clock_bitmap_layer );
  gbitmap_destroy( analog_clock_bitmap );
}
