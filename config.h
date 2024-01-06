#ifndef CONFIG_H
#define CONFIG_H

#define EEPROM_CHECK_STRING "EEPROM003"

/* Change to your screen resolution */
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define HEADER_HEIGHT 30

#define RGB_RED_PIN 4
#define RGB_GREEN_PIN 16
#define RGB_BLUE_PIN 17

#define STEPPER_PULSE_PIN RGB_GREEN_PIN
#define STEPPER_DIR_PIN RGB_BLUE_PIN

#define OTA_PASSWORD "DraaibankStepperPassword"
#define OTA_HOSTNAME "DraaibankStepper"

// NTP server
const char *ntpServer = "pool.ntp.org";

// Timezone strings from https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
// char *timezone = "PST8PDT,M3.2.0,M11.1.0";      char *timezone_text = "Berkeley";
// char *timezone = "MST7MDT,M3.2.0,M11.1.0";      char *timezone_text = "Denver";
// char *timezone = "CST6CDT,M3.2.0,M11.1.0";      char *timezone_text = "Chicago";
// char *timezone = "EST5EDT,M3.2.0,M11.1.0";      char *timezone_text = "New York";
// char *timezone = "GMT0BST,M3.5.0/1,M10.5.0";    char *timezone_text = "London";
// char *timezone = "CET-1CEST,M3.5.0,M10.5.0/3";  char *timezone_text = "Berlin";
char *timezone = "CET-1CEST,M3.5.0,M10.5.0/3";
char *timezone_text = "Amsterdam";
// char *timezone = "JST-9";                       char *timezone_text = "Tokyo";

class SettingsClass {
public:
  char eeprom_check[10];
  char ssid[60];
  char wifi_password[60];
  int brightness;
  int max_speed;
  int acceleration;
  long steps_per_revolution;

  SettingsClass() {
    eeprom_check[0] = 0;
    ssid[0] = 0;
    wifi_password[0] = 0;
    brightness = 50;
    max_speed = 5000;
    acceleration = 2000;
    steps_per_revolution = 800;
  };

  SettingsClass(const char *ssid, const char *wifi_password, int brightness, int max_speed, int acceleration, long steps_per_revolution) {
    strncpy(this->eeprom_check, EEPROM_CHECK_STRING, sizeof(this->eeprom_check));
    strncpy(this->ssid, ssid, sizeof(this->ssid));
    strncpy(this->wifi_password, wifi_password, sizeof(this->wifi_password));
    this->brightness = brightness;
    this->max_speed = max_speed;
    this->acceleration = acceleration;
    this->steps_per_revolution = steps_per_revolution;
  };
};

#endif