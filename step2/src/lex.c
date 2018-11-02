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


void free_liste(L_lexem L, int option)
{
  /* Si option = 0 la liste complète est à vider */
  /* Si option = 1 seuls les pointeurs sur maillons sont à vider */
  while(L != NULL)
  {
    if (option == 0){
      free((L->val).nom);
      (L->val).nom = NULL;
    }
    free_liste(L->suiv,option);
    free(L);
    L = NULL;
  }
}

L_lexem ajoute_lex(T_lexem lex, L_lexem L)
{
  L_lexem nListe = calloc(1,sizeof(*nListe));
  nListe -> val = lex;
  nListe -> suiv = L;
  return nListe;
}

void afficher_lexem(T_lexem lex)
{

  switch (lex.type)
  {

    case 1:
      printf("[ symbole     ] %s\n",lex.nom);
      break;
    case 2:
      printf("[ val_dec     ] %s\n",lex.nom);
      break;
    case 3:
      printf("[ val_hex     ] %s\n",lex.nom);
      break;
    case 4:
      printf("[ registre    ] %s\n",lex.nom);
      break;
    case 5:
      printf("[ etiquette   ] %s\n",lex.nom);
      break;
    case 6:
      printf("[ comment     ] %s\n",lex.nom);
      break;
    case 7:
      printf("[ new_line    ]   \n"        );
      break;
    case 8:
      printf("[ deux_points ] %s\n",lex.nom);
      break;
    case 9:
      printf("[ virgule     ] %s\n",lex.nom);
      break;
    case 10:
      printf("[ parenthese  ] %s\n",lex.nom);
      break;
    case 11:
      printf("[ directive   ] %s\n",lex.nom);
      break;
    case 12:
      printf("[ chaine_car  ] %s\n",lex.nom);
      break;
  }
}

void afficher_liste_lex(L_lexem L)
{
  if(L!=NULL)
  {
    afficher_liste_lex(L->suiv);
    afficher_lexem(L->val);
  }
}

/* note that MIPS assembly supports distinctions between lower and upper case*/
char* getNextToken(char** token, char* current_line, int n_ligne, L_lexem* p_L, char* endline, L_lexem* p_Ls)
{
    char* start = current_line;
    char* end=NULL;
    int token_size=0;

    /* check input parameter*/
    if (current_line ==NULL) return NULL;

    /* eats any blanks before the token*/
    while (*start!='\0' && isblank(*start))
    {
	     start++;
    }
    /* go till next lexem*/

    end=start;
    int type = automate(&end,endline);

    /*compute size : if zero there is no more token to extract*/
    token_size=end-start;
    if ( (token_size>0) && (type != 8) )
    {
      free(*token);
      *token = NULL;
      *token 	= calloc(token_size+1,sizeof(*start));
      strncpy(*token,start,token_size);
      (*token)[token_size]='\0';
      T_lexem n_lex;
      n_lex.type = type;
      n_lex.n_ligne = n_ligne;
      n_lex.nom = calloc(token_size+1,sizeof(*start));
/*      n_lex.nom = strdup(*token);  /!\ segfault POURQUOI? */
      strcpy(n_lex.nom,*token);
      *p_L = ajoute_lex(n_lex, *p_L);

      if(type == 1)
      {
        *p_Ls = ajoute_lex(n_lex, *p_Ls);
      }

      if(type==4){
        check_reg(n_lex);
      }
      return end;
    }
    if (type == 8){
      /* détection d'une étiquette */
      if ( (((*p_L)->val).type == 1) && (((*p_L)->val).n_ligne == n_ligne) ){
        ((*p_L)->val).type = 5;
        /* ajouter une instruction permettant de compléter le dico des étiquettes */
        return end;
      }
      else {
        ERROR_MSG("Error line %d : symbol expected before ':'",n_ligne);
      }
    }
    if(type==0)
    {
      ERROR_MSG("UNKNOWN CHAR line %d : '%c' ", n_ligne, *end);
      end++;
      return getNextToken(token, end, n_ligne, p_L, endline, p_Ls);
    }
    return NULL;
}




/**
 * @param line String of the line of source code to be analysed.
 * @param nline the line number in the source code.
 * @return should return the collection of lexemes that represent the input line of source code.
 * @brief This function performs lexical analysis of one standardized line.
 *
 */
void lex_read_line( char *line, int nline, L_lexem* p_L, char* endline, L_lexem* p_Ls, int* p_etat)
{
    char* token = NULL;
    char* current_address=line;
    int etat;
    while( (current_address= getNextToken(&token, current_address, nline, p_L, endline, p_Ls)) != NULL)
    {
      etat = is_new_section( ((*p_L) -> val), *p_etat );
      if ((etat != 4) && (etat != -1)) /* actualisation de *p_etat si nouvelle section */
      {
        *p_etat = etat;
        first_check(&token, &current_address, nline, p_L, endline, p_Ls, etat);
      }
      if (etat == -1){
        ERROR_MSG("Error line %d : unexpected directive \"%s\" ", nline, ((*p_L) -> val).nom );
      }
    }
    free(token);
    token = NULL;
    return;

}

/**
 * @param file Assembly source code file name.
 * @param nlines Pointer to the number of lines in the file.
 * @return should return the collection of lexemes
 * @brief This function loads an assembly code from a file into memory.
 *
 */
void lex_load_file( char *file, unsigned int *nlines, L_lexem* p_L, L_lexem* p_Ls)
{

    FILE        *fp   = NULL;
    char         line[STRLEN]; /* original source line */



    fp = fopen( file, "r" );
    if ( NULL == fp ) {
        /*macro ERROR_MSG : message d'erreur puis fin de programme ! */
        ERROR_MSG("Error while trying to open %s file --- Aborts",file);
    }

    *nlines = 0;
    int etat = -1; /* etat contient le type d'instruction attendu .set | .text | .data | .bss | error */

    while(!feof(fp)) {

        /*read source code line-by-line */
        if ( NULL != fgets( line, STRLEN-1, fp ) )
        {
            if ( *nlines )
            {
              T_lexem n_lex;
              n_lex.type = 7;
              n_lex.n_ligne = *nlines;
              n_lex.nom = calloc(1,2);
              char* saut = "\n";
              strcpy(n_lex.nom,saut);
              *p_L = ajoute_lex(n_lex, *p_L);
            }
            char* endline = line+strlen(line);
            line[strlen(line)-1] = '\0';  /* eat final '\n' */
            (*nlines)++;
            if ( 0 != strlen(line) )
            {
                lex_read_line(line, *nlines, p_L,endline, p_Ls, &etat);
            }

        }
    }

    fclose(fp);
    return;
}
