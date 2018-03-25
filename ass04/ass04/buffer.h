/* REPLACE the header below with your file header (see CST8152_ASSAMG.pdf for details).
* File Name: buffer.h
* Version: 1.16.2
* Author: S^R
* Date: 6 September 2016
* Preprocessor directives, type declarations and prototypes necessary for buffer implementation
* as required for CST8152, Assignment #1, Fall 2016.
* The file is not completed.
* You must add your function declarations (prototypes).
* You must also add your constant definitions and macros,if any.
*/
#ifndef BUFFER_H_
#define BUFFER_H_

/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */
#include <stdlib.h>
/* constant definitions */
/* You may add your own constant definitions here */
#define R_FAIL1 -1         /* fail return value */
#define R_FAIL2 -2         /* fail return value */
#define LOAD_FAIL -2       /* load fail error */
#define SET_R_FLAG 1       /* realloc flag set value */

#define ZERO 0			   /* Value to set variable to zero */
#define MAX256 256	       /*value to check for maximum 255*/
#define MAX100 100         /*value to check for maximum100*/

/* user data type declarations */
typedef struct BufferDescriptor {
	char *cb_head;   /* pointer to the beginning of character array (character buffer) */
	short capacity;    /* current dynamic memory size (in bytes) allocated to character buffer */
	short addc_offset;  /* the offset (in chars) to the add-character location */
	short getc_offset;  /* the offset (in chars) to the get-character location */
	short mark_offset; /* the offset (in chars) to the mark location */
	char  inc_factor; /* character array increment factor */
	char  r_flag;     /* reallocation flag */
	char  mode;       /* operational mode indicator*/
	int   eob;       /* end-of-buffer flag */
} Buffer, *pBuffer;
/*typedef Buffer *pBuffer;*/

/* function declarations */
/*
Place your function declarations here.
Do not include the function header comments here.
Place them in the buffer.c file
*/

Buffer * b_create(short, char, char);
pBuffer b_addc(pBuffer const, char);
int b_reset(Buffer * const);
void b_free(Buffer * const);
int b_isfull(Buffer * const);
short b_size(Buffer * const);
short b_capacity(Buffer * const);
short b_setmark(Buffer * const, short);
short b_mark(Buffer * const);
int b_mode(Buffer *const);
size_t b_incfactor(Buffer *const);
int b_load(FILE * const, Buffer *const);
int b_isempty(Buffer *const);
int b_eob(Buffer *const);
char b_getc(Buffer *const);
int b_print(Buffer *const);
Buffer *b_pack(Buffer *const);
char b_rflag(Buffer * const);
short b_retract(Buffer *const);
short b_retract_to_mark(Buffer*const);
short b_getoffset(Buffer*const);
char *b_cbhead(Buffer *const);


#endif//DO NOT DELETE THE LINES 82 AND 83 

