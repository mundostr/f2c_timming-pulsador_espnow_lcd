/*
https://github.com/gmag11/QuickEspNow
https://randomnerdtutorials.com/esp-now-two-way-communication-esp32/

Divisor de voltaje control batería: R1 (+) 27k, R2 (-) 100k
*/

#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <QuickEspNow.h>
#include <Button2.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include "BigNumbers_I2C.h"
#include <esp_sleep.h>
#include <Preferences.h>

// #define DEBUG

#define LED_PIN GPIO_NUM_2
#define PUSHBTN_PIN GPIO_NUM_12
#define BUZZER_PIN GPIO_NUM_18
#define WAKEUP_PIN GPIO_NUM_32
#define BATTERY_LEVEL_PIN GPIO_NUM_36

#define BEAT_FREQ 5000
#define BEAT_BACK_FREQ 5100
#define BACKLIGHT_OFF_PERIOD 10000
#define ESPNOW_CHANNEL 1
#define LED_NOTIF_FREQ 200
#define BUZZER_NOTIF_FREQ 100
#define SERIAL_CONSOLE_BAUDS 115200
#define PUSHBTN_DEBOUNCE_TIME 75
#define PUSHBTN_LONG_CLICK_DURATION 2000
#define BIG_DIGIT_X_OFFSET 0
#define BIG_DIGIT_Y_OFFSET 0
#define BIG_DIGIT_DIGITS 3
#define BIG_DIGIT_LEADING true

namespace config {
    int laps = 0;
    int device_id = 2;
    int rssi_value = 0;
    bool connection_on = false;
    bool buzzing = false;
    bool btn_long_pressed = false;
    bool sending_laps = false;
    uint32_t beat_back_timer = 0;
    uint32_t backlight_timer = 0;
    uint8_t espnow_gateway[] = { 0xB4, 0xE6, 0x2D, 0xE9, 0xFE, 0x6E }; // Gateway mac custom
    // uint8_t espnow_gateway[] = { 0x3C, 0x71, 0xBF, 0x4C, 0xF5, 0x8C }; // Gateway mac original

    LiquidCrystal_I2C display(0x27, 16, 2);
    BigNumbers_I2C bigNumber(&display);
    Preferences preferences;
}

/* typedef struct {
    unsigned frame_ctrl: 16;
    unsigned duration_id: 16;
    uint8_t addr1[6];
    uint8_t addr2[6];
    uint8_t addr3[6];
    unsigned sequence_ctrl: 16;
    uint8_t addr4[6];
} wifi_ieee80211_mac_hdr_t;

typedef struct {
    wifi_ieee80211_mac_hdr_t hdr;
    uint8_t payload[0];
} wifi_ieee80211_packet_t; */