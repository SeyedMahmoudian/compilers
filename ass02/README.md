# Lexical Analyzer:
The scanner reads and source program form a text file and produces a stream of token representations. [Scanner](https://github.com/SeyedMahmoudian/compilers/tree/master/ass02/ass02) is a fcuntion that recognizes language tokens and produces token representaitons one at time when called by syntax anaylzer.

In this implementaiton, the input to the lexical analyzer is a source program seen as a stream characters loaded into an input buffer.The output of each call to the Scanner is a single Token, to be requested and used by [Parser](https://github.com/SeyedMahmoudian/compilers/tree/master/ass04/ass04). 

[Scanner](https://github.com/SeyedMahmoudian/compilers/tree/master/ass02/ass02) is a mixture between transition-table driven and token driven scanner. 
1. Transition-table driven scanner are easy to implement with scanner generators.
2. Token-driven scanner every token is processed as a seperate exceptional case.

#### this scanner will take the middle road

Transition-table driven part of the scanner is to recognize only variable identifiers, integer literals, and floating-point literals. 

To build the transition table for those three token we have to transform their grammer definitions into regular expressions, and create the corresponding transition diagram and transition table.

[The full language specification can be seen here](https://github.com/SeyedMahmoudian/compilers/tree/master/ass04)
