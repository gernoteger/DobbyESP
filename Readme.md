# About
DobbyESP is my own ESP8266 based IoT application for controlling things.

The name is inspired by [Dobby, the houself](http://harrypotter.wikia.com/wiki/Dobby) from Harry Potter: serves behind the scenes, but eventually develops a life on its own.

# Requirements
(mostly not yet implemented)
* [ ] enable maintainance without physical access
* [ ] Robust Wifi configuration
* [ ] OTA Updates
* [ ] Admin WebInterface
* [ ] Basic security
* [ ] MQTT for interfacing with OpenHab and crowd control
* [ ] OTA logging
* Devices + sensors
 * [ ] roller blinds / sunblinds
 * [ ] sprinkler with rain sensor
 * [ ] anything that can be switched on or off
 * [ ] Room Heating control & sensing
* make it publicly available, therefore no private acess data must be stored anywhere in version controlled files

 Since the ESP8266 is a Wifi-Only system, it doesn't lend itself well to battery powered operation; this is subject to future research
 
# Architecture

## Software Stack
* [Sming Framework](https://github.com/anakod/Sming "Sming on github")
* [rBoot] (https://github.com/raburton/esp8266 "rBoot on github") for OTA with big flash layout
* spiffs for saving connection information. I originally intended to use the original [xlfe] (https://github.com/xlfe/spiffy "original spiffy") repo; I noticed that the changes needed for Sming are not yet there, so I used [kireefco's fork] (https://github.com/kireevco/spiffy)
* and of course Espressif's SDK, currently V1.2
* Toolchain using chocolatey from sming

## Hardware

* ESP-12 and ESP-07: 4MBit/1MBit Flash, plenty of pins available, including the ADC
* Line powered: off-the-shelf USB Power modules with linear 3.3V controller
* Use Arduino relais modules to switch line: 5V form USB above come in handy, can be controlled with 3,3V from ESP
* as little as possible else

## Project Layout
The TopLevel Project contains all dependencies as git submodules, plus DobbyESP's base directory. 

# Build targets

make ...

* clean
* all

make -f Makefile-sming.mk ...
* sming-clean
* sming-all


* rboot-stuff

