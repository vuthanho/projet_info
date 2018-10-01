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

/* note that MIPS assembly supports distinctions between lower and upper case*/
char* getNextToken(char** token, char* current_line, int n_ligne, L_lexem* p_L, char* endline)
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
    if (token_size>0)
    {
      *token 	= calloc(token_size+1,sizeof(*start));
      strncpy(*token,start,token_size);
      (*token)[token_size]='\0';
      T_lexem n_lex;
      n_lex.type = type;
      n_lex.n_ligne = n_ligne;
      n_lex.nom = calloc(token_size+1,sizeof(*start));
/*      n_lex.nom = strdup(*token);  /!\ segfault POURQUOI? */
      strncpy(n_lex.nom,start,token_size);

      *p_L = ajoute_lex(n_lex, *p_L);
      return end;
    }
    if(type==0)
    {
      WARNING_MSG("CARACTERE INATTENDU ligne %d", n_ligne);
      end++;
      return getNextToken(token, end, n_ligne, p_L, endline);
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
void lex_read_line( char *line, int nline, L_lexem* p_L, char* endline )
{
    char* token = NULL;
    char* current_address=line;



    while( (current_address= getNextToken(&token, current_address, nline, p_L, endline)) != NULL)
    {
      afficher_lexem( ((*p_L) -> val) );
    }

    return;
}

/**
 * @param file Assembly source code file name.
 * @param nlines Pointer to the number of lines in the file.
 * @return should return the collection of lexemes
 * @brief This function loads an assembly code from a file into memory.
 *
 */
void lex_load_file( char *file, unsigned int *nlines, L_lexem* p_L )
{

    FILE        *fp   = NULL;
    char         line[STRLEN]; /* original source line */



    fp = fopen( file, "r" );
    if ( NULL == fp ) {
        /*macro ERROR_MSG : message d'erreur puis fin de programme ! */
        ERROR_MSG("Error while trying to open %s file --- Aborts",file);
    }

    *nlines = 0;

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
              afficher_lexem( ((*p_L) -> val) );
            }
            char* endline = line+strlen(line);
            line[strlen(line)-1] = '\0';  /* eat final '\n' */
            (*nlines)++;

            if ( 0 != strlen(line) )
            {
                lex_read_line(line, *nlines, p_L,endline );
            }

        }
    }

    fclose(fp);
    return;
}
