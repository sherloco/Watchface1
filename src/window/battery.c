#include <pebble.h>
#include "../config.h"
#include "battery.h"

static Layer *battery_canvas;

static float calculate_battery_angle(){
  BatteryChargeState charge_state = battery_state_service_peek();
  uint8_t battery_percentage = charge_state.charge_percent;
  if(battery_percentage<=50){
    return battery_percentage * 6.f;
  } else {
    return 330.f+(2.f/3.f*(battery_percentage-55.f));
  }
}

void update_battery_circle(Layer *layer, GContext *ctx){
  GRect bounds = layer_get_bounds(layer);
  graphics_context_set_fill_color(ctx, BATTERY_COLOR);
  graphics_fill_radial(ctx, bounds, GOvalScaleModeFitCircle, BATTERY_RADIUS, 0, DEG_TO_TRIGANGLE(calculate_battery_angle()));
  //app_log(APP_LOG_LEVEL_DEBUG, "main.c", 89, "Deg: %e", s_battery_percentage*3.6);
}

void load_battery_circle(GRect bounds, Layer *window_layer){
  battery_canvas = layer_create(bounds);
  layer_set_update_proc(battery_canvas, update_battery_circle);
  layer_add_child(window_layer, battery_canvas);
}

void battery_handler(BatteryChargeState charge){
  //s_battery_percentage = charge.charge_percent;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "charge_percent: %d", charge.charge_percent);
  layer_mark_dirty(battery_canvas);
}

void unload_battery_circle(){
  layer_destroy(battery_canvas);
}
