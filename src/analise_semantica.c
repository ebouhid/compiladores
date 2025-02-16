#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../globals.h"

// Keep track of whether main was declared
int main_declared = 0;

void semantic_analysis(No* root, HashTable* symbol_table) {
    if (root == NULL) return;
    
    // Rule 3: Invalid type on declaration (void variable)
    if (strcmp(root->lexmema, "void") == 0 && root->filho[0] != NULL && root->kind_union.decl == var_k) {
        fprintf(stderr, "Error: Variable '%s' declared with invalid type 'void' at line %d.\n", root->filho[0]->lexmema, root->linha);
    }
    
    // Rule 4 & 7: Multiple declarations (same variable/function name in scope)
    char* scope = get_scope(root);
    if (root->filho[0] != NULL) {
        // Symbol* existing = find_symbol(symbol_table, root->filho[0]->lexmema, scope);
        // if (existing) {
        //     fprintf(stderr, "Error: Multiple declarations of '%s' at line %d.\n", root->filho[0]->lexmema, root->linha);
        // }
        int count = count_symbol(root->filho[0]->lexmema, scope, symbol_table);
        if (count > 1) {
            fprintf(stderr, "Error: Multiple declarations of '%s' at line %d.\n", root->filho[0]->lexmema, root->linha);
        }
    }
    
    // Rule 5: Call of undeclared function
    if (root->kind_union.expr == ativ_k) {
        Symbol* func = find_symbol(symbol_table, root->lexmema, "GLOBAL");
        if (!func) {
            fprintf(stderr, "Error: Function '%s' called without declaration at line %d.\n", root->lexmema, root->linha);
        }
    }
    
    // Rule 1: Assign to undeclared variable
    if (root->kind_union.expr == assign_k) {
        Symbol* var = find_symbol(symbol_table, root->filho[0]->lexmema, scope);
        if (!var) {
            fprintf(stderr, "Error: Variable '%s' assigned before declaration at line %d.\n", root->filho[0]->lexmema, root->linha);
        }
    }
    
    // Rule 2: Invalid assign type (assigning void function result)
    // print_node(stderr, root);
    if (strcmp(root->lexmema, "=") == 0  && root->filho[1]->kind_union.expr == ativ_k) {
        Symbol* func = find_symbol(symbol_table, root->filho[1]->lexmema, "GLOBAL");
        if (func && strcmp(func->type, "void") == 0) {
            fprintf(stderr, "Error: Cannot assign return value of void function '%s' at line %d.\n", root->filho[1]->lexmema, root->linha);
        }
    }
    
    // Rule 6: Ensure main function exists and is valid
    if (strcmp(root->lexmema, "main") == 0 && root->kind_union.decl == fun_k) {
        main_declared = 1;
    }
    
    // Recursively analyze child nodes
    for (int i = 0; i < NUMMAXFILHOS; i++) {
        semantic_analysis(root->filho[i], symbol_table);
    }
    
    semantic_analysis(root->irmao, symbol_table);
}

void check_main_function() {
    if (!main_declared) {
        fprintf(stderr, "Error: No declaration of 'main' function.\n");
    }
}
