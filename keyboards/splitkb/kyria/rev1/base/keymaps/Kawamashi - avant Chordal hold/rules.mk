OLED_ENABLE = yes
ENCODER_ENABLE = no       # Enables the use of one or more encoders
RGB_MATRIX_ENABLE = no     # Disable keyboard RGB matrix, as it is enabled by default on rev3
RGBLIGHT_ENABLE = no      # Disable keyboard RGB underglow



#SPLIT_KEYBOARD = yes


#OLED_DRIVER = ssd1306   # Enables the use of OLED displays
#OLED_DRIVER_ENABLE = yes # Enables the use of OLED displays


LTO_ENABLE = yes

ENCODER_MAP_ENABLE = no

TAP_DANCE_ENABLE = no
DEFERRED_EXEC_ENABLE = no
KEY_OVERRIDE_ENABLE = no
CAPS_WORD_ENABLE = yes
COMBO_ENABLE = yes
#LAYER_LOCK_ENABLE = yes
REPEAT_KEY_ENABLE = yes

SRC += features/tap_hold_utilities.c
SRC += features/os4a.c
SRC += features/custom_altgr_keys.c
#SRC += encoder.c
SRC += oled.c
SRC += features/next_keycode.c
SRC += features/clever_keys.c
SRC += features/numword.c
SRC += features/macros.c
SRC += features/oneshot.c
SRC += features_conf.c

INTROSPECTION_KEYMAP_C = features/combos.c

MUSIC_ENABLE = no
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no 