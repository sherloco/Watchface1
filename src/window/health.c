#include <pebble.h>
#include "window/health.h"

static TextLayer *s_health_layer;
static GFont s_health_font;
static time_t last_time_asleep;

static bool data_for_today_available(HealthMetric metric){
  time_t start = time_start_of_today();
  time_t end = time(NULL);

  // Check the metric has data available for today
  HealthServiceAccessibilityMask mask = health_service_metric_accessible(metric, 
                                                                         start, end);
  return mask & HealthServiceAccessibilityMaskAvailable;
}

static void update_metric(HealthMetric metric){
  if(data_for_today_available(metric)){
    static char s_health_buffer[22];
    if(metric == HealthMetricStepCount){
      snprintf(s_health_buffer, sizeof(s_health_buffer), "%d", (int)health_service_sum_today(metric));
    } else{
      int32_t hours = health_service_sum_today(metric)/SECONDS_PER_HOUR;
      int32_t minutes = (health_service_sum_today(metric) - (hours * SECONDS_PER_HOUR))/SECONDS_PER_MINUTE;
      snprintf(s_health_buffer, sizeof(s_health_buffer), "%dh%dm", (int)hours, (int)minutes);
    }
    text_layer_set_text(s_health_layer, s_health_buffer);
  } else {
    text_layer_set_text(s_health_layer, "N/A");
  }
} 

static void update_sleep(){
  last_time_asleep = time(NULL);
  update_metric(HealthMetricSleepSeconds);
}

static void update_steps(){
  if(time(NULL) - last_time_asleep > SECONDS_PER_HOUR){
    update_metric(HealthMetricStepCount);
  }
}

void load_health(GRect bounds, Layer *window_layer){
  // Create the TextLayer with specific bounds
  s_health_layer = text_layer_create(
    GRect(0, 130, bounds.size.w, 40));

  // Improve the layout to be more like a watchface
  s_health_font = fonts_load_custom_font(fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  text_layer_set_background_color(s_health_layer, GColorClear);
  text_layer_set_text_color(s_health_layer, GColorBlack);
  text_layer_set_font(s_health_layer, s_health_font);
  text_layer_set_text_alignment(s_health_layer, GTextAlignmentCenter);

  last_time_asleep = 0;
  update_metric(HealthMetricSleepSeconds);
  update_metric(HealthMetricStepCount);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_health_layer));
}

void health_handler(HealthEventType event, void *context) {
  // Which type of event occured?
  switch(event) {
    case HealthEventSignificantUpdate:
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Received HealthEventSignificantUpdate: %d", HealthEventSignificantUpdate);
    //update_sleep();
    update_metric(HealthMetricSleepSeconds);
    update_steps();
    break;
    case HealthEventMovementUpdate:
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Received HealthEventMovementUpdate: %d", HealthEventMovementUpdate);
    update_steps();
    break;
    case HealthEventSleepUpdate:
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Received HealthEventSleepUpdate: %d", HealthEventSleepUpdate);
    update_sleep();
    break;
  }
}

void unload_health(){
  text_layer_destroy(s_health_layer);
  fonts_unload_custom_font(s_health_font);
}