# WHAT, WHY AND HOW?

I had a lot of frustration trying to run both painlessMesh and BLE at the same time,
hitting errors of size limit on platformio, and not being able to target specific
versions of ESP-IDF which add BLE support. In general, I found the platformio too unstable
to be usable in a real world project just yet.


This repo includes everything that you might need to build and run an esp32 with BLE and painlessMesh,
from ESP-IDF and its compiler to the libraries itself. You can use it as a boilerplate.

## HOW TO USE IT

* Clone this repo and execute `git submodule update --init --recursive` to fetch all the files. The esp-idf repo is quite big, so it might take some time.
* Run `make menuconfig` to personalize your build. I've pre-selected some options, but you can configure it as you'd like. Please note, I've set it up for my board. You might need to change it.
* Run `make` to build everything. If you'd like to flash, run `make flash`. If you'd like to check the serial output, use `make monitor`. If you want to combine everything, use `make flash monitor`. You can also add -jN to speed up the compilation by using parallel jobs. The final command would be: `make -j4 flash monitor` (where 4 is your number of cores).

NOTE: On menuconfig, most probably you'll need to change the python path. I'm using python2, since that's what's available in my OS. If python 2 is your default, you might need to change it to just `python`.

## HOW IT WORKS

The general idea is that you're using ESP-IDF directly, and adding the Arduino layer as a component.

Then each library is added as a component. The `component.mk` files define how it is added/included/compiled.

The main code is at `main/main.cpp`. If you want to use ESP-IDF APIs instead of executing Arduino, disable the Arduino autostart with `make menuconfig`.


## A NOTE ON VERSIONS

* `vendor/esp-idf` is pinned to `release/v2.1`, which is the last known version to work well with painlessMesh.
* `xtensa-esp32-elf` is currently at version 1.22.0-61, which you can get [here](https://dl.espressif.com/dl/xtensa-esp32-elf-linux64-1.22.0-61-gab8375a-5.2.0.tar.gz). This specific version is needed for ESP-IDF 2.1. If you plan to update ESP-IDF, you'll need to update the compiler too.
* `arduino` is pinned to commit [d36e9bb](https://github.com/espressif/arduino-esp32/commit/d36e9bb), which is the latest version I could find that works with esp-idf 2.1 and BLE. The arduino repo for esp32 doesn't have versions yet, so I had to use a specific commit. If you find one that is better / newer, please tell me.
* `ArduinoJson` is at the version 5.11.2, which can be downloaded from [here](https://github.com/bblanchon/ArduinoJson/releases). Adding it as a submodule gave some errors at compile time, and since it's distributed as a single header file, I decided to add it precompiled.
* `ESP32-BLE-Arduino` is at version 0.4.2. You can get the original files from [here](http://www.arduinolibraries.info/libraries/esp32-ble-arduino).
* `painlessMesh` is at master (as of the time of writing). You can update it or pin it to a release tag without any problem.