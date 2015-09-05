#pragma once

#include <pebble.h>

#define STRAP_BUFFER_SIZE 256

void strap_init();

void strap_request_data();

void ping_arduino();


void inbox_received_handler(DictionaryIterator *iterator, void *context);
