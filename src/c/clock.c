#include <pebble.h>
#include "global.h"
#include "clock.h"
#include "chime.h"
#include "app_messaging.h"

static Layer *window_layer = 0;
static BitmapLayer *analog_clock_bitmap_layer = 0;
static Layer *analog_clock_layer = 0;
static GBitmap *analog_clock_bitmap = 0;
static BitmapLayer *batt_gauge_bitmap_layer = 0;
static GBitmap *batt_gauge_bitmap = 0;
static BitmapLayer *date_bitmap_layer = 0;
static TextLayer *date_text_layer = 0;
static GPath *s_gmt_arrow = 0;
static GPath *s_gmt_inlay = 0;
static GPath *s_gs_hour_arrow = 0;
static GPath *s_gs_hour_arrow_left = 0;
static GPath *s_gs_minute_arrow = 0;
static GPath *s_gs_minute_arrow_left = 0;
static GPath *s_sbge001_hour_arrow = 0;
static GPath *s_sbge001_hour_arrow_left = 0;
static GPath *s_sbge001_minute_arrow = 0;
static GPath *s_sbge001_minute_arrow_left = 0;
static AppTimer* secs_display_apptimer = 0; 
static tm tm_time;
static tm tm_gmt;

static void start_seconds_display( AccelAxisType axis, int32_t direction );

// function is "adjusted"" for whole hours or minutes; "after" 9:00 AM or "upto" 9:00 AM.
// "after" includes the hour, "upto" excludes the hour.
bool is_X_in_range( int a, int b, int x ) { return ( ( b > a ) ? ( ( x >= a ) && ( x < b ) ) : ( ( x >= a ) || ( x < b ) ) ); };

void draw_clock( void ) {
  time_t now = time( NULL );
  tm_time = *localtime( &now ); // copy to global
  tm_gmt = *gmtime( &now ); // copy to global
  if ( persist_read_int( MESSAGE_KEY_ANALOG_SECONDS_DISPLAY_TIMEOUT_SECS ) ) accel_tap_service_subscribe( start_seconds_display );
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

static void draw_clock_hand( struct HAND_DRAW_PARAMS *pDP ) {
  // dot outline
  graphics_context_set_stroke_color( pDP->ctx, pDP->dot_outline_color );
  graphics_context_set_stroke_width( pDP->ctx, 1 );
  graphics_draw_circle( pDP->ctx, pDP->center_pt, pDP->dot_radius );
  // hand outline
  graphics_context_set_stroke_color( pDP->ctx, pDP->hand_outline_color );
  graphics_context_set_stroke_width( pDP->ctx, pDP->hand_width + 2);
  graphics_draw_line( pDP->ctx, pDP->from_pt, pDP->to_pt );
  // hand
  graphics_context_set_stroke_color( pDP->ctx, pDP->hand_color );
  graphics_context_set_stroke_width( pDP->ctx, pDP->hand_width );
  graphics_draw_line( pDP->ctx, pDP->from_pt, pDP->to_pt );
  // dot
  graphics_context_set_fill_color( pDP->ctx, pDP->dot_color );
  graphics_fill_circle( pDP->ctx, pDP->center_pt, pDP->dot_radius - 1 );	
}

static void draw_gpath_hands( struct GPATH_HANDS_PARAMS *pGP ) {
  // for hour and minute hands
  graphics_context_set_stroke_width( pGP->ctx, 1 );

  // hour hand
  gpath_rotate_to( pGP->s_hour_arrow, pGP->hour_angle );
  gpath_rotate_to( pGP->s_hour_arrow_left, pGP->hour_angle );
  gpath_move_to( pGP->s_hour_arrow, pGP->center_pt );
  gpath_move_to( pGP->s_hour_arrow_left, pGP->center_pt );

  graphics_context_set_fill_color( pGP->ctx, GColorWhite );
  gpath_draw_filled( pGP->ctx, pGP->s_hour_arrow );
  graphics_context_set_fill_color( pGP->ctx, GColorLightGray );
  gpath_draw_filled( pGP->ctx, pGP->s_hour_arrow_left );
  graphics_context_set_fill_color( pGP->ctx, COLOUR_HOUR_HAND );
  graphics_context_set_stroke_color( pGP->ctx, pGP->hand_outline_color );
  gpath_draw_outline( pGP->ctx, pGP->s_hour_arrow);

  // min hand
  gpath_rotate_to( pGP->s_min_arrow, pGP->min_angle );
  gpath_rotate_to( pGP->s_min_arrow_left, pGP->min_angle );
  gpath_move_to( pGP->s_min_arrow, pGP->center_pt );
  gpath_move_to( pGP->s_min_arrow_left, pGP->center_pt );

  graphics_context_set_fill_color( pGP->ctx, GColorLightGray );
  gpath_draw_filled( pGP->ctx, pGP->s_min_arrow );
  graphics_context_set_fill_color( pGP->ctx, GColorWhite );
  gpath_draw_filled( pGP->ctx, pGP->s_min_arrow_left );
  graphics_context_set_fill_color( pGP->ctx, COLOUR_MIN_HAND );
  graphics_context_set_stroke_color( pGP->ctx, pGP->hand_outline_color );
  gpath_draw_outline( pGP->ctx, pGP->s_min_arrow );

  if ( ! ( (struct ANALOG_LAYER_DATA *) layer_get_data( analog_clock_layer ) )->show_seconds ) {
    graphics_context_set_fill_color( pGP->ctx, GColorBlack );
    graphics_fill_circle( pGP->ctx, pGP->center_pt, 2 );
  } 
}

static void analog_clock_layer_update_proc( Layer *layer, GContext *ctx ) {
  // uses global tm_time

  static struct HAND_DRAW_PARAMS hand_params;
  static struct GPATH_HANDS_PARAMS gpath_params;
  GRect layer_bounds = layer_get_bounds( layer );
  GPoint center_pt = grect_center_point( &layer_bounds );
  uint32_t gmt_angle = ( TRIG_MAX_ANGLE * ( ( ( tm_gmt.tm_hour ) * 6 ) + ( tm_gmt.tm_min / 10 ) ) ) / ( 12 * 6 * 2 );
  uint32_t hour_angle = ( TRIG_MAX_ANGLE * ( ( ( tm_time.tm_hour % 12 ) * 6 ) + ( tm_time.tm_min / 10 ) ) ) / ( 12 * 6 );
  uint32_t min_angle = TRIG_MAX_ANGLE * tm_time.tm_min / 60;

  if ( persist_read_int( MESSAGE_KEY_ANALOG_HANDS_STYLE ) == STYLE_SPIFFY_GS ) {
    gpath_params = (struct GPATH_HANDS_PARAMS) {
      .ctx = ctx,
      .center_pt = center_pt,
      .hour_angle = hour_angle,
      .min_angle = min_angle,
      .s_hour_arrow = s_gs_hour_arrow,
      .s_hour_arrow_left = s_gs_hour_arrow_left,
      .s_min_arrow = s_gs_minute_arrow,
      .s_min_arrow_left = s_gs_minute_arrow_left,
      .hand_outline_color = GColorBlack
    };
    draw_gpath_hands( &gpath_params );
  } else if( persist_read_int( MESSAGE_KEY_ANALOG_HANDS_STYLE ) == STYLE_SBGE001 ) {
    
    // gmt hand
    gpath_rotate_to( s_gmt_arrow, gmt_angle );
    gpath_move_to( s_gmt_arrow, center_pt );
    graphics_context_set_fill_color( ctx, GColorDarkCandyAppleRed );
    gpath_draw_filled( ctx, s_gmt_arrow );
    graphics_context_set_stroke_color( ctx, GColorRed );
    gpath_draw_outline( ctx, s_gmt_arrow );
    // gmt inlay
    gpath_rotate_to( s_gmt_inlay, gmt_angle );
    gpath_move_to( s_gmt_inlay, center_pt );
    graphics_context_set_fill_color( ctx, GColorWhite );
    gpath_draw_filled( ctx, s_gmt_inlay );
    // graphics_context_set_stroke_color( ctx, GColorWhite );
    // gpath_draw_outline( ctx, s_gmt_inlay );
    
    gpath_params = (struct GPATH_HANDS_PARAMS) {
      .ctx = ctx,
      .center_pt = center_pt,
      .hour_angle = hour_angle,
      .min_angle = min_angle,
      .s_hour_arrow = s_sbge001_hour_arrow,
      .s_hour_arrow_left = s_sbge001_hour_arrow_left,
      .s_min_arrow = s_sbge001_minute_arrow,
      .s_min_arrow_left = s_sbge001_minute_arrow_left,
      .hand_outline_color = GColorDarkGray
    };
    draw_gpath_hands( &gpath_params );
  } else { // contemporary
    
    GPoint hour_hand = (GPoint) {
      .x = ( sin_lookup( hour_angle ) * HOUR_HAND_LENGTH / TRIG_MAX_RATIO ) + center_pt.x,
      .y = ( -cos_lookup( hour_angle ) * HOUR_HAND_LENGTH / TRIG_MAX_RATIO ) + center_pt.y
    };

    // hour hand
    hand_params = (struct HAND_DRAW_PARAMS) {
      .ctx = ctx,
      .center_pt = center_pt,
      .from_pt = center_pt,
      .to_pt = hour_hand,
      .hand_width = HOUR_HAND_WIDTH,
      .hand_color = COLOUR_HOUR_HAND,
      .hand_outline_color = COLOUR_HANDS_OUTLINE,
      .dot_radius = CENTER_DOT_RADIUS,
      .dot_color = COLOUR_HOUR_HAND, // COLOUR_DOT,
      .dot_outline_color = COLOUR_DOT_OUTLINE
    };
    draw_clock_hand( &hand_params );

    GPoint min_hand = (GPoint) {
      .x = ( sin_lookup( min_angle ) * MIN_HAND_LENGTH / TRIG_MAX_RATIO ) + center_pt.x,
      .y = ( -cos_lookup( min_angle ) * MIN_HAND_LENGTH / TRIG_MAX_RATIO ) + center_pt.y
    };

    // minute hand
    hand_params = (struct HAND_DRAW_PARAMS) {
      .ctx = ctx,
      .center_pt = center_pt,
      .from_pt = center_pt,
      .to_pt = min_hand,
      .hand_width = MIN_HAND_WIDTH,
      .hand_color = COLOUR_MIN_HAND,
      .hand_outline_color = COLOUR_HANDS_OUTLINE,
      .dot_radius = CENTER_DOT_RADIUS - 4,
      .dot_color = COLOUR_MIN_HAND, // COLOUR_DOT,
      .dot_outline_color = COLOUR_DOT_OUTLINE
    };
    draw_clock_hand( &hand_params );
  }

  if ( ( (struct ANALOG_LAYER_DATA *) layer_get_data( analog_clock_layer ) )->show_seconds ) {
    int32_t sec_angle = TRIG_MAX_ANGLE * tm_time.tm_sec / 60;
    int32_t sec_tail_angle = sec_angle + ( TRIG_MAX_ANGLE / 2 );
    GPoint sec_hand = (GPoint) {
      .x = ( sin_lookup( sec_angle ) * SEC_HAND_LENGTH / TRIG_MAX_RATIO ) + center_pt.x,
      .y = ( -cos_lookup( sec_angle ) * SEC_HAND_LENGTH / TRIG_MAX_RATIO ) + center_pt.y
    };    
    GPoint sec_hand_tail = (GPoint) {
      .x = ( sin_lookup( sec_tail_angle ) * SEC_HAND_TAIL_LENGTH / TRIG_MAX_RATIO ) + center_pt.x,
      .y = ( -cos_lookup( sec_tail_angle ) * SEC_HAND_TAIL_LENGTH / TRIG_MAX_RATIO ) + center_pt.y
    };  

   // second hand
    hand_params = (struct HAND_DRAW_PARAMS) {
      .ctx = ctx,
      .center_pt = center_pt,
      .from_pt = sec_hand,
      .to_pt = sec_hand_tail,
      .hand_width = SEC_HAND_WIDTH,
      .hand_color = COLOUR_SEC_HAND,
      .hand_outline_color = COLOUR_HANDS_OUTLINE,
      .dot_radius = CENTER_DOT_RADIUS - 8,
      .dot_color = COLOUR_SEC_HAND, // COLOUR_DOT,
      .dot_outline_color = COLOUR_DOT_OUTLINE
    };
    draw_clock_hand( &hand_params );

    #if defined( PBL_COLOR ) // second hand tip
    GPoint sec_hand_tip = (GPoint) {
      .x = ( sin_lookup( sec_angle ) * ( SEC_HAND_LENGTH - SEC_HAND_TIP_LENGTH ) / TRIG_MAX_RATIO ) + center_pt.x,
      .y = ( -cos_lookup( sec_angle ) * ( SEC_HAND_LENGTH - SEC_HAND_TIP_LENGTH ) / TRIG_MAX_RATIO ) + center_pt.y
    };
    graphics_context_set_stroke_width( ctx, SEC_HAND_WIDTH );
    graphics_context_set_stroke_color( ctx, COLOUR_SEC_HAND_TIP );
    graphics_draw_line( ctx, sec_hand, sec_hand_tip );
    #endif
  }
}

static void date_bitmap_layer_update_proc( Layer *layer, GContext *ctx ) {
  GRect date_window_bounds = layer_get_bounds( layer );
  date_window_bounds = grect_inset( date_window_bounds, GEdgeInsets( DATE_WINDOW_OUTLINE_THK ) );
  // APP_LOG( APP_LOG_LEVEL_INFO, "date_bitmap_layer_update_proc() (%d, %d, %d, %d)", date_window_bounds.origin.x, date_window_bounds.origin.y, date_window_bounds.size.w, date_window_bounds.size.h );
  graphics_context_set_stroke_width( ctx, DATE_WINDOW_OUTLINE_THK );
  graphics_context_set_stroke_color( ctx, GColorBlack );
  graphics_draw_round_rect( ctx, date_window_bounds, 0 );
  graphics_context_set_stroke_color( ctx, GColorDarkGray );
  graphics_draw_round_rect( ctx, date_window_bounds, DATE_WINDOW_OUTLINE_THK );
}

static void date_text_layer_update_proc( Layer *layer, GContext *ctx ) {
  GRect date_window_bounds = layer_get_bounds( layer );
  // APP_LOG( APP_LOG_LEVEL_INFO, "date_text_layer_update_proc() (%d, %d, %d, %d)", date_window_bounds.origin.x, date_window_bounds.origin.y, date_window_bounds.size.w, date_window_bounds.size.h );
  graphics_context_set_fill_color( ctx, GColorWhite );
  graphics_fill_rect( ctx, date_window_bounds, 0, GCornersAll );
  static char date_text[3] = "";
  GColor text_color = ( tm_time.tm_wday == 0 ) ? GColorRed : ( tm_time.tm_wday == 6 ) ? GColorBlue : GColorBlack;
  graphics_context_set_text_color( ctx, text_color );
  snprintf( date_text, sizeof( date_text ), "%d", tm_time.tm_mday );
  date_window_bounds.origin.y -= 4;
  graphics_draw_text( ctx, date_text, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21), date_window_bounds,
                     GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL );
}

static void batt_gauge_layer_update_proc( Layer *layer, GContext *ctx ) {
  if( persist_read_int( MESSAGE_KEY_ANALOG_HANDS_STYLE ) != STYLE_SBGE001 ) return;
  GRect batt_gauge_window_bounds = layer_get_bounds( layer );
  GPoint center_pt = grect_center_point( &batt_gauge_window_bounds );
  graphics_draw_bitmap_in_rect( ctx, batt_gauge_bitmap, batt_gauge_window_bounds );
  
}
  
static void stop_seconds_display( void* data ) { // after timer elapses
  if ( secs_display_apptimer) app_timer_cancel( secs_display_apptimer ); // Just for fun.
  secs_display_apptimer = 0; // if we are here, we know for sure that timer has expired. 

  ( (struct ANALOG_LAYER_DATA *) layer_get_data( analog_clock_layer ) )->show_seconds = false;

  tick_timer_service_subscribe( MINUTE_UNIT, handle_clock_tick );
}

static void start_seconds_display( AccelAxisType axis, int32_t direction ) {
  if ( ! persist_read_int( MESSAGE_KEY_ANALOG_SECONDS_DISPLAY_TIMEOUT_SECS ) ) return;

  tick_timer_service_subscribe( SECOND_UNIT, handle_clock_tick );

  ( (struct ANALOG_LAYER_DATA *) layer_get_data( analog_clock_layer ) )->show_seconds = true;
  //
  if ( secs_display_apptimer ) {
    app_timer_reschedule( secs_display_apptimer, (uint32_t) persist_read_int( MESSAGE_KEY_ANALOG_SECONDS_DISPLAY_TIMEOUT_SECS ) * 1000 );
  } else {
    secs_display_apptimer = app_timer_register( (uint32_t) persist_read_int( MESSAGE_KEY_ANALOG_SECONDS_DISPLAY_TIMEOUT_SECS ) * 1000,
                                               stop_seconds_display, 0 );
  }
}

void clock_init( Window *window ) {
  window_layer = window_get_root_layer( window );
  GRect window_bounds = layer_get_bounds( window_layer );
  GRect clock_layer_bounds = GRect( window_bounds.origin.x + CLOCK_POS_X, window_bounds.origin.y + CLOCK_POS_Y, 
                                   window_bounds.size.w - CLOCK_POS_X, window_bounds.size.h - CLOCK_POS_Y );
  // background bitmap
  analog_clock_bitmap = gbitmap_create_with_resource( RESOURCE_ID_ANALOG_EMERY_FULL_SLIM );
  analog_clock_bitmap_layer = bitmap_layer_create( clock_layer_bounds );
  bitmap_layer_set_bitmap( analog_clock_bitmap_layer, analog_clock_bitmap );
  layer_add_child( window_layer, bitmap_layer_get_layer( analog_clock_bitmap_layer ) );
  layer_set_hidden( bitmap_layer_get_layer( analog_clock_bitmap_layer ), false );
  // battery
  batt_gauge_bitmap = gbitmap_create_with_resource( RESOURCE_ID_BATTERY_GAUGE );
  GRect batt_gauge_layer_frame = GRect( BATT_GAUGE_LOC_X, BATT_GAUGE_LOC_Y, BATT_GAUGE_SIZE, BATT_GAUGE_SIZE );
  batt_gauge_bitmap_layer = bitmap_layer_create( batt_gauge_layer_frame );
  layer_set_update_proc( bitmap_layer_get_layer( batt_gauge_bitmap_layer ), batt_gauge_layer_update_proc );
  layer_add_child( bitmap_layer_get_layer( analog_clock_bitmap_layer ), bitmap_layer_get_layer( batt_gauge_bitmap_layer ) );
  // date bitmap layer
  GRect date_window_frame = GRect( window_bounds.origin.x + window_bounds.size.w - 3 - DATE_WINDOW_WIDTH,
                                  window_bounds.origin.y + ( ( window_bounds.size.h - DATE_WINDOW_HEIGHT ) / 2 ),
                                  DATE_WINDOW_WIDTH, DATE_WINDOW_HEIGHT );
  date_bitmap_layer = bitmap_layer_create( date_window_frame );
  layer_set_update_proc( bitmap_layer_get_layer( date_bitmap_layer ), date_bitmap_layer_update_proc );
  layer_add_child( bitmap_layer_get_layer( analog_clock_bitmap_layer ), bitmap_layer_get_layer( date_bitmap_layer ) );
  // date text layer
  GRect date_window_bounds = GRect( 0, 0, DATE_WINDOW_WIDTH, DATE_WINDOW_HEIGHT );
  date_window_bounds = grect_inset( date_window_bounds, GEdgeInsets( DATE_WINDOW_OUTLINE_THK * 2 ) );
  date_text_layer = text_layer_create( date_window_bounds );
  layer_set_update_proc( text_layer_get_layer( date_text_layer ), date_text_layer_update_proc );
  layer_add_child( bitmap_layer_get_layer( date_bitmap_layer ), text_layer_get_layer( date_text_layer ) );
  // clock layer
  analog_clock_layer = layer_create_with_data( layer_get_bounds( bitmap_layer_get_layer( analog_clock_bitmap_layer ) ),
                                              sizeof( struct ANALOG_LAYER_DATA ) );
  ( (struct ANALOG_LAYER_DATA *) layer_get_data( analog_clock_layer ) )->show_seconds = false;
  layer_add_child( bitmap_layer_get_layer( analog_clock_bitmap_layer ), analog_clock_layer );
  layer_set_update_proc( analog_clock_layer, analog_clock_layer_update_proc ); 
  layer_set_hidden( analog_clock_layer, false );
  
  s_gmt_arrow = gpath_create( &GMT_HAND );
  s_gmt_inlay = gpath_create( &GMT_HAND_INLAY );
  //
  s_gs_minute_arrow = gpath_create( &MINUTE_HAND_SPIFFY_GS_POINTS );
  s_gs_minute_arrow_left = gpath_create( &MINUTE_HAND_SPIFFY_GS_POINTS_LEFT );
  s_gs_hour_arrow = gpath_create( &HOUR_HAND_SPIFFY_GS_POINTS );
  s_gs_hour_arrow_left = gpath_create( &HOUR_HAND_SPIFFY_GS_POINTS_LEFT );
  //
  s_sbge001_minute_arrow = gpath_create( &MINUTE_HAND_SBGE001_POINTS );
  s_sbge001_minute_arrow_left = gpath_create( &MINUTE_HAND_SBGE001_POINTS_LEFT );
  s_sbge001_hour_arrow = gpath_create( &HOUR_HAND_SBGE001_POINTS );
  s_sbge001_hour_arrow_left = gpath_create( &HOUR_HAND_SBGE001_POINTS_LEFT );

  // subscriptions
  tick_timer_service_subscribe( MINUTE_UNIT, handle_clock_tick );

  // show current time
  draw_clock();
}

void clock_deinit( void ) {
  if ( secs_display_apptimer ) app_timer_cancel( secs_display_apptimer );
  accel_tap_service_unsubscribe(); // are we over-unsubscribing?
  tick_timer_service_unsubscribe();
  bitmap_layer_destroy( batt_gauge_bitmap_layer );
  bitmap_layer_destroy( date_bitmap_layer );
  text_layer_destroy( date_text_layer );
  gpath_destroy( s_sbge001_minute_arrow );
  gpath_destroy( s_sbge001_minute_arrow_left );
  gpath_destroy( s_sbge001_hour_arrow );
  gpath_destroy( s_sbge001_hour_arrow_left );
  gpath_destroy( s_gs_minute_arrow );
  gpath_destroy( s_gs_minute_arrow_left );
  gpath_destroy( s_gs_hour_arrow );
  gpath_destroy( s_gs_hour_arrow_left );
  gpath_destroy( s_gmt_inlay );
  gpath_destroy( s_gmt_arrow );
  bitmap_layer_destroy( analog_clock_bitmap_layer );
  layer_destroy( analog_clock_layer );
  gbitmap_destroy( batt_gauge_bitmap );
  gbitmap_destroy( analog_clock_bitmap );
}
