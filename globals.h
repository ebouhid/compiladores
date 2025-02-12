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
