#pragma once

#include <pebble.h>

#define MAIN_WINDOW_STATE_HEIGHT 20

void main_window_push();

void main_window_show_message(int key);

void main_window_update_name(char* value);

void main_window_update_gender(char* value);

void main_window_init_moves();
