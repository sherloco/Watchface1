#include <pebble.h>
#include <config.h>
#include "window/battery.h"
#include "window/time.h"
#include "window/date.h"
#include "window/background.h"
#include "window/bluetooth.h"
#include "handlerManager.h"

#if defined(PBL_HEALTH)
#include "window/health.h"
#endif

static Window *s_main_window;


static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  load_background(bounds, window_layer);  

  load_time(bounds, window_layer);
  load_date(bounds, window_layer);
  load_battery_circle(bounds, window_layer);
  load_bluetooth(bounds, window_layer);

  #if defined(PBL_HEALTH)
  load_health(bounds, window_layer);
  #endif
}

static void main_window_unload(Window *window) {
  unload_time();
  unload_date();
  unload_battery_circle();
  unload_bluetooth();
  unload_background();
  
  #if defined(PBL_HEALTH)
  unload_health();
  #endif
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  //window_set_background_color(s_main_window, GColorBlack);

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // Make sure the time is displayed from the start
  update_time();
  update_date();
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
