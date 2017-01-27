//
// Copyright (C) 2016, Vinodh Kumar M. <GreenHex@gmail.com>
//
#pragma once
#include <pebble.h>
#include "global.h"

#ifdef INCLUDE_STEPS
#ifdef PBL_HEALTH 

#define HEALTH_WINDOW_POS_X 0
#if PBL_DISPLAY_WIDTH == 200
#define HEALTH_WINDOW_POS_Y 54
#else
#define HEALTH_WINDOW_POS_Y 34
#endif
#define HEALTH_DIGIT_SIZE_W 15
#define HEALTH_DIGIT_SIZE_H 20
#define HEALTH_NUMBER_OF_DIGITS 6
#define HEALTH_DIGIT_GAP 1
#define HEALTH_WINDOW_SIZE_W ( HEALTH_DIGIT_SIZE_W * HEALTH_NUMBER_OF_DIGITS + HEALTH_DIGIT_GAP * ( HEALTH_NUMBER_OF_DIGITS - 1 ) )
#define HEALTH_WINDOW_SIZE_H HEALTH_DIGIT_SIZE_H
#define HEALTH_WINDOW_OUTLINE_THK 1
#define HEALTH_TXT_VERT_ADJ 3
//
#define HEALTH_UNIT_POS_X 0
#define HEALTH_UNIT_POS_Y 0
#define HEALTH_UNIT_SIZE_W 10
#define HEALTH_UNIT_SIZE_H 13
#define HEALTH_UNIT_TXT_VERT_ADJ 3

typedef struct {
  int idx;
} DIGIT_LAYER_DATA;

void health_init( Layer *parent_layer );
void health_deinit( void );

#endif
#endif
