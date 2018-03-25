/*	Function Name:aa_func02
*	Purpose: Accepting function for arithmetic variable (AVID) and keywords
*	param: char lexeme[]
*	version: 2.0
*	author: Seyedamin Seyedmahmoudian
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