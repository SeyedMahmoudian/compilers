/*  File name:stable.h
*  Compiler: [MS Visual Studio 2013]
*  Author: Seyedamin Seyedmahmoudian,040813340
*  Course: CST 8152 – Compilers,
*  Lab Section:11
*  Assignment: The Scanner
*  Date: 2016-11-24
*  Professor:Sv.Ranve
*  Purpose: structures,function prototype and other define required in stable.c
*/
#ifndef STABLE_H
#define STABLE_H

#define DEFAULT 0XFFF8 /*1111 1111 1111 1111 default value field*/
#define FLOAT_M	0X0002 /*0000 0000 0000 0010 float status field*/
#define INT_M	0X0004 /*0000 0000 0000 0100 int status field*/
#define STRING_M 0X0006 /*0000 0000 0000 0110 string status field*/
#define UPDATE_M 0X0001 /*0000 0000 0000 0001 set update flag*/
#define TYPE_MASK 0xFFF8 /* 1111 1111 1111 1000 status field type check*/
#define BUFSIZE 1000 /*Buffer size*/
#define BUFINC 15 /*increment size*/
#define BUFMODE 'a' /*Buffer mode*/
#define FLOATZERO 0.0f /*define for floating point numbers*/
#define PASS_1	1 /*define for return of 1 for functions needed */
#define FAIL_1 -1/*define for return of -1 for functions needed */
#define FAIL_2 -2 /*define for return of -2 for functions needed */
#define INTZERO 0 /*define for integer numbers*/
#define SEOF_S '\0'


typedef union InitialValue {
	int int_val; /* integer variable initial value */
	float fpl_val; /* floating-point variable initial value */
	int str_offset; /* string variable initial value (offset) */
} InitialValue;

typedef struct SymbolTableVidRecord {
	unsigned short status_field; /* variable record status field*/
	char * plex; /* pointer to lexeme (VID name) in CA */
	int o_line; /* line of first occurrence */
	InitialValue i_value; /* variable initial value */
	size_t reserved; /*reserved for future use*/
}STVR;


typedef struct SymbolTableDescriptor {
	STVR *pstvr; /* pointer to array of STVR */
	int st_size; /* size in number of STVR elements */
	int st_offset; /*offset in number of STVR elements */
	Buffer *plsBD; /* pointer to the lexeme storage buffer descriptor */
} STD;

STD st_create(int);/*create a new empty symbol table*/
int st_install(STD, char *, char, int);/*install a new entry VID record in the symbol table*/
int st_lookup(STD, char*);/* search for lexeme in symbol table*/
int st_update_type(STD, int, char);/*update data type indicator in STVD*/
int st_update_value(STD, int, InitialValue);/*update the i_value of the variable specified by vid_offset*/
char st_get_type(STD, int);/*return type of the variable*/
void st_destroy(STD);/*free the memory occupied by symbol table*/
int st_print(STD);/*print the contents of the symbol table*/
int st_store(STD);/*store the symbol table into a file */
int st_sort(STD, char);/*sort function that will sort the table base on the mode*/

#endif

