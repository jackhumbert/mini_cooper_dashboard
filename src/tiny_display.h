#include "widget.hpp"
#include <esp_now.h>
#include <ESP32Time.h>

// Structure example to send data
// Must match the receiver structure
typedef struct peer_data_t {
  float outside_temp;
  unsigned long time;
} peer_data_t;

void setup_peer_data(void);
peer_data_t * get_peer_data(void);
esp_err_t send_peer_data(void);

