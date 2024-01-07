#include <stdlib.h>
#include <string.h>
#include <LovyanGFX.hpp>
#include <lvgl.h>
#include <Preferences.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoOTA.h>
#include <Update.h>
#include "esp_sntp.h"
#include <AccelStepper.h>

#include "lgfx_ESP32_2432S028.h"
#include "config.h"

AccelStepper stepper(AccelStepper::DRIVER, STEPPER_PULSE_PIN, STEPPER_DIR_PIN);

static lv_style_t style_status_label;
static lv_style_t style_default;
static lv_style_t style_small;

void setup() {
  Serial.begin(115200);
  Serial.println("Setup started");

  // Switch off the RED color of the RGB LED
  pinMode(RGB_RED_PIN, OUTPUT);
  digitalWrite(RGB_RED_PIN, HIGH);

  read_settings();

  setup_screen();
  setup_touch();
  if (check_validity(get_settings())) {
    setup_wifi();
    setup_ota();
    setup_ntp();
    setup_stepper();

    main_window();
  } else {
    show_status_label("Not configured", true);
    setup_window(false);
  }

  Serial.println("Setup finished");

  int newest_version = get_update_version();
  if(newest_version != ERROR_VALUE) read_firmware(newest_version);
}

long position = 0;
boolean previous_running_state = false;
void loop() {
  ArduinoOTA.handle();
  lv_timer_handler_run_in_period(5);
  stepper.run();

  if (stepper.isRunning() != previous_running_state) {
    if (!previous_running_state) {
      Serial.println("Disabling buttons...");
      add_state_on_all_objects_of_class(get_screen_main(), &lv_btn_class, LV_STATE_DISABLED);
      previous_running_state = true;
    } else {
      Serial.println("Enabling buttons...");
      clear_state_on_all_objects_of_class(get_screen_main(), &lv_btn_class, LV_STATE_DISABLED);
      previous_running_state = false;
    }
  }
}

void delay_for_millis(int delay) {
  unsigned long start = millis();
  while (millis() - start < delay) {
    lv_timer_handler_run_in_period(5);
    stepper.run();
    ArduinoOTA.handle();
  }
}
