# CONTROL LINE F2C TIMMING 2024
## ESPNOW LAPCOUNTER

## This proyect is related with Aeromodelling, Control Line Racing class F2C.

### Elements needed:
- ESP32 based development board with external 2.4 antenna.
- 16x2 LCD display with I2C adapter.
- 12x12 pushbutton or alternative endstop for la counting.
- 12x12 pushbutton for deepsleep wakeup.
- 1S LiPo or 18650 battery.
- XL6009 stepup module or similar replacement.
- TP4056 1S charger.
- 10 mm cellphone vibrator module.
- 8x8 momentary push switch for on-off.

### Features:
- LCD Display based (this kind of display is the only one fully visible in different conditions, including intense direct sunlight, despite backlight active or not (suggested backlight active)).
- For better readability, the laps are shown with big numbers.
- The same button is used to count laps, reset to 0 (with a long press) and enter deepsleep mode (with a longer press).
- Vibrates when counting / reset laps, to confirmate without the need to see the counter.
- Can be used standalone, or network connected via ESPNow Espressif protocol.
- Can be put in deepsleep mode for battery saving.

### Operation:
- Can be charged with any cellphone charger available, using the microusb/c-type adpater of the TP4056. For a more robust option, a different connector can be soldered instead.
- When powered, it will try to connect to the ESPNow gateway mac address defined in config. If successfull, it will output a NET ON message in the LCD and be ready to send ESPNow messages; otherwise it will show the word SOLO, but still be able to work as standalone.
- Any press of the pushbutton will count laps up, with no limit, vibrating inmediatelly with every hit.
- If the counter is network connected, the vibration will confirm that the new lap message has already been sent to the gateway.
- When desired, the counter can be resetted by a longpress of the button (2 secs), to start the count all over again.
- To save battery between heats, put the lapcounter into deepsleep mode, just keep pressing the button after the laps reset, until the screen light goes off.
- To wakeup the counter, press the little button in the back.