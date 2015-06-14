#include <pebble.h>

static Window *window;
static TextLayer *time_layer;
static GFont beaver_font_56;

static void update_time() {
  time_t tmp = time(NULL);
  struct tm *tick_time = localtime(&tmp);

  static char buffer[] = "00:00";

  if (clock_is_24h_style() == true) {
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  // for screenshot
  //text_layer_set_text(time_layer, "06:10");

  text_layer_set_text(time_layer, buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  beaver_font_56 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_BEAVER_56));

  time_layer = text_layer_create((GRect) { .origin = { 0, 46 }, .size = { bounds.size.w, 58 } });
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  text_layer_set_font(time_layer, beaver_font_56);
  text_layer_set_text_color(time_layer, GColorCyan);
  text_layer_set_background_color(time_layer, GColorClear);

  layer_add_child(window_layer, text_layer_get_layer(time_layer));

  update_time();
}

static void window_unload(Window *window) {
  text_layer_destroy(time_layer);

  fonts_unload_custom_font(beaver_font_56);
}

static void init(void) {
  window = window_create();
  window_set_background_color(window, GColorDarkGray);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
