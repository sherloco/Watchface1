#include <pebble.h>
#include <config.h>
#include "window/battery.h"
#include "window/time.h"
#include "window/date.h"
#include "window/background.h"
#include "window/bluetooth.h"

#if defined(PBL_HEALTH)
#include "window/health.h"
#endif

static Window *s_main_window;

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "tick_handler executed..");

  update_time();
  update_date();    
}

static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  //load_background(bounds, window_layer);  

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

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  //Register with Battery Service
  battery_state_service_subscribe(battery_handler);

  //Register with Blutooth Service
  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = bluetooth_handler
  });

  //Register with health service
  #if defined(PBL_HEALTH)
  if(!health_service_events_subscribe(health_handler, NULL)) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Health not available!");
  }
  #else
  APP_LOG(APP_LOG_LEVEL_ERROR, "Health not available!");
  #endif

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
