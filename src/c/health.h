#pragma once

#ifdef PBL_HEALTH 

#define HEALTH_WINDOW_POS_X 0
#if PBL_DISPLAY_WIDTH == 200
#define HEALTH_WINDOW_POS_Y 46
#else
#define HEALTH_WINDOW_POS_Y 32
#endif
#define HEALTH_DIGIT_SIZE_W 16
#define HEALTH_DIGIT_SIZE_H 20
#define HEALTH_NUMBER_OF_DIGITS 6
#define HEALTH_DIGIT_GAP 1
#define HEALTH_WINDOW_SIZE_W ( HEALTH_DIGIT_SIZE_W * HEALTH_NUMBER_OF_DIGITS + HEALTH_DIGIT_GAP * ( HEALTH_NUMBER_OF_DIGITS - 1 ) )
#define HEALTH_WINDOW_SIZE_H HEALTH_DIGIT_SIZE_H
#define HEALTH_WINDOW_OUTLINE_THK 2
#define HEALTH_TXT_VERT_ADJ 3

typedef struct {
  int idx;
} DIGIT_LAYER_DATA;

void health_init( Layer *parent_layer );
void health_deinit( void );

#endif