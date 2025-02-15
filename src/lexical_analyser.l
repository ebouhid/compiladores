%{
#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "analise_sintatica.tab.h"

int yylinenum = 1;  // Contador de linhas

char * id_lexema;

char tokenWord[MAXLEXEME];


%}

%option noyywrap

%x IN_COMMENT

%%

"/*"              BEGIN(IN_COMMENT);  

<IN_COMMENT>{
  "*/"            BEGIN(INITIAL);     
  [^*\n]+         ;                    
  "*"             ;                    
  "\n"            yylinenum++;         
}

"if"             { return T_IF; }
"else"           { return T_ELSE; }
"while"          { return T_WHILE; }
"int"            { return T_INT; }
"void"           { return T_VOID; }
"return"         { return T_RETURN; }

[A-Za-z_][A-Za-z0-9_]* { 
    id_lexema = strdup(yytext);
    return T_ID;
    

}

[0-9]+          {return T_NUM; }

"{"             { return T_LBRACE; }
"}"             { return T_RBRACE; }
"("             { return T_LPAREN; }
")"             { return T_RPAREN; }
"["             { return T_LBRACKET; }
"]"             { return T_RBRACKET; }
";"             { return T_SEMICOLON; }
","             { return T_COMMA; }
"+"             { return T_PLUS; }
"-"             { return T_MINUS; }
"*"             { return T_TIMES; }
"/"             { return T_OVER; }
"%"             { return T_MOD; }
"!="            { return T_NEQ; }
"=="            { return T_EQ; }
"="             { return T_ASSIGN; }
"<"             { return T_LT; }
">"             { return T_GT; }
"<="            { return T_LEQ; }
">="            { return T_GEQ; }

[ \t\r]+    ;  // Ignora espaços e tabulações
[\n]        { yylinenum++; }  // Conta cada nova linha

%%
