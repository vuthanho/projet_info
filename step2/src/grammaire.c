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
              if (strcmp(".space",lex.nom)){
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


instru_def* load_dico(char* chemin_dico, int* p_nb_inst){
  FILE* dico;
  int i;
  instru_def* tab;
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
  printf("nombre d'instructions : %d\n",*p_nb_inst);
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
      printf("Fermeture du dictionnaire d'instructions\n");
      return NULL;
    }
    printf("%s %c %d ",(tab+i)->instruction, (tab+i)->type, (tab+i)->nb_op);
    int k;
    char next_op[10];
    for (k=0;k<(tab+i)->nb_op;k++)
    {
      fscanf(dico,"%s",next_op);
      ((tab+i)->arguments)[k] = malloc(strlen(next_op)+1);
      strcpy(((tab+i)->arguments)[k],next_op);
      printf("%s ",((tab+i)->arguments)[k]);

    }
    printf("\n");
  }
  fclose(dico);
  printf("Fermeture du dictionnaire d'instructions\n");
  return tab;
}


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


void vider_Q_etiq(L_lexem* p_q_etiq, L_lexem* p_l_etiq, int section, int decalage)
{
  L_lexem a_vider = *p_q_etiq;
  while(a_vider != NULL)
  {
    *p_l_etiq = ajoute_lex(a_vider->val,*p_l_etiq);
    (*p_l_etiq)->section = section;
    (*p_l_etiq)->decalage = decalage;
    a_vider = a_vider->suiv;
  }
  free_liste(*p_q_etiq,1);
  *p_q_etiq = NULL;
}

void in_dico(T_lexem lex, char** p_regle,inst_def* dico,int nb_inst)
{
  int i=0;
  while( (*p_regle == NULL) && (i<nb_inst) )
  {
    if(!strcasecmp( (dico+i)->instruction, lex.nom ))
    {
      *p_regle = malloc( strlen((dico+i)->regle)+1);
      strcpy(*p_regle,(dico+i)->regle);
      (*p_regle)[strlen((dico+i)->regle)] = '\0';
      return;
    }
    i++;
  }
  ERROR_MSG("Instruction inconnue : %s",lex.nom);
}

void get_arg(L_lexem lex)
{
  int n_line = (lex -> val).n_ligne;
  int nb_parenthese = 0;
  int nb_virg = 0;
  L_lexem op = lex->suiv;
  L_lexem pre_op = lex;
  int nb_op = 0;
  while( (op != NULL) && ((op->val).n_ligne == n_line) && ((op->val).type != 7))
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
        break;
      default:
        nb_virg = 0;
        nb_op++;
        pre_op->arg = op;
        pre_op = op;
        break;
    }
    op = op->suiv;
  }

  if (nb_parenthese!=0){
    ERROR_MSG("Error : check the parenthesis line %d",n_line);
  }
  if (nb_virg!=0){
    ERROR_MSG("Error : missing argument after ',' line %d",n_line);
  }
  lex->nb_op = nb_op;

}

void verif_regle(char* regle,L_lexem first_op, int nb_op, int decal_text)
{
  if (strlen(regle) > nb_op)
  {
    ERROR_MSG("Error : not enough arguments");
  }
  if (strlen(regle) < nb_op)
  {
    ERROR_MSG("Error : too many arguments");
  }
  int isok = 1;
  L_lexem op_lu = first_op;
  char* op_voulu = regle;
  while(isok && (op_lu != NULL) )
  {
    op_lu->decalage = decal_text;
    if( *op_voulu-'0' != (op_lu->val).type)
    {
      isok = 0;
    }
    if (!isok)
    {
      ERROR_MSG("Error : wrong type of argument line %d",(op_lu->val).n_ligne);
    }
    op_lu = op_lu->arg;
    op_voulu = op_voulu+1;
  }
}

void verif_data(L_lexem lex, int* p_decal_data)
{
  L_lexem op = lex->arg;
  if( !strcmp(".word", (lex->val).nom) )
  {
    *p_decal_data = 4*((*p_decal_data)/4)+4*((*p_decal_data)%4 != 0);
    while(op!=NULL)
    {
      if( ((op->val).type != 1) && ((op->val).type != 2) && ((op->val).type != 3) )
      {
        ERROR_MSG("Error : wrong type of argument line %d",(op->val).n_ligne);
      }
      op->decalage = *p_decal_data;
      *p_decal_data = *p_decal_data + 4;
      op = op->arg;
    }
  }
}


L_lexem rec_verif_gram(int n_line, L_lexem L, L_lexem* p_q_etiq, L_lexem* p_l_etiq, int* p_etat, inst_def* dico,int nb_inst, int* p_decal_text, int* p_decal_data, int* p_decal_bss)
{
  if ( (L == NULL) || (n_line != (L->val).n_ligne) ) { /* détection d'une fin de ligne */
    return L;
  }
  if ((L->val).type == 5) /* ajout d'une étiquette à la file d'attente */
  {
    *p_q_etiq = ajoute_lex(L->val,*p_q_etiq);
    return rec_verif_gram(n_line, L->suiv, p_q_etiq, p_l_etiq, p_etat, dico, nb_inst, p_decal_text, p_decal_data, p_decal_bss);
  }

  if (((L->val).type == 10)||((L->val).type == 7)) /* passage à la prochaine ligne dans le cas où on lit un commentaire un un \n */
  {
    return rec_verif_gram(n_line, L->suiv, p_q_etiq, p_l_etiq, p_etat, dico, nb_inst, p_decal_text, p_decal_data, p_decal_bss);
  }

  int old_etat = *p_etat;
  update_etat(p_etat,L->val); /* mise à jour de l'état de section */
  if(old_etat != *p_etat)
  {
    L_lexem end_line = L;
    while ( (end_line != NULL) && ((end_line->val).n_ligne == n_line) )
    {
      /* recherche de la prochaine ligne à analyser */
      end_line = end_line->suiv;
    }
    return rec_verif_gram(n_line, end_line, p_q_etiq, p_l_etiq, p_etat, dico, nb_inst, p_decal_text, p_decal_data, p_decal_bss);
  }

  /* analyse grammaticale dans le cas d'une section .text */

  if (*p_etat == 1)
  {

    /* vidage de la file d'attente des étiquettes */
    vider_Q_etiq(p_q_etiq, p_l_etiq, 1, *p_decal_text);
    char* regle = NULL;

    /* recherche dans le dictionnaire des instructions et acquisition des arguments */
    in_dico(L->val, &regle, dico, nb_inst);
    get_arg(L);

    /* vérification du type d'opérandes */
    verif_regle(regle, L->arg, L->nb_op, *p_decal_text);

    /* affichage */
    printf("\ninstruction :\n");
    afficher_lexem(L->val);
    printf("arguments :\n");
    afficher_arg_lex(L->arg);


    *p_decal_text = *p_decal_text + 4;

    free(regle);
    regle = NULL;
    L_lexem end_line = L;
    while ( (end_line != NULL) && ((end_line->val).n_ligne == n_line) )
    {
      /* recherche de la prochaine ligne à analyser */
      end_line = end_line->suiv;
    }
    return rec_verif_gram(n_line, end_line, p_q_etiq, p_l_etiq, p_etat, dico, nb_inst, p_decal_text, p_decal_data, p_decal_bss);
  }

  /* analyse grammaticale dans le cas d'une section .data */

  if (*p_etat == 2)
  {

    /* vidage de la file d'attente des étiquettes */
    vider_Q_etiq(p_q_etiq, p_l_etiq, 1, *p_decal_text);
    /* acquisition des arguments */
    get_arg(L);

    verif_data(L, p_decal_data);

    /* affichage */
    printf("\ndirective :\n");
    afficher_lexem(L->val);
    printf("arguments :\n");
    afficher_arg_lex(L->arg);

    L_lexem end_line = L;
    while ( (end_line != NULL) && ((end_line->val).n_ligne == n_line) )
    {
      /* recherche de la prochaine ligne à analyser */
      end_line = end_line->suiv;
    }
    return rec_verif_gram(n_line, end_line, p_q_etiq, p_l_etiq, p_etat, dico, nb_inst, p_decal_text, p_decal_data, p_decal_bss);
  }

  /* analyse grammaticale dans le cas d'une section .bss */

  if (*p_etat == 3)
  {

    /* vidage de la file d'attente des étiquettes */
    vider_Q_etiq(p_q_etiq, p_l_etiq, 1, *p_decal_text);

    /* acquisition des arguments */
    get_arg(L);

    /* affichage */
    printf("\ndirective :\n");
    afficher_lexem(L->val);
    printf("arguments :\n");
    afficher_arg_lex(L->arg);

    /* ajouter calcul du nouveau décalage ici */

    L_lexem end_line = L;
    while ( (end_line != NULL) && ((end_line->val).n_ligne == n_line) )
    {
      /* recherche de la prochaine ligne à analyser */
      end_line = end_line->suiv;
    }
  }

  /* détection de l'absence de déclaration d'une section en début de fichier */

  if (*p_etat == -1)
  {
    ERROR_MSG("Il n'y a pas de fumée sans feu. Le feu ici étant la déclaration d'une section.");
  }
  return rec_verif_gram(n_line, L->suiv, p_q_etiq, p_l_etiq, p_etat, dico, nb_inst, p_decal_text, p_decal_data, p_decal_bss);
}

void verif_gram(int nb_line, L_lexem L, L_lexem* p_l_etiq, inst_def* dico,int nb_inst)
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
