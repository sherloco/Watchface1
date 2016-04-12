#include <pebble.h>
#include "window/background.h"

static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;

void load_background(GRect bounds, Layer *window_layer){
  // Create GBitmap
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND_EXAMPLE);

  // Create BitmapLayer to display the GBitmap
  s_background_layer = bitmap_layer_create(bounds);

  // Set the bitmap onto the layer and add to the window
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));
}

void unload_background(){
  gbitmap_destroy(s_background_bitmap);
  bitmap_layer_destroy(s_background_layer);
}