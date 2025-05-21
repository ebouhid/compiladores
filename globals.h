#define UNKNOWN_TYPE -1
#define TABLE_SIZE 512
#include <stdio.h>

// Árvore de sintaxe abstrata
#define NUMMAXFILHOS 3
#define MAXLEXEME 25
#define MAX_ARGS_TEMP 255

#ifndef GLOBALS_H
#define GLOBALS_H

typedef enum {statement_k, expression_k, declaration_k} NodeKind;
typedef enum {if_k, while_k, return_k, break_k, continue_k, expression_statement_k} StatementKind;
typedef enum {op_k, constant_k, id_k, type_k, arr_k, ativ_k, assign_k} ExpressionKind;
typedef enum {var_k, fun_k, param_k, arrdecl_k, unknown_k} DeclarationKind;

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
} No; // Nó da árvore de sintaxe abstrata

No * create_node(int linha, const char *lexmema, NodeKind kind, int kind_union);
No * create_tree(int linha, const char *lexmema, NodeKind kind, int kind_union);
No * add_filho(No *pai, No *filho);
No * add_irmao(No *irmao, No *novo);
void free_tree(No *tree);
void print_node(FILE *file, No *node);
void print_tree(FILE *file, No *tree, int depth, int is_irmao);



// Struct dos simbolos
typedef struct Symbol {
    unsigned int hash_key;
    int linha;
    char *name;
    DeclarationKind id_type;
    char *type;
    char *scope;
    struct Symbol *next;
} Symbol;


typedef struct HashTable {
    Symbol *table[TABLE_SIZE];
} HashTable;


extern char *id_lexema;
extern HashTable *symbol_table; // Tabela de simbolos
extern char *current_scope;  // Controla o escopo atual
extern int main_declared;

// Métodos
HashTable* create_table();
unsigned int hash(char *scope, char *name);
void iterate_tree(No* root, HashTable* symbol_table);
void add_to_hash_table(Symbol* symbol, HashTable* symbol_table);
Symbol* create_symbol(char* name, int linha, DeclarationKind id_type, char* type, char* scope);
char* get_scope(No* root);
void print_symbol_table(FILE *file, HashTable* symbol_table);
Symbol* find_symbol(HashTable* symbol_table, char* name, char* scope);

// Análise semântica
void semantic_analysis(No* root, HashTable* symbol_table);
void check_main_function();
int count_symbol(char* name, char* scope, HashTable* symbol_table);

#endif // !GLOBALS_H