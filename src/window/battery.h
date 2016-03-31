#pragma once

void load_battery_circle(GRect bounds, Layer *window_layer);
void battery_handler(BatteryChargeState charge);
void unload_battery_circle();