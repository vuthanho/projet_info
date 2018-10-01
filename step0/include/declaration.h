#ifndef _declaration_H_
#define _declaration_H_

#include <structure.h>

L_lexem ajoute_lex(T_lexem lex, L_lexem L);
void afficher_lexem(T_lexem lex);
char* getNextToken(char** token, char* current_line, int n_ligne, L_lexem* p_L, char* endline);
void lex_read_line( char *line, int nline, L_lexem* p_L,char* endline );
void lex_load_file( char *file, unsigned int *nlines, L_lexem* p_L );
int isseparateur(char* position);
int automate(char** p_end,char* endline );

#endif
