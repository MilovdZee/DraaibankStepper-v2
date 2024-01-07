# README #

To make gears it is nice to have equipment that does the steps needed for the number of teeth in 
the gear. This sketch calculates the steps and allows you to just press the 'FORWARD' 
button to go to the next tooth.

It is also possible to do steps based on the entered angle.

<img src="https://raw.githubusercontent.com/MilovdZee/DraaibankStepper-v2/main/images/main_screen.jpeg" width=200/> <img src="https://raw.githubusercontent.com/MilovdZee/DraaibankStepper-v2/main/images/teeth_screen.jpeg" width=200/> <img src="https://raw.githubusercontent.com/MilovdZee/DraaibankStepper-v2/main/images/degrees_screen.jpeg" width=200/>

## hardware ##
This is a sketch for the following:
- ESP32-2432S028 (ESP32, display combo)
- Hanpose CL57 (but any stepper driver will work)

## software ##
After the initial load the firmware checks for updates on github. If the device does not have internet access but does have wifi then a local OTA update can also be done.

Steps:
- install python
- download [espota.py](https://github.com/esp8266/Arduino/blob/master/tools/espota.py)
- download the latest `.bin` file from [firmware](https://github.com/MilovdZee/DraaibankStepper-v2/tree/main/firmware)
- setup wifi on the device and note the `ip address`
- run `python espota.py -r -i <ip address> --auth=DraaibankStepperPassword -f <firmware file>`

If there is no wifi then an update through USB is also possible (no python needed):
- download [esptool](https://github.com/espressif/esptool) (there are zip files under [releases](https://github.com/espressif/esptool/releases))
- download the latest `.bin` file from [firmware](https://github.com/MilovdZee/DraaibankStepper-v2/tree/main/firmware)
- connect the device through USB and note the `USB port` used
- run `esptool --chip esp32 --port "<USB port>" --baud 921600  --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size 4MB 0x10000 "<firmware file>"`

## setup ##
Copy the file `lv_conf.h`
1. simply next to the `lvgl` library folder
2. or any other places and
   - define `LV_CONF_INCLUDE_SIMPLE`
   - add the path as include path

## wiring ##
There are sockets on the board but there is only one plug with wiring supplied with the display. I opted for soldering directly on the board.

Steps and direction:

<img src="https://raw.githubusercontent.com/MilovdZee/DraaibankStepper-v2/main/images/Pulse_and_direction_1.jpeg" height=300/> <img src="https://raw.githubusercontent.com/MilovdZee/DraaibankStepper-v2/main/images/Pulse_and_direction_2.jpeg" height=300/>

Power:

<img src="https://raw.githubusercontent.com/MilovdZee/DraaibankStepper-v2/main/images/Power_1.jpeg" height=300/> <img src="https://raw.githubusercontent.com/MilovdZee/DraaibankStepper-v2/main/images/Power_2.jpeg" height=300/>