#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>

#include <global.h>
#include <notify.h>
#include <structure.h>
#include <declaration.h>

L_relocation ajout_relocation(L_relocation L_r, relocation reloc){
  L_relocation nLr = calloc(1,sizeof(*nLr));
  nLr -> val = reloc;
  nLr -> suiv = L_r;
  return nLr;
}


relocation creation_reloc (L_lexem liste, char section[10], int type ){

  relocation new_reloc;

  new_reloc.section=calloc(1,sizeof(section));
  strcpy(new_reloc.section,section);

  new_reloc.decalage = (liste->val).decalage;

  new_reloc.symbole = calloc(1,sizeof(T_lexem));
  *new_reloc.symbole = liste->val;

  new_reloc.type=type;

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
  if(L!=NULL)
  {
    afficher_relocation(L->val);
    afficher_liste_relocation(L->suiv);
  }
}

void afficher_relocation(relocation reloc)
{
  printf("Offset : %d       Mode : %d        Value : %s    \n",reloc.decalage,reloc.type,reloc.section);
}
