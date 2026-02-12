# Kawamashi’s QMK configuration

This is a template repository which allows for an external set of QMK keymaps to be defined and compiled. This is useful for users who want to maintain their own keymaps without having to fork the main QMK repository.

![kb](GMKyria-min.jpg)

## Custom features
* [Configuration des Layer-Tap](#Configuration-des-Layer-Tap)
* [Modificatrices](#Modificatrices)
* [One-Shot for All](#One-Shot-for-All)
* [Couches préfixées](#Couches-préfixées)
* [Combos](#Combos)
* [Mod Word](#Mod-Word)
* [Layer Word](#Layer-Word)
* [Clever Keys](#Clever-Keys)

&nbsp;</br> &nbsp;</br>

## Configuration des Layer-Tap
J’utilise des Layer-Tap sur mes touches de pouces pour accéder aux couches dont je me sers le plus (symboles, chiffres, navigation, raccourcis et modificatrices). Transformer les touches [Repeat](https://docs.qmk.fm/features/repeat_key) et [Alt-Repeat](https://docs.qmk.fm/features/repeat_key#alternate-repeating) en Layer-Tap demande quelques lignes de code :

```c
#define LT_REPT LT(_NUMBERS, KC_1)
#define LT_MGC LT(_SHORTNAV, KC_1)

bool process_macros_I(uint16_t keycode, keyrecord_t *record) {

    if (record->tap.count) {
        // Special tap-hold keys (on tap).
        switch (keycode) {
            case LT_REPT:
              repeat_key_invoke(&record->event);
              return false;
        
            case LT_MGC:
              alt_repeat_key_invoke(&record->event);
              return false;
        }
    }
    return true;
}

bool remember_last_key_user(uint16_t keycode, keyrecord_t* record, uint8_t* remembered_mods) {

    switch (keycode) {
        case LT_REPT:
        case LT_MGC:
          return false;
        
        default:
          return true;
    }
}
```
&nbsp;</br> &nbsp;</br>
En ce qui concerne le [paramétrage des Tap-Hold](https://docs.qmk.fm/tap_hold), j’utilise le [Permissive Hold](https://docs.qmk.fm/tap_hold#permissive-hold), qui me permet de ne pas avoir à attendre le *tapping term* en cas de nested tap. Grâce à ça, je n’ai plus de faux‑négatifs avec mes layers‑tap. 


Pour éviter les déclenchements par erreur (lors de roulements par exemple), je me base sur une vérification de la position des touches : avant le *tapping term*, si l’une de mes touches de pouce principales est enfoncée ainsi qu’une autre touche de la même main, alors la touche de pouce produira immédiatement un tap et non un hold. Pour cela, j’ai adopté l’approche de [Filterpaper](https://github.com/filterpaper/qmk_userspace?tab=readme-ov-file#contextual-mod-taps), que je trouve légère et efficace. Elle est basée sur l’utilisation de la fonction `pre_process_record_user` et sur une utilisation avancée de la fonction `get_hold_on_other_key_press` :

```c
#define TAPPING_TERM 200
#define PERMISSIVE_HOLD
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY

bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {

    if (record->event.pressed) {
        // Cache the next input for mod-tap decisions
        next_keycode = keycode;
        next_record  = *record;
    }
    return true;
}

bool approved_chord(uint16_t tap_hold_keycode, keyrecord_t* tap_hold_record, uint16_t other_keycode, keyrecord_t* other_record) {

    switch (tap_hold_keycode) {
        case LT_REPT:
        case LT_MGC:
          return true;
    }

    // Otherwise, follow the opposite hands rule.
    return bilateral_combination(tap_hold_record, other_record);
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {

    if (!approved_chord(keycode, record, next_keycode, &next_record)) {
        // When a layer-tap key overlaps with another key on the same hand, send its base keycode.
        record->tap.interrupted = false;
        record->tap.count = 1;
        return true;
    }
    return false;
}
```
&nbsp;</br> &nbsp;</br>

## Modificatrices
Pour les modificatrices, je n’utilise pas de [Home-Row mods](https://precondition.github.io/home-row-mods#what-are-home-row-mods) à proprement parler. Mes modificatrices sont des [Callum mods](https://github.com/callum-oakley/qmk_firmware/tree/master/users/callum#oneshot-modifiers), autrement dit des [One‑Shot mods](https://docs.qmk.fm/one_shot_keys) situés sur la rangée de repos d’une couche spécifique. J’aime cette approche qui ne produit pas de délai dans l’affichage des frappes, contrairement aux HRM. 


Les Callum mods peuvent être maintenus comme des modificatrices classiques, ou employés comme sticky keys. Ils ont la particularité de ne pas utiliser de timer. Du coup, ils peuvent être combinés sans être obligé de marquer une pause entre chaque modificatrice.

La mise en place de ces modificatrices est très simple. Il faut d’abord définir les keycodes custom des mods ainsi que leur nombre, et les lier à leur modificatrice :

```c
enum custom_keycodes {
  OS_SHFT = SAFE_RANGE,
  OS_CTRL,
  OS_ALT,
  OS_GUI,
};

#define OS_COUNT 4

const oneshot_key_t oneshot_keys[] = {
  {OS_SHFT, KC_LSFT},
  {OS_CTRL, KC_LCTL},
  {OS_ALT, KC_LALT},
  {OS_GUI, KC_LGUI},
};
```

On peut ensuite définir des touches d’annulation, ainsi que des touches qui ne relâchent pas les modificatrices (typiquement des touches de changement de couche, pour appliquer les mods aux touches de la couche) :

```c

bool is_oneshot_cancel_key(uint16_t keycode) {
  switch (keycode) {
    case L_OS4A:
    case R_OS4A:
      return true;

    default:
      return false;
  }
}

bool should_oneshot_stay_pressed(uint16_t keycode) {
  switch (keycode) {
    case OS_FA:       // to be combined with Alt
    case FUNWORD:
    case NUMWORD:     // to combine numbers with mods
      return true;

    default:
      return false;
  }
}
```

J’ai modifié le code de Callum Oakley pour corriger un bug qui affectait les roulements. Dans mon implémentation, un one-shot mod se désactive lorsqu’on appuie une deuxième fois dessus, ou automatiquement au bout d’un certain temps d’inactivité. Le code se trouve [ici](keyboards/splitkb/kyria/rev1/base/keymaps/Kawamashi/features/oneshot.c).

&nbsp;</br>

## One-Shot for All
Les Callum mods sont placés sur des layers secondaires. Je n’aime pas le fait de devoir maintenir une touche pour y accéder, je voulais pouvoir le faire avec une One-Shot Layer. De plus, je voulais conserver des One-Shot Shift sur ma keymap, et je n’avais pas la place de tout mettre. J’ai donc conçu les One-Shot for All (OS4A) pour tout faire à la fois : 
- lors d’un appui maintenu, ces touches produisent `Shift`.
- après un appui simple, elles activent une couche liée.
- après un 2e appui, elles désactivent la couche liée et annulent tous les Callum mods.

Sur la moitié de celle-ci, on trouve les modificatrices, qui peuvent être enchaînées les une avec les autres. Sur l’autre moitié, on retrouve les alphas. En cas d’appui sur une lettre, celle-ci sera shiftée et la couche sera désactivée. On retrouve ainsi le comportement d’un One-Shot Shift. J’ai une touche OS4A pour chaque moitié du clavier, voilà un exemple de couche liée :

![couche OS4A](OS4A.png)

- Pour taper `T` (majuscule), il suffit de taper `OS4A` `T`.
- Pour faire `Ctrl S`, on tape `OS4A` `⎈` `S`.
- Pour faire `Ctrl Shift T`, on tape `OS4A` `⎈` `⇧` `S`. L’ordre des modificatrices n’est pas importante, elles peuvent même être tapées simultanément.

Les OS4A fonctionnent grâce à des [couches préfixées](#Couches-préfixées) et à des [Layer Word](#Layer-Word). Leur fonctionnement implique un timer, puisqu’au fond ce sont des mod‑taps modifiés. Cependant, grâce à la configuration des [Tap-Hold](#Configuration-des-Layer-Tap), on peut faire un roulement sur une touche OS4A et une modificatrice sans aucun souci. En utilisation réelle, le timer n’est jamais contraignant. 

&nbsp;</br>

## Couches préfixées
Mon layout, [Propergol](https://github.com/Kawamashi/Propergol/), utilise une [touche morte de type Lafayette](https://ergol.org/presentation/#impeccable-en-fran%C3%A7ais) (notée 1DK ou `★`) pour taper les caractères accentués, les diacritiques ainsi que les symboles typographiques. Pour m’affranchir de certaines limites liées à cette approche, j’ai remplacé cette touche morte par un one-shot layer, donnant accès à une couche 1DK. Par défaut, un caractère tapé sur cette couche envoie la touche morte avant. Pour l’implémenter, j’ai utilisé [une idée de Pascal Getreuer](https://getreuer.info/posts/keyboards/macros3/index.html#prefixing-layer) :

```c
bool process_prefixing_layers(uint16_t keycode, keyrecord_t *record) {

    if (IS_LAYER_ON(_1DK)) {
        switch (keycode) {
            case PG_Z:
            case PG_UNDS:
            case PG_ECIR:
            case PG_Q:
                return true;                
            default:
                tap_code(PG_1DK);            
        }
    }

    // OS4A keys behave like one-shot shifts for the opposite side of the keyboard
    if (IS_LAYER_ON(_L_MODS) || IS_LAYER_ON(_R_MODS)) {
        if (should_add_shift(keycode, record)) {
            set_oneshot_mods(MOD_BIT(KC_LSFT));
            if (!is_letter(keycode)) { set_last_keycode(S(keycode)); }
        }
    }
    return true;
}
```
&nbsp;</br>

Pour faire `ê` avec l’approche Lafayette, il faut faire `★` `e`. Pour faire `Ê`, il faut faire `★` `Shift`+`e`. Je trouve ça plus logique de faire `Shift`+`ê`, donc `Shift`+`★` `e`. Dans ce même module, j’ai modifié le comportement de la touche 1DK avec `Shift`, pour que ce soit le caractère suivant `★` qui soit shifté :
```c
bool deferred_shift_after_dead_key(uint16_t keycode) {
    // Special behaviour of PG_1DK when shifted
    // Shift must apply to the keycode following PG_1DK.
    const bool is_shifted = (get_mods() | get_weak_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT;

    if (is_shifted) {
        del_weak_mods(MOD_MASK_SHIFT);
        del_oneshot_mods(MOD_MASK_SHIFT);
        unregister_mods(MOD_MASK_SHIFT);
    }

    tap_code(PG_1DK);
    if (is_shifted) { set_oneshot_mods(MOD_BIT(KC_LSFT)); }    // Don't use weak mods !

    return keycode != PG_1DK;
}
```
Vous trouverez mon implémentation complète [ici](https://github.com/Kawamashi/qmk_userspace/blob/main/keyboards/splitkb/kyria/rev1/base/keymaps/Kawamashi/features/prefixing_layers.c).

&nbsp;</br>

## Combos
J’utilise des combos principalement pour les touches d’édition : `Backspace`, `Delete`, `Tab`, `Enter`, `Esc`, `Home`, `End`. Certaines d’entre elles sont placées sur la home-row. J’ai implémenté un timer pour éviter leur déclenchement involontaire, lors d’un roulement par exemple : 

```c
bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {

    // Chorded mods shouldn't be considered as combos.
    if (IS_LAYER_ON(_R_MODS)) { return on_left_hand(record->event.key); }
    if (IS_LAYER_ON(_L_MODS)) { return !on_left_hand(record->event.key); }

    // Some combos should trigger regardless of the idle time.
    switch (combo_index) {
        case R_BKSPC:
        case BK_WORD:
        case ENTER:
        case HOME:
        case END:
        case L_SPACE:
          return true;

        default:
          if (get_idle_time() < TAP_INTERVAL) { return false; }
    }
    return true;
}
```
Pour que certaines combos se déclenchent, il faut donc qu’un temps `TAP_INTERVAL` se soit écoulé entre l’input du dernier caractère et la combo.

J’ai également une combo pour faire Alt-tab facilement. Il suffit de réappuyer sur l’une des touches de la combo pour envoyer Tab ou Shift-Tab, comme décrit [ici](https://docs.qmk.fm/features/combo#customizable-key-repress).

Enfin, j’ai une combo `PANIC`, pour remettre le clavier dans son état nominal. Cette combo :
- annule les [one-shot mods](#Modificatrices)
- désactive les layers autres que la couche de base
- désactive les [Layer Word](#Layer-Word)
- désactive les [Mod Word](#Mod-Word)
- vide le buffer des [Clever Keys](#Clever-Keys)

&nbsp;</br>

## Mod Word
Ce [module](keyboards/splitkb/kyria/rev1/base/keymaps/Kawamashi/features/modword.c) chapeaute mon implémentation de [*Caps Word*](https://docs.qmk.fm/features/caps_word), *Caps List*, *Caps Lock* et *Word Selection*.
### Caps List
J’utilise énormément [Caps Word](https://docs.qmk.fm/features/caps_word), et je suis régulièrement amené à écrire des listes de noms en majuscule, comme par exemple `QMK, ZMK et KRK`. J’ai donc développé *Caps List*, une fonctionnalité qui réactive Caps Word entre chaque mot d’une liste. 


L’utilisateur peut définir les keycodes qui ne doivent pas interrompre Caps List (comme par exemple les lettres, les chiffres, certains symboles, `,` et `␣`). À la saisie d’un caractère différent, Caps List se désactivera automatiquement. \
L’utilisateur doit également définir les séparateurs de liste (comme `,␣`, `␣et␣`, `␣ou␣`). Caps List réactivera automatiquement Caps Word après l’un de ces séparateurs, en se servant du buffer des [Clever Keys](#Clever-Keys).


Pour illustrer le fonctionnement de Caps List, imaginons que je veuille écrire la phrase suivante : `Les principaux firmwares sont QMK, ZMK et KRK, je préfère QMK.` : 
- J’active Caps List avant de taper ma liste. Caps List active automatiquement Caps Word.
- Je tape `qmk`. Tous les caractères étant des lettres, Caps List et Caps Word restent activés.
- Je tape `,`. Caps Word se désactive. La virgule n’interrompt pas Caps List, mais un compteur s’incrémente. Si un séparateur de liste n’est pas détecté avant que celui-ci n’atteigne une certaine valeur, Caps List sera désactivé. En cas de faute de frappe, `Backspace` décrémente le compteur.
- Je tape `␣`. Caps List est toujours actif. Le compteur s’incrémente.
- Je tape `z`. Caps List détecte que la séquence précédente (`,␣`) est un séparateur de liste : Caps Word est réactivé, le compteur est réinitialisé.
- Je tape `mk`. Tous les caractères étant des lettres, Caps List et Caps Word restent activés.
- Je tape `␣`. Caps Word se désactive. Le compteur de Caps List s’incrémente.
- Je tape `et␣`. Le compteur s’incrémente.
- Je tape `k`. Caps List détecte que la séquence précédente (`␣et␣`) est un séparateur final de liste : Caps Word est réactivé, le compteur est réinitialisé et sa valeur limite est modifiée.
- Je tape `rk`. Tous les caractères étant des lettres, Caps List et Caps Word restent activés.
- Je tape `,`. Caps Word se désactive. Le compteur de Caps List s’incrémente.
- Je tape `␣`. Le compteur arrive à sa limite, Caps List se désactive. 
- Je tape `j`. Un nouveau mot commence, mais Caps Word n’est pas réactivé. `j` ne passe donc pas en majuscule.

&nbsp;</br>

Voilà le paramétrage et l’algorithme de Caps List :
```c
bool should_continue_caps_list(uint16_t keycode, keyrecord_t* record) {

    // Keycodes that continue Caps List, but not Caps Word.
    // These keycodes trigger the counter to deactivate Caps List.
    switch (keycode) {
      case KC_BSPC:
        return update_capslist_counter(-1);
      case PG_VIRG:
      case KC_SPC:
          return update_capslist_counter(1);
    }

    if (is_letter(keycode) || is_send_string_macro(keycode)) { return update_capslist_counter(1); }

    // This condition can't be merged with the previous one
    // because caps_word_press_user adds shift to letters and send-string macros.
    if (caps_word_press_user(keycode)) { return update_capslist_counter(1); }

    return false;  // Deactivate Caps List.
}


bool list_separator(void) {

    // Words that continue Caps List.
    if (get_recent_keycode(-1) == KC_SPC) {
        if (get_recent_keycode(-2) == PG_VIRG) { return true; }
        if (word_check((uint16_t[]) {KC_SPC, PG_E, PG_T}, 3, 2)) { return true; }
        if (word_check((uint16_t[]) {KC_SPC, PG_O, PG_U}, 3, 2)) { return true; }
    }
    return false;
}
```

```c

void update_caps_list(uint16_t keycode, keyrecord_t* record) {
    
  if (should_continue_caps_list(keycode, record)) {

      if (list_separator()) {
        caps_word_on();  // Reactivate Caps Word for a new word
        capslist_counter = 1;
        return;
      }
      if (capslist_counter < counter_limit) { return; }
  }
  disable_modword(capslist);
}
```

&nbsp;</br>

### Mod Word
En plus de *Caps Word* et *Caps List*, je suis également amené à utiliser *Caps Lock* régulièrement. Je voulais que l’activation d’une fonctionnalité désactive automatiquement les autres, pour qu’il n’y ait pas d’interférences entre elles. Le module *Mod Word* est là pour les chapeauter. En plus de gérer les activations concurrentielles, *Mod Word* désactive la fonctionnalité en cours au bout d’un certain temps d’inactivité du clavier.
&nbsp;</br>

### Word Selection
*Word Selection* est une macro QMK qui permet de sélectionner un mot, et qui ajoute automatiquement shift (et contrôle si nécessaire) aux touches de navigation tapées ensuite pour étendre ou réduire la sélection : 
- `Ctrl` et `Shift` sont ajoutées à `←` et `→` pour sélectionner ou désélectionner le mot précédent/suivant
- `Shift` est ajouté à `Home` et `End` pour étendre la sélection au début/à la fin de la ligne
- `Shift` est ajouté à `↑` et `↓` pour sélectionner ou déselectionner la ligne précédente/suivante
- `Shift` est ajouté à `Page Up` et `Page Down` pour étendre la sélection au début/à la fin de la page

*Word Selection* supporte les changements de direction lors de la sélection, ce qui rend son utilisation très intuitive.\
Cette macro existe aussi sous la forme de *Line Selection*, qui sélectionne une ligne entière. 


*Word Selection* ajoutant une modificatrice aux inputs successifs, elle est également chapeautée par *Mod Word*.

&nbsp;</br>

## Layer Word
[*Layer Word*](keyboards/splitkb/kyria/rev1/base/keymaps/Kawamashi/features/layerword.c) est un concept similaire à *Caps Word*, mais il s’applique aux couches. Quand on active un *Layer Word*, on active la couche correspondante, et elle restera activée tant que l’utilisateur ne tape pas de caractère “word breaking”. Par exemple, le *Layer Word* lié à ma couche de nombres restera activé tant que je taperai des symboles numériques. Les caractères permettant de continuer chaque *Layer Word* sont à définir par l’utilisateur.


Comme pour *Mod Word*, il ne peut y avoir qu’un seul *Layer Word* actif à la fois. Les *Layer Word* peuvent se désactiver automatiquement au bout d’un certain temps d’inactivité du clavier.


La mise en place d’un Layerword est très simple. Il faut d’abord définir un keycode custom, `NUMWORD` par exemple. On lie ensuite ce keycode à une couche et on peut définir le timeout du Layerword : 

```c

uint8_t layerword_layer_from_trigger(uint16_t keycode) {

  switch (keycode) {
    case L_OS4A: return _L_MODS;
    case R_OS4A: return _R_MODS;
    case NUMWORD: return _NUMBERS;
    case NAVWORD: return _SHORTNAV;
    case FUNWORD: return _FUNCAPPS;
    default: return 0;
  }
}

uint16_t layerword_exit_timeout(uint8_t layer) {

  switch (layer) {
    case _NUMBERS:
    case _SHORTNAV:
    case _L_MODS:
    case _R_MODS:
        return 3000;
    case _FUNCAPPS:
        return 30000;
    default:
        return 0;
  }
}
```

Il ne reste plus qu’à définir les caractères qui continuent le Layerword :

```c
bool should_continue_layerword(uint8_t layer, uint16_t keycode, keyrecord_t *record) {

  switch (layer) {

    case _NUMBERS:
      switch (keycode) {
        // Keycodes that should not disable num word.
        // Numpad keycodes
        case KC_1 ... KC_0:
        case KC_PDOT:
        case PG_MOIN:
        case PG_ASTX: 
        case PG_PLUS:
        case PG_SLSH:
        case PG_EGAL:
        case PG_EXP:
        case PG_IND:
        case PG_H:
        case PG_2PTS:

        // Misc
        case KC_BSPC:
        case PG_1DK:   // Not to exit Numword when chording it with 1DK
            return true; 
        default:
            return false;
      }
    // Other Layerwords
  }
  return false;
}
```

Tout autre symbole de la couche sera saisi, mais le layerword se terminera et la couche sera désactivée quand la touche sera relâchée.  
Le paramétrage de mes Layerwords se trouve [ici](https://github.com/Kawamashi/qmk_userspace/blob/main/keyboards/splitkb/kyria/rev1/base/keymaps/Kawamashi/word_conf.c).

&nbsp;</br>

## Clever Keys
Je me suis beaucoup inspiré de [ce post](https://getreuer.info/posts/keyboards/triggers/index.html#based-on-previously-typed-keys) quand j’ai écrit les [Clever Keys](keyboards/splitkb/kyria/rev1/base/keymaps/Kawamashi/features/clever_keys_utilities.c). Ce concept repose sur le fait de mémoriser les derniers caractères tapés en les stockant dans un buffer, et de potentiellement remplacer un caractère en cours de traitement par un autre. En un mot, les *Clever Keys* permettent de transformer votre layout en **layout adaptatif**.

```c
void process_clever_keys(uint16_t keycode, keyrecord_t* record) {

    if (record->event.pressed) {
      uint16_t ongoing_keycode = get_ongoing_keycode(keycode, record);

      if (ongoing_keycode != KC_NO) {
        get_clever_keycode(&ongoing_keycode, record);
        store_keycode(ongoing_keycode, record);
      }

    } else if (processingCK) {    // On keyrelease
      processingCK = false;
      record->keycode = recent[RECENT_SIZE - 1];
    }
}
```

Dans un premier temps, l’algorithme analyse la frappe en cours pour déterminer si elle sert à taper un caractère ou pas. Par exemple, une touche de navigation ou une touche affectée par Ctrl ou Alt ne sert pas à taper du texte. Elle ne sera donc pas mémorisée, et effacera le contenu du buffer pour éviter des effets de bord potentiellement catastrophiques. Cette fonction est paramétrable par l’utilisateur. 


Quand le résultat d’une frappe est un caractère, une [autre fonction](keyboards/splitkb/kyria/rev1/base/keymaps/Kawamashi/clever_keys.c) (également paramétrable par l’utilisateur) détermine si celui-ci doit être remplacé (par un autre caractère ou par une macro) ou bien si d’autres caractères doivent être insérés avant lui, en fonction du contenu du buffer. Celui-ci mémorise les 8 dernières touches. Enfin, le caractère est ajouté au buffer, en décalant les touches présentes auparavant. Pour éviter des effets intempestifs, le buffer se vide automatiquement au bout d’un certain temps d’inactivité du clavier.


Les Clever Keys me servent notamment :
- à ajouter automatiquement le `U` entre le `Q` et une autre voyelle (ou l’apostrophe)
- à mettre en majuscule la première lettre suivant une espace, lorsqu’elle est précédée par `.`, `?`, or `!`. Autrement dit, `Shift` s’applique automatiquement en début de phrase !
- à changer le comportement de la touche `Repeat` dans certaines circonstances. En français, je l’utilise aussi comme une touche [apostrophe](https://github.com/Kawamashi//blob/main/README.md#pour-le-fran%C3%A7ais-et-langlais).
- à donner des effets “magiques” à n’importe quelle touche, pas seulement la touche `Alt-Repeat`
- à paramétrer plus finement celle-ci, en tenant compte de la série de touches tapées avant et non pas seulement de la dernière


`Backspace` supprime la dernière touche du buffer, et décale les touches dans l’autre sens. La touche `Repeat` interagit avec le buffer, même quand il est mis à jour avec `Backspace`. C’est comme si `Repeat` voyageait dans le temps !


Parfois, une Clever Key se déclenche, mais ce n’est pas le comportement souhaité. Dans ce cas, il suffit d’effacer les touches produites par l’algorithme, et celui-ci effacera complètement le buffer. Comme ça, si on retape le même caractère, les Clever Keys ne s’appliqueront pas ce coup-ci. Par exemple, si je tape `Q` puis `I`, je vais obtenir `QUI`. Si pour une fois je voulais taper `QI`, il me suffirait de taper deux fois `Backspace` pour effacer `UI` et de retaper `I` pour obtenir `QI`. J’ai également une macro *Panique* qui permet de vider le buffer, pour éviter que les Clever Keys ne modifient la frappe suivante.
