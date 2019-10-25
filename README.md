## Feeduino - A Wifi Enabled fish feeder

### Components:

- Arduino Nano or compatible
- ESP-01 Wifi module (ESP 8266 based board)
- 16x2 LCD Display (1602 LCD module)
- 10k potentiometer for display contrast setting (optional, can be replaced by a resistor on the final board as long as the power supply characteristics don't change)
- SG90 9G Servomotor (180 degree servo)
- 3.3V voltage regulator for powering ESP-01 board
- voltage divider for converting Arduino 5V signal pin to 3.3V (I used a 2k and a 1k resistor)
- piezzo buzzer for sound notifications

### Functionality overview:

- the board is configured to trigger automatic feeding cycles by counting milliseconds since the last feed. In case the power is reset, the counter will reset as well. This could be circumvented by using an RTC module and storing the last feed timestamp in EEPROM
- the total number of feeds since last reset is stored in EEPROM (for tracking when to refill the container)
- feeding cycles can be triggered manually as well. The initial prototype provisioned for physical buttons on the device as well, however they were later dropped to simplify the circuit. Manual feeds can be triggered using Wifi calls to the device
- Also using wifi, the user can toggle auto feeds on or off
- Also using the wifi interface, the sound notifications can be toggled on or off, as well as resetting the feed counter
- The LCD will display the following information:
  - a boot splash with the current firmware version
  - wifi connection status
  - a current status screen with the current state (number of feeds, uptime, auto and sound modes states)

### Wifi Communication Description:

MQTT is used for all operations. The SSID, password, MQTT broker IP address, user and password are defined in a `secrets.h` library file that is gitignored.
Every minute, and also after any state change, such as a feed being triggered, a number of MQTT topics are automatically updated:

- `home/feeder_beta/state` tracks the current feed counter
- `home/feeder_beta/attrs` is a topic tracking all current state variables (feed counter, firmware version, auto and sound modes)
- `home/feeder_beta/message` a topic updated on boot and last feed with a status text message
- `home/feeder_beta/control` devices subscribes to this topic for monitoring requests. Valid commands are `start_feed`, `toggle_auto`, `toggle_audio`, `reset_feeds`

Overall, the MQTT system was created so that it would integrate with Home Assistant and use it's services and Lovelace UI for exposing a visual interface.
  

### Schematic: TODO

### Breadboard and perfboard mounting pictures: TODO

### Attributions:
 - Using `pitches.h` and melodies from original Arduino Examples
