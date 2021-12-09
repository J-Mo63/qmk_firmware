/* Copyright 2021 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H


enum layers {
  MAC_BASE,
  MAC_FN,
  WIN_BASE,
  WIN_FN
};

enum custom_keycodes {
    WIN_CMD = SAFE_RANGE,
    MOUSE
};

#define KC_TASK LGUI(KC_TAB)
#define KC_FLXP LGUI(KC_E)

void switch_key(keyrecord_t* record, uint16_t from, uint16_t to)
{
    // Register and deregister switched keys in reverse order
    if (record->event.pressed)
    {
        unregister_code(from);
        register_code(to);
    }
    else
    {
        unregister_code(to);
        register_code(from);
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t* record)
{
    // Define states for FSM
    enum key_states {
        NONE_STATE,
        WIN_CMD_STATE,
        WIN_CMDTAB_STATE,
        MOUSE_STATE
    };

    // Set intial state
    static enum key_states state = NONE_STATE;

    // Define state transitions
    void enter_state_none(void)
    {
        unregister_code(KC_LCTL);
        unregister_code(KC_LALT);
        state = NONE_STATE;
    }

    void enter_state_cmd(void)
    {
        unregister_code(KC_LALT);
        register_code(KC_LCTL);
        state = WIN_CMD_STATE;
    }

    void enter_state_cmdtab(void)
    {
        unregister_code(KC_LCTL);
        register_code(KC_LALT);
        state = WIN_CMDTAB_STATE;
    }

    void enter_state_mouse(void)
    {
        unregister_code(KC_LCTL);
        unregister_code(KC_LALT);
        state = MOUSE_STATE;
    }

    // Run state update for key event
    switch (state)
    {
        case NONE_STATE:
            if (keycode == WIN_CMD && record->event.pressed)
            {
                enter_state_cmd();
            }
            else if (keycode == MOUSE && record->event.pressed)
            {
                enter_state_mouse();
            }
            break;
        case WIN_CMD_STATE:
            switch (keycode)
            {
                case WIN_CMD:
                    if (!record->event.pressed) enter_state_none();
                    break;
                case KC_LEFT:
                    switch_key(record, KC_LCTL, KC_HOME);
                    return false;
                case KC_RGHT:
                    switch_key(record, KC_LCTL, KC_END);
                    return false;
                case KC_UP:
                    switch_key(record, KC_LCTL, KC_PGUP);
                    return false;
                case KC_DOWN:
                    switch_key(record, KC_LCTL, KC_PGDN);
                    return false;
                case KC_TAB:
                    if (record->event.pressed) enter_state_cmdtab();
                    // Allow pass-through for tab
                    break;
            }
            break;
        case WIN_CMDTAB_STATE:
            if (keycode == WIN_CMD && !record->event.pressed)
            {
                enter_state_none();
            }
            else if (keycode != KC_TAB)
            {
                enter_state_cmd();
                return false;
            }
            break;
        case MOUSE_STATE:
            switch (keycode)
            {
                case MOUSE:
                    if (!record->event.pressed)
                    {
                        // Unregister all potential mouse events
                        unregister_code(KC_MS_LEFT);
                        unregister_code(KC_MS_RIGHT);
                        unregister_code(KC_MS_UP);
                        unregister_code(KC_MS_DOWN);
                        unregister_code(KC_MS_BTN1);
                        unregister_code(KC_MS_BTN2);
                        enter_state_none();
                    }
                    break;
                case KC_LEFT:
                    if (record->event.pressed) register_code(KC_MS_LEFT);
                    else unregister_code(KC_MS_LEFT);
                    return false;
                case KC_RGHT:
                    if (record->event.pressed) register_code(KC_MS_RIGHT);
                    else unregister_code(KC_MS_RIGHT);
                    return false;
                case KC_UP:
                    if (record->event.pressed) register_code(KC_MS_UP);
                    else unregister_code(KC_MS_UP);
                    return false;
                case KC_DOWN:
                    if (record->event.pressed) register_code(KC_MS_DOWN);
                    else unregister_code(KC_MS_DOWN);
                    return false;
                case KC_RCTL:
                    if (record->event.pressed) register_code(KC_MS_BTN1);
                    else unregister_code(KC_MS_BTN1);
                    return false;
                case KC_RSFT:
                    if (record->event.pressed) register_code(KC_MS_BTN2);
                    else unregister_code(KC_MS_BTN2);
                    return false;
            }
            break;
    }
    return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[MAC_BASE] = LAYOUT_ansi_82( 
     KC_ESC,             KC_BRID,  KC_BRIU,  KC_F3,    KC_F4,    RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_DEL,   KC_NO,
     KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_PGUP,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,            KC_PGDN,
     MOUSE,    KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,             KC_HOME,
     KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,
     KC_LCTL,  KC_LALT,  KC_LGUI,                                KC_SPC,                                 KC_RGUI, MO(MAC_FN),KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

[MAC_FN] = LAYOUT_ansi_82( 
     KC_TRNS,            KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_TRNS,  KC_TRNS,
     KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,
     RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,
     KC_TRNS,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,            KC_TRNS,
     KC_TRNS,            KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,  KC_TRNS, 
     KC_TRNS,  KC_TRNS,  KC_TRNS,                                KC_TRNS,                                KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS),

[WIN_BASE] = LAYOUT_ansi_82( 
     KC_ESC,             KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_DEL,   KC_NO,
     KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_PGUP,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,            KC_PGDN,
     MOUSE,    KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENT,             KC_HOME,
     KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,
     KC_LGUI,  KC_LALT,  WIN_CMD,                                KC_SPC,                                 KC_RALT, MO(WIN_FN),KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

[WIN_FN] = LAYOUT_ansi_82( 
     KC_TRNS,            KC_BRID,  KC_BRIU,  KC_TASK,  KC_FLXP,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_TRNS,  KC_TRNS,
     KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,
     RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,
     KC_TRNS,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,            KC_TRNS,
     KC_TRNS,            KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,  KC_TRNS, 
     KC_TRNS,  KC_TRNS,  KC_TRNS,                                KC_TRNS,                                KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS)

};