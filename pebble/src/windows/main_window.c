#include "main_window.h"
#include "../strap.h"

#define KEY_UP 1
#define KEY_DOWN 2
#define KEY_LEFT 3
#define KEY_RIGHT 4
#define KEY_UP_LEFT 5
#define KEY_UP_RIGHT 6
#define KEY_DOWN_LEFT 7
#define KEY_DOWN_RIGHT 8

static Window *s_main_window;
static TextLayer *s_connected_state_layer, *s_bytes_read_layer,
                 *s_error_rate_layer, *s_notif_count_layer;
static TextLayer *s_hw_layer;
static int s_up_presses;

static int s_moves_up, s_moves_down, s_moves_left, s_moves_right;

static TextLayer *s_move_up_layer;
static TextLayer *s_move_down_layer;
static TextLayer *s_move_left_layer;
static TextLayer *s_move_right_layer;

static TextLayer* text_layer_init(GRect bounds, char* font, GTextAlignment align, GColor color) {
  TextLayer *this = text_layer_create(bounds);
  text_layer_set_font(this, fonts_get_system_font(font));
  text_layer_set_text_alignment(this, align);
  text_layer_set_text_color(this, color);
  text_layer_set_background_color(this, GColorClear);
  layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(this));
  return this;
}

void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // s_connected_state_layer = text_layer_init(
  //   GRect(0, 0, bounds.size.w, MAIN_WINDOW_STATE_HEIGHT),
  //   FONT_KEY_GOTHIC_18_BOLD, GTextAlignmentCenter, GColorWhite);
  //
  // s_bytes_read_layer = text_layer_init(
  //   GRect(3, MAIN_WINDOW_STATE_HEIGHT, bounds.size.w, 28),
  //   FONT_KEY_GOTHIC_24_BOLD, GTextAlignmentLeft, GColorWhite);
  //
  // s_error_rate_layer = text_layer_init(
  //   GRect(3, (2 * MAIN_WINDOW_STATE_HEIGHT), bounds.size.w, 28),
  //   FONT_KEY_GOTHIC_24_BOLD, GTextAlignmentLeft, GColorWhite);
  //
  // s_notif_count_layer = text_layer_init(
  //   GRect(3, (3 * MAIN_WINDOW_STATE_HEIGHT), bounds.size.w, 28),
  //   FONT_KEY_GOTHIC_24_BOLD, GTextAlignmentLeft, GColorWhite);

  s_hw_layer = text_layer_init(
      GRect(3, (0 * MAIN_WINDOW_STATE_HEIGHT), bounds.size.w, 28),
      FONT_KEY_GOTHIC_18_BOLD, GTextAlignmentCenter, GColorWhite);

  s_move_up_layer = text_layer_init(
      GRect(3, (1 * MAIN_WINDOW_STATE_HEIGHT), bounds.size.w, 28),
      FONT_KEY_GOTHIC_18_BOLD, GTextAlignmentCenter, GColorWhite);

  s_move_down_layer = text_layer_init(
      GRect(3, (2 * MAIN_WINDOW_STATE_HEIGHT), bounds.size.w, 28),
      FONT_KEY_GOTHIC_18_BOLD, GTextAlignmentCenter, GColorWhite);

  s_move_left_layer = text_layer_init(
      GRect(3, (3 * MAIN_WINDOW_STATE_HEIGHT), bounds.size.w, 28),
      FONT_KEY_GOTHIC_18_BOLD, GTextAlignmentCenter, GColorWhite);

  s_move_right_layer = text_layer_init(
      GRect(3, (4 * MAIN_WINDOW_STATE_HEIGHT), bounds.size.w, 28),
      FONT_KEY_GOTHIC_18_BOLD, GTextAlignmentCenter, GColorWhite);

  // Init UI
  // main_window_set_connected_state(false);
  // main_window_set_bytes_read(0);
  // main_window_set_error_rate(0, 0);
  // main_window_set_notif_count(0);
  main_window_init_moves();
}

void main_window_unload(Window *window) {
  text_layer_destroy(s_connected_state_layer);
  text_layer_destroy(s_bytes_read_layer);
  text_layer_destroy(s_error_rate_layer);
  text_layer_destroy(s_notif_count_layer);

  window_destroy(s_main_window);
}

void main_window_up_click_handler(ClickRecognizerRef recognizer, void *context) {
  s_up_presses++;
  static char s_buffer[128];
  snprintf(s_buffer, sizeof(s_buffer), "Up presses:%d", s_up_presses);
  text_layer_set_text(s_hw_layer, s_buffer);
  ping_arduino();
}

void main_window_show_message(int key) {
  switch(key) {
    case KEY_UP:
      text_layer_set_background_color(s_move_up_layer, GColorGreen);
      text_layer_set_background_color(s_move_down_layer, GColorRed);
      text_layer_set_background_color(s_move_left_layer, GColorRed);
      text_layer_set_background_color(s_move_right_layer, GColorRed);
      s_moves_up++;
      break;
    case KEY_DOWN:
      text_layer_set_background_color(s_move_up_layer, GColorRed);
      text_layer_set_background_color(s_move_down_layer, GColorGreen);
      text_layer_set_background_color(s_move_left_layer, GColorRed);
      text_layer_set_background_color(s_move_right_layer, GColorRed);
      s_moves_down++;
      break;
    case KEY_UP_LEFT:
      text_layer_set_background_color(s_move_up_layer, GColorGreen);
      text_layer_set_background_color(s_move_down_layer, GColorRed);
      text_layer_set_background_color(s_move_left_layer, GColorGreen);
      text_layer_set_background_color(s_move_right_layer, GColorRed);
      s_moves_up++;
      s_moves_left++;
      break;
    case KEY_UP_RIGHT:
      text_layer_set_background_color(s_move_up_layer, GColorGreen);
      text_layer_set_background_color(s_move_down_layer, GColorRed);
      text_layer_set_background_color(s_move_left_layer, GColorRed);
      text_layer_set_background_color(s_move_right_layer, GColorGreen);
      s_moves_up++;
      s_moves_right++;
      break;
    case KEY_DOWN_LEFT:
      text_layer_set_background_color(s_move_up_layer, GColorRed);
      text_layer_set_background_color(s_move_down_layer, GColorGreen);
      text_layer_set_background_color(s_move_left_layer, GColorGreen);
      text_layer_set_background_color(s_move_right_layer, GColorRed);
      s_moves_down++;
      s_moves_left++;
      break;
    case KEY_DOWN_RIGHT:
      text_layer_set_background_color(s_move_up_layer, GColorRed);
      text_layer_set_background_color(s_move_down_layer, GColorGreen);
      text_layer_set_background_color(s_move_left_layer, GColorRed);
      text_layer_set_background_color(s_move_right_layer, GColorGreen);
      s_moves_down++;
      s_moves_right++;
      break;
    default:
      APP_LOG(APP_LOG_LEVEL_INFO, "Unknown key: %d", key);
      break;
  }

  main_window_init_moves();
}

void click_config_provider(void *context) {
  // Register the ClickHandlers
  window_single_click_subscribe(BUTTON_ID_UP, main_window_up_click_handler);
}

void main_window_push() {
  if(!s_main_window) {
    s_main_window = window_create();

    // Register callbacks
    app_message_register_inbox_received(inbox_received_handler);

    // Open AppMessage
    app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

    window_set_background_color(s_main_window, GColorBlack);
    window_set_window_handlers(s_main_window, (WindowHandlers) {
      .load = main_window_load,
      .unload = main_window_unload,
    });

    window_set_click_config_provider(s_main_window, click_config_provider);
  }
  window_stack_push(s_main_window, true);
}

void main_window_set_connected_state(bool is_connected) {
  text_layer_set_text(s_connected_state_layer, is_connected ? "CONNECTED" : "NOT CONNECTED");
  text_layer_set_background_color(s_connected_state_layer, is_connected ? GColorGreen : GColorRed);
}

void main_window_set_bytes_read(int bytes) {
  static char s_buff[32];
  snprintf(s_buff, sizeof(s_buff), "Read: %dB", bytes);
  text_layer_set_text(s_bytes_read_layer, s_buff);
}

void main_window_set_error_rate(int errors, int total) {
  static char s_buff[32];
  snprintf(s_buff, sizeof(s_buff), "Errors: %d/%d", errors, total);
  text_layer_set_text(s_error_rate_layer, s_buff);
}

void main_window_set_notif_count(int notification_count) {
  static char s_buff[32];
  snprintf(s_buff, sizeof(s_buff), "Notifs: %d", notification_count);
  text_layer_set_text(s_notif_count_layer, s_buff);
}

void main_window_init_moves() {
  static char s_buffer1[128];
  static char s_buffer2[128];
  static char s_buffer3[128];
  static char s_buffer4[128];

  snprintf(s_buffer1, sizeof(s_buffer1), "MOVE UP: %d", s_moves_up);
  snprintf(s_buffer2, sizeof(s_buffer2), "MOVE DOWN: %d", s_moves_down);
  snprintf(s_buffer3, sizeof(s_buffer3), "MOVE LEFT: %d", s_moves_left);
  snprintf(s_buffer4, sizeof(s_buffer4), "MOVE RIGHT: %d", s_moves_right);

  text_layer_set_text(s_move_up_layer, s_buffer1);
  text_layer_set_text(s_move_down_layer, s_buffer2);
  text_layer_set_text(s_move_left_layer, s_buffer3);
  text_layer_set_text(s_move_right_layer, s_buffer4);
}
