#ifndef CODIGO_INTERMEDIARIO_H
#define CODIGO_INTERMEDIARIO_H

#include "../globals.h" // Inclui No, HashTable, etc.

// Operações do Código Intermediário (Expandido)
// Renomeei alguns para clareza e adicionei os faltantes
typedef enum {
    // Operadores Aritmeticos
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD, OP_UMINUS,
    // Operadores Relacionais / Lógicos (resultado booleano -> 0 ou 1)
    OP_LT, OP_LE, OP_GT, OP_GE, OP_EQ, OP_NE,
    // Atribuição / Movimentação / Carga / Armazenamento
    OP_ASSIGN, // Atribuição simples (ex: t1 = t2)
    OP_LOAD,   // Carrega de array/ponteiro (ex: t1 = V[t0])
    OP_STORE,  // Armazena em array/ponteiro (ex: V[t0] = t1)
    OP_DECL_VAR, // Declaração de variável local
    OP_DECL_ARR, // Declaração de array local
    OP_FORMAL_PARAM, // Declaração de parâmetro formal (ex: int x)
    // Saltos
    OP_GOTO,   // Salto incondicional
    OP_IFF,    // Salto se falso (ifFalse)
    // Funções
    OP_FUN,    // Início de definição de função
    OP_END,    // Fim de definição de função
    OP_PARAM,  // Declaração/passagem de parâmetro
    OP_CALL,   // Chamada de função
    OP_RET,    // Retorno de função
    // Outros
    OP_LAB,    // Definição de Rótulo (Label)
    OP_ARG,    // Passagem de argumento (antes de CALL) - Alternativa a PARAM se usado diferente
    OP_HALT    // Parada do programa
} OperacaoTac;

// Estrutura da Quádrupla (mantida)
typedef struct tacNo {
    OperacaoTac operacao;
    char op1[MAXLEXEME]; // Pode ser ID, constante, temp, label
    char op2[MAXLEXEME]; // Pode ser ID, constante, temp
    char resultado[MAXLEXEME]; // Pode ser ID, temp, label (para saltos)
    struct tacNo *proximo;
} TacNo;

// Estrutura da Lista de Quádruplas (mantida)
typedef struct tac {
    int qtdNos;
    TacNo *inicio;
    TacNo *fim;
} Tac;

// --- Protótipos ---

// Funções básicas da lista TAC
Tac *criarTac();
Tac *criarNoTac(Tac *estrutura_tac, OperacaoTac operacao, const char *op1, const char *op2, const char *resultado);
Tac *liberarTac(Tac *estrutura_tac);
void imprimirTac(FILE *outfile, Tac *tac); // Modificado para aceitar FILE*
const char* operacao_para_string(OperacaoTac op); // Helper para impressão

// Função principal de geração
Tac *gerar_codigo_intermediario(No *raizArvore, HashTable *symbol_table);

#endif // CODIGO_INTERMEDIARIO_H