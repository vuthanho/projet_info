#define _POSIX_C_SOURCE 200112L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

#include "global.h"
#include "notify.h"
#include "structure.h"
#include "declaration.h"

char* reg_mnem_list[] = reg_elmt;

int is_new_section( T_lexem lex, int etat )
{
  if (lex.type == 11)
  {
        if (!strcmp(".set",lex.nom))
        {
          return 0;
        }
        else if (!strcmp(".text",lex.nom))
        {
          return 1;
        }
        else if (!strcmp(".data",lex.nom))
        {
          return 2;
        }
        else if (!strcmp(".bss",lex.nom))
        {
          return 3;
        }
        else
        {
          switch (etat){
            case 2: /* Si on est dans une section data */
              if ( !strcmp(".word",lex.nom) && !strcmp(".byte",lex.nom) && !strcmp(".asciiz",lex.nom) && !strcmp(".space",lex.nom) ){
                return -1;
              }
              break;
            case 3: /* Si on est dans une section bss */
              if (!strcmp(".space",lex.nom)){
                return -1;
              }
              break;
            default: /* Sinon il ne devrait pas y avoir de directive */
              return -1;
              break;
          }
          return 4;
        }
  }
  return 4;
}

void update_etat(int* p_etat, T_lexem lex)
{
  if (lex.type == 11)
  {
        if (!strcmp(".set",lex.nom))
        {
          *p_etat = 0;
        }
        else if (!strcmp(".text",lex.nom))
        {
          *p_etat = 1;
        }
        else if (!strcmp(".data",lex.nom))
        {
          *p_etat = 2;
        }
        else if (!strcmp(".bss",lex.nom))
        {
          *p_etat = 3;
        }
  }
}

void first_check( char** token, char** p_current_adress, int n_ligne, L_lexem* p_L, char* endline, L_lexem* p_Ls, int etat)
{
  switch (etat)
  {
    case 0:
      *p_current_adress = getNextToken( token, *p_current_adress, n_ligne, p_L, endline, p_Ls);
      if ( strcmp( ((*p_L)->val).nom, "noreorder") )
      {
        ERROR_MSG("Error line %d : noreorder intruction expected",n_ligne);
      }
      else {
        *p_current_adress = getNextToken( token, *p_current_adress, n_ligne, p_L, endline, p_Ls);
      }
      if (*p_current_adress != NULL) {
        ERROR_MSG("Error line %d : no arguments expected after noreorder",n_ligne);
      }
      break;
    case 1:
      *p_current_adress = getNextToken( token, *p_current_adress, n_ligne, p_L, endline, p_Ls);
      if (*p_current_adress != NULL) {
        ERROR_MSG("Error line %d : no arguments expected after .text",n_ligne);
      }
      break;
    case 2:
      *p_current_adress = getNextToken( token, *p_current_adress, n_ligne, p_L, endline, p_Ls);
      if (*p_current_adress != NULL) {
        ERROR_MSG("Error line %d : no arguments expected after .data",n_ligne);
      }
      break;
    case 3:
      *p_current_adress = getNextToken( token, *p_current_adress, n_ligne, p_L, endline, p_Ls);
      if (*p_current_adress != NULL) {
        ERROR_MSG("Error line %d : no arguments expected after .bss",n_ligne);
      }
      break;
  }
}

void print_section(int etat)
{
  printf("Nouvelle section détectée : ");
  switch (etat) {
    case 0:
      printf("set\n");
      break;
    case 1:
      printf("text\n");
      break;
    case 2:
      printf("data\n");
      break;
    case 3:
      printf("bss\n");
      break;
  }
}

inst_def* load_dico(char* chemin_dico, int* p_nb_inst){
  FILE* dico;
  int i;
  inst_def* tab;
  dico = fopen(chemin_dico,"r");
  printf("Ouverture du dictionnaire d'instructions\n");
  if (dico == NULL){
    WARNING_MSG("Empty instruction dictionary");
    return NULL;
  }
  if (fscanf(dico,"%d",p_nb_inst) != 1){
    WARNING_MSG("Empty instruction dictionary");
    return NULL;
  }
  tab = calloc(*p_nb_inst,sizeof(*tab));
  if (tab == NULL){
    WARNING_MSG("Empty instruction dictionary");
    return NULL;
  }
  for (i=0;i<*p_nb_inst;i++){

    if (fscanf(dico,"%s %s\n",(tab[i].instruction),(tab[i].regle)) != 2){
      WARNING_MSG("Il y a bug dans le dico ligne %d",i+1);
      printf("%s\n%s\n",(tab[i].instruction),(tab[i].regle));
      free(tab);
      fclose(dico);
      printf("Fermeture du dictionnaire d'instructions\n");
      return NULL;
    }
  }
  fclose(dico);
  printf("Fermeture du dictionnaire d'instructions\n");
  return tab;
}

/*
void free_dico(inst_def* dico, int nb_inst){
  int i;
  for (i=0;i<nb_inst;i++){
    free(dico[i].instruction);
    dico[i].instruction = NULL;
  }
  free(dico);
  dico = NULL;
}*/

int check_reg(T_lexem reg){
  int i = 0;
  return rec_check_reg(reg,i);
}

int rec_check_reg(T_lexem reg,int i){
  char reg_test[3];
  sprintf (reg_test,"%d",i);
  if (i==32){
    ERROR_MSG("Unknown register : '%s'",reg.nom);
  }
  if (strcmp(reg.nom+1,reg_mnem_list[i]) && strcmp(reg.nom+1,reg_test) ){
    return rec_check_reg(reg,i+1);
  }
  return i;
}

LISTE_GENERIQUE add_debut_liste_gen(void* elmt, LISTE_GENERIQUE Liste)
{
  LISTE_GENERIQUE nListe = malloc(sizeof(*nListe));
  nListe -> pval = elmt;
  nListe -> suiv = Liste;
  return nListe;
}

LISTE_GENERIQUE add_liste_gen(void* elmt, LISTE_GENERIQUE Liste)
{

  if (Liste == NULL)
  {
    LISTE_GENERIQUE nListe = malloc(sizeof(*nListe));
    nListe -> pval = elmt;
    nListe -> suiv = Liste;
    return nListe;
  }
  else
  {
    return aux_add_liste_gen(elmt,Liste,Liste);
  }
}

LISTE_GENERIQUE aux_add_liste_gen(void* elmt, LISTE_GENERIQUE Liste, LISTE_GENERIQUE Suivante)
{
  if ((Suivante->suiv)==NULL)
  {
    LISTE_GENERIQUE nListe = malloc(sizeof(*nListe));
    nListe -> pval = elmt;
    nListe -> suiv = NULL;
    Suivante->suiv = nListe;
    return Liste;
  }
  return aux_add_liste_gen(elmt,Liste,Suivante->suiv);
}

void vider_Q_etiq(L_lexem Q_etiq, LISTE_GENERIQUE* p_l_etiq, int section, int decalage)
{
  L_lexem a_vider = Q_etiq;
  while(a_vider != NULL)
  {
    Etiquette etiq;
    etiq.label = calloc(1,strlen( (a_vider->val).nom )+1);
    strcpy(etiq.label,(a_vider->val).nom);
    etiq.label[strlen( (a_vider->val).nom )]='\0';
    etiq.section = section;
    etiq.decalage = decalage;
    *p_l_etiq = add_liste_gen(&etiq, *p_l_etiq);
    a_vider = a_vider->suiv;
  }

}

void in_dico(T_lexem lex, char** p_regle,inst_def* dico,int nb_inst)
{
  int i=0;
  while( (*p_regle == NULL) && (i<nb_inst) )
  {
    printf("%s %s\n",dico[i].instruction, lex.nom);
    if(!strcasecmp(dico[i].instruction, lex.nom))
    {
      *p_regle = malloc( strlen(dico[i].regle) );
      strcpy(*p_regle,dico[i].regle);
    }
    i++;
  }
  ERROR_MSG("Instruction inconnu : %s",lex.nom);
}

void get_arg(L_lexem lex, LISTE_GENERIQUE* p_liste_op)
{
  int n_line = (lex -> val).n_ligne;
  int nb_parenthese = 0;
  int nb_virg = 0;
  L_lexem op = lex;
  int nb_op = 0;
  while( ((op->val).n_ligne == n_line) && (op != NULL) )
  {
    if (nb_parenthese<0){
      ERROR_MSG("Error : missing '(' before ')' line %d",n_line);
    }
    if (nb_virg>1){
      ERROR_MSG("Error : missing argument after ',' line %d",n_line);
    }
    switch ((op->val).type) {
      case 9:
        nb_virg++;
        break;
      case 6:
        if (*((op->val).nom) == 40){
          nb_parenthese++;
        }
        else {
          nb_parenthese--;
        }
        *p_liste_op = add_liste_gen(op,*p_liste_op);
        break;
      default:
        nb_virg = 0;
        nb_op++;
        add_liste_gen(op,*p_liste_op);
        break;
    }
    op = lex->suiv;
  }
  if (nb_parenthese!=0){
    ERROR_MSG("Error : check the parenthesis line %d",n_line);
  }
  if (nb_virg!=0){
    ERROR_MSG("Error : missing argument after ',' line %d",n_line);
  }
  *p_liste_op = add_debut_liste_gen(&nb_op,*p_liste_op);
}

void verif_regle(char* regle, LISTE_GENERIQUE op)
{
  int* nb_op = op->pval;
  if (strlen(regle) > *nb_op)
  {
    ERROR_MSG("Error : not enough arguments");
  }
  if (strlen(regle) < *nb_op)
  {
    ERROR_MSG("Error : too many arguments");
  }
  int isok = 1;
  L_lexem lex_lu = (op->suiv)->pval;
  char* arg_voulu = regle;
  while(isok && (lex_lu != NULL) )
  {
    if(*arg_voulu-'0' != (lex_lu->val).type)
    {
      isok = 0;
    }
    lex_lu = lex_lu->suiv;
    arg_voulu = arg_voulu+1;
  }
  if (!isok)
  {
    ERROR_MSG("Error : wrong type of argument line %d",(lex_lu->val).n_ligne);
  }
}

L_lexem rec_verif_gram(int n_line, L_lexem L, L_lexem* p_q_etiq, LISTE_GENERIQUE* p_l_etiq, int* p_etat, inst_def* dico,int nb_inst, int* p_decal_text, int* p_decal_data, int* p_decal_bss)
{
  if ( (n_line != (L->val).n_ligne) || (L == NULL) ) {
    return L;
  }
  if ((L->val).type == 5)
  {
    *p_q_etiq = ajoute_lex(L->val,*p_q_etiq);
    return rec_verif_gram(n_line, L->suiv, p_q_etiq, p_l_etiq, p_etat, dico, nb_inst, p_decal_text, p_decal_data, p_decal_bss);
  }
  update_etat(p_etat,L->val);
  if ((*p_etat == 1) && ((L->val).type == 1))
  {
    LISTE_GENERIQUE liste_op = NULL;
    vider_Q_etiq(*p_q_etiq, p_l_etiq, *p_etat, *p_decal_text);
    char* regle = NULL;
    in_dico(L->val, &regle, dico, nb_inst);
    get_arg(L, &liste_op);
    verif_regle(regle, liste_op);
    *p_decal_text = *p_decal_text + 4;
    free(regle);
    regle = NULL;

  }
  return rec_verif_gram(n_line, L->suiv, p_q_etiq, p_l_etiq, p_etat, dico, nb_inst, p_decal_text, p_decal_data, p_decal_bss);
}

void verif_gram(int nb_line, L_lexem L, LISTE_GENERIQUE* p_l_etiq, inst_def* dico,int nb_inst)
{
  L_lexem debut_ligne = L;
  int i;
  int etat = -1;
  int decal_text = 0;
  int decal_data = 0;
  int decal_bss = 0;
  L_lexem q_etiq = NULL;

  for (i=0;i<nb_line;i++)
  {
    printf("Analyse de la ligne %d\n",(debut_ligne->val).n_ligne);
    debut_ligne = rec_verif_gram((debut_ligne->val).n_ligne, debut_ligne, &q_etiq, p_l_etiq, &etat, dico, nb_inst, &decal_text, &decal_data, &decal_bss);
  }

}
