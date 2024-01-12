#pragma once

#include "config.h"

namespace pushbtn {
    Button2 btn;
    uint32_t manual_long_pressed_timer = 0;

    void sendLaps() {
        config::sending_laps = true;
        const String payload = ((String)config::device_id + "," + (String)config::laps);
        quickEspNow.send(config::espnow_gateway, (uint8_t*)payload.c_str(), 5);
    }

    void updateDisplay() {
        config::bigNumber.displayLargeInt(config::laps, BIG_DIGIT_X_OFFSET, BIG_DIGIT_Y_OFFSET, BIG_DIGIT_DIGITS, BIG_DIGIT_LEADING);
        main::beat_timer = millis();
    }

    void cb_pressed(Button2& btn) {
        config::display.backlight();
        config::btn_long_pressed = false;
        config::backlight_timer = millis();
    }

    void activate_buzz() {
        config::buzzing = true;
        main::buzzer_timer = millis();
        digitalWrite(BUZZER_PIN, HIGH);
    }

    void check_suspension() {
        if (config::btn_long_pressed) {
            if (pushbtn::btn.isPressed() && millis() - pushbtn::manual_long_pressed_timer >= PUSHBTN_LONG_CLICK_DURATION * 2) {
                config::btn_long_pressed = false;
                pushbtn::manual_long_pressed_timer = millis();
                config::display.noBacklight();

                esp_deep_sleep_start();
            }
        } else {
            pushbtn::manual_long_pressed_timer = millis();
        }
    }

    void cb_long_pressed_detected(Button2& btn) {
        config::laps = 0;
        config::btn_long_pressed = true;
        config::beat_back_timer = millis();
        pushbtn::manual_long_pressed_timer = millis();
        if (!config::connection_on) activate_buzz();
        sendLaps();
        updateDisplay();
        
        #ifdef DEBUG
        Serial.println("Limpia vueltas");
        #endif

    }

    void cb_released(Button2& btn) {
        if (!config::buzzing && !config::btn_long_pressed) {
            config::laps++;
            if (!config::connection_on) activate_buzz();
            sendLaps();
            updateDisplay();
            config::beat_back_timer = millis();
        } else {
            config::btn_long_pressed = false;
        }
    }

    void init() {
        btn.begin(PUSHBTN_PIN);
        btn.setDebounceTime(PUSHBTN_DEBOUNCE_TIME);
        btn.setLongClickTime(PUSHBTN_LONG_CLICK_DURATION);
        btn.setPressedHandler(cb_pressed);
        btn.setReleasedHandler(cb_released);
        btn.setLongClickDetectedHandler(cb_long_pressed_detected);
    }
}
