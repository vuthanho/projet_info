#ifndef _STRUCTURE_H_
#define _STRUCTURE_H_


typedef struct {
  int type;
  int n_ligne;
  char* nom;
} T_lexem;

typedef struct lsuiv{
  T_lexem val;
  struct lsuiv* suiv;
}* L_lexem;

typedef struct maillon {
  void*
  pval;
  struct maillon *suiv;
} *LISTE_GENERIQUE;

typedef struct { char instruction[5]; char regle[4];} inst_def;
enum type_lex{symbole, val_dec, val_hex, registre, etiquette, comment, new_line, deux_points, virgule, parenthese, directive, chaine_car};


/*
symbole = 1
val_dec = 2
val_hex = 3
registre = 4
etiquette = 5
comment = 6
new_line = 7
deux_points = 8
virgule = 9
parenthese = 10
directive = 11
chaine de caractères = 12
*/

#endif
