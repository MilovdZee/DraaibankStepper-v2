#define CHECK_ADDR 0                                                         // char[10]
#define SSID_ADDR (CHECK_ADDR + sizeof(eeprom_check))                        // char[60]
#define WIFI_PASSWORD_ADDR (SSID_ADDR + sizeof(ssid))                        // char[60]
#define BRIGHTNESS_ADDR (WIFI_PASSWORD_ADDR + sizeof(wifi_password))         // int
#define MAXSPEED_ADDR (BRIGHTNESS_ADDR + sizeof(brightness))                 // int
#define ACCELERATION_ADDR (MAXSPEED_ADDR + sizeof(max_speed))                // int
#define STEPS_PER_REVOLTION_ADDR (ACCELERATION_ADDR + sizeof(acceleration))  // long

// MACRO : Combines FROM_BYTES into TO_OBJECT of datatype "DATATYPE"
#define combine_bytes(FROM_BYTES, DATATYPE, TO_OBJECT) TO_OBJECT = *((DATATYPE *)FROM_BYTES);

// MACRO : Splits FROM_DATA of DATATYPE into equivalent byte array TO_BYTES
#define split_bytes(FROM_DATA, DATATYPE, TO_BYTES) *((DATATYPE *)TO_BYTES) = FROM_DATA;

Preferences preferences;

SettingsClass settings;

void read_settings() {
  // For the ESP the flash has to be read to a buffer
  preferences.begin("Stepper", false);

  int settingsSize = preferences.getBytesLength("settings");
  Serial.printf("'settings' data structure has size: %d\n", settingsSize);

  if (!preferences.getBytes("settings", &settings, sizeof(settings))) {
    Serial.println("ERROR: Failed to read preferences from EEPROM!");
    return;
  }

  Serial.printf("settings.eeprom_check: '%s'\n", settings.eeprom_check);
  if (strncmp(settings.eeprom_check, EEPROM_CHECK_STRING, sizeof(EEPROM_CHECK_STRING)) == 0) {
    Serial.println("EEPROM data found");
  } else {
    Serial.println("EEPROM data NOT found");
    settings = SettingsClass();
  }
}

void write_settings(SettingsClass newSettings) {
  preferences.begin("Stepper", false);
  if (!preferences.putBytes("settings", &newSettings, sizeof(newSettings))) {
    Serial.println("ERROR: Failed to write preferences to EEPROM!");
    return;
  }
  settings = newSettings;
}

SettingsClass get_settings() {
  return settings;
}
