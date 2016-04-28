#include <pebble.h>
#include "handlerManager.h"
#include "window/battery.h"
#include "window/time.h"
#include "window/date.h"
#include "window/background.h"
#include "window/bluetooth.h"
#include "window/health.h"

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "tick_handler executed..");

  update_time();
  update_date();    
}

void handler_init(){
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
}

void handler_sleep(){
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
}

void handler_wake_up(){
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  update_time();
  update_date();
  
  battery_state_service_subscribe(battery_handler);
  battery_handler(battery_state_service_peek());
}