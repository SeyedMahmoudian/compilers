/*  File name:table.h
*  Compiler: [MS Visual Studio 2013]
*  Author: Seyedamin Seyedmahmoudian,040813340
*  Course: CST 8152 – Compilers,
*  Lab Section:11
*  Assignment: The Scanner
*  Date: 2016-10-31
*  Professor:Sv.Ranve
*  Purpose: Transition table , Accepting State functions and other defines required for scanner to work
*/

#ifndef  TABLE_H_
#define  TABLE_H_ 

#ifndef BUFFER_H_
#include "buffer.h"
#endif

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*   Source end-of-file (SEOF) sentinel symbol
*    '\0' or only one of the folowing constants: 255, 0xFF , EOF
*/
#define SEOF '\0' /*End of file signals*/
#define SEOF_N 255 /*End of file signals*/
#define INC_FACTOR 15 /* Increment factor for buffer */
#define TWO_INT_MAX 32767 /* 2-byte int max in c */
#define IS -1    /* Inavalid state */
#define ES 12 /* Error state */
#define MAX_STRING 17 /*in case the string is very long just get the first 17 character*/


/* State transition table definition */
/**/

#define TABLE_COLUMNS 7
int  st_table[][TABLE_COLUMNS] = {
	/* State 0 */{ 1, 6, 4, 4, IS, IS, IS },
	/* State 1 */{ 1, 1, 1, 1, 2, 3, 2 },
	/* State 2 */{ IS, IS, IS, IS, IS, IS, IS },
	/* State 3 */{ IS, IS, IS, IS, IS, IS, IS },
	/* State 4 */{ ES, 4, 4, 4, 7, 5, 5 },
	/* State 5 */{ IS, IS, IS, IS, IS, IS, IS },
	/* State 6 */{ ES, 12, 9, ES, 7, ES, 5 },
	/* State 7 */{ 8, 7, 7, 7, 8, 8, 8 },
	/* State 8 */{ IS, IS, IS, IS, IS, IS, IS },
	/* State 9 */{ ES, 9, 9, ES, ES, ES, 11 },
	/*State 10*/{ 11, ES, ES, ES, 11, 11, 11 },
	/* State 11 */{ IS, IS, IS, IS, IS, IS, IS },
	/* State 12 */{ IS, IS, IS, IS, IS, IS, IS }
};

/* Accepting state table definition */
#define NOAS 0 /*not accepting state*/
#define ASWR 1/*accepting state with retract*/
#define ASNR 2 /*accepting state with NO retract*/

int as_table[] = { NOAS, NOAS, ASWR,
ASNR, NOAS, ASWR,
NOAS, NOAS, ASWR,
NOAS, NOAS, ASWR,
ASNR };/*last 2 state are IS*/

/* Accepting action function declarations */

Token aa_func02(char *lexeme);
Token aa_func03(char *lexeme);
Token aa_func05(char *lexeme);
Token aa_func08(char *lexeme);
Token aa_func11(char *lexeme);
Token aa_func12(char *lexeme);

/* defining a new type: pointer to function (of one char * argument)
returning Token
*/

typedef Token(*PTR_AAF)(char *lexeme);


/* Accepting function (action) callback table (array) definition */
/* If you do not want to use the typedef, the equvalent declaration is:
* Token (*aa_table[])(char lexeme[]) = {
*/

PTR_AAF aa_table[] = { NULL, NULL, aa_func02,
aa_func03, NULL, aa_func05,
NULL, NULL, aa_func08,
NULL, NULL, aa_func11,
aa_func12 };

/* Keyword lookup table (.AND. and .OR. are not keywords) */

#define KWT_SIZE  8

char * kw_table[] = {
	"ELSE",
	"IF",
	"INPUT",
	"OUTPUT",
	"PLATYPUS",
	"REPEAT",
	"THEN",
	"USING"
};

#endif
