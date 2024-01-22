#pragma once

#include "config.h"

namespace main {
    static uint32_t beat_timer = 0, buzzer_timer = 0;

    void scanI2C() {
        Wire.begin();

        byte error, address;
        int nDevices;
        
        Serial.println("Scanning for I2C devices...");
        nDevices = 0;
        
        for(address = 1; address < 127; address++ ) {
            Wire.beginTransmission(address);
            error = Wire.endTransmission();
            if (error == 0) {
                Serial.print("I2C device found at address 0x");
                if (address < 16) Serial.print("0");
                Serial.print(address,HEX);
                Serial.println("  !");
                nDevices++;
            } else if (error==4) {
                Serial.print("Unknown error at address 0x");
                if (address < 16) Serial.print("0");
                Serial.println(address,HEX);
            }
        }
        
        if (nDevices == 0) {
            Serial.println("No I2C devices found\n");
        } else {
            Serial.println("done\n");
        }
        
        delay(5000);
    }

    void init() {
        pinMode(LED_PIN, OUTPUT);
        digitalWrite(LED_PIN, LOW);
        pinMode(PUSHBTN_PIN, INPUT_PULLUP);
        pinMode(BUZZER_PIN, OUTPUT);
        digitalWrite(BUZZER_PIN, LOW);
        pinMode(WAKEUP_PIN, INPUT_PULLUP);
        pinMode(BATTERY_LEVEL_PIN, INPUT);

        esp_sleep_enable_ext0_wakeup(WAKEUP_PIN, 0);
    }

    void showStatus() {
        config::display.setCursor(10, 1);
        config::display.print(config::connection_on ? "EN RED": "  SOLO");
    }

    void dataReceived (uint8_t* address, uint8_t* data, uint8_t len, signed int rssi, bool broadcast) {
        char incoming[16];
        config::connection_on = true;
        snprintf(incoming, len + 1, "%s", data);
        
        if (strcmp(incoming, "4,CONN") == 0) {
            config::beat_back_timer = millis();
            showStatus();
        }

        if (strcmp(incoming, "4,START") == 0) {
            config::laps = 0;	
            config::bigNumber.displayLargeInt(config::laps, BIG_DIGIT_X_OFFSET, BIG_DIGIT_Y_OFFSET, BIG_DIGIT_DIGITS, BIG_DIGIT_LEADING);
        }

        #ifdef DEBUG
        Serial.printf ("ESPNOW received: %s\n", incoming);
        #endif
    }

    void dataSent(uint8_t* address, uint8_t status) {
        if (status == ESP_NOW_SEND_SUCCESS && config::sending_laps) {
            config::buzzing = true;
            config::sending_laps = false;
            main::buzzer_timer = millis();
            digitalWrite(BUZZER_PIN, HIGH);
        }
    }

    void wf_reset() {
        WiFi.persistent(false);
        WiFi.disconnect();
        WiFi.mode(WIFI_OFF);
    }

    /* void promiscuous_rx_cb(void *buf, wifi_promiscuous_pkt_type_t type) {
        // All espnow traffic uses action frames which are a subtype of the mgmnt frames so filter out everything else.
        if (type != WIFI_PKT_MGMT) return;
        
        const wifi_promiscuous_pkt_t *ppkt = (wifi_promiscuous_pkt_t *)buf;
        const wifi_ieee80211_packet_t *ipkt = (wifi_ieee80211_packet_t *)ppkt->payload;
        const wifi_ieee80211_mac_hdr_t *hdr = &ipkt->hdr;

        config::rssi_value = ppkt->rx_ctrl.rssi;
    } */

    void init_espnow() {
        wf_reset();

        WiFi.mode(WIFI_STA);

        uint8_t primary_channel = ESPNOW_CHANNEL;
        wifi_second_chan_t secondary_channel = WIFI_SECOND_CHAN_NONE;
        esp_wifi_set_promiscuous(true);
        if (esp_wifi_set_channel(primary_channel, secondary_channel) == ESP_OK) {
            #ifdef DEBUG
            Serial.printf("Canal ESPNOW cambiado a %i\n", ESPNOW_CHANNEL);
            #endif
        } else {
            #ifdef DEBUG
            Serial.printf("ERROR al cambiar canal\n");
            #endif
        }
        esp_wifi_set_promiscuous(false);

        quickEspNow.onDataRcvd(dataReceived);
        quickEspNow.onDataSent(dataSent);
        quickEspNow.begin(ESPNOW_CHANNEL);

        #ifdef DEBUG
        Serial.println("ESP-NOW inicializado");
        Serial.println(WiFi.localIP());
        Serial.println(WiFi.channel());
        Serial.println(WiFi.macAddress());
        // Serial.println(WiFi.softAPmacAddress());
        // Serial.printf("AP %s inicializado en %u.%u.%u.%u\n", WIFI_SSID, ip & 0xff, (ip >> 8) & 0xff, (ip >> 16) & 0xff, ip >> 24);
        #endif

        // Para control RSSI
        // esp_wifi_set_promiscuous(true);
        // esp_wifi_set_promiscuous_rx_cb(&promiscuous_rx_cb);
    }

    void init_display() {
        Wire.begin();
        
        config::display.init(21, 22);
        config::display.backlight();

        config::bigNumber.begin();

        showStatus();

        #ifdef DEBUG
        Serial.println("Display LCD: OK");
        #endif
    }

    void beat() {
        if (millis() - beat_timer >= BEAT_FREQ) {
            if (!config::buzzing) {
                config::sending_laps = false;
                const String payload = ((String)config::device_id + ",BEAT");
                quickEspNow.send(config::espnow_gateway, (uint8_t*)payload.c_str(), 6);
                
                #ifdef DEBUG
                // Serial.printf("SN %i\n", config::rssi_value);
                #endif
            }

            beat_timer = millis();
        }

        if (millis() - config::beat_back_timer >= BEAT_BACK_FREQ) {
            config::connection_on = false;
            showStatus();
        }

        if (millis() - config::backlight_timer >= BACKLIGHT_OFF_PERIOD) {
            config::display.noBacklight();
            config::backlight_timer = millis();
        }
    }

    void buzz() {
        if (config::buzzing && millis() - buzzer_timer >= BUZZER_NOTIF_FREQ) {
            config::buzzing = false;
            digitalWrite(BUZZER_PIN, LOW);
            buzzer_timer = millis();
        }
    }

    void get_preferences() {
        config::preferences.begin("lapcounter", false);
        config::device_id = config::preferences.getUInt("device_id", 1);

        if (!digitalRead(WAKEUP_PIN)) {
            while (!digitalRead(WAKEUP_PIN)) {
                config::device_id < 3 ? config::device_id++: config::device_id = 1;
                config::display.setCursor(14, 0);
                config::display.printf("P%d", config::device_id);
                config::preferences.putUInt("device_id", config::device_id);
                delay(1500);
            }
        } else {
            config::display.setCursor(14, 0);
            config::display.printf("P%d", config::device_id);
        }
        config::bigNumber.displayLargeInt(config::laps, BIG_DIGIT_X_OFFSET, BIG_DIGIT_Y_OFFSET, BIG_DIGIT_DIGITS, BIG_DIGIT_LEADING);

        config::preferences.end();
    }

    void show_battery_status() {
        int battery_reading = analogRead(BATTERY_LEVEL_PIN);
        float battery_reading_converted = (battery_reading * 3.3) / 4096;
        float voltage_reading = (battery_reading_converted * 4.2) / 3.3;

        config::display.clear();
        config::display.setCursor(0, 0);
        config::display.print(voltage_reading <= 3.3 ? "BATERIA RECARGAR": voltage_reading <= 3.7 ? "BATERIA BIEN": "BATERIA OPTIMA");

        config::display.setCursor(0, 1);
        config::display.print(String(voltage_reading) + "V");

        #ifdef DEBUG
        Serial.println(voltage_reading);
        #endif

        delay(2000);
    }
}