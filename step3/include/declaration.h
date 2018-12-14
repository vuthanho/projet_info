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
char* getNextToken( char** token, char* current_line, int n_ligne, L_lexem* p_L, char* endline);
void lex_read_line( char *line, int nline, L_lexem* p_L, char* endline, int* p_etat);
void lex_load_file( char *file, unsigned int *nlines, L_lexem* p_L);
int isseparateur(char* position);
int automate(char** p_end, char* endline );
int is_new_section( T_lexem lex, int etat );
void first_check( char** token, char** p_current_adress, int n_ligne, L_lexem* p_L, char* endline, int etat);
void print_section(int etat);
instru_def* load_dico(char* chemin_dico, int* p_nb_inst);
int check_reg(char* reg);
int rec_check_reg(char* reg,int i);
void update_etat(int* p_etat, T_lexem lex);
void vider_Q_etiq(L_lexem* p_q_etiq, L_lexem* p_l_etiq, int section, int decalage);
void free_dico(instru_def** p_dico,int len_dico);
int in_dico(T_lexem lex,instru_def* dico,int nb_inst);
void get_arg(L_lexem lex);
unsigned long int_to_int(unsigned long k);
int verif_arg(char* op_voulu, L_lexem op_lu);
void verif_regle(instru_def* dico,int pos, L_lexem L, int decal_text);
void verif_data(L_lexem lex, int* p_decal_data);
L_lexem rec_verif_gram(int n_line, L_lexem L, L_lexem* p_q_etiq, L_lexem* p_l_etiq, int* p_etat, instru_def* dico,int nb_inst, int* p_decal_text, int* p_decal_data, int* p_decal_bss);
void verif_gram(int nb_line, L_lexem L, L_lexem* p_l_etiq, instru_def* dico,int nb_inst);
void verif_bss(L_lexem lex, int* p_decal_bss);
void free_liste_relocation(L_relocation L);
void afficher_liste_relocation(L_relocation L);
void afficher_relocation(relocation reloc);
void detection_base_offset(L_lexem L);

#endif
