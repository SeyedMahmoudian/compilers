/*  File name:stable.c
*  Compiler: [MS Visual Studio 2013]
*  Author: Seyedamin Seyedmahmoudian,040813340
*  Course: CST 8152 – Compilers,Lab Section:11
*  Assignment: The parser
*  Date:2016-11-24
*  Professor:Sv.Ranve
*  Purpose: write a Recursive Descent Predictive Parser (RDPP) for the PLATYPUS
*  Function list:
*				-parser(Buffer*);
*				-match(int, int);
*				-syn_eh(int);
*				-syn_printe(void);
*				-program(void);
*				-gen_incode(char*);
*				-opt_statements(void);
*				-statements(void);
*				-statements_p(void);
*				-statement(void);
*				-assignment_statement(void);
*				-assignment_expression(void);
*				-selection_statement(void);
*				-iteration_statement(void);
*				-input_statement(void);
*				-variable_identifier(void);
*				-variable_list(void);
*				-variable_list_p(void);
*				-output_statement(void);
*				-output_list(void);
*				-arithmetic_expression(void);
*				-unary_arithmetic_expression(void);
*				-additive_arithmetic_expression(void);
*				-additive_arithmetic_expression_p(void);
*				-multiplicative_arithmetic_expression(void);
*				-multiplicative_arithmetic_expression_p(void);
*				-primary_arithmetic_expression(void);
*				-string_expression(void);
*				-string_expression_p(void);
*				-primary_string_expression(void);
*				-conditional_expression(void);
*				-logical_OR_expression(void);
*				-logical_OR_expression_p(void);
*				-logical_AND_expression(void);
*				-logical_AND_expression_p(void);
*				-relational_expression(void);
*				-primary_a_relational_expression(void);
*				-primary_s_relational_expression(void);
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include"buffer.h"
#include"token.h"
#include"stable.h"
#include "parser.h"

/*	Function Name:parser
*	Purpose: This function initilize the buffer and get token provided by mlwpar_next_token of scanner.c, then call the program function to start the application
*			 then it calls the match function for the SEOF_T(source end of file token) with no atrribuite to finish the process
*	param: Buffer* in_buf
*	version: 1.0.0
*	author: Sv.Ranve
*	return: void
*/
void parser(Buffer * in_buf){
	sc_buf = in_buf;
	lookahead = mlwpar_next_token(sc_buf);
	program();
	match(SEOF_T, NO_ATTR);
	gen_incode("Source file parsed\n");
}

/*	Function Name:match
*	Purpose: matches 2 token , 1 the current input token and the token required by parser
*	param: int pr_token_code, int pr_token_attribute
*	version: 1.0.0
*	author: Seyedamin Seyedmahmoudian
*	return: void
*/
void match(int pr_token_code, int pr_token_attribute){
	
	if (pr_token_code == lookahead.code)
	{
		switch (pr_token_code)
		{
		case ART_OP_T:
		case REL_OP_T:
		case LOG_OP_T:
		case KW_T:	
			if (pr_token_attribute != lookahead.attribute.get_int)
				break;
		default:
			if (lookahead.code == SEOF_T){
				return;
			}
			lookahead = mlwpar_next_token(sc_buf);
			if (lookahead.code == ERR_T)
			{
				syn_printe();
				++synerrno;
				lookahead = mlwpar_next_token(sc_buf);
			}
			return;
		}
	}
	syn_eh(pr_token_code);
}

/*	Function Name:syn_eh
*	Purpose: error handling function which implement panic mode , it will advance
*			 the input token
*	param: int sync_token_code
*	version: 1.0.0
*	author: Seyedamin Seyedmahmoudian
*	return: void
*/
void syn_eh(int sync_token_code){
	syn_printe();
	synerrno++;
	while (lookahead.code != SEOF_T){
		lookahead = mlwpar_next_token(sc_buf);
		if (lookahead.code == sync_token_code){
			if (lookahead.code != SEOF_T){
				lookahead = mlwpar_next_token(sc_buf);
			}
			return;
		}
	}
	if (sync_token_code != SEOF_T){
		exit(synerrno);
	}
}

/*	Function Name:syn_printe
*	Purpose: print appropiate error for a token
*	param: void
*	version: 1.0.0
*	author: Sv.Ranve
*	return: void
*/
void syn_printe(){
	Token t = lookahead;

	printf("PLATY: Syntax error:  Line:%3d\n", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code){
	case  ERR_T: /* ERR_T     0   Error token */
		printf("%s\n", t.attribute.err_lex);
		break;
	case  SEOF_T: /*SEOF_T    1   Source end-of-file token */
		printf("NA\n");
		break;
	case  AVID_T: /* AVID_T    2   Arithmetic Variable identifier token */
	case  SVID_T:/* SVID_T    3  String Variable identifier token */
		printf("%s\n", sym_table.pstvr[t.attribute.get_int].plex);
		break;
	case  FPL_T: /* FPL_T     4  Floating point literal token */
		printf("%5.1f\n", t.attribute.flt_value);
		break;
	case INL_T: /* INL_T      5   Integer literal token */
		printf("%d\n", t.attribute.get_int);
		break;
	case STR_T:/* STR_T     6   String literal token */
		printf("%s\n", b_cbhead(str_LTBL) + t.attribute.str_offset);
		break;

	case SCC_OP_T: /* 7   String concatenation operator token */
		printf("NA\n");
		break;

	case  ASS_OP_T:/* ASS_OP_T  8   Assignment operator token */
		printf("NA\n");
		break;
	case  ART_OP_T:/* ART_OP_T  9   Arithmetic operator token */
		printf("%d\n", t.attribute.get_int);
		break;
	case  REL_OP_T: /*REL_OP_T  10   Relational operator token */
		printf("%d\n", t.attribute.get_int);
		break;
	case  LOG_OP_T:/*LOG_OP_T 11  Logical operator token */
		printf("%d\n", t.attribute.get_int);
		break;

	case  LPR_T: /*LPR_T    12  Left parenthesis token */
		printf("NA\n");
		break;
	case  RPR_T: /*RPR_T    13  Right parenthesis token */
		printf("NA\n");
		break;
	case LBR_T: /*    14   Left brace token */
		printf("NA\n");
		break;
	case RBR_T: /*    15  Right brace token */
		printf("NA\n");
		break;

	case KW_T: /*     16   Keyword token */
		printf("%s\n", kw_table[t.attribute.get_int]);
		break;

	case COM_T: /* 17   Comma token */
		printf("NA\n");
		break;
	case EOS_T: /*    18  End of statement *(semi - colon) */
		printf("NA\n");
		break;
	default:
		printf("PLATY: Scanner error: invalid token code: %d\n", t.code);
	}/* end switch*/
}/* end syn_printe()*/
/*	Function Name:gen_incode
*	Purpose: takes a string as an argument and prints it
*	param: char* gen
*	version: 1.0.0
*	author: Seyedamin Seyedmahmoudian
*	return: void
*/
void gen_incode(char * c){
	printf("PLATY: %s", c);
}

/*	Function Name:  program
*	Purpose: call match funciton with token and its attribuite
*	param: void
*	version: 1.0.0
*	author:S.Ranev
*	return: void
*/
void program(void){
	match(KW_T, PLATYPUS);
	match(LBR_T, NO_ATTR);
	opt_statements();
	match(RBR_T, NO_ATTR);
	gen_incode("Program parsed\n");
}

/*	Function Name: opt_statements
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author: Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm: FIRST set:{AVID_T,SVID_T,KW_T (no PLATYPUS,ELSE,THEN,REPEAT),AVID_T,SVID_T,and e}
*				<opt_statements>->
*							<statements>|e
*
*/
void opt_statements(){
	switch (lookahead.code){
	case AVID_T:
	case SVID_T:
		statements();
		break;
	case KW_T:
		if (lookahead.attribute.get_int != PLATYPUS 
			&& lookahead.attribute.get_int != ELSE 
			&& lookahead.attribute.get_int != THEN  
			&& lookahead.attribute.get_int != REPEAT){
			statements();
			break;
		}
	default: ;
		gen_incode("Opt_statements parsed\n");
	}
}
/*	Function Name: statements
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author:Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm:FIRST set={AVID,SVID,IF,USING,INPUT,OUTPUT}
*			<statements>->
*					 <statement><statements'>
*/
void statements(){
	statement();
	statements_p();
}
/*	Function Name: statement
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author:Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm:FIRST(statemetnt)={AVID,SVID,IF,USING,INPUT,OUTPUT}
*	<statement>->
*			<assignment statement>
*			|<selection statement>
*			|<iteration statement>
*			|<input statement>
*			|<output statement>
*/
void statement(){
	switch (lookahead.code){
	case AVID_T:
	case SVID_T:
		
		assignment_statement();
		break;
	case KW_T:
		switch (lookahead.attribute.kwt_idx){
		case IF:
			
			selection_statement();
			break;
		case USING:
			
			iteration_statement();
			break;
		case INPUT:
			
			input_statement();
			break;
		case OUTPUT:
			
			output_statement();
			break;
		}
		break;
	default:
		syn_printe();
	}
}
/*	Function Name: statements_p
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author:Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm:FIRST(STATEMENTS')={AVID,SVID,IF,USING,INPUT,OUTPUT,e}
*		<statements'>->
*				<statement><statements'>|e
*/
void statements_p(){
	switch (lookahead.code){
	case KW_T:
		switch (lookahead.attribute.kwt_idx){
		case IF:
		case USING:
		case INPUT:
		case OUTPUT:
			break;
		default:
			return;
		}
	case AVID_T:
	case SVID_T:
		statement();
		statements_p();
		break;
	default:
		break;
	}
}
/*	Function Name:assignment_statement
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author: Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm: FIRST(assignment statement)={AVID,SVID}
*				<assignment statement>->
*							<assignment expression>;
*/
void assignment_statement(){
	assignment_expression();
	match(EOS_T, NO_ATTR);
	gen_incode("Assignment statement parsed\n");
}
/*	Function Name:arithmetic_expression
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author: Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm: FIRST(assignment expression)={AVID,SVID}
*			<assignment expression>->
*						AVID=<artimetic expression>
*					   |SVID=<string expression>
*/
void assignment_expression(){
	switch (lookahead.code){
	case AVID_T:
		match(AVID_T, NO_ATTR);
		match(ASS_OP_T, NO_ATTR);
		arithmetic_expression();
		gen_incode("Assignment expression (arithmetic) parsed\n");
		break;
	case SVID_T:
		match(SVID_T, NO_ATTR);
		match(ASS_OP_T, NO_ATTR);
		string_expression();
		gen_incode("Assignment expression (string) parsed\n");
		break;
	default:
		syn_printe();
		return;
	}
}
/*	Function Name: selection_statement
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author:Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm:FIRST(assignment expression)={IF}
*	<selection statement>->
*			IF(<conditional expression>)THEN<opt_statements>
*			ELSE{<opt_statements>};
*/
void selection_statement(){
	match(KW_T, IF);
	match(LPR_T, NO_ATTR);
	conditional_expression();
	match(RPR_T, NO_ATTR);
	match(KW_T, THEN);
	opt_statements();
	match(KW_T, ELSE);
	match(LBR_T, NO_ATTR);
	opt_statements();
	match(RBR_T, NO_ATTR);
	match(EOS_T, NO_ATTR);
	gen_incode("IF statement parsed\n");
}
/*	Function Name:iteration_statement
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author: Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm: FIRST(iteration statement)={USING}
*				<iteration statement>->
*							USING(<assignment expression>,<conditional expression>,<assignment expression>)
*					REPEAT{
*						<opt_statement>
*					  };
*/ 
void iteration_statement(){
	match(KW_T, USING);
	match(LPR_T, NO_ATTR);
	assignment_expression();
	match(COM_T, NO_ATTR);
	conditional_expression();
	match(COM_T, NO_ATTR);
	assignment_expression();
	match(RPR_T, NO_ATTR);
	match(KW_T, REPEAT);
	match(LBR_T, NO_ATTR);
	opt_statements();
	match(RBR_T, NO_ATTR);
	match(EOS_T, NO_ATTR);
	gen_incode("USING statement parsed\n");
}
/*	Function Name: input_statement
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author: Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm: FIRST(input statement)={INPUT}
*				<input statement> ->
*							INPUT(<variable list>);
*/
void input_statement(){
	match(KW_T, INPUT);
	match(LPR_T, NO_ATTR);
	variable_list();
	match(RPR_T, NO_ATTR);
	match(EOS_T, NO_ATTR);
	gen_incode("INPUT statement parsed\n");
}
/*	Function Name: variable_list
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author:Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm:FIRST(variable list)={variable identifier}
*			<variable list>->
*					<variable identifier><variable list'>
*/
void variable_list(){
	variable_identifier();
	variable_identifier_p();
	gen_incode("Variable list parsed\n");
}
/*	Function Name: variable_identifier
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author:Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm:FIRST(variable identifier)={AVID_T,SVID_T}
*			<variable identifier> -> AVID_T|SVID_T
*/
void variable_identifier(){
	switch (lookahead.code){
	case AVID_T:
	case SVID_T:
		match(lookahead.code, NO_ATTR);
		break;
	default:
		syn_printe();
		return;
	}
}
/*	Function Name: variable_p
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author:Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm:FIRST(variable identifier')={, , E }
*			<variable identifier’> -> ,<variable identifier> <variable identifier’>|
*/
void variable_identifier_p(){
	if (lookahead.code == COM_T){
		match(lookahead.code, NO_ATTR);
		variable_identifier();
		variable_identifier_p();
	}
}
/*	Function Name: output_statement
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author: Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm: FIRST(input statement)={OUTPUT}
*				<out statement> ->
*							OUTPUT(<variable list>);
*/
void output_statement(){
	match(KW_T, OUTPUT);
	match(LPR_T, NO_ATTR);
	opt_variable_list();
	match(RPR_T, NO_ATTR);
	match(EOS_T, NO_ATTR);
	gen_incode("OUTPUT statement parsed\n");
}
/*	Function Name: opt_variable_list
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author:Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm:FIRST(variable identifier)={AVID_T,SVID_T,e}
*			<variable list>-> variablelist | STR | Ɛ
*/

void opt_variable_list(){
	switch (lookahead.code){
	case AVID_T:
	case SVID_T:
		variable_list();
		break;
	case STR_T:
		match(lookahead.code, NO_ATTR);
		gen_incode("Output list (string literal) parsed\n");
		break;
	default:
		gen_incode("Output list (empty) parsed\n");
	}
}
/*	Function Name:additive_arithmetic_expression
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author: Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm: FIRST(additive arithemtic expression)={ -, +, AVID_T, FPL_T, INL_T, ( }
*              <additive arithmetic expression>->
*							<unary arithmetic expression> | <additive arithmetic expression>
*/

void arithmetic_expression(){
	switch (lookahead.code){
	case ART_OP_T:
		switch (lookahead.attribute.arr_op){
		case MULT:
		case DIV:
			syn_printe();
			return;
		default:
			break;
		}
		unary_arithmetic_expression();
		break;
	case AVID_T:
	case FPL_T:
	case INL_T:
	case LPR_T:
		additive_arithmetic_expression();
		break;
	default:
		syn_printe();
		return;
	}
	gen_incode("Arithmetic expression parsed\n");
}
/*	Function Name: unary_arithmetic_expression
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author:Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm:FIRST(unary arithemetic expression)={-,+}
*				<unary arithemetic expression>->
*						- <primary arithemetic expression>
*					  | + <primary arithemetic expression>
*/

void unary_arithmetic_expression(){
	switch (lookahead.code){
	case ART_OP_T:
		switch (lookahead.attribute.arr_op){
		case MULT:
		case DIV:
			syn_printe();
			return;
		default:
			break;
		}
		match(lookahead.code, lookahead.attribute.arr_op);
		primary_arithmetic_expression();
		gen_incode("Unary arithmetic expression parsed\n");
		break;
	default:
		syn_printe();
		return;
	}
}
/*	Function Name:additive_arithmetic_expression
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author: Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm: FIRST(additive arithemtic expression)={AVID_T,FPL_T,INL_T,(}
*              <additive arithmetic expression>->
*							<multiplicative arithmetic expression> <additive arithmetic expression'>
*/

void additive_arithmetic_expression(){
	multiplicative_arithmetic_expression();
	additive_arithmetic_expression_p();
}
/*	Function Name:additive_arithmetic_expression_p
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author: Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm: FIRST(additive arithmetic expression')={+,-,e}
*		<additive arithmetic expression'>->
*								+ <multiplicative arithmetic expression> <additive arithmetic expression'>
*							   |- <multiplicative arithmetic expression> <additive arithmetic expression'>
*							   | e
*/
void additive_arithmetic_expression_p(){
	if (lookahead.code == ART_OP_T){
		switch (lookahead.attribute.arr_op){
		case MULT:
		case DIV:
			return;
		default:
			break;
		}
		match(lookahead.code, lookahead.attribute.arr_op);
		multiplicative_arithmetic_expression();
		additive_arithmetic_expression_p();
		gen_incode("Additive arithmetic expression parsed\n");
	}
}
/*	Function Name: multiplicative_arithmetic_expression
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author: Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm: FIRST(multiplicative arithmetic expression)={AVID_T,FPL_T,INL_T,(}
*		<multiplicative arithmetic expression>->
*						<primary arithmetic expression> <multuplicative arithmetic expression'>
*/

void multiplicative_arithmetic_expression(){
	primary_arithmetic_expression();
	multiplicative_arithmetic_expression_pe();
}
/*	Function Name: multiplicative_arithmetic_expression_pe
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author: Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm: FIRST(multiplicative arithmetic expression')={*,/,e}
*				<multiplicative arithmetic expression'> ->
*			  *<primary arithmetic expression> <multuplicative arithmetic expression'>
*			| /<primary arithmetic expression> <multuplicative arithmetic expression'>
*			| e
*/
void multiplicative_arithmetic_expression_pe(){
	if (lookahead.code == ART_OP_T){
		switch (lookahead.attribute.arr_op){
		case PLUS:
		case MINUS:
			return;
		default:
			break;
		}
		match(lookahead.code, lookahead.attribute.arr_op);
		primary_arithmetic_expression();
		multiplicative_arithmetic_expression_pe();
		gen_incode("Multiplicative arithmetic expression parsed\n");
	}
}
/*	Function Name:  primary_arithmetic_expression
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author: Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm: FIRST(primary arithmetic expression)={AVID_T,FPL_T,INL_T,(}
*				<primary arithmetic expression> ->
*													AVID_T
*												  | FPL_T
*												  | INL_T
*						                          |(<arithmetic expression>)
*/
void primary_arithmetic_expression(){
	switch (lookahead.code){
	case AVID_T:
	case FPL_T:
	case INL_T:
		match(lookahead.code, lookahead.attribute.arr_op);
		break;
	case LPR_T:
		match(lookahead.code, lookahead.attribute.arr_op);
		arithmetic_expression();
		match(RPR_T, NO_ATTR);
		break;
	default:
		syn_printe();
		return;
	}
	gen_incode("Primary arithmetic expression parsed\n");
}
/*	Function Name: string_expression
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author:Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm:FIRST (string expression)={SVID_T,STR_T}
*			<string expression> ->
*					<primary string expression> <string expression'>
*/
void string_expression(){
	primary_string_expression();
	string_expression_p();

}

/*	Function Name: string_expression_p
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author:Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm:FIRST(string expression')={#,e}
*			<string expression'>->
*					<primary string expression> <string expression'>|e
*/
void string_expression_p(){
	if (lookahead.code == SCC_OP_T){
		match(lookahead.code, lookahead.attribute.arr_op);
		primary_string_expression();
		string_expression_p();
	}
	else{
		gen_incode("String expression parsed\n");
	}
}
/*	Function Name:  primary_string_expression
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author: Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm: FIRST(primary string expression={SVID_T,STR_T}
*			<primary string expression> ->
*							SVID_T|STR_T
*/
void primary_string_expression(){
	switch (lookahead.code){
	case SVID_T:
	case STR_T:
		match(lookahead.code, lookahead.attribute.arr_op);
		break;
	default:
		syn_printe();
	}
	gen_incode("Primary string expression parsed\n");
}

/*	Function Name: conditional_expression
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author: Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm: FIRST(conditional expression)={AVID_T,FPL_T,INL_T,SVID_T,STR_T}
*				<conditional expression> ->
*							<logical OR expression>
*/
void conditional_expression(){
	logical_OR_expression();
	gen_incode("Conditional expression parsed\n");
}
/*	Function Name: logical_OR_expression
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author: Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm: FIRST(logical OR expression)={AVID_T,FPL_T,INL_T,SVID_T,STR_T}
*					<logical OR expression>->
*								<logical AND expression>
*								|<logical OR expression'>
*/
void logical_OR_expression(){
	logical_AND_expression();
	logical_OR_expression_p();
}
/*	Function Name: logical_OR_expression_p
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author: Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm: FIRST(logical OR expression')={.OR.,e}
*					<logical OR expression'> ->
*								.OR. <logical AND expression> <logical OR expression'>|e
*/
void logical_OR_expression_p(){
	if (lookahead.code == LOG_OP_T){
		switch (lookahead.attribute.log_op){
		case AND:
			return;
		default:
			break;
		}
		match(lookahead.code, lookahead.attribute.arr_op);
		logical_AND_expression();
		logical_OR_expression_p();
		gen_incode("Logical OR expression parsed\n");
	}
}
/*	Function Name: logical_AND_expression
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author: Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm: FIRST(logical AND expression)={AVID_T,FPL_T,INL_T,SVID_T,STR_T}
*				<logical AND expression> ->
*							<relational expression> <logical AND expression'>
*/
void logical_AND_expression(){
	relational_expression();
	logical_AND_expression_p();
}
/*	Function Name: logical_AND_expression_p
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author: Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm: FIRST (logical AND expression')={.AND.,e}
*					<logical AND expression'> ->
*								.AND. <relational expression><logical AND expression'>|e
*/
void logical_AND_expression_p(){
	if (lookahead.code == LOG_OP_T){
		switch (lookahead.attribute.log_op){
		case OR:
			return;
		default:
			break;
		}
		match(lookahead.code, lookahead.attribute.arr_op);
		relational_expression();
		logical_AND_expression_p();
		gen_incode("Logical AND expression parsed\n");
	}
}

/*	Function Name: relational_expression
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author:Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm:FIRST(relational expression)={AVID_T,FPL_T}
*		<relational expression>->
*						<primary a_relational expression> <operator> <primary a_relational expression>
*					   |<primary s_relational expression> <operator> <primary s_relational expression>
*/
void relational_expression(){
	switch (lookahead.code){
	case AVID_T:
	case FPL_T:
	case INL_T:
		primary_a_relational_expression();
		relational_expression_p();
		break;
	case SVID_T:
	case STR_T:
		primary_s_relational_expression();
		relational_expression_pp();
		break;
	default:
		syn_printe();
	}
	gen_incode("Relational expression parsed\n");
}
/*	Function Name: relational_expression_p
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author:Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm:FIRST={ ==, !=, >, < }
*		<relational_expression’> ->
*							== <primary a_relational_expression>
*						   |<> <primary a_relational_expression>
*						   | > <primary a_relational_expression>
*                          | <  <primary a_relational_expression>
*/
void relational_expression_p(){
	if (lookahead.code == REL_OP_T){
		switch (lookahead.attribute.rel_op){
		case EQ:
		case NE:
		case GT:
		case LT:
			match(lookahead.code, lookahead.attribute.arr_op);
			primary_a_relational_expression();
			break;
		default:
			syn_printe();
		}
	}
	else{
		syn_printe();
	}
}
/*	Function Name: relational_expression_pp
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author:Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm:FIRST={ ==, !=, >, < }
*		<relational_expression’> ->
*							== <primary a_relational_expression>
*						   |<> <primary a_relational_expression>
*						   | > <primary a_relational_expression>
*                          | <  <primary a_relational_expression>
*/

void relational_expression_pp(){
	if (lookahead.code == REL_OP_T){
		switch (lookahead.attribute.rel_op){
		case EQ:
		case NE:
		case GT:
		case LT:
			match(lookahead.code, lookahead.attribute.arr_op);
			primary_s_relational_expression();
			break;
		default:
			syn_printe();
		}
	}
	else{
		syn_printe();
	}
}
/*	Function Name:  primary_a_relational_expression
*	Purpose: see the algorithm production
*	param: void
*	version: 1.0.0
*	author: Seyedamin Seyedmahmoudian
*	return: void
*	Algorithm: FIRST ={AVID_T,FPL_T,INL_T}
*				<primary a_relational expression>->
*											AVID_T | FPL_T | INL_T
*/

void primary_a_relational_expression(){
	switch (lookahead.code){
	case AVID_T:
	case FPL_T:
	case INL_T:
		match(lookahead.code, NO_ATTR);
		break;
	default:
		syn_printe();
	}
	gen_incode("Primary a_relational expression parsed\n");
}

	/*	Function Name:  primary_s_relational_expression
	*	Purpose: see the algorithm production
	*	param: void
	*	version: 1.0.0
	*	author: Seyedamin Seyedmahmoudian
	*	return: void
	*	Algorithm: FIRST (primary s_relational expression)={SVID_T,STR_T}
	*						<primary s_relational expressino>->
	*												<primary string expression>
	*/
void primary_s_relational_expression(){
	primary_string_expression();
	gen_incode("Primary s_relational expression parsed\n");
}
