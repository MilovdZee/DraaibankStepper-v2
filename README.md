# README #

To make gears it is nice to have equipment that does the steps needed for the number of teeth in 
the gear. This sketch calculates the steps and allows you to just press the 'FORWARD' 
button to go to the next tooth.

It is also possible to do steps based on the entered angle.

## hardware ##
This is a sketch for the following:
- ESP32-2432S028 (ESP32, display combo)
- Hanpose CL57 (but any stepper driver will work)

## setup ##
Copy the file `lv_conf.h`
1. simply next to the `lvgl` library folder
2. or any other places and
   - define `LV_CONF_INCLUDE_SIMPLE`
   - add the path as include path
