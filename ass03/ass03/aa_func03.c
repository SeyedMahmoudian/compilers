/*	Function Name:aa_func03
*	Purpose: Accepting function for string variable (SVID)
*	@param: char lexeme[]
*	@version: 2..0
*	@author: Seyedamin Seyedmahmoudian
*	@return: Token
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