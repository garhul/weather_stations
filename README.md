# Weather Stations

## what is this?
This is a small project using an esp8266 and a bunch of extra components to make a standalone weather station reporting humidity, pressure and temperature to a Mosquitto broker on a regular interval.
The actual device runs on a lipo battery and uses deepsleep in order to preserve power.


## Configuration & setup
There's an endpoint for updating eeprom settings /info which needs the following fields:

-ssid
-pass
-ap_name
-etc