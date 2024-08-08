#include "tiny_display.h"
#include "messages.h"
#include <WiFi.h>
#include "sd_card.h"

bool usePeer = false;

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = { 0x84, 0xfc, 0xe6, 0x52, 0x20, 0xfc };

// Create a struct_message called peer_data
peer_data_t peer_data;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    if (status != ESP_NOW_SEND_SUCCESS) {
        add_message_fmt("Peer Delivery Failed");
    }
}

peer_data_t * get_peer_data(void) {
    if (usePeer && !sd_card_is_logging()) {
        return &peer_data;
    } else {
        return nullptr;
    }
}

esp_err_t send_peer_data(void) {
    if (usePeer && !sd_card_is_logging()) {
        return esp_now_send(broadcastAddress, (uint8_t *)&peer_data, sizeof(peer_data));
    } else {
        return 0;
    }
}

void setup_peer_data(void) {
    if (usePeer)
        return;

    // Set device as a Wi-Fi Station
    WiFi.useStaticBuffers(true);
    WiFi.mode(WIFI_STA);

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        add_message_fmt("Error initializing ESP-NOW");
    } else {
        // Once ESPNow is successfully Init, we will register for Send CB to
        // get the status of Transmitted packet
        esp_now_register_send_cb(OnDataSent);
        
        // Register peer
        memcpy(peerInfo.peer_addr, broadcastAddress, 6);
        peerInfo.channel = 0;  
        peerInfo.encrypt = false;

        // Add peer        
        if (esp_now_add_peer(&peerInfo) != ESP_OK){
            add_message_fmt("Failed to add peer");
        } else {
            usePeer = true;
        }
    }

    if (usePeer) {
        esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&peer_data, sizeof(peer_data));
    }
}