#define CLOSE_BTN 1
#define BACK_BTN 3
#define FORWARD_BTN 4

#define MAX_DEGREES 360
#define MAX_DEGREES_PER_BTN_PRESS MAX_DEGREES
#define DEGREE_DIVIDER 10

#define POSITIONS 1000

lv_obj_t *degrees_win;
lv_obj_t *degrees_spinner;
lv_obj_t *current_degree_step_label;

static long degrees_positions[POSITIONS];
static int current_degree_step = POSITIONS / 2;

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

void recalc_degrees_positions() {
  float degrees_per_btn_press = (float)lv_spinbox_get_value(degrees_spinner) / (float)DEGREE_DIVIDER;

  SettingsClass settings = get_settings();
  float steps_per_revolution = (float)settings.steps_per_revolution;
  float steps_per_btn_press = steps_per_revolution * degrees_per_btn_press / 360.0;
  Serial.printf("Number of steps per btn press: %.1f\n", steps_per_btn_press);

  // Fill the array with all tooth positions for BACKWARD
  for (int i = 1; i <= POSITIONS / 2; i++) {
    degrees_positions[POSITIONS / 2 - i] = -steps_per_btn_press * i;
  }
  // Fill the array with all tooth positions for FORWARD
  for (int i = 0; i < POSITIONS / 2; i++) {
    degrees_positions[i + POSITIONS / 2] = steps_per_btn_press * i;
  }

  // Always reset after changing the number of teeth
  current_degree_step = POSITIONS / 2;
  stepper.setCurrentPosition(0);
  update_step_label();
}

static void degrees_increment_event_cb(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
    lv_spinbox_increment(degrees_spinner);
    recalc_degrees_positions();
  }
}

static void degrees_decrement_event_cb(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
    lv_spinbox_decrement(degrees_spinner);
    recalc_degrees_positions();
  }
}

static void degrees_window_button_handler(lv_event_t *event) {
  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_CLICKED) {
    if (stepper.isRunning()) return;

    if (lv_spinbox_get_value(degrees_spinner) == 0) {
      show_status_label("Degrees per step not set", true);
      return;
    }

    float degrees_per_btn_press = (float)lv_spinbox_get_value(degrees_spinner) / (float)DEGREE_DIVIDER;
    lv_obj_t *obj = lv_event_get_target(event);
    int btnId = lv_obj_get_index(obj);
    Serial.printf("Button %d clicked: ", btnId);
    long new_position;
    lv_obj_t *mbox;
    char buffer[BUFFER_SIZE];
    switch (btnId) {
      case BACK_BTN:
        if (current_degree_step == 0) {
          Serial.println("On first step");
          show_status_label("On first step", true);
          return;
        }
        current_degree_step--;
        new_position = degrees_positions[current_degree_step];
        Serial.printf("BACKWARD with %.1f degrees to position %ld for step %d\n", degrees_per_btn_press, new_position, current_degree_step);
        update_step_label();

        stepper.moveTo(new_position);
        break;
      case FORWARD_BTN:
        if (current_degree_step == POSITIONS - 1) {
          Serial.println("On last step");
          show_status_label("On last step", true);
          return;
        }
        current_degree_step++;
        new_position = degrees_positions[current_degree_step];
        Serial.printf("FORWARD with %.1f degrees to position %ld for step %d\n", degrees_per_btn_press, new_position, current_degree_step);
        update_step_label();
        stepper.moveTo(new_position);
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
  button(degrees_win, "BACK", button_width, 70, -button_width / 2 - 10, 70, degrees_window_button_handler);
  button(degrees_win, "FORWARD", button_width, 70, button_width / 2 + 10, 70, degrees_window_button_handler);
  current_degree_step_label = label(degrees_win, "-", 0, 160);

  recalc_degrees_positions();
}

void create_degrees_spinner() {
  lv_obj_t *cont = lv_win_get_content(degrees_win);

  degrees_spinner = lv_spinbox_create(cont);
  lv_spinbox_set_range(degrees_spinner, 0, MAX_DEGREES_PER_BTN_PRESS * DEGREE_DIVIDER);
  lv_spinbox_set_digit_format(degrees_spinner, 4, 4 - log10(DEGREE_DIVIDER));
  lv_spinbox_set_cursor_pos(degrees_spinner, 0);
  lv_spinbox_step_prev(degrees_spinner);
  lv_obj_set_width(degrees_spinner, 90);
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

void update_step_label() {
  char buffer[BUFFER_SIZE];
  float degrees_per_btn_press = (float)lv_spinbox_get_value(degrees_spinner) / (float)DEGREE_DIVIDER;
  float requested_degrees = degrees_per_btn_press * (float)(current_degree_step - POSITIONS / 2);
  float position = (float)degrees_positions[current_degree_step];
  SettingsClass settings = get_settings();
  float steps_per_revolution = (float)settings.steps_per_revolution;
  float real_degrees = position * 360.0 / steps_per_revolution;
  if(abs(real_degrees - requested_degrees) > .05) {
    snprintf(buffer, BUFFER_SIZE, "%d: %.1f (req %.1f)", current_degree_step - POSITIONS / 2, real_degrees, requested_degrees);
  } else {
    snprintf(buffer, BUFFER_SIZE, "%d: %.1f", current_degree_step - POSITIONS / 2, real_degrees);
  }
  lv_label_set_text(current_degree_step_label, buffer);
}
