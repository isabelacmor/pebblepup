#include <pebble.h>
#include "strap.h"
#include "windows/main_window.h"

#define KEY_UP 1
#define KEY_DOWN 2
#define KEY_LEFT 3
#define KEY_RIGHT 4
#define KEY_UP_LEFT 5
#define KEY_UP_RIGHT 6
#define KEY_DOWN_LEFT 7
#define KEY_DOWN_RIGHT 8
#define KEY_UPDATE_NAME 9
#define KEY_UPDATE_BOTH 10
#define KEY_SHOW_OFF 11

static int s_bytes_read = 0, s_errors = 0, s_notifs = 0;

// Declare an attribute pointer
SmartstrapAttribute *attribute;

static char* smartstrap_result_to_string(SmartstrapResult result) {
  switch(result) {
    case SmartstrapResultOk:                   return "SmartstrapResultOk";
    case SmartstrapResultInvalidArgs:          return "SmartstrapResultInvalidArgs";
    case SmartstrapResultNotPresent:           return "SmartstrapResultNotPresent";
    case SmartstrapResultBusy:                 return "SmartstrapResultBusy";
    case SmartstrapResultServiceUnavailable:   return "SmartstrapResultServiceUnavailable";
    case SmartstrapResultAttributeUnsupported: return "SmartstrapResultAttributeUnsupported";
    case SmartstrapResultTimeOut:              return "SmartstrapResultTimeOut";
    default: return "Not a SmartstrapResult value!";
  }
}

void ping_arduino(char* key) {
  // Declare a buffer to be used
  size_t buff_size;
  uint8_t *buffer;

  // Begin the write request, getting the buffer and its length
  smartstrap_attribute_begin_write(attribute, &buffer, &buff_size);

  // Store the data to be written to this attribute
  snprintf((char*)buffer, buff_size, key);

  // End the write request, and send the data, expecting a response
  SmartstrapResult r = smartstrap_attribute_end_write(attribute, strlen((char*)buffer), false);
  if(r != SmartstrapResultOk) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "---Smartstrap error: %s", smartstrap_result_to_string(r));
  }
}

void inbox_received_handler(DictionaryIterator *iterator, void *context) {
  // Get the first pair
  Tuple *t = dict_read_first(iterator);

  // Process all pairs present
  while(t != NULL) {
    // Process this pair's key
    switch(t->key) {
      case KEY_UP:
        // Trigger up movement
        main_window_show_message(KEY_UP);
        ping_arduino("KEY_UP");
        break;
      case KEY_DOWN:
        // Trigger down movement
        main_window_show_message(KEY_DOWN);
        ping_arduino("KEY_DOWN");
        break;
      case KEY_UP_LEFT:
        // Trigger up left movement
        main_window_show_message(KEY_UP_LEFT);
        ping_arduino("KEY_UP_LEFT");
        break;
      case KEY_UP_RIGHT:
        // Trigger up right movement
        main_window_show_message(KEY_UP_RIGHT);
        ping_arduino("KEY_UP_RIGHT");
        break;
      case KEY_DOWN_LEFT:
        // Trigger down left movement
        main_window_show_message(KEY_DOWN_LEFT);
        ping_arduino("KEY_DOWN_LEFT");
        break;
      case KEY_DOWN_RIGHT:
        // Trigger down right movement
        main_window_show_message(KEY_DOWN_RIGHT);
        ping_arduino("KEY_DOWN_RIGHT");
        break;
      case KEY_LEFT:
        // Trigger left movement
        main_window_show_message(KEY_LEFT);
        ping_arduino("KEY_LEFT");
        break;
      case KEY_RIGHT:
        // Trigger right movement
        main_window_show_message(KEY_RIGHT);
        ping_arduino("KEY_RIGHT");
        break;
      case KEY_SHOW_OFF:
        APP_LOG(APP_LOG_LEVEL_INFO, "showing off");
        ping_arduino("KEY_SHOW_OFF");
        break;
      case KEY_UPDATE_NAME:
        APP_LOG(APP_LOG_LEVEL_INFO, "name to: %s", t->value->cstring);
        main_window_update_name(t->value->cstring);
        break;
      case KEY_UPDATE_BOTH:
        APP_LOG(APP_LOG_LEVEL_INFO, "both to: %s", t->value->cstring);
        main_window_update_gender(t->value->cstring);

        if(strncmp("BOY", t->value->cstring, 3) == 0) {
          ping_arduino("KEY_BOY");
        } else {
          ping_arduino("KEY_GIRL");
        }
        break;
      default:
        APP_LOG(APP_LOG_LEVEL_INFO, "Unknown key: %d", (int)t->key);
        break;
    }

    // Get next pair, if any
    t = dict_read_next(iterator);
  }
}

static void strap_availability_handler(SmartstrapServiceId service_id, bool is_available) {
  // A service's availability has changed
  APP_LOG(APP_LOG_LEVEL_INFO, "Service %d is %s available", (int)service_id, is_available ? "now" : "NOT");

  // If the raw service is available, the strap is connected
  main_window_set_connected_state(is_available && service_id == SMARTSTRAP_RAW_DATA_SERVICE_ID);
}

static void strap_read_handler(SmartstrapAttribute *attribute, SmartstrapResult result, const uint8_t *data, size_t length) {
  if(result == SmartstrapResultOk) {
    // Update counter
    s_bytes_read += length;

    // Read the data
    APP_LOG(APP_LOG_LEVEL_INFO, "Smartstrap says: %s", (char*)data);
  } else {
    s_errors++;
    APP_LOG(APP_LOG_LEVEL_ERROR, "Error receiving data from smartstrap!");
  }
}

static void strap_notify_handler(SmartstrapAttribute *attribute) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Smartstrap sent notification.");
  s_notifs++;
  main_window_set_notif_count(s_notifs);
}

void strap_init() {
  // Create the attribute, and allocate a buffer for its data
  attribute = smartstrap_attribute_create(SMARTSTRAP_RAW_DATA_SERVICE_ID, SMARTSTRAP_RAW_DATA_ATTRIBUTE_ID, STRAP_BUFFER_SIZE);

  smartstrap_subscribe((SmartstrapHandlers) {
    .availability_did_change = strap_availability_handler,
    .did_read = strap_read_handler,
    .notified = strap_notify_handler
  });
}

void strap_request_data() {
  // // Declare a buffer to be used
  // size_t buff_size;
  // uint8_t *buffer;
  //
  // // Begin the write request, getting the buffer and its length
  // smartstrap_attribute_begin_write(attribute, &buffer, &buff_size);
  //
  // // Store the data to be written to this attribute
  // snprintf((char*)buffer, buff_size, "Hello, smartstrap!");
  //
  // // End the write request, and send the data, expecting a response
  // SmartstrapResult r = smartstrap_attribute_end_write(attribute, strlen((char*)buffer), true);
  // if(r != SmartstrapResultOk) {
  //   //APP_LOG(APP_LOG_LEVEL_ERROR, "Smartstrap error: %s", smartstrap_result_to_string(r));
  // }
}
