/*	Function Name:aa_func05
*	Purpose:  Accepting function for decimal constant (DIL)
*	param: char lexeme[]
*	version: 2.0.0
*	author: Seyedamin Seyedmahmoudian
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