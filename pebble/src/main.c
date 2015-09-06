/**
 * App to test the ArduinoPebbleSerial library
 */

#include <pebble.h>
#include "strap.h"
#include "windows/main_window.h"


static void init() {
  strap_init();
  main_window_push();
  app_comm_set_sniff_interval(SNIFF_INTERVAL_REDUCED);
}

static void deinit() { }

int main() {
  init();
  app_event_loop();
  deinit();
}
