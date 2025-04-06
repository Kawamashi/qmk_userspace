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
          case FG_EXLM:
          case FG_QUES:
          case FG_3PTS:
          case FG_2PTS:
            // Add space between punctuation and letters.
            process_next_key(KC_SPC, record);

          case KC_SPC:
            switch (recent[RECENT_SIZE - 2]) {
              case FG_EXLM:
              case FG_QUES:
              case FG_3PTS:
              case FG_POIN:
                // Add OS shift at the beginning of a sentence.
                if (!is_caps_lock_on()) { set_oneshot_mods(MOD_BIT(KC_LSFT)); }
                break;
            }
      }
    }

    // Ajout automatique du "u" après le "q"
    if (prev_keycode == FG_Q) {
        switch (next_keycode) {

          case FG_E:
          case FG_I:
          case FG_A:
          case FG_O:
          case FG_EACU:
          case E_CIRC:
          case FG_APOS:
            process_next_key(FG_U, record);
            break;

          // Raccourci pour "quelq"
          case FG_Q:
            clear_recent_keys();  // To prevent infinite loop
            process_word((uint16_t[]) {FG_U, FG_E, FG_L}, 3, record);
            break;

          // Raccourci pour "quoi", ça évite un aller-retour sur la main gauche.
          case FG_Z:
            return finish_word((uint16_t[]) {FG_U, FG_O, FG_I}, 3, record);

          // Raccourci pour "quand"
          case FG_D:
            process_word((uint16_t[]) {FG_U, FG_A, FG_N}, 3, record);
            break;
        }


    } else if (next_keycode == FG_AROB && !isLetter(recent[RECENT_SIZE - 2])) {
        switch (prev_keycode) {

          case FG_T:
            // "t@" -> "toujours"
            return finish_word((uint16_t[]) {FG_O, FG_U, FG_J, FG_O, FG_U, FG_R, FG_S}, 7, record);

          case FG_P:
            // "p@" -> "peut-être"
            return finish_word((uint16_t[]) {FG_E, FG_U, FG_T, FG_MOIN, FG_ACIR, FG_E, FG_T, FG_R, FG_E}, 9, record);

          case FG_A:
            // "a@" -> "aujourd'hui"
            return finish_word((uint16_t[]) {FG_U, FG_J, FG_O, FG_U, FG_R, FG_D, FG_APOS, FG_H, FG_U, FG_I}, 10, record);
          
          case FG_B:
            // "b@" -> "beaucoup"
            return finish_word((uint16_t[]) {FG_E, FG_A, FG_U, FG_C, FG_O, FG_U, FG_P}, 7, record);
          
          case FG_E:
            // "e@" -> "est-ce qu"
            return finish_word((uint16_t[]) {FG_S, FG_T, FG_MOIN, FG_C, FG_E, KC_SPC, FG_Q}, 7, record);
          
          case FG_D:
            // "d@" -> "déjà"
            return finish_word((uint16_t[]) {FG_EACU, FG_J, FG_AGRV}, 3, record);
        }

    } else if (prev_keycode == FG_P) {
        switch (next_keycode) {

          case FG_M:
            // "pas"
            return finish_word((uint16_t[]) {FG_A, FG_S}, 2, record);
          
          case FG_APOS:
            // "par"
            return finish_word((uint16_t[]) {FG_A, FG_R}, 2, record);
          
          case FG_POIN:
            if (!isLetter(recent[RECENT_SIZE - 2])) {
              // "pour"
              return finish_word((uint16_t[]) {FG_O, FG_U, FG_R}, 3, record);
            }
          
        }
        
    } else if (next_keycode == FG_A && prev_keycode == FG_O) {
        // "oa" -> "oi"
        process_next_key(FG_I, record);
        return false;

    } else if (next_keycode == FG_O && prev_keycode == FG_U && recent[RECENT_SIZE - 2] != FG_Q) {
        // "uo" -> "un"
        process_next_key(FG_N, record);
        return false;
    }
    
    switch (next_keycode) {
      case FG_QUES:
      case FG_EXLM:
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

          case FG_T:
            process_next_key(FG_I, record);
          case FG_I:
            return finish_word((uint16_t[]) {FG_O, FG_N}, 2, record);
          
          case FG_C:
            return finish_word((uint16_t[]) {FG_APOS, FG_E, FG_S, FG_T}, 4, record);
          
          case FG_D:
            return finish_word((uint16_t[]) {FG_A, FG_N, FG_S}, 3, record);
          
          case FG_P:
            return finish_word((uint16_t[]) {FG_L, FG_U, FG_S}, 3, record);
          
          case FG_A:
            return finish_word((uint16_t[]) {FG_V, FG_E, FG_C}, 3, record);
          
          case FG_S:
            return finish_word((uint16_t[]) {FG_U, FG_R}, 2, record);
          
          case FG_B:
            process_word((uint16_t[]) {FG_O, FG_N, FG_J}, 3, record);
          
          case FG_J:
            return finish_word((uint16_t[]) {FG_O, FG_U, FG_R}, 3, record);
          
          case FG_M:
            // "même"
            return finish_word((uint16_t[]) {FG_ACIR, FG_E, FG_M, FG_E}, 4, record);
          
          default:
            return false;
        }

      case FG_Q:
        if (prev_keycode == FG_J) {
            // "jq" -> "jusqu"
            process_word((uint16_t[]) {FG_U, FG_S}, 2, record);
        }
        break;
      
      case CA_CED:
        return finish_word((uint16_t[]) {FG_CCED, FG_A}, 2, record);

      case OU_GRV:
        return finish_word((uint16_t[]) {FG_O, ALGR(FG_U)}, 2, record);

      case AGRV_SPC:
        return finish_word((uint16_t[]) {FG_AGRV, KC_SPC}, 2, record);

      case E_CIRC:
          return process_accent(FG_ACIR, FG_E, record);

      case I_CIRC:
          return process_accent(FG_ACIR, FG_I, record);

      case A_CIRC:
          return process_accent(FG_ACIR, FG_A, record);

      case O_CIRC:
          return process_accent(FG_ACIR, FG_O, record);

      case U_CIRC:
          return process_accent(FG_ACIR, FG_U, record);

      case I_TREM:
          return process_accent(FG_TREM, FG_I, record);
    }

    store_keycode(next_keycode, record);
  }
  return true; // Process all other keycodes normally
}