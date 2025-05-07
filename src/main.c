#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../globals.h"
#include "../analise_sintatica.tab.h"
#include "codigo_intermediario.h"

extern char *yytext;
extern int yylinenum;
extern No *raizArvore;

int main(int argc, char **argv) {
    system("mkdir -p outputs");
    FILE *arvore = fopen("outputs/arvore.txt", "w");
    FILE *tabsimb = fopen("outputs/tabsimb.txt", "w");
    FILE *codinter = fopen("outputs/codint.txt", "w");
    
    if (!arvore || !tabsimb || !codinter) {
        fprintf(stderr, "Erro: Falha ao abrir arquivos de saída.\n");
        // if (yyin) fclose(yyin);
        exit(1);
    }

    // yydebug = 1; // Descomente para debug do Bison
    printf("Iniciando análise...\n");
    int sintatica = yyparse();

    if (sintatica == 0 && raizArvore != NULL) {
        printf("Análise sintática concluída com sucesso.\n");
        print_tree(arvore, raizArvore, 0, 0);
        printf("Árvore sintática impressa em outputs/arvore.txt\n");

        HashTable *hashTable = create_table(TABLE_SIZE); // Use TABLE_SIZE aqui
        iterate_tree(raizArvore, hashTable);
        print_symbol_table(tabsimb, hashTable);
        printf("Tabela de símbolos impressa em outputs/tabsimb.txt\n");

        printf("Iniciando análise semântica...\n");
        semantic_analysis(raizArvore, hashTable);
        check_main_function();
        // Assumindo que semantic_analysis não retorna erro fatal ou usa exit()
        printf("Análise semântica concluída.\n");

        // --- Geração de Código Intermediário --- // *** ADICIONADO ***
        printf("Iniciando geração de código intermediário...\n");
        Tac *codigo_gerado = gerar_codigo_intermediario(raizArvore, hashTable);
        if (codigo_gerado) {
            imprimirTac(codinter, codigo_gerado);
            printf("Código intermediário impresso em outputs/codigo_intermediario.txt\n");
            // liberarTac(codigo_gerado); // Liberar memória se não for mais usar
        } else {
            fprintf(stderr, "Erro durante a geração de código intermediário.\n");
        }
        // --- Fim Geração --- //

        // free_tree(raizArvore); // Liberar memória da árvore
        // Liberar tabela de símbolos também seria bom
    } else {
        fprintf(stderr, "Erro durante a análise sintática ou árvore vazia.\n");
        fclose(arvore);
        fclose(tabsimb);
        fclose(codinter); // *** ADICIONADO ***
        // if (yyin) fclose(yyin); // *** ADICIONADO ***
        exit(1);
    }

    printf("Compilação concluída (fases iniciais).\n");
    fclose(arvore);
    fclose(tabsimb);
    fclose(codinter); // *** ADICIONADO ***
    // if (yyin) fclose(yyin); // *** ADICIONADO ***

    return 0;
}
