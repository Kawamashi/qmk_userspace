// Correction automatique

#include "clever_keys.h"


bool process_clever_keys(uint16_t keycode, keyrecord_t* record) {

  uint16_t next_keycode = get_next_keycode(keycode, record);
  const uint8_t mods = get_mods();

  if (next_keycode != KC_NO) {

    uint16_t prev_keycode = recent[RECENT_SIZE - 1];
    //const uint8_t mods = get_mods();

    if (is_letter(next_keycode) || next_keycode == E_CIRC) {
      switch (prev_keycode) {
          case PG_EXCL:
          case PG_QUES:
          case PG_3PTS:
          case PG_2PTS:
            // Add space between punctuation and letters.
            process_next_key(KC_SPC, record);

          case KC_SPC:
            switch (recent[RECENT_SIZE - 2]) {
              case PG_EXCL:
              case PG_QUES:
              case PG_3PTS:
              case PG_X:
                // Add OS shift at the beginning of a sentence.
                if (!is_caps_lock_on()) { set_oneshot_mods(MOD_BIT(KC_LSFT)); }
                break;
            }
      }
    }

    // Ajout automatique du "u" après le "q"
    if (prev_keycode == PG_Q) {
        switch (next_keycode) {

          case PG_E:
          case PG_I:
          case PG_A:
          case PG_O:
          case PG_U:
          case E_CIRC:
          case PG_APOS:
            process_next_key(PG_L, record);
            break;

          // Raccourci pour "quelq"
          case PG_Q:
            clear_recent_keys();  // To prevent infinite loop
            process_word((uint16_t[]) {PG_L, PG_E, PG_C}, 3, record);
            break;

          // Raccourci pour "quoi", ça évite un aller-retour sur la main gauche.
          case PG_Z:
            return finish_word((uint16_t[]) {PG_L, PG_O, PG_I}, 3, record);

          // Raccourci pour "quand"
          case PG_H:
            process_word((uint16_t[]) {PG_L, PG_A, PG_S}, 3, record);
            break;
        }


    } else if (next_keycode == PG_AROB && !is_letter(recent[RECENT_SIZE - 2])) {
        switch (prev_keycode) {

          case PG_N:
            // "t@" -> "toujours"
            return finish_word((uint16_t[]) {PG_O, PG_L, PG_J, PG_O, PG_L, PG_R, PG_T}, 7, record);

          case PG_P:
            // "p@" -> "peut-être"
            return finish_word((uint16_t[]) {PG_E, PG_L, PG_N, PG_MOIN, PG_ACIR, PG_E, PG_N, PG_R, PG_E}, 9, record);

          case PG_A:
            // "a@" -> "aujourd'hui"
            return finish_word((uint16_t[]) {PG_L, PG_J, PG_O, PG_L, PG_R, PG_H, PG_APOS, PG_F, PG_L, PG_I}, 10, record);
          
          case PG_B:
            // "b@" -> "beaucoup"
            return finish_word((uint16_t[]) {PG_E, PG_A, PG_L, PG_D, PG_O, PG_L, PG_P}, 7, record);
          
          case PG_E:
            // "e@" -> "est-ce qu"
            return finish_word((uint16_t[]) {PG_T, PG_N, PG_MOIN, PG_D, PG_E, KC_SPC, PG_Q}, 7, record);
          
          case PG_H:
            // "d@" -> "déjà"
            return finish_word((uint16_t[]) {PG_U, PG_J, PG_AGRV}, 3, record);
        }

    } else if (prev_keycode == PG_P) {
        switch (next_keycode) {

          case PG_M:
            // "pas"
            return finish_word((uint16_t[]) {PG_A, PG_T}, 2, record);
          
          case PG_APOS:
            // "par"
            return finish_word((uint16_t[]) {PG_A, PG_R}, 2, record);
          
          case PG_X:
            if (!is_letter(recent[RECENT_SIZE - 2])) {
              // "pour"
              return finish_word((uint16_t[]) {PG_O, PG_L, PG_R}, 3, record);
            }
          
        }
        
    } else if (next_keycode == PG_A && prev_keycode == PG_O) {
        // "oa" -> "oi"
        process_next_key(PG_I, record);
        return false;

    } else if (next_keycode == PG_O && prev_keycode == PG_L && recent[RECENT_SIZE - 2] != PG_Q) {
        // "uo" -> "un"
        process_next_key(PG_S, record);
        return false;
    }
    
    switch (next_keycode) {
      case PG_QUES:
      case PG_EXCL:
        // On ajoute un espace insécable s'il n'a pas été entré avant le point d'exclamation.
        // Il ne faut pas tester cette fonctionnalité avec Word, qui ajoute cet espace automatiquement.
        if (is_letter(recent[RECENT_SIZE - 1])) {
          
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

          case PG_N:
            process_next_key(PG_I, record);
          case PG_I:
            return finish_word((uint16_t[]) {PG_O, PG_S}, 2, record);
          
          case PG_D:
            return finish_word((uint16_t[]) {PG_APOS, PG_E, PG_T, PG_N}, 4, record);
          
          case PG_H:
            return finish_word((uint16_t[]) {PG_A, PG_S, PG_T}, 3, record);
          
          case PG_P:
            return finish_word((uint16_t[]) {PG_C, PG_L, PG_T}, 3, record);
          
          case PG_A:
            return finish_word((uint16_t[]) {PG_G, PG_E, PG_D}, 3, record);
          
          case PG_T:
            return finish_word((uint16_t[]) {PG_L, PG_R}, 2, record);
          
          case PG_B:
            process_word((uint16_t[]) {PG_O, PG_S, PG_J}, 3, record);
          
          case PG_J:
            return finish_word((uint16_t[]) {PG_O, PG_L, PG_R}, 3, record);
          
          case PG_M:
            // "même"
            return finish_word((uint16_t[]) {PG_ACIR, PG_E, PG_M, PG_E}, 4, record);
          
          default:
            return false;
        }

      case PG_Q:
        if (prev_keycode == PG_J) {
            // "jq" -> "jusqu"
            process_word((uint16_t[]) {PG_L, PG_T}, 2, record);
        }
        break;
      
      case CA_CED:
        return finish_word((uint16_t[]) {PG_CCED, PG_A}, 2, record);

      case OU_GRV:
        return finish_word((uint16_t[]) {PG_O, ALGR(PG_L)}, 2, record);

      case AGRV_SPC:
        return finish_word((uint16_t[]) {PG_AGRV, KC_SPC}, 2, record);

      case E_CIRC:
          return process_accent(PG_ACIR, PG_E, record);

      case I_CIRC:
          return process_accent(PG_ACIR, PG_I, record);

      case A_CIRC:
          return process_accent(PG_ACIR, PG_A, record);

      case O_CIRC:
          return process_accent(PG_ACIR, PG_O, record);

      case U_CIRC:
          return process_accent(PG_ACIR, PG_L, record);

      case I_TREM:
          return process_accent(PG_NREM, PG_I, record);
    }

    store_keycode(next_keycode, record);
  }
  return true; // Process all other keycodes normally
}