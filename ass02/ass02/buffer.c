/*  File name:buffer.c
*  Compiler: [MS Visual Studio 2013]
*  Author: SeyedAmin SeyedMahmoudian,040813340
*  Course: CST 8152 – Compilers,Lab Section:11
*  Assignment:Buffer
*  Date:
*  Professor:Sv.Ranve
*  Purpose: to create a Buffer
*  Function list: b_create(),b_addc(),b_reset(),b_free(),b_isfull(),b_size()
*                 b_capacity(),b_setmark(),b_mark(),b_mode(),b_incfactor(),
*				  b_load(),b_isempty(),b_eob(),b_getc(),b_print(),b_pack(),
*                 b_rflag(),b_retract(),b_retract_to_mark(),b_getoffset(),
*                 b_cbhead()
*/

#include "buffer.h"

/*WARNING : single line comment warning from sal.h this is a
library function I could have disabled the warning but worried it might
violet assignment rules */

/*	Function Name:b_create
*	Purpose: Create a Buffer at the begning of the program using calloc and check for options
*	param: short init_capacity, char inc_factor, char o_mod
*	version: 1.1.0
*	author: SeyedAmin SeyedMahmoudian,040813340
*	date:
*	return: Pointer to the Buffer
*/
Buffer *b_create(short init_capacity, char inc_factor, char o_mode) {
	if (inc_factor == NULL && o_mode == NULL && o_mode != 'f' && o_mode != 'm' && o_mode != 'a'){/*WARNING CONVERSION */
		printf("WRONG MODE PROGRAM EXIT");
		return R_FAIL1;/*WARNING HENCE RETURNING DIFFERENT TYPE FROM THE BUFFER*/
	}
	Buffer* buffer = NULL;
	char *tempHead = (char*)malloc(init_capacity);
	buffer = (Buffer*)calloc(1, sizeof(Buffer));

	if (tempHead == NULL){
		free(buffer);
	}
	else{
		buffer->cb_head = tempHead;
	}
	if (buffer == NULL){
		return NULL;
	}

	if (o_mode == 'f' || inc_factor == ZERO) {
		buffer->mode = ZERO;
		buffer->inc_factor = ZERO;
	}
	else{
		if (o_mode == 'f') {
			buffer->mode = ZERO;
			buffer->inc_factor = ZERO;
		}
		else{
			if (o_mode == 'a' && inc_factor >= SET_R_FLAG) {
				buffer->mode = SET_R_FLAG;
				buffer->inc_factor = inc_factor;
			}
			else{
				if (o_mode == 'm' && inc_factor >= SET_R_FLAG && inc_factor <= MAX100) {

					buffer->mode = R_FAIL1;
					buffer->inc_factor = inc_factor;
				}
				else{

					free(buffer->cb_head);
					free(buffer);
					return NULL;
				}
			}
		}
	}


	buffer->capacity = init_capacity;

	return buffer;
}

/*	Function Name:b_addc
*	Purpose: Add all the character of the file into the character array
*	param: pBuffer const pBD, char symbol
*	version: 1.0.0
*	author: SeyedAmin SeyedMahmoudian,040813340
*	date: 29 September 2016
*	return: Pointer to the Buffer
*/
pBuffer b_addc(pBuffer const pBD, char symbol) {
	if (pBD == NULL)
	{
		return NULL;
	}
	pBD->r_flag = ZERO;
	short newCap = ZERO;
	short availSpace = ZERO;
	float newInc = ZERO;
	char *temp;

	if (b_isfull(pBD)){
		switch (pBD->mode){
		case 0:
			return NULL;
			break;
		case 1:
			if (pBD->inc_factor > ZERO){
				newCap = pBD->capacity + pBD->inc_factor;

				if (newCap<0 || newCap >SHRT_MAX){
					return NULL;
				}
				else{
					break;
				}
			}
			else{
				return NULL;
			}
			break;
		case -1:
			if (pBD->capacity < SHRT_MAX) {

				availSpace = SHRT_MAX - pBD->capacity;
				newInc = (float)availSpace * pBD->inc_factor / 100;
				if (newInc < SHRT_MAX){

					if ((pBD->capacity + newInc)>SHRT_MAX){
						newCap = SHRT_MAX;
					}
					else{
						if ((short)newInc == ZERO){
							newCap = pBD->capacity + SET_R_FLAG;
						}
						else{
							newCap = pBD->capacity + (short)newInc;
						}
					}
				}
				else{
					return NULL;
				}
			}
			else{
				return NULL;
			}
			break;

		default:
			return NULL;
		}

		temp = (char *)realloc(pBD->cb_head, newCap);

		if (temp == NULL){
			return NULL;
		}
		else{
			if (pBD->cb_head != temp){
				pBD->r_flag = SET_R_FLAG;
			}
			pBD->cb_head = temp;
			pBD->cb_head[pBD->addc_offset] = symbol;
			++pBD->addc_offset;
			pBD->capacity = newCap;
			return pBD;
		}

	}
	else{//if buffer is empty
		pBD->cb_head[pBD->addc_offset] = symbol;
		++pBD->addc_offset;
		return pBD;
	}

}

/*	Function Name:b_reset
*	Purpose: Reset all the variable to zero
*	param: Buffer *const pBD
*	version: 1.0.0
*	Author : SeyedAmin SeyedMahmoudian - 040813304
*	date: 29 September 2016
*	return: Integer
*/

int b_reset(Buffer *const pBD) {
	if (pBD != NULL){
		if (pBD->cb_head != NULL){
			pBD->addc_offset = ZERO;
			pBD->getc_offset = ZERO;
			pBD->mark_offset = ZERO;
		}
		else{
			return R_FAIL1;
		}

	}
	else{
		return R_FAIL1;
	}
	return R_FAIL1;
}
/*	Function Name: b_free
*	Purpose: Free pointer
*	param: Buffer *const pBD
*	version: 1.0.0
*	Author : SeyedAmin SeyedMahmoudian - 040813304
*	date: 29 September 2016
*	return: void
*/
void b_free(Buffer *const pBD) {

	if (pBD != NULL)
	{
		if (pBD->cb_head != NULL){
			free(pBD->cb_head);
		}

		free(pBD);
	}
}
/*	Function Name: b_isfull
*	Purpose: check to see if the buffer is full or not
*	param: Buffer *const pBD
*	version: 1.0.0
*	Author : SeyedAmin SeyedMahmoudian - 040813304
*	date: 29 September 2016
*	return: Integer
*/
int b_isfull(Buffer *const pBD) {

	if (pBD == NULL){
		return R_FAIL1;
	}
	if (pBD->addc_offset == pBD->capacity)
	{
		return SET_R_FLAG;
	}
	else{
		return ZERO;
	}
}

/*	Function Name: b_size
*	Purpose: give the size of the buffer
*	param: Buffer *const pBD
*	version: 1.0.0
*	Author : SeyedAmin SeyedMahmoudian - 040813304
*	date: 29 September 2016
*	return: Short
*/
short b_size(Buffer *const pBD) {

	if (pBD == NULL)
	{
		return R_FAIL1;
	}
	else{

		return pBD->addc_offset;
	}

}
/*	Function Name:b_capacity
*	Purpose: Return the current buffer capacity
*	param: Buffer *const pBD
*	version: 1.0.0
*	Author : SeyedAmin SeyedMahmoudian - 040813304
*	date: 29 September 2016
*	return: Short
*/
short b_capacity(Buffer *const pBD) {

	if (pBD == NULL)
	{
		return R_FAIL1;
	}
	else{

		return pBD->capacity;
	}

}
/*	Function Name:b_setmark
*	Purpose:
*	param: Buffer *const pBD , short mark
*	version: 1.0.0
*	Author : SeyedAmin SeyedMahmoudian - 040813304
*	date: 29 September 2016
*	return: Short
*/
short b_setmark(Buffer *const pBD, short mark) {

	if (pBD != NULL && mark >= 0 && mark <= pBD->capacity) {
		pBD->mark_offset = mark;
		return mark;
	}
	else{

		return R_FAIL1;
	}
}
/*	Function Name:b_mark
*	Purpose:
*	param: Buffer *const pBD
*	version: 1.0.0
*	Author : SeyedAmin SeyedMahmoudian - 040813304
*	date: 29 September 2016
*	return: Short
*/
short b_mark(Buffer*const pBD) {

	if (pBD == NULL) {
		return R_FAIL1;
	}
	else{
		return  pBD->mark_offset;
	}

}
/*	Function Name:b_mode
*	Purpose:
*	param: Buffer *const pBD
*	version: 1.0.0
*	Author : SeyedAmin SeyedMahmoudian - 040813304
*	date: 29 September 2016
*	return: Integer
*/
int b_mode(Buffer*const pBD) {

	if (pBD == NULL) {
		return R_FAIL1;

	}
	else{

		return pBD->mode;
	}
}


/*	Function Name:b_incfactor
*	Purpose:
*	param: Buffer *const pBD
*	version: 1.0.0
*	Author : SeyedAmin SeyedMahmoudian - 040813304
*	date: 29 September 2016
*	return: size_t
*/
size_t b_incfactor(Buffer *const pBD) {

	if (pBD == NULL) {
		return MAX256;
	}
	else{

		return pBD->inc_factor;
	}
}
/*	Function Name:b_load
*	Purpose:
*	param: FILE*const fi , Buffer *const pBD
*	version: 1.0.0
*	Author : SeyedAmin SeyedMahmoudian - 040813304
*	date: 29 September 2016
*	return: Integer
*/
int b_load(FILE*const fi, Buffer *const pBD) {
	int counter = ZERO;/*counter for how many character have been added */
	char ch;/*hold the currenct character from the file */
	Buffer *temp;
	if (fi != NULL && pBD != NULL){
		for (;;){
			ch = fgetc(fi);
			if (feof(fi)){
				return counter;
			}
			temp = b_addc(pBD, ch);

			if (temp == NULL){
				return LOAD_FAIL;
			}
			else{
				counter++;
			}
		}
		return counter;
	}
	else{
		return R_FAIL1;
	}
}
/*	Function Name:b_isempty
*	Purpose:
*	param: Buffer *const pBD
*	version: 1.0.0
*	Author : SeyedAmin SeyedMahmoudian - 040813304
*	date: 29 September 2016
*	return: Integer
*/

int b_isempty(Buffer * const pBD){

	if (pBD == NULL){
		return R_FAIL1;
	}
	if (pBD->addc_offset == ZERO){
		return  SET_R_FLAG;
	}

	return ZERO;


}
/*	Function Name:b_eob
*	Purpose:
*	param: Buffer *const pBD
*	version: 1.0.0
*	Author : SeyedAmin SeyedMahmoudian - 040813304
*	date: 29 September 2016
*	return: Integer
*/
int b_eob(Buffer *const pBD) {
	if (pBD == NULL)
	{
		return R_FAIL1;
	}
	return pBD->eob;

}
/*	Function Name:b_getc
*	Purpose:
*	param: Buffer *const pBD
*	version: 1.0.0
*	Author : SeyedAmin SeyedMahmoudian - 040813304
*	date: 29 September 2016
*	return: Character
*/
char b_getc(Buffer *const pBD) {
	char ch;
	if (pBD == NULL){
		return LOAD_FAIL;
	}
	else{
		if (pBD->getc_offset == pBD->addc_offset){
			pBD->eob = SET_R_FLAG;
			return R_FAIL1;
		}
		else{
			pBD->eob = 0;
			ch = pBD->cb_head[pBD->getc_offset];
			++pBD->getc_offset;
		}
	}

	return ch;
}
/*	Function Name:b_print
*	Purpose:
*	param: Buffer *const pBD
*	version: 1.0.0
*	Author : SeyedAmin SeyedMahmoudian - 040813304
*	date: 29 September 2016
*	return: Integer
*/
int b_print(Buffer *const pBD) {

	if (pBD == NULL){
		return R_FAIL1;
	}
	short tempVariable = ZERO;
	int numChar = ZERO;
	char ch = ZERO;

	if (b_isempty(pBD)){
		printf("The buffer is empty.\n");
		return ZERO;
	}
	else{
		tempVariable = b_getoffset(pBD);
		pBD->getc_offset = 0;

		for (;;){

			ch = b_getc(pBD);
			if (b_eob(pBD)){
				break;
			}

			printf("%c", ch);

			numChar++;
		}
	}
	printf("\n");
	pBD->getc_offset = tempVariable;

	return numChar;

}
/*	Function Name:b_pack
*	Purpose:
*	param: Buffer *const pBD
*	version: 1.0.0
*	Author : SeyedAmin SeyedMahmoudian - 040813304
*	date: 29 September 2016
*	return: Buffer
*/
Buffer *b_pack(Buffer *const pBD) {

	if (pBD == NULL){
		return NULL;
	}
	short newCap = 0;
	char *temp;

	pBD->r_flag = 0;
	newCap = pBD->addc_offset + 1;

	if (newCap <SHRT_MAX && newCap>0){
		/*use realloc to extend the memory of the character array with new capacity*/
		temp = (char*)realloc(pBD->cb_head, newCap);

		if (temp == NULL){
			return NULL;
		}
		else{
			/*in case the memory location has been moved set the r_flag*/
			if (temp != pBD->cb_head){
				pBD->r_flag = SET_R_FLAG;
			}
			pBD->cb_head = temp;
			pBD->capacity = newCap;
		}
	}

	return pBD;
}
/*	Function Name:b_rflag
*	Purpose:
*	param: Buffer *const pBD
*	version: 1.0.0
*	Author : SeyedAmin SeyedMahmoudian - 040813304
*	date: 29 September 2016
*	return: Character
*/
char b_rflag(Buffer * const pBD) {

	if ((pBD) == NULL)
	{
		return R_FAIL1;
	}

	return pBD->r_flag;

}
/*	Function Name:b_retract
*	Purpose:
*	param:Buffer *const pBD
*	version: 1.0.0
*	Author : SeyedAmin SeyedMahmoudian - 040813304
*	date: 29 September 2016
*	return: Short
*/
short b_retract(Buffer *const pBD) {


	if (pBD == NULL || pBD->getc_offset <= 0)
	{
		return R_FAIL1;
	}
	if (pBD->getc_offset > 0){
		--pBD->getc_offset;
		return ZERO;
	}
	return R_FAIL1;

}
/*	Function Name: b_retract_to_mark
*	Purpose:
*	param:Buffer *const pBD
*	version: 1.0.0
*	Author : SeyedAmin SeyedMahmoudian - 040813304
*	date: 29 September 2016
*	return: Short
*/
short b_retract_to_mark(Buffer *const pBD) {

	if (pBD == NULL)
	{
		return R_FAIL1;
	}
	else
		pBD->getc_offset = pBD->mark_offset;

	return pBD->getc_offset;

}
/*	Function Name: b_getoffset
*	Purpose:
*	param:Buffer *const pBD
*	version: 1.0.0
*	Author : SeyedAmin SeyedMahmoudian - 040813304
*	date: 29 September 2016
*	return: Short
*/
short b_getoffset(Buffer *const pBD) {

	if (pBD == NULL)
	{
		return R_FAIL1;
	}

	return pBD->getc_offset;

}
/*	Function Name: b_cbhead
*	Purpose:
*	param:Buffer *const pBD
*	version: 1.0.0
*	Author : SeyedAmin SeyedMahmoudian - 040813304
*	date: 29 September 2016
*	return: Character pointer
*/
char *b_cbhead(Buffer *const pBD) {

	if (pBD == NULL)
	{
		return NULL;
	}

	return pBD->cb_head;
}
