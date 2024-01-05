#pragma once

#include "config.h"

namespace pushbtn {
	Button2 btn;

    void sendLaps() {
        config::sending_laps = true;
        const String payload = ((String)DEVICE_ID + "," + (String)config::laps);
        quickEspNow.send(config::espnow_gateway, (uint8_t*)payload.c_str(), 5);
    }

    void updateDisplay() {
        config::bigNumber.displayLargeInt(config::laps, BIG_DIGIT_X_OFFSET, BIG_DIGIT_Y_OFFSET, BIG_DIGIT_DIGITS, BIG_DIGIT_LEADING);
		main::beat_timer = millis();
    }

	void cb_pressed(Button2& btn) {
        config::btn_long_pressed = false;
	}

	void activate_buzz() {
		config::buzzing = true;
		main::buzzer_timer = millis();
		digitalWrite(BUZZER_PIN, HIGH);
	}

	void cb_longpressed(Button2& btn) {
        config::laps = 0;
        config::btn_long_pressed = true;
        config::beat_back_timer = millis();
        if (!config::connection_on) activate_buzz();
        sendLaps();
        updateDisplay();
        
        if (DEBUG) Serial.println("Limpia vueltas");

	}

	void cb_released(Button2& btn) {
		if (!config::buzzing && !config::btn_long_pressed) {
			config::laps++;
            if (!config::connection_on) activate_buzz();
            sendLaps();
            updateDisplay();
            config::beat_back_timer = millis();
		}
	}

	void init() {
		btn.begin(PUSHBTN_PIN);
		btn.setDebounceTime(PUSHBTN_DEBOUNCE_TIME);
		btn.setLongClickTime(PUSHBTN_LONG_CLICK_DURATION);
		btn.setPressedHandler(cb_pressed);
		btn.setReleasedHandler(cb_released);
		btn.setLongClickDetectedHandler(cb_longpressed);
	}
}
