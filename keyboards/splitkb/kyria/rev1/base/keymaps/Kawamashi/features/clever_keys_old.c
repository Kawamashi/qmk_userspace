// Correction automatique

#include "clever_keys.h"


bool process_clever_keys(uint16_t keycode, keyrecord_t* record) {

  uint16_t next_keycode = get_next_keycode(keycode, record);
  const uint8_t mods = get_mods();

  if (next_keycode != KC_NO) {

    uint16_t prev_keycode = recent[RECENT_SIZE - 1];
    //const uint8_t mods = get_mods();

    if (isLetter(next_keycode) || next_keycode == E_CIRC) {
      switch (prev_keycode) {
          case FR_EXLM:
          case FR_QUES:
          case FR_3PTS:
          case FR_2PTS:
            // Add space between punctuation and letters.
            process_next_key(KC_SPC, record);

          case KC_SPC:
            switch (recent[RECENT_SIZE - 2]) {
              case FR_EXLM:
              case FR_QUES:
              case FR_3PTS:
              case FR_POIN:
                // Add OS shift at the beginning of a sentence.
                if (!is_caps_lock_on()) { set_oneshot_mods(MOD_BIT(KC_LSFT)); }
                break;
            }
      }
    }

    // Ajout automatique du "u" après le "q"
    if (prev_keycode == FR_Q) {
        switch (next_keycode) {

          case FR_E:
          case FR_I:
          case FR_A:
          case FR_O:
          case FR_EACU:
          case E_CIRC:
          case FR_APOS:
            process_next_key(FR_U, record);
            break;

          // Raccourci pour "quelq"
          case FR_Q:
            clear_recent_keys();  // To prevent infinite loop
            process_word((uint16_t[]) {FR_U, FR_E, FR_L}, 3, record);
            break;

          // Raccourci pour "quoi", ça évite un aller-retour sur la main gauche.
          case FR_Z:
            return finish_word((uint16_t[]) {FR_U, FR_O, FR_I}, 3, record);

          // Raccourci pour "quand"
          case FR_D:
            process_word((uint16_t[]) {FR_U, FR_A, FR_N}, 3, record);
            break;
        }


    } else if (next_keycode == FR_AROB && !isLetter(recent[RECENT_SIZE - 2])) {
        switch (prev_keycode) {

          case FR_T:
            // "t@" -> "toujours"
            return finish_word((uint16_t[]) {FR_O, FR_U, FR_J, FR_O, FR_U, FR_R, FR_S}, 7, record);

          case FR_P:
            // "p@" -> "peut-être"
            return finish_word((uint16_t[]) {FR_E, FR_U, FR_T, FR_MOIN, FR_ACIR, FR_E, FR_T, FR_R, FR_E}, 9, record);

          case FR_A:
            // "a@" -> "aujourd'hui"
            return finish_word((uint16_t[]) {FR_U, FR_J, FR_O, FR_U, FR_R, FR_D, FR_APOS, FR_H, FR_U, FR_I}, 10, record);
          
          case FR_B:
            // "b@" -> "beaucoup"
            return finish_word((uint16_t[]) {FR_E, FR_A, FR_U, FR_C, FR_O, FR_U, FR_P}, 7, record);
          
          case FR_E:
            // "e@" -> "est-ce qu"
            return finish_word((uint16_t[]) {FR_S, FR_T, FR_MOIN, FR_C, FR_E, KC_SPC, FR_Q}, 7, record);
          
          case FR_D:
            // "d@" -> "déjà"
            return finish_word((uint16_t[]) {FR_EACU, FR_J, FR_AGRV}, 3, record);
        }

    } else if (prev_keycode == FR_P) {
        switch (next_keycode) {

          case FR_M:
            // "pas"
            return finish_word((uint16_t[]) {FR_A, FR_S}, 2, record);
          
          case FR_APOS:
            // "par"
            return finish_word((uint16_t[]) {FR_A, FR_R}, 2, record);
          
          case FR_POIN:
            if (!isLetter(recent[RECENT_SIZE - 2])) {
              // "pour"
              return finish_word((uint16_t[]) {FR_O, FR_U, FR_R}, 3, record);
            }
          
        }
        
    } else if (next_keycode == FR_A && prev_keycode == FR_O) {
        // "oa" -> "oi"
        process_next_key(FR_I, record);
        return false;

    } else if (next_keycode == FR_O && prev_keycode == FR_U && recent[RECENT_SIZE - 2] != FR_Q) {
        // "uo" -> "un"
        process_next_key(FR_N, record);
        return false;
    }
    
    switch (next_keycode) {
      case FR_QUES:
      case FR_EXLM:
        // On ajoute un espace insécable s'il n'a pas été entré avant le point d'exclamation.
        // Il ne faut pas tester cette fonctionnalité avec Word, qui ajoute cet espace automatiquement.
        if (isLetter(recent[RECENT_SIZE - 1])) {
          
          if ((mods | get_oneshot_mods() | get_weak_mods()) & MOD_MASK_SHIFT) {
            del_weak_mods(MOD_MASK_SHIFT);
            del_oneshot_mods(MOD_MASK_SHIFT);
            unregister_mods(MOD_MASK_SHIFT);
          }
          process_next_key(ALGR(KC_SPC), record);
          set_mods(mods);
          process_next_key(next_keycode, record);
          return false;
        }
        break;

      case MAGIC:
        switch (prev_keycode) {

          case FR_T:
            process_next_key(FR_I, record);
          case FR_I:
            return finish_word((uint16_t[]) {FR_O, FR_N}, 2, record);
          
          case FR_C:
            return finish_word((uint16_t[]) {FR_APOS, FR_E, FR_S, FR_T}, 4, record);
          
          case FR_D:
            return finish_word((uint16_t[]) {FR_A, FR_N, FR_S}, 3, record);
          
          case FR_P:
            return finish_word((uint16_t[]) {FR_L, FR_U, FR_S}, 3, record);
          
          case FR_A:
            return finish_word((uint16_t[]) {FR_V, FR_E, FR_C}, 3, record);
          
          case FR_S:
            return finish_word((uint16_t[]) {FR_U, FR_R}, 2, record);
          
          case FR_B:
            process_word((uint16_t[]) {FR_O, FR_N, FR_J}, 3, record);
          
          case FR_J:
            return finish_word((uint16_t[]) {FR_O, FR_U, FR_R}, 3, record);
          
          case FR_M:
            // "même"
            return finish_word((uint16_t[]) {FR_ACIR, FR_E, FR_M, FR_E}, 4, record);
          
          default:
            return false;
        }

      case FR_Q:
        if (prev_keycode == FR_J) {
            // "jq" -> "jusqu"
            process_word((uint16_t[]) {FR_U, FR_S}, 2, record);
        }
        break;
      
      case CA_CED:
        return finish_word((uint16_t[]) {FR_CCED, FR_A}, 2, record);

      case OU_GRV:
        return finish_word((uint16_t[]) {FR_O, ALGR(FR_U)}, 2, record);

      case AGRV_SPC:
        return finish_word((uint16_t[]) {FR_AGRV, KC_SPC}, 2, record);

      case E_CIRC:
          return process_accent(FR_ACIR, FR_E, record);

      case I_CIRC:
          return process_accent(FR_ACIR, FR_I, record);

      case A_CIRC:
          return process_accent(FR_ACIR, FR_A, record);

      case O_CIRC:
          return process_accent(FR_ACIR, FR_O, record);

      case U_CIRC:
          return process_accent(FR_ACIR, FR_U, record);

      case I_TREM:
          return process_accent(FR_TREM, FR_I, record);
    }

    store_keycode(next_keycode, record);
  }
  return true; // Process all other keycodes normally
}