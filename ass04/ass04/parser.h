/*  File name:parser.h
*  Compiler: [MS Visual Studio 2013]
*  Author: Seyedamin Seyedmahmoudian,040813340
*  Course: CST 8152 – Compilers,
*  Lab Section:11
*  Assignment: The Parser
*  Date: 2016-12-08
*  Professor:Sv.Ranve
*  Purpose: define,extern and global variable + function prototype required for parser.c
*/

#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "buffer.h"
#include "stable.h"

static Token lookahead;
static Buffer *sc_buf;
int synerrno;

enum
{
	ELSE,
	IF,
	INPUT,
	OUTPUT,
	PLATYPUS,
	REPEAT,
	THEN,
	USING
};

/*  DEFINES  */
#define NO_ATTR -1

extern char * kw_table[];
extern int line;
extern STD sym_table;
extern Buffer * str_LTBL;
extern Token mlwpar_next_token(Buffer *);

/*   FUNCTION DECLARATIONS  */
void parser(Buffer *);
void match(int , int );
void syn_eh(int );
void syn_printe();
void gengen_incode(char *);
void program(void);
void opt_statements();
void statements();
void statement();
void statements_p();
void assignment_statement();
void assignment_expression();
void selection_statement();
void iteration_statement();
void input_statement();
void variable_list();
void variable_identifier_p();
void variable_identifier();
void output_statement();
void opt_variable_list();
void arithmetic_expression();
void unary_arithmetic_expression();
void additive_arithmetic_expression();
void additive_arithmetic_expression_p();
void multiplicative_arithmetic_expression();
void multiplicative_arithmetic_expression_pe();
void primary_arithmetic_expression();
void string_expression();
void string_expression_p();
void primary_string_expression();
void conditional_expression();
void logical_OR_expression();
void logical_OR_expression_p();
void logical_AND_expression();
void logical_AND_expression_p();
void relational_expression();
void relational_expression_p();
void relational_expression_pp();
void primary_a_relational_expression();
void primary_s_relational_expression();

#endif

