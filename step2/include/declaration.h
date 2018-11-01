#ifndef _declaration_H_
#define _declaration_H_

#include <structure.h>

L_lexem ajoute_lex(T_lexem lex, L_lexem L);
void afficher_lexem(T_lexem lex);
void afficher_liste_lex(L_lexem L);
void free_liste(L_lexem L, int option);
char* getNextToken( char** token, char* current_line, int n_ligne, L_lexem* p_L, char* endline, L_lexem* p_Ls);
void lex_read_line( char *line, int nline, L_lexem* p_L, char* endline, L_lexem* p_Ls, int* p_etat);
void lex_load_file( char *file, unsigned int *nlines, L_lexem* p_L, L_lexem* p_Ls);
int isseparateur(char* position);
int automate(char** p_end, char* endline );
int is_new_section( T_lexem lex, int etat );
void first_check( char** token, char** p_current_adress, int n_ligne, L_lexem* p_L, char* endline, L_lexem* p_Ls, int etat);
void print_section(int etat);
inst_def* load_dico(char* chemin_dico, int* p_nb_inst);
void free_dico(inst_def* dico, int nb_inst);

#endif
