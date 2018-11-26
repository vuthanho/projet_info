#ifndef _declaration_H_
#define _declaration_H_


#define reg_elmt {"zero","at","v0","v1","a0","a1","a2","a3","t0","t1","t2","t3","t4","t5","t6","t7","s0","s1","s2","s3","s4","s5","s6","s7","t8","t9","k0","k1","gp","sp","fp","ra"}
#include <structure.h>

extern char* reg_mnem_list[];

L_lexem ajoute_lex(T_lexem lex, L_lexem L);
void afficher_lexem(T_lexem lex);
void afficher_liste_lex(L_lexem L);
void afficher_arg_lex(L_lexem L);
L_lexem reverse_list_lex(L_lexem L);
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
int check_reg(T_lexem reg);
int rec_check_reg(T_lexem reg,int i);
void update_etat(int* p_etat, T_lexem lex);
void vider_Q_etiq(L_lexem* p_q_etiq, L_lexem* p_l_etiq, int section, int decalage);
void in_dico(T_lexem lex, char** p_regle,inst_def* dico,int nb_inst);
void get_arg(L_lexem lex);
void verif_regle(char* regle,L_lexem first_op, int nb_op, int decal_text);
void verif_data(L_lexem lex, int* p_decal_data);
L_lexem rec_verif_gram(int n_line, L_lexem L, L_lexem* p_q_etiq, L_lexem* p_l_etiq, int* p_etat, inst_def* dico,int nb_inst, int* p_decal_text, int* p_decal_data, int* p_decal_bss);
void verif_gram(int nb_line, L_lexem L, L_lexem* p_l_etiq, inst_def* dico,int nb_inst);
void verif_bss(L_lexem lex, int* p_decal_bss);

#endif
