#include <pebble.h>
#include "window/bluetooth.h"

static TextLayer *s_bluetooth_layer;
static GFont s_bluetooth_font;

void load_bluetooth(GRect bounds, Layer *window_layer){
   // Create the TextLayer with specific bounds
  s_bluetooth_layer = text_layer_create(
      GRect(0, 110, bounds.size.w, 40));

  // Improve the layout to be more like a watchface
  s_bluetooth_font = fonts_load_custom_font(fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  text_layer_set_background_color(s_bluetooth_layer, GColorClear);
  text_layer_set_text_color(s_bluetooth_layer, GColorBlack);
  text_layer_set_font(s_bluetooth_layer, s_bluetooth_font);
  text_layer_set_text_alignment(s_bluetooth_layer, GTextAlignmentCenter);
  
  bluetooth_handler(connection_service_peek_pebble_app_connection());

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_bluetooth_layer));
}

void bluetooth_handler(bool connected){
  if(connected){
    text_layer_set_text(s_bluetooth_layer, "connected");
  } else{
    text_layer_set_text(s_bluetooth_layer, "disconnected");
  }
  
}

void unload_bluetooth(){
  text_layer_destroy(s_bluetooth_layer);
  fonts_unload_custom_font(s_bluetooth_font);
}