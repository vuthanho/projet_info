Squelette de code de l'assembleur MIPS du projet info PHELMA.

Le répertoire est organisé ainsi :
.
├── tests
│   ├── miam.s
│   └── test.s
├── src
│   ├── main.c
│   ├── automate.c
│   ├── grammaire.c
│   └── lex.c
├── include
│   ├── notify.h
│   ├── structure.h
│   ├── declaration.h
│   └── global.h
├── dico
│   └── dico_instr.txt
├── doc
│   └── livrables
├── README.txt
├── Makefile
└── Doxyfile

- tests : contient les fichiers pour tester le programme
- src : qui contient le code C de votre interpréteur.
- include : qui contient toutes les définitions de types et prototypes de votre programme. Lisez attentivement les headers avant de commencer à coder et de les modifier.
- doc : contient la documentation et le répertoire ou stocker les rapports de livrable.
- README.txt : ce document
- Makefile pour compiler soit en mode debug (afficher les traces du programme) soit en mode release (produit final)
- Doxyfile : fichier de configuration du générateur automatique de documentation doxygen

--- pour compiler le code en mode debug (il créé l'exécutable 'as-mips' qui affiche les traces)
$ make debug



--- pour l'executer
$ ./as-mips FICHIER.S



--- pour compiler le code en mode release (il créé l'exécutable 'as-mips' qui n'affiche pas les traces. En tant que développeur, vous utiliserez très rarement cette commande)
$ make release


--- Génération de l'archive

Tapez "make archive". Vous aurez produit un fichier se terminant par "tgz". C'est l'archive
que vous nous enverrez par email.



--- Documentation du code

Si vous voulez documenter votre code, regardez la syntaxe appropriée pour
les tags Doxygen :

http://www.stack.nl/~dimitri/doxygen/docblocks.html

Une fois votre code commente, tapez "make documentation" (nous vous fournissons
un Doxyfile qui ira bien pour ce que vous avez a faire).

Note : il faut avoir installé le package doxygen. Sous Ubuntu / Debian, tapez :
sudo apt-get install doxygen

--- Erreurs détectées

* Les caractères n'ayant rien à faire dans un fichier .s
* Les directives non répertoriées
  -> .word | .byte | .asciiz | .space sont les seules directives acceptées dans la section .data
  -> .space est la seule directive acceptée dans la section .bss
* Les directives dans une section .text
* Seule l'instruction noreorder est acceptée dans la section .set
* Les erreurs de registre
* Les erreurs sur les nombres hexadécimaux
