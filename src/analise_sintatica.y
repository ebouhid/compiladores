%{
// GLC to C- language

#include <stdio.h>
#include "globals.h"

int yylex(void);
int yyerror(char *);
extern char *yytext;
extern int yylinenum;
%}
%debug

%start programa

%token T_INT T_VOID T_IF T_ELSE T_WHILE T_RETURN
%token T_ID T_NUM
%token T_SEMICOLON T_COMMA T_ASSIGN T_LPAREN T_RPAREN T_LBRACE T_RBRACE T_LBRACKET T_RBRACKET
%token T_LT T_LEQ T_GT T_GEQ T_EQ T_NEQ
%token T_PLUS T_MINUS T_TIMES T_OVER T_MOD

%left T_LT T_LEQ T_GT T_GEQ T_EQ T_NEQ
%left T_PLUS T_MINUS
%left T_TIMES T_OVER T_MOD
%right T_ASSIGN
%nonassoc LOWER_THAN_ELSE
%nonassoc T_ELSE

%%

programa:
    declaracao_lista
;

declaracao_lista:
      declaracao_lista declaracao
    | declaracao
;

declaracao:
      var_declaracao
    | fun_declaracao
;

var_declaracao:
      tipo_especificador T_ID T_SEMICOLON
    | tipo_especificador T_ID T_LBRACKET T_NUM T_RBRACKET T_SEMICOLON
;

tipo_especificador:
      T_INT { printf("Parser: got int\n"); }
    | T_VOID
;

fun_declaracao:
      tipo_especificador T_ID T_LPAREN params T_RPAREN composto_decl
    | tipo_especificador T_ID T_LPAREN T_RPAREN composto_decl
;

params:
      param_lista
    | T_VOID
;

param_lista:
      param
    | param_lista T_COMMA param
;

param:
      tipo_especificador T_ID
    | tipo_especificador T_ID T_LBRACKET T_RBRACKET
;

composto_decl:
      T_LBRACE local_declaracoes statement_lista T_RBRACE
;

local_declaracoes:
      local_declaracoes var_declaracao
    | /* vazio */
;

statement_lista:
      statement
    | statement_lista statement
;

statement:
      expressao_decl
    | composto_decl
    | selecao_decl
    | iteracao_decl
    | retorno_decl
;

expressao_decl:
      expressao T_SEMICOLON
;

selecao_decl:
      T_IF T_LPAREN expressao T_RPAREN statement %prec LOWER_THAN_ELSE
    | T_IF T_LPAREN expressao T_RPAREN statement T_ELSE statement
;

iteracao_decl:
      T_WHILE T_LPAREN expressao T_RPAREN statement
;

retorno_decl:
      T_RETURN T_SEMICOLON
    | T_RETURN expressao T_SEMICOLON
;

expressao:
      var T_ASSIGN expressao
    | simples_expressao
;

var:
      T_ID
    | T_ID T_LBRACKET expressao T_RBRACKET
;

simples_expressao:
      soma_expressao
    | soma_expressao relacional soma_expressao
;

relacional:
      T_LT
    | T_LEQ
    | T_GT
    | T_GEQ
    | T_EQ
    | T_NEQ
;

soma_expressao:
      termo
    | soma_expressao T_PLUS termo
    | soma_expressao T_MINUS termo
;

termo:
      fator
    | termo T_TIMES fator
    | termo T_OVER fator
    | termo T_MOD fator
;

fator:
      T_LPAREN expressao T_RPAREN
    | var
    | ativacao
    | T_NUM
;

ativacao:
      T_ID T_LPAREN args T_RPAREN
    | T_ID T_LPAREN T_RPAREN
;

args:
    args_lista
;

args_lista:
      expressao
    | args_lista T_COMMA expressao
;

%%

void print_token(int token_val) {
      typedef enum {
    T_IF = 300, T_ELSE, T_WHILE, T_INT, T_VOID, T_RETURN, 
    T_ID, T_NUM, T_PRC, T_LBRACE, T_RBRACE, T_LPAREN, T_RPAREN, 
    T_LBRACKET, T_RBRACKET, T_SEMICOLON, T_DOT, T_COMMA, 
    T_PLUS, T_MINUS, T_TIMES, T_OVER, T_MOD, 
    T_AND, T_OR, T_NOT, T_NEQ, T_EQ, T_ASSIGN, 
    T_LT, T_GT, T_LEQ, T_GEQ, T_INVCHAR
} TokenType;
    switch (token_val) {
        case T_IF:
            printf("Token: T_IF (300)\n");
            break;
        case T_ELSE:
            printf("Token: T_ELSE (%d)\n", T_ELSE);
            break;
        case T_WHILE:
            printf("Token: T_WHILE (%d)\n", T_WHILE);
            break;
        case T_INT:
            printf("Token: T_INT (%d)\n", T_INT);
            break;
        case T_VOID:
            printf("Token: T_VOID (%d)\n", T_VOID);
            break;
        case T_RETURN:
            printf("Token: T_RETURN (%d)\n", T_RETURN);
            break;
        case T_ID:
            printf("Token: T_ID (%d)\n", T_ID);
            break;
        case T_NUM:
            printf("Token: T_NUM (%d)\n", T_NUM);
            break;
        case T_PRC:
            printf("Token: T_PRC (%d)\n", T_PRC);
            break;
        case T_LBRACE:
            printf("Token: T_LBRACE (%d)\n", T_LBRACE);
            break;
        case T_RBRACE:
            printf("Token: T_RBRACE (%d)\n", T_RBRACE);
            break;
        case T_LPAREN:
            printf("Token: T_LPAREN (%d)\n", T_LPAREN);
            break;
        case T_RPAREN:
            printf("Token: T_RPAREN (%d)\n", T_RPAREN);
            break;
        case T_LBRACKET:
            printf("Token: T_LBRACKET (%d)\n", T_LBRACKET);
            break;
        case T_RBRACKET:
            printf("Token: T_RBRACKET (%d)\n", T_RBRACKET);
            break;
        case T_SEMICOLON:
            printf("Token: T_SEMICOLON (%d)\n", T_SEMICOLON);
            break;
        case T_DOT:
            printf("Token: T_DOT (%d)\n", T_DOT);
            break;
        case T_COMMA:
            printf("Token: T_COMMA (%d)\n", T_COMMA);
            break;
        case T_PLUS:
            printf("Token: T_PLUS (%d)\n", T_PLUS);
            break;
        case T_MINUS:
            printf("Token: T_MINUS (%d)\n", T_MINUS);
            break;
        case T_TIMES:
            printf("Token: T_TIMES (%d)\n", T_TIMES);
            break;
        case T_OVER:
            printf("Token: T_OVER (%d)\n", T_OVER);
            break;
        case T_MOD:
            printf("Token: T_MOD (%d)\n", T_MOD);
            break;
        case T_AND:
            printf("Token: T_AND (%d)\n", T_AND);
            break;
        case T_OR:
            printf("Token: T_OR (%d)\n", T_OR);
            break;
        case T_NOT:
            printf("Token: T_NOT (%d)\n", T_NOT);
            break;
        case T_NEQ:
            printf("Token: T_NEQ (%d)\n", T_NEQ);
            break;
        case T_EQ:
            printf("Token: T_EQ (%d)\n", T_EQ);
            break;
        case T_ASSIGN:
            printf("Token: T_ASSIGN (%d)\n", T_ASSIGN);
            break;
        case T_LT:
            printf("Token: T_LT (%d)\n", T_LT);
            break;
        case T_GT:
            printf("Token: T_GT (%d)\n", T_GT);
            break;
        case T_LEQ:
            printf("Token: T_LEQ (%d)\n", T_LEQ);
            break;
        case T_GEQ:
            printf("Token: T_GEQ (%d)\n", T_GEQ);
            break;
        case T_INVCHAR:
            printf("Token: T_INVCHAR (%d)\n", T_INVCHAR);
            break;
        default:
            printf("Unknown token: %d\n", token_val);
    }
}

int main() {
    yydebug = 0;
      int token = 1;
      /*while ((token = yylex()) != 0) {  // yylex() returns 0 at the end of input
        print_token(token);
    }*/
    int sintatica = yyparse();
    if (sintatica == 0) {
        printf("Sucesso na análise sintática\n");
    } else {
        printf("Erro na análise sintática\n");
    }
    return 0;
}

int yyerror(char *msg) {
    fprintf(stderr, "Erro de sintaxe na linha %d: %s. Token inesperado: '%s'\n", yylinenum, msg, yytext);
}
