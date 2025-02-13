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
    NodeKind kind_node;
    union {
        StatementKind stmt;
        ExpressionKind expr;
        DeclarationKind decl;
    } kind_union;
    struct no *pai;
    struct no *filho[NUMMAXFILHOS];
    struct no *irmao;
} No;

No * create_node(int linha, const char *lexmema, NodeKind kind, int kind_union){
    No *node = malloc(sizeof(No));
    node->linha = linha;
    strncpy(node->lexmema, lexmema, MAXLEXEME);
    node->kind_node = kind;
    node->pai = NULL;
    node->filho[0] = NULL;
    node->filho[1] = NULL;
    node->filho[2] = NULL;
    node->irmao = NULL;

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

No * create_tree(int linha, const char *lexmema, NodeKind kind, int kind_union){
    No *tree = create_node(linha, lexmema, kind, kind_union);
    return tree;
}

No * add_filho(No *pai, int linha, const char *lexmema, NodeKind kind, int kind_union){
    No *filho = create_node(linha, lexmema, kind, kind_union);

    int i = 0;

    while (pai->filho[i] != NULL && i < NUMMAXFILHOS){
        i++;
    }

    if (i < NUMMAXFILHOS) {
        pai->filho[i] = filho;
    } else {
        printf("Erro: Número máximo de filhos excedido.\n");
        free(filho);
    }
    
    return pai;
}

No * add_irmao(No *irmao, int linha, const char *lexmema, NodeKind kind, int kind_union){
    No *novo = create_node(linha, lexmema, kind, kind_union);

    while (irmao->irmao != NULL){
        irmao = irmao->irmao;
    }

    irmao->irmao = novo;
    
    return novo;
}

void free_tree(No *tree){
    if (tree == NULL){return;}
    free_tree(tree->filho[0]);
    free_tree(tree->filho[1]);
    free_tree(tree->filho[2]);
    free_tree(tree->irmao);
    free(tree);
}

void print_node(No *node){
    if (node == NULL){return;}
    char *kind_node;

    switch (node->kind_node){
        case statement_k:
            kind_node = "statement";
            break;
        case expression_k:
            kind_node = "expression";
            break;
        case declaration_k:
            kind_node = "declaration";
            break;
    }
    printf("Linha: %d, Lexema: %s, Tipo: %s\n", node->linha, node->lexmema, kind_node);
}

void print_tree(No *tree, int depth){
    if (tree == NULL){return;}
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    print_node(tree);
    print_tree(tree->filho[0], depth + 1);
    print_tree(tree->filho[1], depth + 1);
    print_tree(tree->filho[2], depth + 1);
    print_tree(tree->irmao, depth);
}

int main(){
    No *tree = create_tree(0, "raiz", statement_k, 0);
    add_filho(tree, 1, "filho1", statement_k, 0);
    add_filho(tree, 2, "filho2", expression_k, 0);
    add_filho(tree, 3, "filho3", declaration_k, 0);
    add_filho(tree->filho[0], 4, "filho1.1", statement_k, 0);
    add_filho(tree->filho[0], 5, "filho1.2", expression_k, 0);
    add_filho(tree->filho[0], 6, "filho1.3", declaration_k, 0);
    add_irmao(tree->filho[0]->filho[0], 7, "irmao1.1", statement_k, 0);

    print_tree(tree, 0);
    free_tree(tree);
    return 0;
}
