#define SETUP_BTN 1
#define TEETH_BTN 0
#define DEGREES_BTN 1

static void main_window_header_event_handler(lv_event_t *event) {
  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_CLICKED) {
    lv_obj_t *obj = lv_event_get_target(event);
    int btnId = lv_obj_get_index(obj);
    Serial.printf("Button %d clicked: ", btnId);
    switch (btnId) {
      case SETUP_BTN:
        Serial.println("Setup button");
        setup_window(true);
        break;
      default:
        Serial.println("Unknown button");
    }
  }
}

static void main_window_content_event_handler(lv_event_t *event) {
  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_CLICKED) {
    lv_obj_t *obj = lv_event_get_target(event);
    int btnId = lv_obj_get_index(obj);
    Serial.printf("Button %d clicked: ", btnId);
    switch (btnId) {
      case TEETH_BTN:
        Serial.println("Teeth button");
        teeth_window();
        break;
      case DEGREES_BTN:
        Serial.println("Degrees button");
        degrees_window();
        break;
      default:
        Serial.println("Unknown button");
    }
  }
}

void main_window() {
  lv_obj_t *win = lv_win_create(get_screen_main(), HEADER_HEIGHT);
  lv_obj_t *cont = lv_win_get_content(win);

  lv_win_add_title(win, "DraaiStepper");
  lv_obj_t *btn = lv_win_add_btn(win, LV_SYMBOL_SETTINGS, 30);
  lv_obj_add_event_cb(btn, main_window_header_event_handler, LV_EVENT_CLICKED, NULL);

  button(win, "Teeth", SCREEN_WIDTH / 2, 70, 0, 10, main_window_content_event_handler);
  button(win, "Degrees", SCREEN_WIDTH / 2, 70, 0, 110, main_window_content_event_handler);
}
