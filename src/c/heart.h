//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//
#pragma once

#include <pebble.h>
#include "global.h"

#ifdef INCLUDE_HR
#ifdef PBL_HEALTH

#define HR_WINDOW_POS_X 22
#if PBL_DISPLAY_WIDTH == 200
#define HR_WINDOW_POS_Y 154
#else
#define HR_WINDOW_POS_Y 112
#endif
#define HR_WINDOW_SIZE_W 54
#define HR_WINDOW_SIZE_H 28
#define HR_WINDOW_OUTLINE_THK 2
#define HR_TEXT_VERT_ADJ 4
#define HR_ICON_SIZE_W 13
#define HR_ICON_SIZE_H 12

void heart_init( Layer* parent_layer );
void heart_deinit( void );

#endif
#endif
