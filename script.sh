# Generate the lexical analyzer
flex lexical_analyser.l

# Generate the parser
bison -d analise_sintatica.y

# Compile the generated C code along with any additional source files
gcc -o compiler lex.yy.c analise_sintatica.tab.c -lfl

# Run the compiled program
./compiler < test_codes/sort_ok.c
