%{
#include <stdio.h>
#include "globals.h"
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

(if)                                                          {return IF;}
(else)                                                        {return ELSE;}
(while)                                                       {return WHILE;}
(int)                                                         {return INT;}
(void)                                                        {return VOID;}
(return)                                                      {return RETURN;}
[A-Za-z]+([A-Za-z])*                                          {return ID;}
[1-9][0-9]*                                                   {return NUM;}
#[ \t]*(include|define)                                       {return PRC;}
(\{)                                                          {return LBRACE;}
(\})                                                          {return RBRACE;}
(\()                                                          {return LPAREN;}
(\))                                                          {return RPAREN;}
(\[)                                                          {return LBRACKET;}
(\])                                                          {return RBRACKET;}
(\;)                                                          {return SEMICOLON;}
(\.)                                                          {return DOT;}
(\,)                                                          {return COMMA;}
(\+)                                                          {return PLUS;}
(\-)                                                          {return MINUS;}
(\*)                                                          {return TIMES;}
(\/)                                                          {return OVER;}
(\%)                                                          {return MOD;}
(\&\&)                                                        {return AND;}
(\|\|)                                                        {return OR;}
(\!)                                                          {return NOT;}
(\!\=)                                                        {return NEQ;}
(\=\=)                                                        {return EQ;}
(\=)                                                          {return ASSIGN;}
(\<)                                                          {return LT;}
(\>)                                                          {return GT;}
(\<\=)                                                        {return LEQ;}
(\>\=)                                                        {return GEQ;}
[ \t\n]+                                                      {/* Ignore whitespace */}
.                                                             { return INVCHAR; }
%%

