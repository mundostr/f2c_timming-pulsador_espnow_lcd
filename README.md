# CONTROL LINE F2C TIMMING 2024
## LAP COUNTER

## This proyect is related with Aeromodelling, Control Line Racing class F2C.

### Elements needed:
- ESP32 based development board with external 2.4 antenna.
- 16x2 LCD display with I2C adapter.
- 12x12 pushbutton or alternative end stop.
- 1S LiPo or 18650 battery.
- XL6009 stepup module or similar replacement.
- TP4056 1S charger.
- 10 mm cellphone vibrator module.
- 6x6 momentary push switch for on-off.

### Features:
- LCD Display based (this kind of display is the only one fully visible in different conditions, including intense direct sunlight).
- For better readability, the laps are shown with big numbers.
- The same pushbutton is used to count laps and reset to 0 (with a long press).
- Can be used standalone, or network connected via ESPNow Espressif protocol.
- Vibrates when counting / reset laps, to confirmate without the need to see the counter.

### Operation:
- Can be charged with any cellphone charger available, using the microusb/c-type adpater of the TP4056. For a more robust option, a different connector can be soldered instead.
- When powered, it will try to connect to the ESPNow gateway mac address defined in config. If successfull, it will output a CONN message in the LCD, otherwise it will show the word SOLO, but in this case it could be used standalone just to count.
- Any press of the pushbutton will count laps up, with no limit, vibrating inmediatelly after.
- If the counter is network connected, the vibration will confirm that the new lap message has already been sent to the gateway.
- When desired, the counter can be resetted by a longpress of the pushbutton, to start the count all over again.