#pragma once
#include <pebble.h>
#include "global.h"

#define DATE_WINDOW_WIDTH 34
#define DATE_WINDOW_HEIGHT 28
#define DATE_LEFT_GAP 2
#define DATE_WINDOW_OUTLINE_THK 2

#ifdef GARNISH_HOLIDAYS
typedef struct {
  int date;
  int month; // use normal month, not unix month
  uint32_t iconID;
} HOLIDAY;
#endif

void date_init( Layer *parent_layer );
void date_deinit( void );
