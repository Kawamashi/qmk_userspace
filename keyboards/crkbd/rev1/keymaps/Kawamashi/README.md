# Krypton

Le projet Krypton a pour but de fournir une configuration de base fiable et fonctionnelle pour accueillir Propergol et ses spécificités. En effet, Propergol utilise 4 touches de pouce, ce qui impose un certain nombre de contraintes pour le clavier (je recommande d’avoir 3 touches par pouces) et sa configuration.

Avec Krypton, je n’ai pas pour ambition de fournir une configuration “parfaite”, mais plutôt une base solide que vous pourrez par la suite modifier pour qu’elle colle au mieux à vos besoins. *Tweak it until you love it !* 


## Cahier des charges

Le projet [Selenium](https://onedeadkey.github.io/selenium/) vient de se lancer au sein de la communauté des Ergonautes. J’ai essayé de rendre Krypton aussi proche de Selenium que possible, tout en tenant compte de mon expérience personnelle. Je souhaite que quelqu’un venant d’une configuration Selenium ne soit pas dépaysé en testant Krypton. 


Propergol utilisant quatre touches de pouce, il n’est pas possible d’utiliser Selenium telle quelle avec cette disposition. Par exemple, mettre des touches d’édition (backspace, enter, tab et esc) sous les pouces n’est pas vraiment possible.


Krypton s’adapte à l’utilisateur. Quelqu’un qui débute préfèrera sans doute la configuration à six colonnes tandis que quelqu’un de plus expérimenté se dirigera naturellement vers une configuration utilisant les [Home-Row Mods](https://precondition.github.io/home-row-mods) (HRM). Enfin, si vous voulez aller encore plus loin, je vous encourage à utiliser des combos. 



## Les touches de pouce

Cinq des six touches de pouces sont des Layer-Tap. Par exemple, la touche `Espace` vous envoie sur la couche de navigation quand on la maintient. 

Shift est une modificatrice particulière. Elle est principalement utilisée en cours de frappe, contrairement à Ctrl, Alt et Gui. Par conséquent, il est plus confortable d’avoir Shift sous les pouces. À gauche, Krypton utilise un [One-Shot](https://docs.qmk.fm/one_shot_keys) Shift. Ce genre de modificatrices fonctionne comme une touche morte : on presse et on relâche le one-shot, et la touche suivante (et rien qu’elle !) sera affectée par la modificatrice. Quand on maintient appuyé le one-shot shift, il se comporte comme une modificatrice classique : ça permet de l’utiliser avec la souris, ou pour mettre en majuscule plusieurs lettres.

Propergol a le `E` et la touche magique sous le pouce de gauche. Il faut donc un shift à droite. Le layer-tap de la couche des symboles est paramétré de manière particulière pour ça : quand on la maintient enfoncée, elle active la couche des symboles mais quand on appuie dessus simplement, elle se comporte comme un one-shot shift.



## Les colonnes extérieures

Idéale quand on débute, la configuration à 42 touches est simple à appréhender. Elle utilise les colonnes extérieures pour les modificatrices (Ctrl, Alt et Gui) et les principales touches d’édition (Backspace, Delete, Enter, Escape, Tab et reverse Tab). Pour cela, elle utilise les [Mod-Tap](https://docs.qmk.fm/mod_tap#changing-hold-function) de QMK. Pour envisager sereinement une future migration vers une configuration utilisant les home-row mods, il est préférable d’utiliser les mod-taps opposés à la lettre. Par exemple, si vous voulez faire `Gui L`, `L` étant sur la moitié droite du clavier, utilisez la modificatrice de gauche.
De plus, je vous conseille de prendre l’habitude de relâcher les touches sitôt que vous avez appuyé dessus, d’avoir une frappe “snappy”.

