#include <pebble.h>
#include "date.h"

static BitmapLayer *date_bitmap_layer = 0;
static TextLayer *date_text_layer = 0;
#ifdef GARNISH_HOLIDAYS
#define NUM_HOLIDAYS 3
HOLIDAY holidays[ NUM_HOLIDAYS ] = {
  { .date = 1, .month = 1, .iconID = RESOURCE_ID_ICON_NEW_YEAR },
  { .date = 31, .month = 10, .iconID = RESOURCE_ID_ICON_HALOWEEN },
  { .date = 25, .month = 12, .iconID = RESOURCE_ID_ICON_CHRISTMAS }
};
#endif

extern tm tm_time;
extern tm tm_gmt;

static void date_bitmap_layer_update_proc( Layer *layer, GContext *ctx ) {
  GRect date_window_bounds = layer_get_bounds( layer );
  date_window_bounds = grect_inset( date_window_bounds, GEdgeInsets( DATE_WINDOW_OUTLINE_THK ) );
  // APP_LOG( APP_LOG_LEVEL_INFO, "date_bitmap_layer_update_proc() (%d, %d, %d, %d)", date_window_bounds.origin.x, date_window_bounds.origin.y, date_window_bounds.size.w, date_window_bounds.size.h );
  graphics_context_set_stroke_width( ctx, DATE_WINDOW_OUTLINE_THK );
  graphics_context_set_stroke_color( ctx, GColorBlack );
  graphics_draw_round_rect( ctx, date_window_bounds, 0 );
  graphics_context_set_stroke_color( ctx, PBL_IF_COLOR_ELSE( GColorDarkGray, GColorBlack ) );
  graphics_draw_round_rect( ctx, date_window_bounds, DATE_WINDOW_OUTLINE_THK );
}

static void date_text_layer_update_proc( Layer *layer, GContext *ctx ) {
  GRect date_window_bounds = layer_get_bounds( layer );
  // APP_LOG( APP_LOG_LEVEL_INFO, "date_text_layer_update_proc() (%d, %d, %d, %d)", date_window_bounds.origin.x, date_window_bounds.origin.y, date_window_bounds.size.w, date_window_bounds.size.h );
  graphics_context_set_fill_color( ctx, GColorWhite );
  graphics_fill_rect( ctx, date_window_bounds, 0, GCornersAll );

  static char date_text[3] = "";
  GColor text_color = ( tm_time.tm_wday == 0 ) ? GColorOrange : ( tm_time.tm_wday == 6 ) ? GColorBlueMoon : GColorBlack;
  graphics_context_set_text_color( ctx, text_color );
  snprintf( date_text, sizeof( date_text ), "%d", tm_time.tm_mday );
  date_window_bounds.origin.y -= 4;
  graphics_draw_text( ctx, date_text, fonts_get_system_font( FONT_KEY_ROBOTO_CONDENSED_21 ), date_window_bounds,
                     GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL );
  #ifdef GARNISH_HOLIDAYS
  date_window_bounds.origin.y += 4;
  static GBitmap *holiday_bitmap = 0;
  for ( int i = 0; i < NUM_HOLIDAYS; i++ ) {
    if ( ( holidays[i].date == tm_time.tm_mday ) && ( ( holidays[i].month - 1 ) == tm_time.tm_mon ) ) {
      holiday_bitmap = gbitmap_create_with_resource( holidays[i].iconID );
      graphics_draw_bitmap_in_rect( ctx, holiday_bitmap, date_window_bounds );
      break;
    }
  }
  if ( holiday_bitmap ) gbitmap_destroy( holiday_bitmap );
  holiday_bitmap = 0;
  #endif
}

void date_init( Layer *parent_layer ) {
  GRect parent_layer_bounds = layer_get_bounds( parent_layer );
  // date bitmap layer
  GRect date_window_frame = GRect( parent_layer_bounds.origin.x + parent_layer_bounds.size.w - DATE_WINDOW_WIDTH - PBL_IF_COLOR_ELSE( DATE_LEFT_GAP, DATE_LEFT_GAP - 2 ),
                                  parent_layer_bounds.origin.y + ( ( parent_layer_bounds.size.h - DATE_WINDOW_HEIGHT ) / 2 ),
                                  DATE_WINDOW_WIDTH, DATE_WINDOW_HEIGHT );
  date_bitmap_layer = bitmap_layer_create( date_window_frame );
  bitmap_layer_set_compositing_mode( date_bitmap_layer, GCompOpAssign );
  layer_set_update_proc( bitmap_layer_get_layer( date_bitmap_layer ), date_bitmap_layer_update_proc );
  layer_add_child( parent_layer, bitmap_layer_get_layer( date_bitmap_layer ) );
  // date text layer
  GRect date_window_bounds = GRect( 0, 0, DATE_WINDOW_WIDTH, DATE_WINDOW_HEIGHT );
  date_window_bounds = grect_inset( date_window_bounds, GEdgeInsets( DATE_WINDOW_OUTLINE_THK * 2 ) );
  date_text_layer = text_layer_create( date_window_bounds );
  layer_set_update_proc( text_layer_get_layer( date_text_layer ), date_text_layer_update_proc );
  layer_add_child( bitmap_layer_get_layer( date_bitmap_layer ), text_layer_get_layer( date_text_layer ) );
}

void date_deinit( void ) {
  bitmap_layer_destroy( date_bitmap_layer );
  text_layer_destroy( date_text_layer );
}
