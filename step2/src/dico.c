#define _POSIX_C_SOURCE 200112L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#include <limits.h>
#include <errno.h>

#include "global.h"
#include "notify.h"
#include "structure.h"
#include "declaration.h"

void free_dico(instru_def** p_dico,int len_dico)
{
  int i;
  for(i=0;i<len_dico;i++)
  {
    free(((*p_dico+i)->arguments)[0]);
    free(((*p_dico+i)->arguments)[1]);
    free(((*p_dico+i)->arguments)[2]);
    ((*p_dico+i)->arguments)[0] = NULL;
    ((*p_dico+i)->arguments)[1] = NULL;
    ((*p_dico+i)->arguments)[2] = NULL;
  }
}

instru_def* load_dico(char* chemin_dico, int* p_nb_inst){
  FILE* dico;
  int i;
  instru_def* tab;
  dico = fopen(chemin_dico,"r");
  /*printf("Ouverture du dictionnaire d'instructions\n");*/
  if (dico == NULL){
    WARNING_MSG("Empty instruction dictionary");
    return NULL;
  }
  if (fscanf(dico,"%d",p_nb_inst) != 1){
    WARNING_MSG("Empty instruction dictionary");
    return NULL;
  }
  tab = calloc(*p_nb_inst,sizeof(*tab));
  /*printf("nombre d'instructions : %d\n",*p_nb_inst);*/
  if (tab == NULL){
    WARNING_MSG("Empty instruction dictionary");
    return NULL;
  }
  for (i=0;i<*p_nb_inst;i++){
    ((tab+i)->arguments)[0] = NULL;
    ((tab+i)->arguments)[1] = NULL;
    ((tab+i)->arguments)[2] = NULL;
    if (fscanf(dico,"%s %c %d ", (tab+i)->instruction, &((tab+i)->type), &((tab+i)->nb_op) ) != 3){
      WARNING_MSG("Il y a bug dans le dico ligne %d",i+1);
      free(tab);
      fclose(dico);
      /*printf("Fermeture du dictionnaire d'instructions\n");*/
      return NULL;
    }
    /*printf("%s %c %d ",(tab+i)->instruction, (tab+i)->type, (tab+i)->nb_op);*/
    int k;
    char next_op[10];
    for (k=0;k<(tab+i)->nb_op;k++)
    {
      fscanf(dico,"%s",next_op);
      ((tab+i)->arguments)[k] = malloc(strlen(next_op)+1);
      strcpy(((tab+i)->arguments)[k],next_op);
      /*printf("%s ",((tab+i)->arguments)[k]);*/

    }
    /*printf("\n");*/
  }
  fclose(dico);
  /*printf("Fermeture du dictionnaire d'instructions\n");*/
  return tab;
}
