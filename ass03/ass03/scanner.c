/*  File name:scanner.c
*  Compiler: [MS Visual Studio 2013]
*  Author: SeyedAmin SeyedMahmoudian,040813340
*  Course: CST 8152 – Compilers,Lab Section:11
*  Assignment: The Scanner
*  Date:2016-10-31
*  Professor:Sv.Ranve
*  Purpose: implements and tests the foundation of your lexical analyzer:
*  Function list:
*				-scanner_init(Buffer * sc_buf)
*				-mlwpar_next_token(Buffer * sc_buf)
*				-get_next_state(int state, char c, int *accept)
*				-char_class(char c)
*				-aa_func02(char lexeme[])
*				-aa_func03(char lexeme[])
*				-aa_func05(char lexeme[])
*				-aa_func08(char lexeme[])
*				-aa_func11(char lexeme[])
*				-aa_func12(char lexeme[])
*				-atool(char * lexeme)
*				-isKeyword(char * lexeme)
*				-runTimeError()
*/

/* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
* to suppress the warnings about using "unsafe" functions like fopen()
* and standard sting library functions defined in string.h.
* The define does not have any effect in Borland compiler projects.
*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */	/*THIS HAS FUNCTION TO OPEN THE A-Z AND 1-9*/
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */
#include <math.h>	/*used in last function for pow*/

/*#define NDEBUG        to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */

#include "token.h"
#include "table.h"
#include "buffer.h"
#include "stable.h"
#define DEBUG  /* for conditional processing */
#undef  DEBUG

/* Global objects - variables */
/* This buffer is used as a repository for string literals.
It is defined in platy_st.c */

extern STD sym_table; /* global Symbol Table Descriptor */
extern Buffer * str_LTBL; /*String literal table */
int line; /* current line number of the source code */
extern int scerrnum;     /* defined in platy_st.c - run-time error number */

/* Local(file) global objects - variables */
static Buffer *lex_buf;/*pointer to temporary lexeme buffer*/

/* No other global variable declarations/definitiond are allowed */

/* scanner.c static(local) function  prototypes */
static int char_class(char c); /* character class function */
static int get_next_state(int, char, int *); /* state machine function */
static int isKeyword(char * kw_lexeme); /*keywords lookup functuion */
static long atool(char * lexeme); /* converts octal string to decimal value */
Token runTimeError();

int scanner_init(Buffer * sc_buf) {
	if (b_isempty(sc_buf)) return EXIT_FAILURE;/*1*/
	/* in case the buffer has been read previously  */
	b_setmark(sc_buf, 0);
	b_retract_to_mark(sc_buf);
	b_reset(str_LTBL);
	line = 1;
	return EXIT_SUCCESS;/*0*/
	/*   scerrnum = 0;  *//*no need - global ANSI C */
}
/*	Function Name: mlwpar_next_token
*	Purpose: Read the lexeme from the buffer one character at the time. if found a token pattern it will return
token structure which match the lexeme found in the stream of input symbol.if token can not be
found it will set the error token.
*	param: Buffer * sc_buf
*	version: 1.4.0
*	Author: SeyedAmin SeyedMahmoudian
*	return: Token
*/
Token mlwpar_next_token(Buffer * sc_buf)
{
	Token t; /* token to return after recognition */
	unsigned char c; /* input symbol */
	int state = 0; /* initial state of the FSM */
	short lexstart = 0;  /*start offset of a lexeme in the input buffer */
	short lexend = 0;    /*end   offset of a lexeme in the input buffer */
	int accept = NOAS; /* type of state - initially not accepting */
	short size = 0;
	int ch = 0;

	memset(t.attribute.err_lex, '\0', ERR_LEN + 1);/*clean up the the array from garbage or any other values and fill everything with \0*/

	for (;;) {
		c = b_getc(sc_buf);/*get the next character*/
		
		/* special cases or token driven processing*/

		switch (c){
		case ' ': /*empty space or tab*/
		case'\t':
		case '\v':
		case '\f':
			continue;
		case'\n':/*new line , line number increment*/
			++line;

			continue;
		case '\r':
			c = b_getc(sc_buf);
			if (c != '\n')
				b_retract(sc_buf);
			++line;
			continue;
			/*check for comment*/
		case '!':

			c = b_getc(sc_buf);/*start getting next character*/

			if (c == '<'){
				while (c != '\n' && c != '\r' && c != SEOF && c != SEOF_N){
					c = b_getc(sc_buf);
				}
				b_retract(sc_buf);
				continue;
			}
			else
			{
				t.code = ERR_T;
				while (c != '\n'&&c != '\r'&&c != SEOF&&c != SEOF_N){
					c = b_getc(sc_buf);
				}
				b_retract(sc_buf);
				return t;
			}

		case SEOF_N:case SEOF:/*checking for source end of file*/
			t.code = SEOF_T;/*set the token for source end of file*/
			return t;
		case '#':/*check for concatenation*/
			t.code = SCC_OP_T;/*set the concatenation token*/
			return t;

		case '=':/*check for assignment operation*/
			c = b_getc(sc_buf);
			if (c == '='){
				t.code = REL_OP_T;
				t.attribute.rel_op = EQ;
			}
			else{
				b_retract(sc_buf);
				t.code = ASS_OP_T;
			}
			return t;
		case '>':/*check for relational operation*/
			t.code = REL_OP_T;
			t.attribute.rel_op = GT;
			return t;

		case '<':
			c = b_getc(sc_buf);
			if (c == '>'){
				t.code = REL_OP_T;
				t.attribute.rel_op = NE;
				return t;
			}
			t.code = REL_OP_T;
			t.attribute.rel_op = LT;
			b_retract(sc_buf);
			return t;

			/*check for math operations*/
		case '-':
			t.code = ART_OP_T;
			t.attribute.arr_op = MINUS;
			return t;
		case '/':
			t.code = ART_OP_T;
			t.attribute.arr_op = DIV;
			return t;
		case '+':
			t.code = ART_OP_T;
			t.attribute.arr_op = PLUS;
			return t;

		case '*':
			t.code = ART_OP_T;
			t.attribute.arr_op = MULT;
			return t;


		case '.':/*check for logical operation*/
			b_setmark(sc_buf, b_getoffset(sc_buf));
			c = b_getc(sc_buf);

			if (c == 'A' &&b_getc(sc_buf) == 'N'&&b_getc(sc_buf) == 'D'&&b_getc(sc_buf) == '.'){
				t.code = LOG_OP_T;
				t.attribute.log_op = AND;
				return t;
			}
			else if (c == 'O' &&b_getc(sc_buf) == 'R'&&b_getc(sc_buf) == '.'){

				t.code = LOG_OP_T;
				t.attribute.log_op = OR;
				return t;
			}
			/*in case was neither the AND or OR*/
			t.code = ERR_T;
			/*sprintf(t.attribute.err_lex, ".");*/
			b_retract_to_mark(sc_buf);
			return t;
			/*check for paranthesis and braces*/
		case '(':
			t.code = LPR_T;
			return t;
		case')':
			t.code = RPR_T;
			return t;
		case'{':
			t.code = LBR_T;
			return t;
		case '}':
			t.code = RBR_T;
			return t;
			/*check for string*/
		case'"':
			size = b_size(sc_buf);
			b_setmark(sc_buf, b_getoffset(sc_buf));
			do{
				++ch;
				c = b_getc(sc_buf);
			} while (c != '"'&&c != SEOF && c != SEOF_N);
			b_retract_to_mark(sc_buf);

			if (c == '"'){
				c = b_getc(sc_buf);
				while (c != '"'){
					if (c == '\r' || c == '\n'){
						++line;
						c = b_getc(sc_buf);
						if (c != '\n')
							b_retract(sc_buf);
					}
					b_addc(str_LTBL, c);
					c = b_getc(sc_buf);

				}
				b_addc(str_LTBL, '\0');
				t.code = STR_T;
				t.attribute.str_offset = size;
				return t;

			}
			else{
				b_retract(sc_buf);
				if (ch > ERR_LEN){
					int i = 0;
					for (i = 0; i < MAX_STRING; i++){
						c = b_getc(sc_buf);
						t.attribute.err_lex[i] = c;
					}
					for (; i < ERR_LEN; ++i){
						t.attribute.err_lex[i] = '.';
					}
					t.attribute.err_lex[ERR_LEN] = '\0';/*check*/
				}
				while (c != '\0'&&c != UCHAR_MAX){
					c = b_getc(sc_buf);
				}
				b_retract(sc_buf);
				t.code = ERR_T;
				return t;
			}
			/*comma token*/
		case',':
			t.code = COM_T;
			return t;
			/*end of statement token*/
		case';':
			t.code = EOS_T;
			return t;
		}
		break;
	}

	if (isdigit(c) || isalpha(c)){
		b_setmark(sc_buf, b_getoffset(sc_buf) - 1);/*return the mark to the begning of lexeme*/
		state = get_next_state(state, c, &accept);
		while (accept == NOAS){
			c = b_getc(sc_buf);
			state = get_next_state(state, c, &accept);
			
		}
		lexstart = b_mark(sc_buf);
		lex_buf = b_create(b_capacity(sc_buf), INC_FACTOR, 'm');/*temporary buffer*/
		
		if (!lex_buf){
			scerrnum = SEOF_N;/*buffer is empty set the error*/
			t = runTimeError();
			t.attribute.err_lex[ERR_LEN] = SEOF;
			return t;
		}

		if (accept == ASWR)
				b_retract(sc_buf);

		lexend = b_getoffset(sc_buf);
		b_retract_to_mark(sc_buf);
		

		while (b_getoffset(sc_buf)<lexend)
			b_addc(lex_buf, b_getc(sc_buf));
		
		b_addc(lex_buf, '\0');
	
		t = aa_table[state](lex_buf->cb_head);

		b_free(lex_buf);
		return t;
	}
	else{
		t.code = ERR_T;
		/*t.attribute.err_lex[0] = c;*/
		sprintf(t.attribute.err_lex, "%c", c);
		return t;
	}
}


/*
Purpose: Get the next state
Author: Svillen Ranev
Version: 1
Called functions: assert(), printf()
Parameters: int state, char c, int *accept
Return value: next

*/
int get_next_state(int state, char c, int *accept)
{
	int col;
	int next;
	col = char_class(c);
	next = st_table[state][col];
#ifdef DEBUG
	printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
#endif

	assert(next != IS);

#ifdef DEBUG
	if (next == IS){
		printf("Scanner Error: Illegal state:\n");
		printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
		exit(1);
	}
#endif
	*accept = as_table[next];
	return next;
}
/*	Function Name:char_class
*	Purpose: Get the column number in the transition table
*	param: char c
*	version: 1.1.0
*	Author: SeyedAmin SeyedMahmoudian
*	return: int
*/
int char_class(char c)
{
	int val;

	if (isalpha(c)){
		val = 0;
	}
	else if (c == '0'){

		val = 1;
	}
	else if (c >= '1'&& c <= '7'){
		val = 2;
	}
	else if (c == '8' || c == '9'){
		val = 3;
	}
	else if (c == '.'){
		val = 4;
	}
	else if (c == '%'){
		val = 5;
	}
	else{
		val = 6;
	}

	return val;
}
/*	Function Name:aa_func02
*	Purpose: Accepting function for arithmetic variable (AVID) and keywords
*	param: char lexeme[]
*	version: 2.0
*	Author: SeyedAmin SeyedMahmoudian
*	return: Token
*/
Token aa_func02(char lexeme[]){
	Token t; /* Token to be returned. */
	unsigned char data_type;
	int k_index = R_FAIL1; /* Keyword index in tbe keyword table array. Begin with illegal index. */
	int vid_offset = ZERO;
	int length;

	/* This will return -1 if the lexeme is not a keyword. */
	k_index = isKeyword(lexeme);

	/* If k_index index is 0 or above, a keyword is matched. Set the token to KW. */
	if (k_index != R_FAIL1 ) {
		t.code = KW_T;
		t.attribute.kwt_idx = k_index; /* Set the KW attribtue to the keyword index in the keyword table array. */
		
	}
	/* If a keyword is not found, set the token to AVID. */
	else {
		t.code = AVID_T;
		
		/* Copy VID_LEN characters from the AVID lexeme into the token attribute. */
		if (lexeme[0] == 'i' || lexeme[0] == 'o' || lexeme[0] == 'd' || lexeme[0] == 'n') {
			data_type = 'I';
		}
		else {
			data_type = 'F';
		}
		length = strlen(lexeme);
		if (length> VID_LEN)
			lexeme[VID_LEN] = '\0';


		vid_offset = st_install(sym_table, lexeme, data_type, line);
	}

	if (vid_offset == R_FAIL1) {
		printf("\nError: The Symbol Table is full - install failed.\n");
		st_store(sym_table);
		b_free(lex_buf);
		exit(1);
	}

	t.attribute.vid_offset = vid_offset;

	return t;
}
/*	Function Name:aa_func03
*	Purpose: Accepting function for string variable (SVID)
*	param: char lexeme[]
*	version: 2..0
*	Author: SeyedAmin SeyedMahmoudian
*	return: Token
*/
Token aa_func03(char lexeme[]){
	Token t; /* Token to be returned. */
	int vid_offset = 0;

	t.code = SVID_T;

	/* Copy VID_LEN characters from the lexeme into the token attribute. */
	if (strlen(lexeme) <= VID_LEN) {
		t.attribute.vid_offset = st_install(sym_table, lexeme, 'S', line);
	}
	else {
		lexeme[VID_LEN - 1] = '%';
		lexeme[VID_LEN] = '\0';
		vid_offset = st_install(sym_table, lexeme, 'S', line);
	}

	if (vid_offset == -1) {
		printf("\nError: The Symbol Table is full - install failed.\n");
		st_store(sym_table);
		b_free(lex_buf);
		exit(0);
	}

	t.attribute.vid_offset = vid_offset;

	return t;
}
/*	Function Name:aa_func05
*	Purpose:  Accepting function for decimal constant (DIL)
*	param: char lexeme[]
*	version: 1.1.0
*	Author: SeyedAmin SeyedMahmoudian
*	return: Token
*/
Token aa_func05(char lexeme[]){
	Token t;
	int intValue = 0; /* Integer value of the lexeme char. */

	intValue = atoi(lexeme);

	/* If the int value is in range, set the token to INL. Otherwise, return an error token. */
	if ((intValue >= SHRT_MIN && intValue <= SHRT_MAX) || intValue == 0) {
		t.code = INL_T;
		t.attribute.int_value = intValue;
	}
	else {
		t.code = ERR_T;
		strncpy(t.attribute.err_lex, lexeme, ERR_LEN);
		t.attribute.err_lex[ERR_LEN] = '\0';
	}

	return t;
}

/*	Function Name:aa_func08
*	Purpose:  Accepting function for floating point literal (FPL)
*	param: char lexeme[]
*	version: 1.1.0
*	Author: SeyedAmin SeyedMahmoudian
*	return: Token
*/
Token aa_func08(char lexeme[]){
	Token t;
	double floatVal = 0.0;
	floatVal = atof(lexeme);

	if ((floatVal >= FLT_MIN || floatVal <= FLT_MAX) || floatVal == 0){
		t.code = FPL_T;
		t.attribute.flt_value = (float)floatVal;
	}
	else{
		t.code = ERR_T;
		strncpy(t.attribute.err_lex, lexeme, ERR_LEN);
		t.attribute.err_lex[ERR_LEN] = '\0';
	}


	return t;
}
/*	Function Name:aa_func11
*	Purpose:  Accepting function for octal constant (OIL)
*	param: char lexeme[]
*	version: 1.1.0
*	Author: SeyedAmin SeyedMahmoudian
*	return: Token
*/
Token aa_func11(char lexeme[]){
	Token t;
	long longVal = 0;

	longVal = atool(lexeme);

	if ((longVal >= SHRT_MIN && longVal <= SHRT_MAX || longVal == 0)){
		t.code = INL_T;
		t.attribute.int_value = (int)longVal;
	}
	else{
		t.code = ERR_T;
		strncpy(t.attribute.err_lex, lexeme, ERR_LEN);
		t.attribute.err_lex[ERR_LEN] = '\0';
	}

	return t;
}
/*	Function Name:aa_func12
*	Purpose:  Set the error token (ES)
*	param: char lexeme[]
*	version: 1.1.0
*	Author: SeyedAmin SeyedMahmoudian
*	return: Token
*/
Token aa_func12(char lexeme[]){
	Token t;
	t.code = ERR_T;

	strncpy(t.attribute.err_lex, lexeme, ERR_LEN);
	t.attribute.err_lex[ERR_LEN] = '\0';
	return t;
}

long atool(char * lexeme){
	long val;

	if (lexeme == NULL){
		return ZERO;
	}
	val = strtol(lexeme, NULL, 8);
	return val;

}
/*	Function Name:isKeyword
*	Purpose: To check for key words
*	param: char * lexeme
*	version: 1.0.0
*	Author: SeyedAmin SeyedMahmoudian
*	return: Token
*/
int isKeyword(char * lexeme){
	int i = ZERO;
	int kIndex = R_FAIL1 ;
	
	for (i = 0; i < KWT_SIZE; i++){

		if (strcmp(lexeme, kw_table[i]) == ZERO){
			 kIndex=i;
			 break;
		}
	}
	return kIndex;
}
/*	Function Name:runTimeError
*	Purpose: Handle run time errors
*	param: none
*	version: 1.2.0
*	Author: SeyedAmin SeyedMahmoudian
*	return: Token
*/
Token runTimeError(){
	Token t;
	int i;
	char *error;

	t.code = ERR_T;
	scerrnum++;

	error = "RUN TIME ERROR ACCOURD :";

	memset(t.attribute.err_lex, '\0', ERR_LEN + 1);
	for (i = 0; i < ERR_LEN; i++){
		t.attribute.err_lex[i] = error[i];
	}
	return t;
}
