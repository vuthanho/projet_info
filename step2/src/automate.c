#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

#include <global.h>
#include <notify.h>
#include <structure.h>
#include <declaration.h>

int isseparateur(char* position)
{
  switch (*position)
  {
    case 44: /* virgule */
      return 9;
      break;
    case 40: /* parenthese */
      return 10;
      break;
    case 41: /* parenthese */
      return 10;
      break;
    case 58: /* deux_points */
      return 8;
      break;
  }
  return 0;
}


int automate(char** p_end, char* endline)
{


  /* détecte les commentaires */

  if (**p_end == 35) /* dièse */
  {

    *p_end = *p_end + strlen(*p_end);

    return 6;
  }

  /* détecte les chaines de caractères */

  if (**p_end == 34) /* guillemet */
  {
    (*p_end)++;
    while( !( (**p_end == 34) && (*(*p_end-1) != 92 ) ) && (*p_end!=endline) ) /*  92 -> '\' */
    {
      (*p_end)++;
    }
    if (**p_end == 34)
    {
      (*p_end)++;
    }

    return 12;
  }

  /* détecte les séparatateurs */

  int res = isseparateur(*p_end);
  if (res)
  {
    (*p_end)++;
    return res;
  }

  /* détecte les directives */

  if (**p_end == 46)
  {

    (*p_end)++;
    while(isalpha(**p_end))
    {
      (*p_end)++;
    }
    return 11;
  }

  /* détecte les registre */

  if (**p_end == 36) /* 36 -> '$' */
  {

    (*p_end)++;
    while(isalnum(**p_end))
    {
      (*p_end)++;
    }
    return 4;
  }

  /* détecte les nombres décimaux et hexadécimaux */

  if (isdigit(**p_end) || (**p_end == 45))
  {
    if ( (**p_end == 48) && (*(*p_end+1) == 120)) /* 48 -> 0 ; 120 -> 'x' */
    {
      *p_end = *p_end + 2;
      while ( isalnum(**p_end) )
      {
        (*p_end)++;
      }
      return 3;
    }
    else
    {
      (*p_end)++;
      while ( isdigit(**p_end) )
      {
        (*p_end)++;
      }
      return 2;
    }
  }

  /* détecte les symboles */

  if (isalpha(**p_end) || (**p_end == '_'))
  {
    while ( isalnum(**p_end) || (**p_end == '_'))
    {
      (*p_end)++;
    }
    return 1;
  }

  /* détecte les caractères incongrus */

  if( (*p_end != endline) && (**p_end!='\0') )
  {

    return 0;
  }
  return -1;
}
