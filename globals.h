#define UNKNOWN_TYPE -1
#define TABLE_SIZE 100
#include <stdio.h>

// Árvore de sintaxe abstrata
#define NUMMAXFILHOS 3
#define MAXLEXEME 25

typedef enum {statement_k, expression_k, declaration_k} NodeKind;
typedef enum {if_k, while_k, return_k, break_k, continue_k, expression_statement_k} StatementKind;
typedef enum {op_k, constant_k, id_k, type_k, arr_k, ativ_k, assign_k} ExpressionKind;
typedef enum {var_k, fun_k} DeclarationKind;

typedef struct no {
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

// Métodos
HashTable* create_table();
unsigned int hash(char *scope, char *name);
void iterate_tree(No* root, HashTable* symbol_table);
void add_to_hash_table(Symbol* symbol, HashTable* symbol_table);
Symbol* create_symbol(char* name, DeclarationKind id_type, char* type, char* scope);
char* get_scope(No* root);
void print_symbol_table(FILE *file, HashTable* symbol_table);
