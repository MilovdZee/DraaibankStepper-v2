#define CLOSE_BTN 1
#define BACK_BTN 3
#define FORWARD_BTN 4

#define MAX_DEGREES 360

static lv_obj_t *degrees_win;

static lv_obj_t *degrees_spinner;

static void degrees_window_header_event_handler(lv_event_t *event) {
  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_CLICKED) {
    lv_obj_t *obj = lv_event_get_target(event);
    int btnId = lv_obj_get_index(obj);
    Serial.printf("Button %d clicked: ", btnId);
    switch (btnId) {
      case CLOSE_BTN:
        Serial.println("Close button");
        lv_obj_del_async(degrees_win);
        break;
      default:
        Serial.println("Unknown button");
    }
  }
}

static void degrees_increment_event_cb(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
    lv_spinbox_increment(degrees_spinner);
  }
}

static void degrees_decrement_event_cb(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
    lv_spinbox_decrement(degrees_spinner);
  }
}

static void degrees_window_button_handler(lv_event_t *event) {
  int degrees = lv_spinbox_get_value(degrees_spinner);

  SettingsClass settings = get_settings();
  float steps_per_degree = (float)settings.steps_per_revolution / 360.0;

  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_CLICKED) {
    if(stepper.isRunning()) return;

    lv_obj_t *obj = lv_event_get_target(event);
    int btnId = lv_obj_get_index(obj);
    Serial.printf("Button %d clicked: ", btnId);
    switch (btnId) {
      case BACK_BTN:
        Serial.printf("BACKWARD with %d degrees\n", degrees);
        stepper.setCurrentPosition(0);
        stepper.moveTo(-steps_per_degree * degrees);
        break;
      case FORWARD_BTN:
        Serial.printf("FORWARD with %d degrees\n", degrees);
        stepper.setCurrentPosition(0);
        stepper.moveTo(steps_per_degree * degrees);
        break;
      default:
        Serial.println("Unknown button");
    }
  }
}

void degrees_window() {
  degrees_win = lv_win_create(get_screen_main(), HEADER_HEIGHT);
  lv_win_add_title(degrees_win, "Degrees");
  lv_obj_t *btn = lv_win_add_btn(degrees_win, LV_SYMBOL_CLOSE, 30);
  lv_obj_add_event_cb(btn, degrees_window_header_event_handler, LV_EVENT_CLICKED, NULL);

  create_degrees_spinner();
  int button_width = SCREEN_WIDTH / 2 - 30;
  button(degrees_win, "BACK", button_width, 70, -button_width / 2 - 10, 100, degrees_window_button_handler);
  button(degrees_win, "FORWARD", button_width, 70, button_width / 2 + 10, 100, degrees_window_button_handler);
}

void create_degrees_spinner() {
  lv_obj_t *cont = lv_win_get_content(degrees_win);
  degrees_spinner = lv_spinbox_create(cont);
  lv_spinbox_set_range(degrees_spinner, 1, 360);
  lv_spinbox_set_digit_format(degrees_spinner, 3, 0);
  lv_spinbox_step_prev(degrees_spinner);
  lv_obj_set_width(degrees_spinner, 70);
  lv_obj_align(degrees_spinner, LV_ALIGN_TOP_MID, 0, 5);

  int32_t h = lv_obj_get_height(degrees_spinner);

  lv_obj_t *btn = lv_btn_create(cont);
  lv_obj_set_size(btn, h, h);
  lv_obj_align_to(btn, degrees_spinner, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
  lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_PLUS, 0);
  lv_obj_add_event_cb(btn, degrees_increment_event_cb, LV_EVENT_ALL, NULL);

  btn = lv_btn_create(cont);
  lv_obj_set_size(btn, h, h);
  lv_obj_align_to(btn, degrees_spinner, LV_ALIGN_OUT_LEFT_MID, -5, 0);
  lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_MINUS, 0);
  lv_obj_add_event_cb(btn, degrees_decrement_event_cb, LV_EVENT_ALL, NULL);
}
