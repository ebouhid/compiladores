#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../globals.h"
#include "../analise_sintatica.tab.h"
#include "codigo_intermediario.h"

extern char *yytext;
extern int yylinenum;
extern No *raizArvore;

int main(int argc, char **argv)
{
    fprintf(stderr, "Compilador - Análise Sintática e Semântica\n");
    FILE *arvore = fopen("outputs/arvore.txt", "w");
    FILE *tabsimb = fopen("outputs/tabsimb.txt", "w");
    FILE *codinter = fopen("outputs/codint.txt", "w");

    if (!arvore || !tabsimb || !codinter)
    {
        fprintf(stderr, "Erro: Falha ao abrir arquivos de saída.\n");
        exit(1);
    }

    printf("Iniciando análise...\n");
    int sintatica = yyparse();

    if (sintatica == 0 && raizArvore != NULL)
    {
        printf("Análise sintática concluída com sucesso.\n");
        print_tree(arvore, raizArvore, 0, 0);
        printf("Árvore sintática impressa em outputs/arvore.txt\n");

        HashTable *hashTable = create_table(TABLE_SIZE);
        iterate_tree(raizArvore, hashTable);
        print_symbol_table(tabsimb, hashTable);
        printf("Tabela de símbolos impressa em outputs/tabsimb.txt\n");

        printf("Iniciando análise semântica...\n");
        semantic_analysis(raizArvore, hashTable);
        printf("Análise semântica concluída.\n");

        // --- Geração de Código Intermediário ---
        printf("Iniciando geração de código intermediário...\n");
        Tac *codigo_gerado = gerar_codigo_intermediario(raizArvore, hashTable);
        if (codigo_gerado)
        {
            imprimirTac(codinter, codigo_gerado);
            printf("Código intermediário impresso em outputs/codigo_intermediario.txt\n");
            // liberarTac(codigo_gerado);
        }
        else
        {
            fprintf(stderr, "Erro durante a geração de código intermediário.\n");
        }
    }
    else
    {
        fprintf(stderr, "Erro durante a análise sintática ou árvore vazia.\n");
        fclose(arvore);
        fclose(tabsimb);
        fclose(codinter);
        exit(1);
    }

    printf("Compilação concluída (fases iniciais).\n");
    fclose(arvore);
    fclose(tabsimb);
    fclose(codinter);
    return 0;
}
