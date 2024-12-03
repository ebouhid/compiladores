%{
#include <stdio.h>
int yylinenum = 1;  // Contador de linhas
%}

%option noyywrap

/* Definição dos estados */
%x IN_COMMENT

%%
"/*"              BEGIN(IN_COMMENT);  

<IN_COMMENT>{
  "*/"            BEGIN(INITIAL);     // Sai do estado de comentário
  [^*\n]+         ;                   // Consome texto que não seja '*' ou '\n'
  "*"             ;                   // Consome '*' isolado
  "\n"            yylinenum++;         // Conta novas linhas dentro do comentário
}

(if|else|while|int|void|return)                               { printf("Palavra Reservada: %s\n", yytext); }
[A-Za-z]+([A-Za-z])*                                          { printf("Identificador: %s\n", yytext); }
[1-9][0-9]*                                                   { printf("Numero: %s\n", yytext); }
#[ \t]*(include|define)                                       { printf("Macro: %s\n", yytext); }
(\{|\}|\(|\)|\[|\]|\;|\.|,)                                   { printf("Simbolo: %s\n", yytext); }
(\+|\-|\/|\*|\%|\&\&|\|\||\!|\!\=|\=\=|\=|\<|\>|\<\=|\>\=)    { printf("Operador: %s\n", yytext); }
[ \t\n]+                              ;  
.                                                             { printf("Caractere inválido: %s\n", yytext); }
%%

int main(int argc, char **argv) {
    yylex();
    return 0;
}
