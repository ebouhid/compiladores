# Generate the lexical analyzer
flex src/lexical_analyser.l

# Generate the parser
bison -d src/analise_sintatica.y

# Compile the generated C code along with any additional source files
gcc -g lex.yy.c analise_sintatica.tab.c src/arvore.c src/tabSimbolos.c src/analise_semantica.c src/main.c src/codigo_intermediario.c -ll -o compiler

# If Linux, use the following command instead:
# gcc -g -o compiler lex.yy.c analise_sintatica.tab.c src/arvore.c src/tabSimbolos.c src/analise_semantica.c globals.h src/main.c src/codigo_intermediario.c -lfl

# Create a directory to store the output files
if [ ! -d "outputs" ]; then
  mkdir outputs
fi

# Run the compiled program
./compiler < test_codes/sort_ok.c > outputs/output.txt 2>&1
