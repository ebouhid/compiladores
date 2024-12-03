# Compiladores
Repositório criado para armazenar o projeto final da disciplina de compiladores ministrada pelo discente Galão na UNIFESP. O projeto consiste na implementação do analisador léxico e semântico da linguagem de programação c-. Os autores deste projeto são:
- Eduardo Bouhid
- Ícaro Travain

## Scanner (Analisador léxico)
A primeira etapa deste projeto consiste na implementação de um analisador léxico com auxílio do software flex para gerar o progema em c a partir das expressões regulares da linguagem. 
| Tokens              | Exp. Regular                                                                                                                   |
|---------------------|--------------------------------------------------------------------------------------------------------------------------------|
| Identificador       | [A-Za-z]+([A-Za-z])*                                                                                                           |
| Numero              | [1-9]+[0-9]*                                                                                                                   |
| Palavras Reservadas | (if \| else \| while \| int \| void \| return)                                                                                 |
| Comentarios         | (/\*(Identificador \| Simbolo \| Operador \| Numero)*\*/ \| // (Identificador \| Simbolo \| Operador \| Numero) *(\\n \| EOF)) |
| Macros              | (#include \| #define)                                                                                                          |
| Símbolos            | ({ \| } \| ( \| ) \| [ \| ] ; \| . \| ,)                                                                                       |
| Operadores          | (+ \| - \| / \| \* \| % \| && \| \|\| ! \| != \| == \| = | >= | <= | > | <)                                                    |

## Tabela de símbolos
tiny
## Analisador sintático
Bison
## Árvore sintática 
## Analisador semântico
## Gerador de código intermediário  
