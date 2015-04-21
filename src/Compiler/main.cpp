# include "lexer.h"

int main(){
	// Print the logo
	InitLexer();
	printf ( "Lexical Analyzer Demo\n" );
	printf ( "\n" );

	// Create a file pointer for the script

	FILE * pSourceFile;

	// Open the script and print an error if it's not found

	if ( ! ( pSourceFile = fopen ( "../Test/test3.txt", "rb" ) ) )
	{
		printf ( "File I/O error.\n" );
		return 0;
	}

	// Allocate a script of the proper size

	fseek ( pSourceFile, 0, SEEK_END );
	int iSourceSize = ftell ( pSourceFile );
	fseek ( pSourceFile, 0, SEEK_SET );
	extern char* g_pstrSource ;
	g_pstrSource = ( char * ) malloc ( iSourceSize + 1 );

	// Load the file in and append a null terminator

	char cCurrChar;
	for ( int iCurrCharIndex = 0; iCurrCharIndex < iSourceSize; ++ iCurrCharIndex )
	{
		// Analyze the current character

		cCurrChar = fgetc ( pSourceFile );
		if ( cCurrChar == 13 )
		{
			// If a two-character line break is found, replace it with a single newline

			fgetc ( pSourceFile );
			-- iSourceSize;
			g_pstrSource [ iCurrCharIndex ] = '\n';
		}
		else
		{
			// Otheriwse use it as-is

			g_pstrSource [ iCurrCharIndex ] = cCurrChar;
		}
	}
	g_pstrSource [ iSourceSize ] = '\0';

	// Close the script

	fclose ( pSourceFile );

	// ---- Tokenize the file and print the results

	// The current token

	Token CurrToken;

	// The token count

	int iTokenCount = 0;

	// String to hold the token type

	char pstrToken [ 128 ];

	// Tokenize the entire source file

	while ( TRUE )
	{
		// Get the next token

		CurrToken = GetNextToken ();

		// Make sure the token stream hasn't ended

		if ( CurrToken == TOKEN_EOF )
			break;

		// Convert the token code to a descriptive string

		switch ( CurrToken )
		{
			// Integer

		case TOKEN_INT:
			strcpy ( pstrToken, "Integer" );
			break;

			// Float

		case TOKEN_FLOAT:
			strcpy ( pstrToken, "Float" );
			break;

			// Identifier

		case TOKEN_IDENT:
			strcpy ( pstrToken, "Identifier" );
			break;

			// Reserved Words

		case TOKEN_KEYWORD_VAR:
			strcpy ( pstrToken, "var" );
			break;

		case TOKEN_KEYWORD_TRUE:
			strcpy ( pstrToken, "true" );
			break;

		case TOKEN_KEYWORD_FALSE:
			strcpy ( pstrToken, "false" );
			break;

		case TOKEN_KEYWORD_IF:
			strcpy ( pstrToken, "if" );
			break;

		case TOKEN_KEYWORD_ELSE:
			strcpy ( pstrToken, "else" );
			break;

		case TOKEN_KEYWORD_BREAK:
			strcpy ( pstrToken, "break" );
			break;

		case TOKEN_KEYWORD_CONTINUE:
			strcpy ( pstrToken, "continue" );
			break;

		case TOKEN_KEYWORD_FOR:
			strcpy ( pstrToken, "for" );
			break;

		case TOKEN_KEYWORD_WHILE:
			strcpy ( pstrToken, "while" );
			break;

		case TOKEN_KEYWORD_FUNC:
			strcpy ( pstrToken, "func" );
			break;

		case TOKEN_KEYWORD_RETURN:
			strcpy ( pstrToken, "return" );
			break;

			// Operators

		case TOKEN_OP:
			sprintf ( pstrToken, "Operator %d", GetCurrentOp () );
			break;

			// Delimiters

		case TOKEN_DELIM_COMMA:
			strcpy ( pstrToken, "Comma" );
			break;

		case TOKEN_DELIM_OPEN_PAREN:
			strcpy ( pstrToken, "Opening Parenthesis" );
			break;

		case TOKEN_DELIM_CLOSE_PAREN:
			strcpy ( pstrToken, "Closing Parenthesis" );
			break;

		case TOKEN_DELIM_OPEN_BRACE:
			strcpy ( pstrToken, "Opening Brace" );
			break;

		case TOKEN_DELIM_CLOSE_BRACE:
			strcpy ( pstrToken, "Closing Brace" );
			break;

		case TOKEN_DELIM_OPEN_CURLY_BRACE:
			strcpy ( pstrToken, "Opening Curly Brace" );
			break;

		case TOKEN_DELIM_CLOSE_CURLY_BRACE:
			strcpy ( pstrToken, "Closing Curly Brace" );
			break;

		case TOKEN_DELIM_SEMICOLON:
			strcpy ( pstrToken, "Semicolon" );
			break;

			// Strings

		case TOKEN_STRING:
			strcpy ( pstrToken, "String" );
			break;
		}

		// Print the token and the lexeme

		printf ( "%d: Token: %s, Lexeme: \"%s\"\n", iTokenCount, pstrToken, GetCurrLexeme () );

		// Increment the token count

		++ iTokenCount;
	}

	// Print the token count

	printf ( "\n" );
	printf ( "\tToken count: %d\n", iTokenCount );
	return 0;
}