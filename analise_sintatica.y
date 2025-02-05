%{
//GLC to c- language

#include <iostream>
using namespace std;

extern "C"
{
//  int yyparse(void);
  int yylex(void);
  void abrirArq();
}

void yyerror(char *);
%}


%start programa

%token <int> NUM
%token <string> ID
%token IF ELSE RETURN WHILE INT VOID
%token PRC
%token LBRACE RBRACE LPAREN RPAREN LBRACKET RBRACKET SEMICOLON DOT COMMA
%token PLUS MINUS TIMES OVER MOD AND OR NOT NEQ EQ ASSIGN LT GT LEQ GEQ
%token INVCHAR


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
      tipo_especificador ID SEMICOLON
    | tipo_especificador ID LBRACKET NUM RBRACKET SEMICOLON
;

tipo_especificador:
      INT
    | VOID
;

fun_declaracao:
      tipo_especificador ID LPAREN params RPAREN composto_decl
;

params:
      param_lista
    | VOID
;

param_lista:
      param_lista COMMA param
    | param
;

param:
      tipo_especificador ID
    | tipo_especificador ID LBRACKET RBRACKET
;

composto_decl:
      LBRACE local_declarações statement_lista RBRACE
;

local_declarações:
      local_declarações var_declaracao
    | /* vazio */
;

statement_lista:
      statement_lista statement
    | /* vazio */
;

statement:
      expressão_decl
    | composto_decl
    | seleção_decl
    | iteração_decl
    | retorno_decl
;

expressão_decl:
      expressão SEMICOLON
    | /* vazio */
;

seleção_decl:
      IF LPAREN expressão RPAREN statement
    | IF LPAREN expressão RPAREN statement ELSE statement
;

iteração_decl:
      WHILE LPAREN expressão RPAREN statement
;

retorno_decl:
      RETURN SEMICOLON
    | RETURN expressão SEMICOLON
;

expressão:
      var ASSIGN expressão
    | simples_expressão
;

var:
      ID
    | ID LBRACKET expressão RBRACKET
;

simples_expressão:
      soma_expressão relacional soma_expressão
    | soma_expressão
;

relacional:
      LT
    | LEQ
    | GT
    | GEQ
    | EQ
    | NEQ
;

soma_expressão:
      soma_expressão PLUS termo
    | soma_expressão MINUS termo
    | termo
;

termo:
      termo TIMES fator
    | termo OVER fator
    | termo MOD fator
    | fator
;

fator:
      LPAREN expressão RPAREN
    | var
    | ativação
    | NUM
;

ativação:
      ID LPAREN args RPAREN
;

args:
      args_lista
    | /* vazio */
;

args_lista:
      args_lista COMMA expressão
    | expressão
;

%%

int main() {
    yyparse();
    return 0;
}

int yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
    return 0;
}