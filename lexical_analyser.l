%{
#include <stdio.h>
%}

/* Definição das expressões regulares */
%%
[A-Za-z]+([A-Za-z]|[0-9]|_)*                  { printf("Identificador: %s\n", yytext); }
[0-9]+\.?[0-9]*                               { printf("Número: %s\n", yytext); }
(if|else|while|for|int|float|double|unsigned|do|void|char)  { printf("Palavra Reservada: %s\n", yytext); }
/\*(.|\n)*?\*/                                { printf("Comentário (bloco): %s\n", yytext); }
\/\/[^\n]*                                    { printf("Comentário (linha): %s\n", yytext); }
(#include|#define)                             { printf("Macro: %s\n", yytext); }
(->|'|"|{|}|\(|\)|\[|\]|;|\.|,)               { printf("Símbolo: %s\n", yytext); }
(\+|-|/|\*|%|&&|\|\||!|!=|==|=)               { printf("Operador: %s\n", yytext); }
[ \t\n]+                                      { /* Ignorar espaços e quebras de linha */ }
.                                             { printf("Caractere desconhecido: %s\n", yytext); }
%%

int main(int argc, char **argv) {
    yylex();
    return 0;
}

