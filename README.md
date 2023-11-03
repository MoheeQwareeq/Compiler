# Compiler

That's a great project idea writing a simple compiler can be a challeng but highly educational experience,A compiler typically consists of several stages, starting from a sequence of characters and ending with assmply code ,These stages include a Scanner, Parser, Semantic Checker, and Code Generator.

This project provides a unique opportunity to explore various concepts, such as variable scoping techniques in different programming languages when implemented in a symbol table. It also allows for an understanding of different call routines, such as call by reference and call by value. Additionally, it enables the definition of different types of errors that can be detected by the compiler and types that cannot be detected, such as runtime errors.



<img width="704" alt="Screen Shot 2023-11-03 at 11 34 31 PM" src="https://github.com/MoheeQwareeq/Compiler/assets/143301303/fdeddb42-9734-42bb-b970-6fc08717b738">

Compiler stage:

Scanner (Lexical Analysis): It reads the source code as a stream of characters using fileDescriptor and converts it into a stream of tokens based on regular expressions. Tokens are basic building blocks like keywords, identifiers, number (integer and float), operators and comments.


Parser (Syntax Analysis):  The parser takes the stream of tokens produced by the scanner and checks whether it follows the syntax rules of the programming language (Grammer). It builds a  abstract syntax tree (AST) to represent the structure of the code.


Semantic Checker:It take Abstract syntax tree produced by the parser to determine the types of expressions and find some simple semantic errors. If the program does not contain semantic errors, the next phase generates code for the language.

Code Generator: The code generator translates the abstract syntax tree of code into machine code or assembly language for a MIPS architecture.

<img width="962" alt="Screen Shot 2023-11-03 at 11 29 18 PM" src="https://github.com/MoheeQwareeq/Compiler/assets/143301303/f4957d73-c9ae-49d5-9e88-91f65bd3e7bc">

Ps: Regular expression and Grammar files in main branch. 

