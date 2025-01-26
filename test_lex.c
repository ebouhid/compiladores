#include <stdio.h>
#include "globals.h"

extern int yylex();
extern int yylinenum;
extern char *yytext;  // Provided by flex to access the matched text

// Function to print token information
void print_token(int token) {
    switch (token) {
        case IF: printf("Token: IF (\"%s\")\n", yytext); break;
        case ELSE: printf("Token: ELSE (\"%s\")\n", yytext); break;
        case WHILE: printf("Token: WHILE (\"%s\")\n", yytext); break;
        case INT: printf("Token: INT (\"%s\")\n", yytext); break;
        case VOID: printf("Token: VOID (\"%s\")\n", yytext); break;
        case RETURN: printf("Token: RETURN (\"%s\")\n", yytext); break;
        case ID: printf("Token: ID (\"%s\")\n", yytext); break;
        case NUM: printf("Token: NUM (\"%s\")\n", yytext); break;
        case PRC: printf("Token: PRC (\"%s\")\n", yytext); break;
        case LBRACE: printf("Token: LBRACE (\"%s\")\n", yytext); break;
        case RBRACE: printf("Token: RBRACE (\"%s\")\n", yytext); break;
        case LPAREN: printf("Token: LPAREN (\"%s\")\n", yytext); break;
        case RPAREN: printf("Token: RPAREN (\"%s\")\n", yytext); break;
        case LBRACKET: printf("Token: LBRACKET (\"%s\")\n", yytext); break;
        case RBRACKET: printf("Token: RBRACKET (\"%s\")\n", yytext); break;
        case SEMICOLON: printf("Token: SEMICOLON (\"%s\")\n", yytext); break;
        case DOT: printf("Token: DOT (\"%s\")\n", yytext); break;
        case COMMA: printf("Token: COMMA (\"%s\")\n", yytext); break;
        case PLUS: printf("Token: PLUS (\"%s\")\n", yytext); break;
        case MINUS: printf("Token: MINUS (\"%s\")\n", yytext); break;
        case TIMES: printf("Token: TIMES (\"%s\")\n", yytext); break;
        case OVER: printf("Token: OVER (\"%s\")\n", yytext); break;
        case MOD: printf("Token: MOD (\"%s\")\n", yytext); break;
        case AND: printf("Token: AND (\"%s\")\n", yytext); break;
        case OR: printf("Token: OR (\"%s\")\n", yytext); break;
        case NOT: printf("Token: NOT (\"%s\")\n", yytext); break;
        case NEQ: printf("Token: NEQ (\"%s\")\n", yytext); break;
        case EQ: printf("Token: EQ (\"%s\")\n", yytext); break;
        case ASSIGN: printf("Token: ASSIGN (\"%s\")\n", yytext); break;
        case LT: printf("Token: LT (\"%s\")\n", yytext); break;
        case GT: printf("Token: GT (\"%s\")\n", yytext); break;
        case LEQ: printf("Token: LEQ (\"%s\")\n", yytext); break;
        case GEQ: printf("Token: GEQ (\"%s\")\n", yytext); break;
        case INVCHAR: printf("Token: INVCHAR (\"%s\")\n", yytext); break;
        default: printf("Unknown Token: %d (\"%s\")\n", token, yytext);
    }
}

// Test function
void test_lexer() {
    int token;
    while ((token = yylex()) != 0) {  // yylex() returns 0 at the end of input
        print_token(token);
    }
    printf("Lexical analysis complete. Total lines: %d\n", yylinenum);
}

int main() {
    printf("Starting lexical analysis...\n");
    test_lexer();
    return 0;
}
