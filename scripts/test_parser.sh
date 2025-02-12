#!/bin/bash

# Compile the lexical analyzer
flex lexical_analyser.l

# Compile the parser
bison -d -t parser.y

# Compile the generated C files
g++ parser.tab.c lex.yy.c -o parser -lfl

# Run the parser with the input file
./parser < examples/programa.c
