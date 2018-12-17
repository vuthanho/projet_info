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

enum type_lex{unknown, symbole, val_dec, val_hex, registre, etiquette, parenthese, new_line, deux_points, virgule, comment, directive, chaine_car, base_offset};

enum type_reloc{R_MIPS_32=2, R_MIPS_26=4, R_MIPS_HI16=5, R_MIPS_LO16=6};

typedef struct {
  char* section;
  unsigned int decalage;
  int type;
  T_lexem* symbole;
} relocation;

typedef struct lsuiv2{
  relocation val;
  struct lsuiv2* suiv;
}* L_relocation;

struct R{
  unsigned int func:6,
  sa:5,
  rd:5,
  rt:5,
  rs:5,
  opcode:6;
};

struct I{
  unsigned int imm:16,
  rt:5,
  rs:5,
  opcode:6;
};

struct J{
  unsigned int targ:26,
  opcode:6;
};

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
