#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//#include "../globals.h"
#define NUMMAXFILHOS 3
#define MAXLEXEME 25

typedef enum {statement_k, expression_k, declaration_k} NodeKind;
typedef enum {if_k, while_k, return_k, break_k, continue_k, expression_statement_k} StatementKind;
typedef enum {op_k, constant_k, id_k, type_k, arr_k, ativ_k, assign_k} ExpressionKind;
typedef enum {var_k, fun_k, unknown_k} DeclarationKind;






enum operacoes {FUN, ARG, LOAD, EQUAL, IFF, RET, GOTO, LAB, PARAM, DIV, MUL, SUB, CALL, END, STORE, HALT};

typedef struct tacNo{
    enum operacoes operacao;
    char op1[20];
    char op2[20];
    char resultado[20];
    struct tacNo *proximo;
}TacNo;

typedef struct tac{   
    int qtdNos;
    TacNo *inicio;
    TacNo *fim;
}Tac;

Tac *criarTac(Tac *estrutura_tac){
    estrutura_tac = malloc(sizeof(Tac));
    estrutura_tac->qtdNos = 0;
    estrutura_tac->fim = NULL;
    estrutura_tac->inicio = NULL;
    return estrutura_tac;
}

Tac *criarNoTac(Tac *estrutura_tac, int operacao, char op1[MAXLEXEME], char op2[MAXLEXEME], char resultado[MAXLEXEME]){
    TacNo* novoNo = malloc(sizeof(TacNo));
    //Preencher informações
    novoNo->operacao = operacao;
    strcpy(novoNo->op1, op1);
    strcpy(novoNo->op2, op2);
    strcpy(novoNo->resultado, resultado);
    novoNo->proximo = NULL;

    //Alocar
    if (estrutura_tac->fim == NULL)
    {
        estrutura_tac->inicio = novoNo;
        estrutura_tac->fim = novoNo;
    }
    else{
        estrutura_tac->fim->proximo = novoNo;
        estrutura_tac->fim = novoNo;
    }
    estrutura_tac->qtdNos++;
    return estrutura_tac; 
}

Tac *liberarTac(Tac *estrutura_tac){
    if (estrutura_tac == NULL)
    {
        return NULL;
    }
    else if(estrutura_tac->qtdNos != 0){
        TacNo *atual = estrutura_tac->inicio;
        TacNo *proximo = NULL;
        while (atual->proximo != NULL)
        {
            proximo = atual->proximo;
            free(atual);
            atual = proximo;
        }
        free(atual);       
    }
    free(estrutura_tac);
    return NULL;    
}

void imprimirTac(Tac *tac){
    if (tac == NULL)
    {
        printf("Não existe estrutura\n");
        return;
    }
    else if(tac->qtdNos == 0){
        printf("Estrutura vazia\n");
    }
    else{
        printf("Qtd de nos: %i\n", tac->qtdNos);
        TacNo *percorre = tac->inicio;
        int contador = 1;
        //imprimir todos os nós
        printf("------------------------\nInstrucao %i:\n\tOperacao: %i\n\tOp1: %s\n\tOp2: %s\n\tResultado: %s\n", contador, percorre->operacao, percorre->op1, percorre->op2, percorre->resultado);
        while(percorre->proximo != NULL){
            contador++;
            percorre = percorre->proximo;
            printf("------------------------\nInstrucao %i:\n\tOperacao: %i\n\tOp1: %s\n\tOp2: %s\n\tResultado: %s\n", contador, percorre->operacao, percorre->op1, percorre->op2, percorre->resultado);
        }        
    }
}


typedef struct noPilhaTac
{
    char lexema[MAXLEXEME];
    NodeKind kind_node;
    union {
        StatementKind stmt;
        ExpressionKind expr;
        DeclarationKind decl;
    } kind_union;
    struct noPilhaTac *proximo;
}NoPilhaTac;


typedef struct pilhaTac
{
    int qtd_elementos;
    struct noPilhaTac **ponteiros;
}PilhaTac;

PilhaTac *createpilha(){
    PilhaTac *pilha = malloc(sizeof(PilhaTac));
    if (pilha == NULL) {
        printf("Erro: Falha ao alocar memória para a pilha.\n");
        return NULL;
    }
    pilha->qtd_elementos = 0;
    pilha->ponteiros = malloc(3 * sizeof(NoPilhaTac*)); // Allocate memory for the array
    for (int i = 0; i < 3; i++) {
        pilha->ponteiros[i] = NULL;
    }
    return pilha;
}

PilhaTac *empilhar(PilhaTac *pilha, const char *lexmema, NodeKind kind, int kind_union) {
    if (pilha == NULL) {
        printf("Não existe pilha.\n");
        return NULL;
    }

    // Verificar se a pilha está cheia (máximo 3 elementos)
    if (pilha->qtd_elementos >= 3) {
        printf("Pilha cheia.\n");
        return pilha;
    }

    // Criar No
    NoPilhaTac *novoNo = malloc(sizeof(NoPilhaTac));
    if (novoNo == NULL) {
        printf("Erro: Falha ao alocar memória para novo nó.\n");
        return pilha;
    }

    // Preencher informações do nó
    novoNo->kind_node = kind;
    strcpy(novoNo->lexema, lexmema);
    switch (kind) {
        case statement_k:
            novoNo->kind_union.stmt = (StatementKind)kind_union;
            break;
        case expression_k:
            novoNo->kind_union.expr = (ExpressionKind)kind_union;
            break;
        case declaration_k:
            novoNo->kind_union.decl = (DeclarationKind)kind_union;
            break;
    }

    // Deslocar os ponteiros existentes
    if (pilha->qtd_elementos > 0) {
        for (int i = pilha->qtd_elementos; i > 0; i--) {
            pilha->ponteiros[i] = pilha->ponteiros[i-1];
        }
    }

    // Inserir novo nó no topo
    pilha->ponteiros[0] = novoNo;
    pilha->qtd_elementos++;

    return pilha;
}

void desempilhar(PilhaTac *pilha) {
    if (pilha == NULL) {
        printf("Erro: A pilha fornecida é NULL.\n");
        return;
    }
    
    if (pilha->qtd_elementos == 0) {
        printf("Pilha vazia\n");
        return;
    }

    // Libera o nó do topo
    free(pilha->ponteiros[0]);

    // Atualiza os ponteiros
    for (int i = 0; i < 2; i++) {
        pilha->ponteiros[i] = pilha->ponteiros[i + 1];
    }
    pilha->ponteiros[2] = NULL;

    // Decrementa a quantidade de elementos
    pilha->qtd_elementos--;
}

void imprimirPilha(PilhaTac *pilha) {
    if (pilha == NULL) {
        printf("Pilha não existe\n");
        return;
    }
    
    if (pilha->qtd_elementos == 0) {
        printf("Pilha vazia\n");
        return;
    }

    printf("Quantidade de elementos: %d\n", pilha->qtd_elementos);
    printf("------------------------\n");

    // Imprime os elementos na ordem do topo para base
    for (int i = 0; i < pilha->qtd_elementos && i < 3; i++) {
        if (pilha->ponteiros[i] != NULL) {
            printf("Elemento %d:\n", i);
            printf("\tLexema: %s\n", pilha->ponteiros[i]->lexema);
            printf("\tKind: %d\n", pilha->ponteiros[i]->kind_node);
            
            // Imprime o tipo específico da union baseado no kind_node
            switch (pilha->ponteiros[i]->kind_node) {
                case statement_k:
                    printf("\tStatement Kind: %d\n", pilha->ponteiros[i]->kind_union.stmt);
                    break;
                case expression_k:
                    printf("\tExpression Kind: %d\n", pilha->ponteiros[i]->kind_union.expr);
                    break;
                case declaration_k:
                    printf("\tDeclaration Kind: %d\n", pilha->ponteiros[i]->kind_union.decl);
                    break;
            }
            printf("------------------------\n");
        }
    }
}


int main(){
    Tac *estrutura_principal = NULL;
    char op1[MAXLEXEME] = "op1";
    char op2[MAXLEXEME] = "op2";
    char resultado[MAXLEXEME] = "resultado";

    PilhaTac *pilha = createpilha();
    
    // Test with some elements
    pilha = empilhar(pilha, "test1", expression_k, op_k);
    pilha = empilhar(pilha, "test2", statement_k, if_k);
    pilha = empilhar(pilha, "test3", statement_k, if_k);

    
    printf("%s", pilha->ponteiros[1]->lexema);
    
    return 0;
}