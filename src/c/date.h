//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//

#pragma once
#include <pebble.h>
#include "global.h"

#define DATE_WINDOW_WIDTH 32
#define DATE_WINDOW_HEIGHT 26
#define DATE_LEFT_GAP 6
#define DATE_WINDOW_OUTLINE_THK 3
#define DATE_TXT_VERT_ADJ 4

#ifdef GARNISH_HOLIDAYS
typedef struct {
  int date;
  int month; // use normal month, not unix month
  uint32_t iconID;
} HOLIDAY;
#endif

void date_init( Layer *parent_layer );
void date_deinit( void );
