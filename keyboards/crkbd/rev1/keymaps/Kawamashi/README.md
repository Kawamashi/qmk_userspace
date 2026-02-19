# Krypton

Le projet Krypton a pour but de fournir une configuration de base fiable et fonctionnelle pour accueillir Propergol et ses spécificités. En effet, Propergol utilise 4 touches de pouce, ce qui impose un certain nombre de contraintes pour le clavier (je recommande d’avoir 3 touches par pouces) et sa configuration.

Avec Krypton, je n’ai pas pour ambition de fournir une configuration “parfaite”, mais plutôt une base solide que vous pourrez par la suite modifier pour qu’elle colle au mieux à vos besoins. *Tweak it until you love it !* 


## Cahier des charges

Le projet [Selenium](https://onedeadkey.github.io/selenium/) vient de se lancer au sein de la communauté des Ergonautes. J’ai essayé de rendre Krypton aussi proche de Selenium que possible, tout en tenant compte de mon expérience personnelle. Je souhaite que quelqu’un venant d’une configuration Selenium ne soit pas dépaysé en testant Krypton. 


Propergol utilisant quatre touches de pouce, il n’est pas possible d’utiliser Selenium telle quelle avec cette disposition. Par exemple, mettre des touches d’édition (Backspace, Enter, Tab et Escape) sous les pouces n’est pas vraiment possible.


Krypton s’adapte à l’utilisateur. Quelqu’un qui débute préfèrera sans doute la configuration à six colonnes tandis que quelqu’un de plus expérimenté se dirigera naturellement vers une configuration utilisant les [Home-Row Mods](https://precondition.github.io/home-row-mods) (HRM). Enfin, si vous voulez aller encore plus loin, je vous encourage à utiliser des combos. 



## Les touches de pouce

Cinq des six touches de pouces sont des Layer-Tap. Par exemple, la touche `Espace` vous envoie sur la couche de navigation quand on la maintient plus d’un certain temps (le [tapping term](https://precondition.github.io/home-row-mods#default)).

Shift est une modificatrice particulière. Elle est principalement utilisée en cours de frappe, contrairement à Ctrl, Alt et Gui. Par conséquent, il est plus confortable d’avoir Shift sous les pouces. À gauche, Krypton utilise un [One-Shot](https://docs.qmk.fm/one_shot_keys) Shift. Ce genre de modificatrices fonctionne comme une touche morte : on presse et on relâche le one-shot, et la touche suivante (et rien qu’elle !) sera affectée par la modificatrice. Quand on maintient appuyé le one-shot shift, il se comporte comme une modificatrice classique : ça permet de l’utiliser avec la souris, ou pour mettre en majuscule plusieurs lettres.

Propergol a le `E` et la touche magique sous le pouce de gauche. Il faut donc un shift à droite. Le layer-tap de la couche des symboles est paramétré de manière particulière pour ça : quand on la maintient enfoncée, elle active la couche des symboles mais quand on appuie dessus simplement, elle se comporte comme un one-shot shift.



## Les colonnes extérieures

Idéale quand on débute, la configuration à 42 touches est simple à appréhender. Elle utilise les colonnes extérieures pour les modificatrices (Ctrl, Alt et Gui) et les principales touches d’édition (Backspace, Delete, Enter, Escape, Tab et reverse Tab). Pour cela, elle utilise les [Mod-Tap](https://docs.qmk.fm/mod_tap#changing-hold-function) de QMK. Pour plus de confort et afin d’envisager sereinement une future migration vers une configuration utilisant les home-row mods, il est préférable d’utiliser les mod-taps opposés à la lettre. Par exemple, si vous voulez faire `Gui L`, `L` étant sur la moitié droite du clavier, utilisez la modificatrice de gauche.
De plus, je vous conseille de prendre l’habitude de relâcher les touches sitôt que vous avez appuyé dessus, d’avoir une [frappe “snappy”](https://precondition.github.io/home-row-mods#quick-swift-taps).


## Passer à 5 colonnes
### La couche de navigation
Pour passer à 5 colonnes, il faut avoir les touches d’édition sur le pavé principal. Krypton leur dédie la moitié droite de la couche de navigation. On accède très facilement à celle-ci en maintenant enfoncée l’une des touches repos de pouce. Sur la rangée de repos, on y trouve Backspace, Enter et Escape, ainsi que Ctrl + Backspace, qui permet d’effacer un mot entier. Tab et Delete sont sous l’index, pour une bonne accessibilité. On trouve également sur cette moitié de clavier les touches de mon module [Mod Word](https://github.com/Kawamashi/qmk_userspace/tree/main?tab=readme-ov-file#mod-word), dont [Select Word](https://github.com/Kawamashi/qmk_userspace/tree/main?tab=readme-ov-file#word-selection] et [Caps Word](https://docs.qmk.fm/features/caps_word).


Les touches de navigation se trouvent sur la moitié gauche de la couche, ainsi que des raccourcis claviers usuels (couper/copier/coller, annuler/rétablir et tout sélectionner). Leur placement à gauche les rend accessibles même avec la souris dans la main droite.

### Les Home-Row Mods
Supprimer les colonnes extérieures implique également de rapprocher les modificatrices. Le mieux pour ça est de passer aux Home-Row Mods (HRM), qui permettent l’accès à toutes les modificatrices sans quitter la rangée de repos. [développer les avantages des HRM ainsi que la difficulté de leur paramétrage].

Faire reposer tout le paramétrage des HRM sur le tapping term peut poser problème :
- si le tapping term est trop court, on risque d’activer une modificatrice par erreur au cours de la frappe, notamment en cas de roulement de plusieurs touches.
- s’il est trop long, l’utilisation des modificatrices sera peu naturelle et donc pénible.

Urob a élaboré une solution qui permet d’oublier le tapping term, les [Timerless HRM](https://github.com/urob/zmk-config?tab=readme-ov-file#timeless-homerow-mods). En quelques mots : 
- pour éviter le premier problème, on met une valeur élevée pour le tapping term (250 ms).
- pour éviter le deuxième problème, on utilise le [Permissive Hold](https://precondition.github.io/home-row-mods#permissive-hold). C’est une option de QMK qui permet d’activer la modificatrice sans avoir besoin d’attendre le tapping term, quand la 2e touche est pressée puis relâchée alors que la 1ère est encore enfoncée. Cela correspond au geste qu’on fait naturellement quand on utilise un raccourci clavier.
- malheureusement, cette option augmente le risque de déclenchement inopiné des HRM lors des roulements de plusieurs lettres. Pour éviter ça, on active l’option [Chordal Hold](https://docs.qmk.fm/tap_hold#chordal-hold), qui n’active pas les HRM lorsque la touche mod-tap et l’autre touche sont du même côté du clavier.

Ainsi, simplement en utilisant les modificatrices avec une touche de la main opposée, vous n’aurez plus de faux positifs ni de faux négatifs liés au tapping term !
