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

typedef struct {
  char* label;
  int section;
  int decalage;
} Etiquette;

typedef struct maillon {
  void* pval;
  struct maillon *suiv;
} *LISTE_GENERIQUE;

typedef struct { char* instruction; char* regle;} inst_def;
enum type_lex{symbole, val_dec, val_hex, registre, etiquette, parenthese, new_line, deux_points, virgule, comment, directive, chaine_car};


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
