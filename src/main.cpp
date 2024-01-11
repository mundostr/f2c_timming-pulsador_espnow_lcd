/*
CONTROL TIEMPOS VCC
Cliente ESP32, envía mensajes ESP-NOW a un AP ESP32. Se utiliza como cuentavueltas con display local
https://github.com/m1cr0lab-esp32/esp-now-network-and-wifi-gateway
https://www.survivingwithandroid.com/esp-now-esp32-esp8266/
https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino

ATENCION!: recordar borrar la flash completa
(esptool.py --port PUERTO erase_flash) en caso de problemas
*/

#include "config.h"
#include "main.h"
#include "pushbtn.h"

void setup() {
    #ifdef DEBUG
    Serial.begin(SERIAL_CONSOLE_BAUDS);
    #endif
    
    main::init();
    pushbtn::init();
    main::init_espnow();
    main::init_display();
    main::show_battery_status();
    main::get_preferences();
}

void loop() {
    pushbtn::btn.loop();
    pushbtn::check_suspension();
    main::beat();
    main::buzz();
}
