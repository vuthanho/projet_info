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
  /*char s[512];*/
  inst_def* tab;
  dico = fopen(chemin_dico,"r");
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

    if (fscanf(dico,"%s %s",&(tab[i].instruction),&(tab[i].regle)) != 2){
      free(tab);
      return NULL;
    }
/*    tab[i].instruction=calloc(1,sizeof(s));
    strcpy(tab[i].instruction,s);*/
  }
  fclose(dico);
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
