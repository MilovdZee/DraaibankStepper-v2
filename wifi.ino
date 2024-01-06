void setup_wifi() {
  SettingsClass settings = get_settings();
  if (settings.ssid[0] == 0) return;

  int bufSize = 128;
  char status[bufSize];
  snprintf(status, bufSize, "Connecting to\n#ffff00 %s#\n.", settings.ssid);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(settings.ssid, settings.wifi_password);
  int max_delay = 20;
  while (WiFi.status() != WL_CONNECTED && max_delay-- > 0) {
    // Update screen
    show_status_label(status);
    delay_for_millis(1000);
    strncat(status, ".", sizeof(status));
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect to WiFi");
    snprintf(status, bufSize, "Failed to connect to\n#ffff00 %s#", settings.ssid);
    show_status_label(status);
  } else {
    String ipAddress = WiFi.localIP().toString();
    Serial.printf("Connected to WiFi: %s\n", ipAddress);
    snprintf(status, bufSize, "Connected to\n#ffff00 %s#\n#00ffff %s#", settings.ssid, ipAddress);
    show_status_label(status);
  }

  delay_for_millis(2000);
  hide_status_label();
}