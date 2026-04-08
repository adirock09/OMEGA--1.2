
#include QMK_KEYBOARD_H

enum layers {
    _L1,
    _L2,
    _L3,
    _L4
};

static bool input_mode = false;
static bool startup_checked = false;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_L1] = {
    {KC_COPY, KC_PASTE, KC_DEL},
    {KC_CUT,  KC_UNDO,  KC_AGAIN},
    {KC_ESC,  KC_TAB,   KC_ENT},
    {KC_NO,   KC_NO,    KC_NO}
},

[_L2] = {
    {LGUI(KC_R), KC_NO, KC_NO},
    {KC_NO, KC_NO, KC_NO},
    {KC_NO, KC_NO, KC_NO},
    {KC_NO, KC_NO, KC_NO}
},

[_L3] = {
    {LCTL(KC_Z), LCTL(KC_Y), LCTL(KC_S)},
    {LCTL(KC_C), LCTL(KC_V), KC_DEL},
    {KC_NO, KC_NO, KC_NO},
    {KC_NO, KC_NO, KC_NO}
},

[_L4] = {
    {LCTL(KC_D), LCTL(KC_E), LCTL(KC_B)},
    {KC_NO, KC_NO, KC_NO},
    {KC_NO, KC_NO, KC_NO},
    {KC_NO, KC_NO, KC_NO}
}

};

void matrix_scan_user(void) {
    if (!startup_checked) {
        startup_checked = true;
        wait_ms(50);

        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
            if (matrix_get_row(row)) {
                input_mode = true;
                break;
            }
        }
    }
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    switch (get_highest_layer(layer_state)) {

        case _L1:
            if (clockwise) tap_code(KC_BRIU);
            else tap_code(KC_BRID);
            break;

        case _L2:
            if (clockwise) tap_code(KC_VOLU);
            else tap_code(KC_VOLD);
            break;

        case _L3:
            if (clockwise) tap_code16(LCTL(KC_EQL));
            else tap_code16(LCTL(KC_MINS));
            break;

        case _L4:
            if (clockwise) tap_code(KC_MS_WH_UP);
            else tap_code(KC_MS_WH_DOWN);
            break;
    }
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    if (!input_mode) {
        return false;
    }

    if (record->event.pressed) {
        if (keycode == KC_COPY) {
            uint8_t current = get_highest_layer(layer_state);
            layer_move((current + 1) % 4);
            return false;
        }
    }
    return true;
}

#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_0;
}

bool oled_task_user(void) {

    oled_clear();
    oled_set_cursor(0, 0);

    uint8_t layer = get_highest_layer(layer_state);

    for (uint8_t i = 0; i < 4; i++) {
        if (i == layer) {
            oled_write_P(PSTR("> "), false);
        } else {
            oled_write_P(PSTR("  "), false);
        }

        oled_write_char('1' + i, false);
        oled_write_ln_P(PSTR(""), false);
    }

    oled_set_cursor(0, 6);

    if (input_mode) {
        oled_write_ln_P(PSTR("INPUT"), false);
    } else {
        oled_write_ln_P(PSTR("OUTPUT"), false);
    }

    return false;
}

#endif
