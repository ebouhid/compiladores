#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../globals.h"

extern char *yytext;
extern int yylinenum;
extern No *raizArvore;

int main(int argc, char **argv) {
    FILE *arvore = fopen("arvore.txt", "w");
    FILE *tabsimb = fopen("tabsimb.txt", "w");
    // yydebug = 0;
    int token = 1;
    int sintatica = yyparse();
    if (sintatica == 0) {
        fprintf(stderr, "Sucesso na análise sintática\n");
    } else {
        fprintf(stderr, "Erro na análise sintática\n");
    }
    print_tree(arvore, raizArvore, 0, 0);
    
    HashTable *hashTable = create_table(TABLE_SIZE);
    iterate_tree(raizArvore, hashTable);
    print_symbol_table(tabsimb, hashTable);

    fprintf(stderr, "Semantic analysis\n");
    semantic_analysis(raizArvore, hashTable);
    fprintf(stderr, "Checking main function...\n");
    check_main_function();

    return 0;
}