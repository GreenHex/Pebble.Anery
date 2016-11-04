#pragma once

typedef struct {
  GContext *ctx;
  GPoint center_pt;
  GPoint from_pt;
  GPoint to_pt;
  uint8_t hand_width;
  GColor hand_color;
  GColor hand_outline_color;
  uint16_t dot_radius;
  GColor dot_color;
  GColor dot_outline_color;
} HAND_DRAW_PARAMS;

typedef struct {
  GContext *ctx;
  GPoint center_pt;
  uint32_t hour_angle;
  uint32_t min_angle;
  GPath *s_hour_hand;
  GPath *s_hour_hand_highlight;
  GPath *s_min_hand;
  GPath *s_min_hand_highlight;
  GColor hour_hand_colour;
  GColor hour_hand_highlight_colour;
  GColor min_hand_colour;
  GColor min_hand_highlight_colour;
  GColor hand_outline_color;
  bool show_seconds;
} GPATH_HANDS_PARAMS;

void draw_clock_hand( HAND_DRAW_PARAMS *pDP );
void draw_gpath_hands( GPATH_HANDS_PARAMS *pGP );
