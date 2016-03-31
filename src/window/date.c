#include <pebble.h>
#include "window/date.h"

static TextLayer *s_date_layer;
static GFont s_date_font;

void update_date() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  static char s_date_buffer[22];
  strftime(s_date_buffer, sizeof(s_date_buffer), "%B %d", tick_time);
  text_layer_set_text(s_date_layer, s_date_buffer);
}

void load_date(GRect bounds, Layer *window_layer){
  // Create the TextLayer with specific bounds
  s_date_layer = text_layer_create(
      GRect(0, 40, bounds.size.w, 50));

  // Improve the layout to be more like a watchface
  s_date_font = fonts_load_custom_font(fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorBlack);
  text_layer_set_font(s_date_layer, s_date_font);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
}

void unload_date(){
  text_layer_destroy(s_date_layer);
  fonts_unload_custom_font(s_date_font);
}