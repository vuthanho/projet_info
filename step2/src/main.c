
/**
 * @file main.c
 * @author François Portet <francois.portet@imag.fr> from François Cayre
 * @brief Main entry point for MIPS assembler.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#include <limits.h>

#include "global.h"
#include "notify.h"
#include "structure.h"
#include "declaration.h"

/**
 * @param exec Name of executable.
 * @return Nothing.
 * @brief Print usage.
 *
 */
void print_usage( char *exec ) {
    fprintf(stderr, "Usage: %s file.s\n",exec);
}



/**
 * @param argc Number of arguments on the command line.
 * @param argv Value of arguments on the command line.
 * @return Whether this was a success or not.
 * @brief Main entry point for MIPS assembler.
 *
 */
int main ( int argc, char *argv[] ) {

    unsigned int 	nlines 	= 0;
    char         	 *file 	= NULL;


    if ( argc <2 ) {
        print_usage(argv[0]);
        exit( EXIT_FAILURE );
    }


    file  	= argv[argc-1];


    if ( NULL == file ) {
        fprintf( stderr, "Missing ASM source file, aborting.\n" );
        exit( EXIT_FAILURE );
    }

    L_lexem  L = NULL;
    L_lexem  L_symb = NULL;
    L_lexem  l_etiq = NULL;

    int nb_inst;
    instru_def* dico = load_dico("dico/dico_instr",&nb_inst);

    /* ---------------- do the lexical analysis -------------------*/
    lex_load_file( file, &nlines, &L, &L_symb);
    DEBUG_MSG("source code got %d lines", nlines);
    L_lexem nL = reverse_list_lex(L);
    free_liste(L,1);
    afficher_liste_lex(nL);
    char test[20];
    sprintf(test,"%ld",int_to_int(4096));
    printf("\n%ld %s\n",strlen(test),test);
    printf("\n%ld \n",sizeof(unsigned long));
    verif_gram(nlines, nL, &l_etiq, dico, nb_inst);

    /* ---------------- Free memory and terminate -------------------*/

    /* TODO free everything properly*/

    free_liste(nL,0);
    free_liste(L_symb,1);
    free_liste(l_etiq,1);
    free(dico);
    dico = NULL;
    exit( EXIT_SUCCESS );
}
