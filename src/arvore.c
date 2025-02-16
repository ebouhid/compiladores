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
    int max_index; /* -1 para variáveis normais, a partir de 0 para vetor */
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
    node->max_index = -1;
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

No * add_filho(No *pai, No *filho){
    int i = 0;

    while (pai->filho[i] != NULL && i < NUMMAXFILHOS){
        i++;
    }

    if (i < NUMMAXFILHOS) {
        pai->filho[i] = filho;
        filho->pai = pai;
    } else {
        printf("Erro: Número máximo de filhos excedido.\n");
        free(filho);
    }
    
    return pai;
}

No * add_irmao(No *irmao, No *novo){
    while (irmao->irmao != NULL){
        irmao = irmao->irmao;
    }

    novo->pai = irmao->pai;
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
            kind_union = 42;
            break;
    }
    fprintf(file, "Linha: %d, Lexema: %s, Tipo: %s, Tipo_Union: %d\n", node->linha, node->lexmema, kind_node, kind_union);
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
    print_tree(file, tree->filho[0], depth + 1, 0);
    print_tree(file, tree->filho[1], depth + 1, 0);
    print_tree(file, tree->filho[2], depth + 1, 0);
    print_tree(file, tree->irmao, depth, 1);
}


// int main(){
//     No *tree = create_tree(1, "if", statement_k, if_k);
//     No *stmt = create_node(1, "while", statement_k, while_k);
//     No *expr = create_node(1, "op", expression_k, op_k);
//     No *decl = create_node(1, "var", declaration_k, var_k);
//     No *stmt2 = create_node(1, "return", statement_k, return_k);
//     No *stmt3 = create_node(1, "break", statement_k, break_k);

//     add_filho(tree, stmt);
//     add_filho(tree, expr);
//     add_filho(tree, decl);
//     add_irmao(expr, stmt2);
//     add_filho(expr, stmt3);

//     FILE *fp = fopen("tree.txt", "w");
//     if(fp == NULL){
//         perror("Erro ao abrir o arquivo para escrita");
//         exit(1);
//     }
//     print_tree(fp, tree, 0, 0);
//     fclose(fp);

//     free_tree(tree);
//     return 0;
// }