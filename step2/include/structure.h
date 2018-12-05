#ifndef _STRUCTURE_H_
#define _STRUCTURE_H_


typedef struct {
  int type;
  int n_ligne;
  char* nom;
  int decalage;
} T_lexem;

typedef struct lsuiv{
  T_lexem val;
  int section;
  int nb_op;
  struct lsuiv* suiv;
  struct lsuiv* arg;
}* L_lexem;


typedef struct {
  char instruction[10];
  char type;
  int nb_op;
  char* arguments[3];
} instru_def;

enum type_lex{unknown, symbole, val_dec, val_hex, registre, etiquette, parenthese, new_line, deux_points, virgule, comment, directive, chaine_car};


/*
symbole = 1
val_dec = 2
val_hex = 3
registre = 4
etiquette = 5
parenthese = 6
new_line = 7
deux_points = 8
virgule = 9
comment = 10
directive = 11
chaine de caractères = 12
*/

#endif
