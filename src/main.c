#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../globals.h"
#include "../analise_sintatica.tab.h"

extern char *yytext;
extern int yylinenum;
extern No *raizArvore;

int main(int argc, char **argv) {
    FILE *arvore = fopen("outputs/arvore.txt", "w");
    FILE *tabsimb = fopen("outputs/tabsimb.txt", "w");
    // yydebug = 0;
    int token = 1;
    int sintatica = yyparse();
    if (sintatica == 0) {
        print_tree(arvore, raizArvore, 0, 0);
        
        HashTable *hashTable = create_table(TABLE_SIZE);
        iterate_tree(raizArvore, hashTable);
        print_symbol_table(tabsimb, hashTable);
    
        semantic_analysis(raizArvore, hashTable);
        check_main_function();
    } else {
        exit(1);
    }

    return 0;
}