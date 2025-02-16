#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUMMAXFILHOS 3
#define MAXLEXEME 25

typedef enum {statement_k, expression_k, declaration_k} NodeKind;
typedef enum {if_k, while_k, return_k, break_k, continue_k, expression_statement_k} StatementKind;
typedef enum {op_k, constant_k, id_k, type_k} ExpressionKind;
typedef enum {var_k, fun_k} DeclarationKind;

typedef struct no
{
    int linha;
    char lexmema[MAXLEXEME];
    int max_index;
    NodeKind kind_node;
    union {
        StatementKind stmt;
        ExpressionKind expr;
        DeclarationKind decl;
    } kind_union;
    struct no *pai;
    struct no *filho[NUMMAXFILHOS];
    struct no *irmao;
    struct no *prev_irmao;
} No;

No * create_node(int linha, const char *lexmema, NodeKind kind, int kind_union){
    No *node = malloc(sizeof(No));
    node->linha = linha;
    strncpy(node->lexmema, lexmema, MAXLEXEME);
    node->max_index = -1;
    node->kind_node = kind;
    node->pai = NULL;
    node->irmao = NULL;
    node->prev_irmao = NULL;
    for (int i = 0; i < NUMMAXFILHOS; i++) {
        node->filho[i] = NULL;
    }

    switch (kind){
        case statement_k:
            node->kind_union.stmt = (StatementKind)kind_union;
            break;
        case expression_k:
            node->kind_union.expr = (ExpressionKind)kind_union;
            break;
        case declaration_k:
            node->kind_union.decl = (DeclarationKind)kind_union;
            break;
    }

    return node;
}

No * add_filho(No *pai, No *filho){
    for (int i = 0; i < NUMMAXFILHOS; i++){
        if (pai->filho[i] == NULL) {
            pai->filho[i] = filho;
            filho->pai = pai;
            return pai;
        }
    }
    printf("Erro: Número máximo de filhos excedido.\n");
    free(filho);
    return pai;
}

No * add_irmao(No *irmao, No *novo){
    while (irmao->irmao != NULL){
        irmao = irmao->irmao;
    }
    irmao->irmao = novo;
    novo->prev_irmao = irmao;
    return novo;
}

void free_tree(No *tree){
    if (tree == NULL){return;}
    for (int i = 0; i < NUMMAXFILHOS; i++) {
        free_tree(tree->filho[i]);
    }
    free_tree(tree->irmao);
    free(tree);
}

void print_node(FILE *file, No *node){
    if (node == NULL){return;}
    char *kind_node;
    int kind_union;

    switch (node->kind_node){
        case statement_k:
            kind_node = "statement";
            kind_union = node->kind_union.stmt;
            break;
        case expression_k:
            kind_node = "expression";
            kind_union = node->kind_union.expr;
            break;
        case declaration_k:
            kind_node = "declaration";
            kind_union = node->kind_union.decl;
            break;
        default:
            kind_node = "unknown";
            kind_union = -1;
            break;
    }
    fprintf(file, "Linha: %d, Lexema: %s, Tipo: %s, Tipo_Union: %d", node->linha, node->lexmema, kind_node, kind_union);
    if (node->pai != NULL){
        fprintf(file, ", Pai: %s", node->pai->lexmema);
    }
    if (node->prev_irmao != NULL){
        fprintf(file, ", Irmao Anterior: %s", node->prev_irmao->lexmema);
    }
    fprintf(file, "\n");
}

void print_tree(FILE *file, No *tree, int depth, int is_irmao){
    if (tree == NULL){return;}
    for (int i = 0; i < depth; i++) {
        fprintf(file, "  ");
    }
    if (is_irmao){
        fprintf(file, "-");
    }
    print_node(file, tree);
    for (int i = 0; i < NUMMAXFILHOS; i++) {
        print_tree(file, tree->filho[i], depth + 1, 0);
    }
    print_tree(file, tree->irmao, depth, 1);
}
