#include <stdlib.h>
#include <string.h>
#include <LovyanGFX.hpp>
#include <lvgl.h>
#include <Preferences.h>
#include <WiFi.h>
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

  read_settings();

  setup_screen();
  setup_touch();
  char *ssid = get_settings().ssid;
  if (ssid[0] != 0) {
    setup_wifi();
    setup_ntp();
    setupStepper();
  } else {
    show_status_label("Not configured");
    delay_for_millis(2000);
    hide_status_label();
  }

  Serial.println("Setup finished");

  main_window();

  pinMode(STEPPER_PULSE_PIN, OUTPUT);
}

long position = 0;
void loop() {
  delay_for_millis(2000);
}

void delay_for_millis(int delay) {
  unsigned long start = millis();
  while (millis() - start < delay) {
    lv_timer_handler();
    stepper.run();
  }
}
