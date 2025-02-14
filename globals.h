#define UNKNOWN_TYPE -1

// Struct dos simbolos
typedef struct Symbol {
    char *name;
    int type;       // Data type (e.g., INT, VOID, etc.)
    int scope;      // Scope level (0 for global, >0 for local)
    struct Symbol *next;
} Symbol;

extern Symbol *symbol_table; // Tabela de simbolos

// Metodos
void add_symbol(char *name, int type, int scope);
Symbol *lookup_symbol(char *name);
void check_declaration(char *name, int scope);
void check_type_compatibility(int type1, int type2, const char *operation);


//Árvore de sintaxe abstrata
#define NUMMAXFILHOS 3
#define MAXLEXEME 25

typedef enum {statement_k, expression_k, declaration_k} NodeKind;
typedef enum {if_k, while_k, return_k, break_k, continue_k, expression_statement_k} StatementKind;
typedef enum {op_k, constant_k, id_k, type_k, arr_k, ativ_k, assign_k} ExpressionKind; /* A gente precisa do type_k? */
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
} No; // Nó da árvore de sintaxe abstrata

No * create_node(int linha, const char *lexmema, NodeKind kind, int kind_union);
No * create_tree(int linha, const char *lexmema, NodeKind kind, int kind_union);
No * add_filho(No *pai, No *filho);
No * add_irmao(No *irmao, No *novo);
void free_tree(No *tree);
void print_node(No *node);
void print_tree(No *tree, int depth);

