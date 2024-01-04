void setup_ota() {
  if (WiFi.status() != WL_CONNECTED) return;

  // Setup OTA updates
  ArduinoOTA.setPort(3232);  // default 3232

  char buffer[50];
  snprintf(buffer, sizeof(buffer), "%s_%d", OTA_HOSTNAME, random(9999));
  ArduinoOTA.setHostname(buffer);
  ArduinoOTA.setPassword(OTA_PASSWORD);

  ArduinoOTA.onStart([]() {
    Serial.println("OTA update start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("OTA update end");
  });
  ArduinoOTA.onProgress([](int progress, int total) {
    int percentage = progress * 100 / total;
    Serial.printf("OTA update progress: %u\r\n", percentage);
  });
  ArduinoOTA.onError([](ota_error_t error) {
    char* errorMessage = "Unknown";
    if (error == OTA_AUTH_ERROR) errorMessage = "Auth Failed";
    else if (error == OTA_BEGIN_ERROR) errorMessage = "Begin Failed";
    else if (error == OTA_CONNECT_ERROR) errorMessage = "Connect Failed";
    else if (error == OTA_RECEIVE_ERROR) errorMessage = "Receive Failed";
    else if (error == OTA_END_ERROR) errorMessage = "End Failed";
    Serial.printf("Error[%u]: %s\n", error, errorMessage);
  });
  ArduinoOTA.begin();
}
