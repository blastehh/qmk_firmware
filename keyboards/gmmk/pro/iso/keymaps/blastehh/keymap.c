/* Copyright 2021 Glorious, LLC <salman@pcgamingrace.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include "print.h"

#define _HSV_CAPSLOCK_ENABLED_COLOR 0, 255 , 255
#define _HSV_GUILOCK_ENABLED_COLOR 0, 255 , 255
static bool gui_locked = false;
// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

//      ESC      F1       F2       F3       F4       F5       F6       F7       F8       F9       F10      F11      F12	     Prt           Rotary(Mute)
//      ~        1        2        3        4        5        6        7        8        9        0         -       (=)	     BackSpc           Del
//      Tab      Q        W        E        R        T        Y        U        I        O        P        [        ]                          PgUp
//      Caps     A        S        D        F        G        H        J        K        L        ;        "        #        Enter             PgDn
//      Sh_L     /        Z        X        C        V        B        N        M        ,        .        ?                 Sh_R     Up       End
//      Ct_L     Win_L    Alt_L                               SPACE                               Alt_R    FN       Ct_R     Left     Down     Right


        // The FN key by default maps to a momentary toggle to layer 1 to provide access to the RESET key (to put the board into bootloader mode). Without
        // this mapping, you have to open the case to hit the button on the bottom of the PCB (near the USB cable attachment) while plugging in the USB
        // cable to get the board into bootloader mode - definitely not fun when you're working on your QMK builds. Remove this and put it back to KC_RGUI
        // if that's your preference.
        //
        // To put the keyboard in bootloader mode, use FN+backspace. If you accidentally put it into bootloader, you can just unplug the USB cable and
        // it'll be back to normal when you plug it back in.
        //
        // This keyboard defaults to 6KRO instead of NKRO for compatibility reasons (some KVMs and BIOSes are incompatible with NKRO).
        // Since this is, among other things, a "gaming" keyboard, a key combination to enable NKRO on the fly is provided for convenience.
        // Press Fn+N to toggle between 6KRO and NKRO. This setting is persisted to the EEPROM and thus persists between restarts.
    [0] = LAYOUT(KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_DEL, KC_F13, KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC, KC_HOME, KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_PGUP, KC_CAPS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT, KC_PGDN, KC_LSFT, KC_NUBS, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_UP, KC_END, KC_LCTL, KC_LGUI, KC_LALT, KC_SPC, KC_RALT, MO(1), KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT),
    [1] = LAYOUT(RESET, KC_MYCM, KC_WHOM, KC_CALC, KC_MSEL, KC_MPRV, KC_MNXT, KC_MPLY, KC_MSTP, KC_MUTE, KC_VOLD, KC_VOLU, KC_TRNS, KC_PSCR, KC_TRNS, KC_TRNS, RGB_TOG, KC_F14, KC_F15, KC_F16, KC_F17, KC_F18, KC_F19, KC_F20, KC_F21, KC_F22, KC_F23, KC_F24, KC_TRNS, KC_INS, KC_TRNS, RGB_HUI, RGB_VAI, RGB_SAI, KC_TRNS, RGB_M_T, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RGB_HUD, RGB_VAD, RGB_SAD, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RGB_M_P, RGB_M_X, KC_TRNS, KC_TRNS, KC_TRNS, NK_TOGG, KC_TRNS, LGUI(LSFT(KC_LEFT)), LGUI(LSFT(KC_RGHT)), KC_TRNS, KC_TRNS, RGB_MOD, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RGB_SPD, RGB_RMOD, RGB_SPI)
};
// clang-format on

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (clockwise) {
        tap_code(KC_VOLU);
    } else {
        tap_code(KC_VOLD);
    }
    return true;
}
#endif

RGB get_rgb_from_hsv_values (int h, int s, int v){
    HSV hsv_color = {h, s, v};
    RGB rgb_result = hsv_to_rgb(hsv_color);
    return rgb_result;
}

// RGB LED layout

// led number, function of the key

//  68, Side led 01    0, ESC      6, F1       12, F2       18, F3       23, F4       28, F5       34, F6       39, F7       44, F8       50, F9       56, F10      61, F11      66, F12      70, Prt                   Rotary(Mute)   69, Side led 12
//  71, Side led 02    1, `~       7, 1        13, 2        19, 3        24, 4        29, 5        35, 6        40, 7        45, 8        51, 9        57, 0        62, -_       79, (=+)     86, BackSpc               73, Del        72, Side led 13
//  74, Side led 03    2, Tab      8, Q        14, W        20. E        25, R        30, T        36, Y        41, U        46, I        52, O        58, P        63, [{       90, ]}                                 76, PgUp       75, Side led 14
//  77, Side led 04    3, Caps     9, A        15, S        21, D        26, F        31, G        37, H        42, J        47, K        53, L        59, ;:       64, '"       95, #        97, Enter                 87, PgDn       78, Side led 15
//  81, Side led 05    4, Sh_L     67,\        10, Z        16, X        22, C        27, V        32, B        38, N        43, M        48, ,<       54, .<       60, /?                    91, Sh_R     94, Up       83, End        82, Side led 16
//  84, Side led 06    5, Ct_L     11,Win_L    17, Alt_L                                           33, SPACE                                           49, Alt_R    55, FN       65, Ct_R     96, Left     98, Down     80, Right      85, Side led 17
//  88, Side led 07                                                                                                                                                                                                                    89, Side led 18
//  92, Side led 08              

void rgb_matrix_indicators_user(void) {
    int caps_indicator_keys[] = { 3 };
        // Set Underglow of keyboard to a different color when capslock is enabled
        // Animation returns when not enabled
    if (host_keyboard_led_state().caps_lock) {
        RGB capslock_color_rgb = get_rgb_from_hsv_values(_HSV_CAPSLOCK_ENABLED_COLOR);
        for (uint8_t i = 0; i < DRIVER_LED_TOTAL; ++i) {
            if (HAS_ANY_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
                rgb_matrix_set_color(i, capslock_color_rgb.r, capslock_color_rgb.g, capslock_color_rgb.b);
            }
        }
        // Set capslock indicator keys
        for (int i = 0; i < (sizeof(caps_indicator_keys)/sizeof(caps_indicator_keys[0])); i++) {
            rgb_matrix_set_color(caps_indicator_keys[i], capslock_color_rgb.r, capslock_color_rgb.g, capslock_color_rgb.b);
        }
    }
    // Set gui lock indicator
    if (gui_locked) {
        RGB guilock_color_rgb = get_rgb_from_hsv_values(_HSV_GUILOCK_ENABLED_COLOR);
        rgb_matrix_set_color(11, guilock_color_rgb.r, guilock_color_rgb.g, guilock_color_rgb.b);
    }
}

static uint16_t end_held_timer;
static bool end_held;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static bool mo_held;
    static uint16_t sft_timer;

    switch (keycode) {
        case KC_END:
            if (record->event.pressed) {
                end_held_timer = timer_read();
                end_held = true;
                //register_code(KC_END); // Change the key to be held here
            } else {
                //unregister_code(KC_END); // Change the key that was held here, too!
                if (timer_elapsed(end_held_timer) < TAPPING_TERM) {
                    tap_code(KC_PENT); // send enter if tapped
                } else {
                    //tap_code(KC_END); // send end if held
                    unregister_code(KC_END);
                }
                end_held = false;
            }
            return false; // We handled this keypress

        case MO(1):
            if (record->event.pressed) {
                mo_held = true;
            } else {
                mo_held = false;
            }
            return true;

        case KC_LGUI:
            if (record->event.pressed) {
                if (mo_held) {
                    gui_locked = !gui_locked;
                    process_magic(GUI_TOG, record);
                    /* if (gui_locked) {
                        process_magic(GUI_ON, record);
                        gui_locked = false;
                    } else {
                        process_magic(GUI_OFF, record);
                        gui_locked = true;
                    } */
                    return false;
                }
                return true;
            }
            break;

        case KC_LSFT:
            if (record->event.pressed) {
                //print("shift pressed\n");
                if (sft_timer && (timer_elapsed(sft_timer) < 200)) {
                    //print("tapping caps\n");
                    tap_code(KC_CAPS);
                }
                sft_timer = timer_read();
            }
            return true;
    }
    return true; // We didn't handle other keypresses
}

void matrix_scan_user(void) {
    if (end_held) {
        if (timer_elapsed(end_held_timer) >= TAPPING_TERM) {
            register_code(KC_END);
            end_held = false;
        }
    }
}

/* 
Functions to suspend RGB when computer is off or sleeping
See: https://beta.docs.qmk.fm/using-qmk/hardware-features/lighting/feature_rgb_matrix#suspended-state-id-suspended-state
*/
void suspend_power_down_kb(void) {
    rgb_matrix_set_suspend_state(true);
    suspend_power_down_user();
}

void suspend_wakeup_init_kb(void) {
    rgb_matrix_set_suspend_state(false);
    suspend_wakeup_init_user();
}
