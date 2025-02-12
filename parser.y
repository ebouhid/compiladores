%{
// GLC to C- language

#include <iostream>
using namespace std;

extern int yyparse(void);

extern "C"
{
  int yylex(void);
}

%union {
    int num;
    char *str;
}

%start programa

%token <num> NUM
%token <str> ID
%token IF ELSE RETURN WHILE INT VOID
%token LBRACE RBRACE LPAREN RPAREN LBRACKET RBRACKET SEMICOLON COMMA
%token PLUS MINUS TIMES OVER MOD NEQ EQ ASSIGN LT GT LEQ GEQ


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
      LBRACE local_declaracoes statement_lista RBRACE
;

local_declaracoes:
      local_declaracoes var_declaracao
    | /* vazio */
;

statement_lista:
      statement_lista statement
    | /* vazio */
;

statement:
      expressao_decl
    | composto_decl
    | selecao_decl
    | iteracao_decl
    | retorno_decl
;

expressao_decl:
      expressao SEMICOLON
    | /* vazio */
;

selecao_decl:
      IF LPAREN expressao RPAREN statement
    | IF LPAREN expressao RPAREN statement ELSE statement
;

iteracao_decl:
      WHILE LPAREN expressao RPAREN statement
;

retorno_decl:
      RETURN SEMICOLON
    | RETURN expressao SEMICOLON
;

expressao:
      var ASSIGN expressao
    | simples_expressao
;

var:
      ID
    | ID LBRACKET expressao RBRACKET
;

simples_expressao:
      soma_expressao relacional soma_expressao
    | soma_expressao
;

relacional:
      LT
    | LEQ
    | GT
    | GEQ
    | EQ
    | NEQ
;

soma_expressao:
      soma_expressao PLUS termo
    | soma_expressao MINUS termo
    | termo
;

termo:
      termo TIMES fator
    | termo OVER fator
    | termo MOD fator
    | fator
;

fator:
      LPAREN expressao RPAREN
    | var
    | ativacao
    | NUM
;

ativacao:
      ID LPAREN args RPAREN
;

args:
      args_lista
    | /* vazio */
;

args_lista:
      args_lista COMMA expressao
    | expressao
;

%%

int main() {
      yyparse();
}
yyerror(char *s) {
    std::cout << "Error: " << s << std::endl;

    return 0;
}