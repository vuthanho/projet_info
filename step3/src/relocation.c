#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

#include <global.h>
#include <notify.h>
#include <structure.h>
#include <declaration.h>

L_relocation ajout_relocation(L_relocation L_r, relocation reloc)
{
  L_relocation nLr = calloc(1,sizeof(*nLr));
  nLr -> val = reloc;
  nLr -> suiv = L_r;
  return nLr;
}


relocation creation_reloc (T_lexem lex, int section, char type )
{
  relocation new_reloc;

    if(section==1)
    {
      new_reloc.section = calloc(10,sizeof(char));
      new_reloc.section=".text";
    }

    if(section==2)
    {
      new_reloc.section = calloc(10,sizeof(char));
      new_reloc.section=".data";
    }


  new_reloc.decalage = lex.decalage;

  new_reloc.symbole = calloc(1,sizeof(lex));
  *new_reloc.symbole = lex;

  switch (type) {
    case 'W':
      new_reloc.type=2;
      break;
    case 'I':
      new_reloc.type=6;
      break;
    case 'J':
      new_reloc.type=4;
      break;
    }
  return new_reloc;
}

void free_liste_relocation(L_relocation L)
{
  while(L != NULL)
  {
    free_liste_relocation(L->suiv);
    free(L);
    L = NULL;
  }
}

void afficher_liste_relocation(L_relocation L)
{
printf("??\n");

    while(L!=NULL)
    {
      afficher_relocation(L->val);
      L=L->suiv;
    }
    /*afficher_relocation(L->val);
    afficher_liste_relocation(L->suiv);*/

}



void afficher_relocation(relocation reloc)
{
  printf("Offset : %d       Mode : %d        Value : %s    \n",reloc.decalage,reloc.type,reloc.section);
}

char in_dico_reloc(T_lexem lex,instru_def* dico,int nb_inst,int* p_is_inst)
{
  int i=0;
  while(i<nb_inst)
  {
    if (!strcasecmp((dico[i]).instruction,lex.nom))
    {
      *p_is_inst=1;
      return (dico[i]).type;
    }
    i++;
  }
}

void reloc(L_relocation* p_rel_text, L_relocation* p_rel_data, L_lexem L, instru_def* dico, int nb_instru)
{

  if (L != NULL)
  {
    while (L->suiv != NULL)
    {
      if (L->val.type == symbole)
      {
        int is_inst = 0;
        char type = NULL;
        type = in_dico_reloc(L->val,dico,nb_instru,&is_inst);
        if ( is_inst == 1)
        {
          int i=0;
          L_lexem l_arg = L->arg;
          for(i=0;i<L->nb_op;i++)
          {
            if ( (l_arg->val).type == symbole )
            {
              relocation n_rel = creation_reloc( l_arg->val , 1 , type );
              *p_rel_text = ajout_relocation(*p_rel_text, n_rel);
            }
            if(l_arg->arg != NULL)
            {
              l_arg = l_arg->arg;
            }
          }
        }
      }

      if ((L->val.type == directive) && (!strcmp(".word", (L->val).nom)))
      {
        L_lexem l_arg=L->arg;
        /*if( (l_arg->val).type == symbole )
        {
          relocation n_rel = creation_reloc( l_arg->val , 2 , 'W' );
          *p_rel_data = ajout_relocation(*p_rel_data, n_rel);
        }*/

        while (l_arg->arg != NULL);
        {
          if( (l_arg->val).type == symbole )
          {
            relocation n_rel = creation_reloc( l_arg->val , 2 , 'W' );
            *p_rel_data = ajout_relocation(*p_rel_data, n_rel);
          }
          l_arg=l_arg->arg;
        }
        printf("\n");
      }
      L=L->suiv;
    }
  }

}
