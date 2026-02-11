# Kawamashi’s QMK configuration

This is a template repository which allows for an external set of QMK keymaps to be defined and compiled. This is useful for users who want to maintain their own keymaps without having to fork the main QMK repository.

![kb](GMKyria-min.jpg)

## Custom features
* [Configuration des Layer-Tap](#Configuration-des-Layer-Tap)
* [Modificatrices](#Modificatrices)
* [One-shot for All](#One-Shot-for-All)
* [Couches préfixées](#Couches-préfixées)
* [Mod Word](#Mod-Word)
* [Layer Word](#Layer-Word)
* [Clever Keys](#Clever-Keys)

&nbsp;</br> &nbsp;</br>

## Configuration des Layer-Tap
J’utilise des layer-tap sur mes touches de pouces pour accéder aux couches dont je me sers le plus (symboles, chiffres, navigation, raccourcis et modificatrices).


J’utilise le [Permissive Hold](https://docs.qmk.fm/tap_hold#tap-or-hold-decision-modes), qui me permet de ne pas avoir à attendre le *tapping term* en cas de nested tap. Grâce à ça, je n’ai plus de faux‑négatifs avec mes layers‑tap. 


Pour éviter les déclenchements par erreur (lors de roulements par exemple), je me base sur une vérification de la position des touches : avant le typing_term, si l’une de mes touches de pouce principales est enfoncée ainsi qu’une autre touche de la même main, alors la touche de pouce produira immédiatement un tap et non un hold. Pour cela, j’ai adopté l’approche de [Filterpaper](https://github.com/filterpaper/qmk_userspace?tab=readme-ov-file#contextual-mod-taps), que je trouve légère et efficace. Elle est basée sur l’utilisation de la fonction `pre_process_record_user` et sur une utilisation avancée de la fonction `get_hold_on_other_key_press` :

```c
bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {

    if (record->event.pressed) {
        // Cache the next input for mod-tap decisions
        next_keycode = keycode;
        next_record  = *record;
    }
    return true;
}
```
```c
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
Pour les modificatrices, je n’utilise pas de Home-row Mods à proprement parler. Mes modificatrices sont des [Callum mods]([https://github.com/callum-oakley/qmk_firmware/tree/master/users/callum](https://github.com/callum-oakley/qmk_firmware/tree/master/users/callum#oneshot-modifiers)), autrement dit des [one‑shot mods](https://docs.qmk.fm/one_shot_keys) situés sur la rangée de repos d’une couche spécifique. J’aime cette approche qui ne produit pas de délai dans l’affichage des frappes, contrairement aux HRM. 


Les Callum mods peuvent être maintenus comme des modificatrices classiques, ou employés comme sticky keys. Ils ont la particularité de ne pas utiliser de timer. Du coup, ils peuvent être combinés sans être obligé de marquer une pause entre chaque modificatrice.


J’ai modifié le code de Callum Oakley pour corriger un bug qui affectait les roulements. Dans mon implémentation, un one-shot mod se désactive lorsqu’on appuie une deuxième fois dessus, ou automatiquement au bout d’un certain temps d’inactivité. Le code se trouve [ici](keyboards/splitkb/kyria/rev1/base/keymaps/Kawamashi/features/oneshot.c).

&nbsp;</br>

## One-shot for All
Les Callum mods sont placés sur des layers secondaires. Je n’aime pas le fait de devoir maintenir une touche pour y accéder, je voulais pouvoir le faire avec une one-shot layer. De plus, je voulais conserver des one-shot shift sur ma keymap, et je n’avais pas la place de tout mettre. J’ai donc conçu les One-Shot for All (OS4A) pour tout faire à la fois : 
- lors d’un appui maintenu, ces touches produisent `Shift`.
- après un appui simple, elles activent une couche liée.
- après un 2e appui, elles désactivent la couche liée et annulent tous les Callum mods.

Sur la moitié de celle-ci, on trouve les modificatrices, qui peuvent être enchaînées les une avec les autres. Sur l’autre moitié, on retrouve les alphas. En cas d’appui sur une lettre, celle-ci sera shiftée et la couche sera désactivée. On retrouve ainsi le comportement d’un one-shot shift. J’ai une touche OS4A pour chaque moitié du clavier, voilà un exemple de couche liée :

![couche OS4A](OS4A.png)

- Pour taper `T` (majuscule), il suffit de taper `OS4A` `T`.
- Pour faire `Ctrl S`, on tape `OS4A` `⎈` `S`.
- Pour faire `Ctrl Shift T`, on tape `OS4A` `⎈` `⇧` `S`. L’ordre des modificatrices n’est pas importante, elles peuvent même être tapées simultanément.

Les OS4A fonctionnent grâce à des [couches préfixées](#Couches-préfixées) et à des [Layer Word](#Layer-Word). Les OS4A impliquent des timers, puisqu’au fond ce sont des mod-taps modifiés. Cependant, grâce à la configuration des [Tap-Hold](#Configuration-des-Layer-Tap), on peut faire un roulement sur une touche OS4A et une modificatrice sans aucun souci. En utilisation réelle, il n’y a pas de timer à respecter. 

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

J’ai également une combo `PANIC`, pour remettre le clavier dans son état nominal. Cette combo :
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


Pour chaque séparateur, l’utilisateur peut définir un compteur, pour indiquer à la fonction qu’elle doit se désactiver après le prochain mot. Par exemple, ` et ` déclenche ce compteur mais pas `, `. 


Pour illustrer le fonctionnement de Caps List, si je veux écrire `Les principaux firmwares sont QMK, ZMK et KRK, je préfère QMK.` : 
- j’active Caps List avant de taper ma liste.
- Caps List active automatiquement Caps Word.
- je tape `qmk`. Tous les caractères étant des lettres, ni Caps List ni Caps Word ne se désactivent.
- je tape `,`. Caps Word se désactive. Caps List sera désactivé si un séparateur de liste n’est pas détecté dans les prochains caratères, grâce à un compteur.
- je tape `␣`. Caps List est toujours actif. Le compteur s’incrémente.
- je tape `z`. Caps List détecte que la séquence précédente (`,␣`) est un séparateur de liste : Caps Word est réactivé, le compteur est réinitialisé.
- je tape `mk`. Tous les caractères étant des lettres, ni Caps List ni Caps Word ne se désactivent.
- je tape `␣`. Caps Word se désactive. Le compteur de Caps List s’incrémente.
- je tape `et␣`. Le compteur s’incrémente.
- je tape `k`. Caps List détecte que la séquence précédente (`,␣`) est un séparateur de liste : Caps Word est réactivé, le compteur est réinitialisé. Ce séparateur a été configuré comme séparateur final de liste : il change la valeur limite du compteur.
- je tape `rk`. Tous les caractères étant des lettres, ni Caps List ni Caps Word ne se désactivent.
- je tape `,`. Caps Word se désactive. Le compteur de Caps List s’incrémente.
- je tape `␣`. Le compteur arrive à sa limite, Caps List se désactive. Le prochain mot ne réactivera donc pas Caps Word.

`Backspace` décrémente le compteur.

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
- `Shift` est ajouté à `PgUp` et `PgDn` pour étendre la sélection au début/à la fin de la page

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
