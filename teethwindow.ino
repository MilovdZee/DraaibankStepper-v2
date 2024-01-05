#define CLOSE_BTN 1
#define BACK_BTN 3
#define FORWARD_BTN 4

#define MAX_TEETH 500

static lv_obj_t *teeth_win;

static lv_obj_t *teeth_count_spinner;

static long positions[MAX_TEETH + 1];
static int current_tooth;

static void teeth_window_header_event_handler(lv_event_t *event) {
  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_CLICKED) {
    lv_obj_t *obj = lv_event_get_target(event);
    int btnId = lv_obj_get_index(obj);
    Serial.printf("Button %d clicked: ", btnId);
    switch (btnId) {
      case CLOSE_BTN:
        Serial.println("Close button");
        lv_obj_del_async(teeth_win);
        break;
      default:
        Serial.println("Unknown button");
    }
  }
}

void recalc_teeth_positions() {
  int teeth = lv_spinbox_get_value(teeth_count_spinner);
  SettingsClass settings = get_settings();
  float steps_per_tooth = (float)settings.steps_per_revolution / (float)teeth;
  Serial.printf("Number of steps per tooth: %f\n", steps_per_tooth);

  // Fill the array with all tooth positions
  for (int i = 0; i < teeth; i++) {
    positions[i] = steps_per_tooth * i;
  }
  positions[teeth] = settings.steps_per_revolution;

  // Always reset after changing the number of teeth
  current_tooth = 0;
  stepper.setCurrentPosition(0);
}

static void teeth_increment_event_cb(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
    lv_spinbox_increment(teeth_count_spinner);
    recalc_teeth_positions();
  }
}

static void teeth_decrement_event_cb(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
    lv_spinbox_decrement(teeth_count_spinner);
    recalc_teeth_positions();
  }
}

static void teeth_window_button_handler(lv_event_t *event) {
  int teeth = lv_spinbox_get_value(teeth_count_spinner);

  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_CLICKED) {
    if(stepper.isRunning()) return;

    lv_obj_t *obj = lv_event_get_target(event);
    int btnId = lv_obj_get_index(obj);
    Serial.printf("Button %d clicked: ", btnId);
    switch (btnId) {
      case BACK_BTN:
        current_tooth--;
        if (current_tooth < 0) {
          current_tooth = teeth - 1;
          stepper.setCurrentPosition(positions[teeth]);
        }

        Serial.printf("BACKWARD to tooth %d on position %ld\n", current_tooth, positions[current_tooth]);
        stepper.moveTo(positions[current_tooth]);
        break;
      case FORWARD_BTN:
        current_tooth++;
        if (current_tooth > teeth) {
          current_tooth = 1;
          stepper.setCurrentPosition(0);
        }

        Serial.printf("FORWARD to tooth %d on position %ld\n", current_tooth, positions[current_tooth]);
        stepper.moveTo(positions[current_tooth]);
        break;
      default:
        Serial.println("Unknown button");
    }
  }
}

void teeth_window() {
  teeth_win = lv_win_create(get_screen_main(), HEADER_HEIGHT);
  lv_win_add_title(teeth_win, "Teeth");
  lv_obj_t *btn = lv_win_add_btn(teeth_win, LV_SYMBOL_CLOSE, 30);
  lv_obj_add_event_cb(btn, teeth_window_header_event_handler, LV_EVENT_CLICKED, NULL);

  create_teeth_count_spinner();
  int button_width = SCREEN_WIDTH / 2 - 30;
  button(teeth_win, "BACK", button_width, 70, -button_width / 2 - 10, 100, teeth_window_button_handler);
  button(teeth_win, "FORWARD", button_width, 70, button_width / 2 + 10, 100, teeth_window_button_handler);
}

void create_teeth_count_spinner() {
  lv_obj_t *cont = lv_win_get_content(teeth_win);
  teeth_count_spinner = lv_spinbox_create(cont);
  lv_spinbox_set_range(teeth_count_spinner, 1, MAX_TEETH);
  lv_spinbox_set_digit_format(teeth_count_spinner, 3, 0);
  lv_spinbox_step_prev(teeth_count_spinner);
  lv_obj_set_width(teeth_count_spinner, 70);
  lv_obj_align(teeth_count_spinner, LV_ALIGN_TOP_MID, 0, 5);

  int32_t h = lv_obj_get_height(teeth_count_spinner);

  lv_obj_t *btn = lv_btn_create(cont);
  lv_obj_set_size(btn, h, h);
  lv_obj_align_to(btn, teeth_count_spinner, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
  lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_PLUS, 0);
  lv_obj_add_event_cb(btn, teeth_increment_event_cb, LV_EVENT_ALL, NULL);

  btn = lv_btn_create(cont);
  lv_obj_set_size(btn, h, h);
  lv_obj_align_to(btn, teeth_count_spinner, LV_ALIGN_OUT_LEFT_MID, -5, 0);
  lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_MINUS, 0);
  lv_obj_add_event_cb(btn, teeth_decrement_event_cb, LV_EVENT_ALL, NULL);
}
