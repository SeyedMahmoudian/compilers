/*  File name:stable.c
*  Compiler: [MS Visual Studio 2013]
*  Author: SeyedAmin SeyedMahmoudian,040813340
*  Course: CST 8152 – Compilers,Lab Section:11
*  Assignment: Symbol Table Database
*  Date:2016-11-24
*  Professor:Sv.Ranve
*  Purpose: implements and tests the foundation of your lexical analyzer:
*  Function list:
*				-st_create(int)
*				-st_install(STD, char *, char, int)
*				-st_lookup(STD, char*)
*				-st_update_type(STD, int, char)
*				-st_update_value(STD, int, InitialValue)
*				-st_get_type(STD, int)
*				-st_destroy(STD)
*				-st_print(STD)
*				-st_store(STD
*				-static ascending(const void *, const void *)
*				-static descending(const void *, const void *)
*				-static st_setsize(void)
*				-static st_incoffset(void)
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> /* standard input / output */
#include <string.h> /* string functions */
/* project header files */
#include "buffer.h" /*buffer header*/
#include "stable.h" /*scanner header*/

extern STD sym_table; /* global Symbol Table Descriptor */


static int ascending(const void *, const void *);/*Helper function to sort in ascending */
static int descending(const void *, const void *);/*Helper function to sort in descending */
static void st_setsize(void);/*Internal function used by functions that does not have access to the global variable*/
static void st_incoffset(void);/*Increment st_offset by 1, used when function want to increment st_offset if it does not have access to the global variable*/

/*	Function Name:st_create
*	Purpose: This function creates a new (empty) symbol table
*	param: int st_size
*	version: 1.0.0
*	Author: SeyedAmin SeyedMahmoudian
*	return: SymbolTableDescriptor
*/
STD st_create(int st_size){


	STD sym_temp;/*local variable of type STD*/

	sym_temp.pstvr = (STVR *)malloc(sizeof(STVR)*st_size);

	if (!sym_temp.pstvr){
		sym_temp.st_size = INTZERO;
		return sym_temp;
	}
	sym_temp.plsBD = b_create(BUFSIZE, BUFINC, BUFMODE);

	if (!sym_temp.plsBD){
		sym_temp.st_size = INTZERO;
		free(sym_temp.pstvr);
		return sym_temp;
	}
	sym_temp.st_offset = INTZERO;
	sym_temp.st_size = st_size;

	return sym_temp;
}
/*	Function Name:st_install
*	Purpose: This function installs a new entry (VID record) in the symbol table
*	param: STD sym_table, char* lexeme, char type, int line
*	version: 1.0.0
*	Author: SeyedAmin SeyedMahmoudian
*	return: offset integer
*/
int st_install(STD sym_table, char* lexeme, char type, int line){
	if (sym_table.st_size == INTZERO){
		return FAIL_1;
	}
	int offset;
	char flag = INTZERO;
	int i = INTZERO;
	int size = INTZERO;

	offset = st_lookup(sym_table, lexeme);

	if (offset != FAIL_1)
		return offset;

	offset = sym_table.st_offset;

	/* check if the symbol table is full */
	if (offset == sym_table.st_size) return FAIL_1;

	sym_table.pstvr[offset].plex = b_cbhead(sym_table.plsBD) + b_size(sym_table.plsBD);

	/* store lexeme into the symbol table Buffer */
	for (i = INTZERO; i <= strlen(lexeme); ++i) {
		b_addc(sym_table.plsBD, lexeme[i]);
		if (b_rflag(sym_table.plsBD))
			flag = b_rflag(sym_table.plsBD);
	}
	/*
	b_addc(sym_table.plsBD, '\0');
	if (b_rflag(sym_table.plsBD))
	flag = b_rflag(sym_table.plsBD);
	*/

	sym_table.pstvr[offset].o_line = line;
	sym_table.pstvr[offset].status_field = DEFAULT;

	if (type == 'F'){
		sym_table.pstvr[offset].status_field |= FLOAT_M;
		sym_table.pstvr[offset].i_value.fpl_val = FLOATZERO;
	}
	else if (type == 'I'){
		sym_table.pstvr[offset].status_field |= INT_M;
		sym_table.pstvr[offset].i_value.int_val = INTZERO;
	}
	else if (type == 'S'){
		sym_table.pstvr[offset].status_field |= STRING_M;
		sym_table.pstvr[offset].status_field |= UPDATE_M;
		sym_table.pstvr[offset].i_value.int_val = FAIL_1;
	}

	if (flag){
		offset = INTZERO;
		sym_table.pstvr[0].plex = b_cbhead(sym_table.plsBD);


		for (i = 1; i <= sym_table.st_offset; i++){
			size += strlen(sym_table.pstvr[i - 1].plex) + 1;

			sym_table.pstvr[i].plex = size + b_cbhead(sym_table.plsBD);

		}
	}
	st_incoffset();

	return offset;

}
/*	Function Name: st_lookup
*	Purpose: This function searches for a lexeme (variable name) in the symbol table
*	param: STD sym_table, char* lexeme
*	version: 1.0.0
*	Author: SeyedAmin SeyedMahmoudian
*	return: integer offset
*/
int st_lookup(STD sym_table, char*lexeme){
	if (sym_table.st_size == INTZERO){
		return FAIL_2;
	}
	int i = INTZERO;
	for (i = sym_table.st_offset - 1; i >= INTZERO; --i){
		if (strcmp(sym_table.pstvr[i].plex, lexeme) == INTZERO)
			return i;
	}

	return FAIL_1;
}
/*	Function Name: st_update_type
*	Purpose: The function updates the data type indicator
*	param: STD sym_table, int vid_offset, char v_type
*	version: 1.0.0
*	Author: SeyedAmin SeyedMahmoudian
*	return: integer vid_offset
*/
int st_update_type(STD sym_table, int vid_offset, char v_type){
	if (sym_table.st_size == INTZERO || vid_offset <= 0)
		return FAIL_1;

	if ((sym_table.pstvr[vid_offset].status_field & UPDATE_M)){
		return FAIL_1;
	}
	if (v_type == 'F'){
		sym_table.pstvr[vid_offset].status_field |= FLOAT_M;
	}
	else if (v_type == 'I'){
		sym_table.pstvr[vid_offset].status_field |= INT_M;
	}

	sym_table.pstvr[vid_offset].status_field |= UPDATE_M;

	return vid_offset;
}
/*	Function Name: st_update_value
*	Purpose: The function updates the i_value
*	param: STD sym_table, int vid_offset, InitialValue i_value
*	version: 1.0.0
*	Author: SeyedAmin SeyedMahmoudian
*	return: integer vid_offset
*/
int st_update_value(STD sym_table, int vid_offset, InitialValue i_value){
	if (sym_table.st_size == INTZERO || vid_offset <= 0)
		return FAIL_1;

	sym_table.pstvr[vid_offset].i_value = i_value;
	return vid_offset;

}
/*	Function Name: st_get_type
*	Purpose: The function returns the type of the variable specified by vid_offset
*	param: STD sym_table, int vid_offset
*	version: 1.0.0
*	Author: SeyedAmin SeyedMahmoudian
*	return: char base on the type of the variable
*/
char st_get_type(STD sym_table, int vid_offset){
	if (sym_table.st_size == INTZERO || vid_offset <= 0)
		return FAIL_1;

	unsigned short temp;

	temp = sym_table.pstvr[vid_offset].status_field;

	if ((temp&STRING_M) == STRING_M){
		return'S';
	}
	else if ((temp&INT_M) == INT_M){
		return 'I';
	}
	else if ((temp&FLOAT_M) == FLOAT_M){
		return 'F';
	}


	return FAIL_1;
}
/*	Function Name: st_destroy
*	Purpose: Free the dynamic memory occupied by the symbol table
*	param: STD sym_table
*	version: 1.0.0
*	Author: SeyedAmin SeyedMahmoudian
*	return: void
*/
void st_destroy(STD sym_table){

	if (sym_table.plsBD)
		b_free(sym_table.plsBD);

	if (sym_table.pstvr)
		free(sym_table.pstvr);


	st_setsize();

}
/*	Function Name: st_print
*	Purpose: This function prints the contents of the symbol table
*	param: STD sym_table
*	version: 1.0.0
*	Author: SeyedAmin SeyedMahmoudian
*	return: number of line in int
*/
int st_print(STD sym_table){
	if (sym_table.st_size == INTZERO)
		return FAIL_1;
	int i;

	printf("\nSymbol Table");
	printf("\n____________");
	printf("\n\nLine Number Variable Identifier\n");

	for (i = 0; i < sym_table.st_offset; ++i){
		printf("%*d          ", 2, sym_table.pstvr[i].o_line);
		printf("%s", sym_table.pstvr[i].plex);
		printf("\n");
	}
	return i;
}
/*	Function Name: st_setsize
*	Purpose: Internal function that will set the size to 0 for functions withouth the access to the size of the buffer
*	param: void
*	version: 1.0.0
*	Author: SeyedAmin SeyedMahmoudian
*	return: void
*/
static void st_setsize(void){
	sym_table.st_size = INTZERO;
}
/*	Function Name: st_incoffset
*	Purpose: Internal function that will increment the st_offset for function that have no access to the variable
*	param: void
*	version: 1.0.0
*	Author: SeyedAmin SeyedMahmoudian
*	return: void
*/
static void st_incoffset(void){
	++sym_table.st_offset;

}
/*	Function Name: st_store
*	Purpose: This function stores the symbol table into a file named $stable.ste
*	param: sym_table
*	version: 1.0.0
*	Author: SeyedAmin SeyedMahmoudian
*	return: int i , number of the record it has stored
*/
int st_store(STD sym_table){


	FILE *fp = NULL;
	int i;
	if (sym_table.st_size == INTZERO)
		return FAIL_1;
	if ((fp = fopen("$stable.ste", "wt")) == NULL){
		printf("Cannot create file:$stable.ste");
		return FAIL_1;
	}

	fprintf(fp, "%d", sym_table.st_size);

	for (i = 0; i < sym_table.st_offset; i++){
		fprintf(fp, " %hX ", sym_table.pstvr[i].status_field);
		fprintf(fp, " %lu ", strlen(sym_table.pstvr[i].plex));
		fprintf(fp, " %s ", sym_table.pstvr[i].plex);
		fprintf(fp, " %d ", sym_table.pstvr[i].o_line);

		switch (st_get_type(sym_table, i)){
		case 'I':
			fprintf(fp, " %d ", sym_table.pstvr[i].i_value.int_val);
			break;
		case 'F':
			fprintf(fp, " %.2f ", sym_table.pstvr[i].i_value.fpl_val);
			break;
		case 'S':
			fprintf(fp, " %d ", sym_table.pstvr[i].i_value.str_offset);
			break;
		}
	}
	fclose(fp);
	printf("\nSymbol Table stored.\n");
	return i;

}
/*	Function Name: st_sort
*	Purpose: Bonus function that will sort the array of symbol table entries base on the input
*			 A for ascending and D for descending
*	param: STD sym_table, char s_order
*	version: 1.0.0
*	Author: SeyedAmin SeyedMahmoudian
*	return: The return in this function are to show  the success or failure of the sorting but
*			 the actual return will be showing either on display or in file
*/
int st_sort(STD sym_table, char s_order){
	if (sym_table.st_size > INTZERO){
		switch (s_order){
		case 'A':
			qsort(sym_table.pstvr, sym_table.st_offset, sizeof(STVR), ascending);
			return PASS_1;
		case 'D':
			qsort(sym_table.pstvr, sym_table.st_offset, sizeof(STVR), descending);
			return PASS_1;
		}
	}
	return FAIL_1;
}
/*	Function Name: ascendingt
*	Purpose: Helper function to order the function in ascending order
*	param: const void *ptr, const void *ptr2
*	version: 1.0.0
*	Author: SeyedAmin SeyedMahmoudian
*	return: return int
*/
static int ascending(const void *ptr, const void *ptr2){
	return strcmp(((STVR*)ptr)->plex, ((STVR*)ptr2)->plex);
}
/*	Function Name: descending
*	Purpose: Helper function to order the function in descending order
*	param: const void *ptr, const void *ptr2
*	version: 1.0.0
*	Author: SeyedAmin SeyedMahmoudian
*	return: return int
*/
static int descending(const void *ptr, const void *ptr2){
	return strcmp(((STVR*)ptr2)->plex, ((STVR*)ptr)->plex);
}
