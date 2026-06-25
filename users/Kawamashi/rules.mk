OLED_ENABLE = no
ENCODER_ENABLE = no       # Enables the use of one or more encoders
RGB_MATRIX_ENABLE = no     # Disable keyboard RGB matrix, as it is enabled by default on rev3
RGBLIGHT_ENABLE = no      # Disable keyboard RGB underglow



#SPLIT_KEYBOARD = yes

#CONSOLE_ENABLE = yes 

LTO_ENABLE = yes

ENCODER_MAP_ENABLE = no

TAP_DANCE_ENABLE = no
DEFERRED_EXEC_ENABLE = no
KEY_OVERRIDE_ENABLE = no
CAPS_WORD_ENABLE = no
COMBO_ENABLE = yes
REPEAT_KEY_ENABLE = yes

#SRC += oled.c
SRC += features/modword.c
SRC += conf_clever_keys.c
SRC += conf_features.c
SRC += conf_words.c
SRC += conf_prefixing_layers.c

INTROSPECTION_KEYMAP_C = combos.c

MUSIC_ENABLE = no
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no 