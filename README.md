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
A tabela de símbolos é implementada utilizando uma tabela hash para armazenar informações sobre variáveis e funções declaradas no código-fonte. Cada entrada na tabela de símbolos contém o nome, tipo, escopo e outras informações relevantes sobre a variável ou função.

## Analisador sintático
O analisador sintático é implementado utilizando o Bison para gerar o parser a partir da gramática da linguagem c-. Ele constrói a árvore de sintaxe abstrata (AST) que representa a estrutura do código-fonte.

## Árvore sintática
A árvore sintática (AST) é uma representação hierárquica do código-fonte, onde cada nó representa uma construção sintática (como uma declaração de variável ou uma expressão). A AST é utilizada nas etapas posteriores de análise semântica e geração de código.

## Analisador semântico
O analisador semântico verifica regras semânticas do código-fonte, como tipos de dados, escopos e declarações múltiplas. Ele utiliza a tabela de símbolos para garantir que todas as variáveis e funções sejam declaradas corretamente e que não haja conflitos de tipos.

## Como compilar e executar
Para compilar e executar o compilador, utilize o script `script.sh` fornecido no repositório. Este script gera o analisador léxico, o parser, compila o código-fonte e executa o compilador com um arquivo de teste.

```bash
# Generate the lexical analyzer
flex src/lexical_analyser.l

# Generate the parser
bison -d src/analise_sintatica.y

# Compile the generated C code along with any additional source files
gcc -g -o compiler lex.yy.c analise_sintatica.tab.c src/arvore.c src/tabSimbolos.c src/analise_semantica.c globals.h src/main.c -lfl

# Create a directory to store the output files
if [ ! -d "outputs" ]; then
  mkdir outputs
fi

# Run the compiled program
./compiler < test_codes/sort_ok.c > outputs/output.txt 2>&1
```

## Estrutura do repositório
- `src/`: Contém os arquivos-fonte do compilador.
- `outputs/`: Diretório onde os arquivos de saída são gerados.
- `test_codes/`: Contém arquivos de teste para o compilador.
- `script.sh`: Script para compilar e executar o compilador.
- `README.md`: Este arquivo de documentação.

## Autores
- Eduardo Bouhid
- Ícaro Travain
