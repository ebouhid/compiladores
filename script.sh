# Generate the lexical analyzer
flex src/lexical_analyser.l

# Generate the parser
bison -d src/analise_sintatica.y

# Compile the generated C code along with any additional source files
gcc -g -o compiler src/main.c lex.yy.c analise_sintatica.tab.c src/arvore.c src/tabSimbolos.c src/analise_semantica.c globals.h -lfl

# Run the compiled program
./compiler < test_codes/very_basic_ok.c > output.txt 2>&1
