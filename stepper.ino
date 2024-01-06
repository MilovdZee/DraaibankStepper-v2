void setup_stepper() {
  SettingsClass settings = get_settings();
  Serial.println("Configuring stepper driver:");
  Serial.printf("  - max_speed    : %d\n", settings.max_speed);
  Serial.printf("  - acceleration : %d\n", settings.acceleration);
  stepper.setMaxSpeed(settings.max_speed);
  stepper.setAcceleration(settings.acceleration);
  stepper.setPinsInverted(false, false, false);
  stepper.setMinPulseWidth(100); // minimal 100us pulsewidth
}

AccelStepper get_stepper() {
  return stepper;
}