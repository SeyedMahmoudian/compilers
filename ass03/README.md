# Symbol Table:

This is a symbol table compontent of Platypus compiler. The symbol table component consists of two parts:
1. Symbol Table Manager (STM)
2. Symbol Table Database (STDB)

STM provides utilities for manipulation of the STDB. The STDB is a repository for VID atrributes. Each Variable identifier is associated with one record in the database. Five VID attributes will be defined in the symbol table: variable name, type, initial value, line number,
and one reserved attribute.

The [Scanner](https://github.com/SeyedMahmoudian/compilers/tree/master/ass02) can identify and store in the symbol table only four of those attributes:
1. The lexeme for the VID.
2. The line number of the line where corresponding VID apears for the first time in the source program.
3. The default type.
4. Initial value of the variable.

Memory Map:
![alt text](https://github.com/SeyedMahmoudian/compilers/blob/master/ass03/memory_map.jpg)
