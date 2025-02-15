#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../globals.h"

HashTable* create_table() {
    HashTable *ht = (HashTable*)malloc(sizeof(HashTable));
    if (!ht) return NULL;
    for (int i = 0; i < TABLE_SIZE; i++)
        ht->table[i] = NULL;
    return ht;
}

// hash
unsigned int hash(char *scope, char *name) {
    char key[200];  // Buffer to store "scope+name"
    snprintf(key, sizeof(key), "%s+%s", scope, name);

    unsigned int hash = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hash = (hash * 31) + key[i];  // Simple hash function
    }

    return hash % HASH_SIZE;
}

void iterate_tree(No* root, HashTable* symbol_table) {
    if (root == NULL) {
        return;
    }

    if (strcmp(root->lexmema, "int") == 0 || strcmp(root->lexmema, "void") == 0) {
        char scope[MAXLEXEME];
        get_scope(root, scope);
        create_symbol(root->filho[0]->lexmema, root->kind_union.decl, root->lexmema, scope);
    }

    for (int i = 0; i < NUMMAXFILHOS; i++) {
        iterate_tree(root->filho[i], symbol_table);
    }

    iterate_tree(root->irmao, symbol_table);
}

void add_to_hash_table(Symbol* new_symbol, HashTable* symbol_table) {
    unsigned int hash_key = new_symbol->hash_key;
    int index = hash_key % HASH_SIZE;
    
    new_symbol->next = symbol_table->table[index];  // Insert at the head (chaining)
    
    symbol_table->table[index] = new_symbol;
}


Symbol* create_symbol(char* name, DeclarationKind id_type, char* type, char* scope) {
    Symbol* new_symbol = malloc(sizeof(Symbol));
    new_symbol->hash_key = hash(current_scope, name);
    new_symbol->name = name;
    new_symbol->id_type = id_type;
    new_symbol->type = type;
    new_symbol->scope = scope;
    new_symbol->next = NULL;

    return new_symbol;
}

void get_scope(No* node, char* scope_variable){
    if (node->pai == NULL) {
        strncpy(scope_variable, node->lexmema, MAXLEXEME);
        return;
    } else {
        get_scope(node->pai, scope_variable);
        //strncpy(scope_variable, get_scope(node->pai->lexmema), MAXLEXEME);
        return;
    }
}

void print_symbol_table(FILE* file, HashTable* symbol_table) {
    for (int i = 0; i < HASH_SIZE; i++) {
        Symbol* current = symbol_table->table[i];
        while (current != NULL) {
            fprintf(file, "Hash: %d Name: %s, ID Type: %d, Type: %s, Scope: %s\n", current->hash_key, current->name, current->id_type, current->type, current->scope);
            current = current->next;
        }
    }
}