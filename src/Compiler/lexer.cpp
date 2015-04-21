# include "lexer.h"


int g_iCurrentOpType;								 // ������¼���յ�op����
char* g_pstrSource;									 // �洢Ҫ������Դ�ļ��ַ���
int g_iCurrentTokenStart;							// ��ǰToken��ʼλ��
int g_iCurrentTokenEnd;							// ��ǰToken����λ��
char g_pstrCurrentToken[MAX_TOKEN_SIZE];  // ��ǰToken�Ļ�����
char cDelims[MAX_DELIM_SIZE] = {',', '(', ')', '[', ']', '{', '}', ';' }; // ����������֪�ķָ���
// �����ֻ��һ���ַ�����ʾ��
/*
��������һ���������������ô�����ģ�
1. �����¶�����ַ���`<`����`g_OpChars1`���ҵ�`{'<', 15, 2, 12}`����������֪����Ҫ��ʼ����һ��������ˡ�
2. �����һ���ַ�������״̬�ַ�����ֱ�ӷ���`12`������ʾ`<`��
3. �����һ���ַ�����״̬������ô����`{'<', 15, 2, 12}`����֪����Ҫ����`g_OpChars2`��������`15`��ʼ��`2`���ַ�������������ַ���`=`�����ݱ�����`{ '=', 0, 0, 29 }`������ֱ�ӷ���`29`����ʾ`<=`������������ַ���`<`������ݱ�����`{ '<', 0, 1, 30 }`�����ǻ���Ҫ����`g_OpChars3`��������`0`��ʼ��`1`���ַ���
4. �����һ���ַ�������״̬����ô����`{ '<', 0, 1, 30 }`ֱ�ӷ���`30`����ʾ`<<`��
5. �����һ���ַ�����״̬����ô����`{'=', 0, 0, 33}`���Ϳ���ֱ�ӷ���`33`����ʾ`<<=`��
*/
OpState g_OpChars1[MAX_OP_SIZE] = {
	{'+', 0, 2, 0},
	{'-', 2, 2, 1},
	{'*', 4, 1, 2},
	{'/', 5, 1, 3},
	{'%', 6, 1, 4},
	{'^', 7, 1, 5},
	{'&', 8, 2, 6},
	{'|', 10, 2, 7},
	{'#', 12, 1, 8},
	{'~', 0, 0, 9},
	{'!', 13, 1, 10},
	{'=', 14, 1, 11},
	{'<', 15, 2, 12},
	{'>', 17, 2, 13}
};
// ������������ַ�����ʾ��
OpState g_OpChars2[MAX_OP_SIZE] = {
	{ '=', 0, 0, 14 },  // +=
	{ '+', 0, 0, 15 },  // ++   
	{ '=', 0, 0, 16 },  // -=
	{ '-', 0, 0, 17 },  // --
	{ '=', 0, 0, 18 },  // *=
	{ '=', 0, 0, 19 },  // /=       
	{ '=', 0, 0, 20 },  // %=                      
	{ '=', 0, 0, 21 },  // ^=                      
	{ '=', 0, 0, 22 },  // &=
	{ '&', 0, 0, 23 },  // &&   
	{ '=', 0, 0, 24 },  // |=
	{ '|', 0, 0, 25 },  // ||   
	{ '=', 0, 0, 26 },  // #=                      
	{ '=', 0, 0, 27 },  // ~=                      
	{ '=', 0, 0, 28 },  // !=                      
	{ '=', 0, 0, 29 },  // <=
	{ '<', 0, 1, 30 },  // <<   
	{ '=', 0, 0, 31 },  // >=
	{ '>', 1, 1, 32 }   // >>
};
// ������������ַ�����ʾ��
OpState g_OpChars3[MAX_OP_SIZE] = {
	{'=', 0, 0, 33},   // <<=
	{'=', 0, 0, 34}    // >>=
};

void InitLexer()
{
	g_iCurrentTokenStart = 0;
	g_iCurrentTokenEnd = 0;
	g_iCurrentOpType  = 0;
}

int IsCharWhiteSpace(char c){
	if(c == ' ' || c == '\t' || c == '\n' )  return true;
	else return false;
}

int IsCharNumeric(char c){
	if(c >='0' && c <='9') return true;
	else return false;
}

int IsCharIdent(char c){
	if ((c >='0' && c <='9') ||
		(c >='a' && c <='z') ||
		(c >='A' && c <='Z') ||
		c == '_')
		return TRUE;
	else
		return FALSE;
}
// ��Ҫָ��Ҫ������ĸ�op��
// �������<!���ַǷ�������������!��ʱ���Ӧ����g_OpChars2���ң��ᷢ���Ҳ�����ֱ���׳��ʷ���������
int IsCharOperator(char cChar, int iOpTableIndex)
{
	char cOpChar;
	for(int i=0; i< MAX_OP_SIZE; i++)
	{
		switch(iOpTableIndex)
		{
		case 0:
			cOpChar = g_OpChars1[i].cChar;
			break;
		case 1:
			cOpChar = g_OpChars2[i].cChar;
			break;
		case 2:
			cOpChar = g_OpChars3[i].cChar;
		}
		if(cChar == cOpChar)
			return TRUE;
	}
	return FALSE;
}

int IsCharDelim(char cChar)
{
	// �������е�ÿһ��Ԫ�ؽ��бȽ���ȷ���Ƿ���һ���ָ���
	for(int i=0; i<MAX_DELIM_SIZE; i++)
	{
		if(cChar == cDelims[i])
		{
			return TRUE;
		}
	}
	return FALSE;
}

int GetOpStateIndex ( char cChar, int iCharIndex, int iSubStateIndex, int iSubStateCount )
{
	int iStartStateIndex;
	int iEndStateIndex;
	// �ַ������Ƿ�Ϊ0
	if(iCharIndex == 0){
		iStartStateIndex = 0;
		iEndStateIndex = MAX_OP_SIZE;
	}else{
		iStartStateIndex = iSubStateIndex;
		iEndStateIndex = iStartStateIndex + iSubStateCount;
	}
	// ѭ������ÿ�����ܵĺ��״̬
	for(int iCurrentOpStateIndex = iStartStateIndex; iCurrentOpStateIndex < iEndStateIndex; iCurrentOpStateIndex ++){
		char op;
		switch (iCharIndex)
		{
		case 0:
			op = g_OpChars1[iCurrentOpStateIndex].cChar;
			break;
		case 1:
			op = g_OpChars2[iCurrentOpStateIndex].cChar;
			break;
		case 2:
			op = g_OpChars3[iCurrentOpStateIndex].cChar;
			break;
		}
		if(op == cChar) return iCurrentOpStateIndex;
	}
	return -1;
}

OpState GetOpState(int opTableNumber, int opIndex)
{
	OpState state;
	switch(opTableNumber)
	{
	case 0:
		state = g_OpChars1[opIndex];
		break;
	case 1:
		state = g_OpChars2[opIndex];
		break;
	case 2:
		state = g_OpChars3[opIndex];
		break;
	}
	return state;
}
void ExitOnInvalidInputError(char c){
	printf("Invalid Input: %c", c);
	exit(0);
}
// �����ж���һ���ַ�
char GetNextChar()
{
	return g_pstrSource[g_iCurrentTokenEnd++];
}
char * GetCurrLexeme ()
{
	return g_pstrCurrentToken;
}
int GetCurrentOp(){
	return g_iCurrentOpType;
}
Token GetNextToken(){
	g_iCurrentTokenStart = g_iCurrentTokenEnd;
	if(g_iCurrentTokenStart >= (int) strlen(g_pstrSource))
	{
		return TOKEN_EOF;
	}
	State iCurrentLexState = LEX_STATE_START;
	State iLexemeDone = FALSE;
	char cCurrentChar;
	int iNextLexemeCharIndex = 0;
	int iAddCurrentChar = TRUE;
	int iCurrOpStateIndex = 0; // op state������
	OpState CurrOpState;
	int iCurrentOpTableIndex = 0; // ȡֵ��Χ0~2
	// ������Ҫ��ʼ�ִ�ѭ��
	while(TRUE)
	{
		cCurrentChar = GetNextChar();
		if (cCurrentChar == '\0')
		{
			// �����ļ�ĩβ���˳�
			break;
		}
		iAddCurrentChar = TRUE;
		switch(iCurrentLexState)
		{
		case LEX_STATE_START:
			if(IsCharWhiteSpace(cCurrentChar))
			{
				++ g_iCurrentTokenStart;
				iAddCurrentChar = FALSE;
			}else if(IsCharNumeric(cCurrentChar))
			{
				iCurrentLexState = LEX_STATE_INT;
			}else if(cCurrentChar == '.')
			{
				iCurrentLexState = LEX_STATE_FLOAT;
			}else if(IsCharIdent(cCurrentChar)){
				// �������ڼ�����ֵĺ��棬���Է�ֹ���������ֿ�ͷ�����
				iCurrentLexState = LEX_STATE_IDENT;
			}else if(IsCharDelim(cCurrentChar)){
				iCurrentLexState = LEX_STATE_DELIMITER;
			}else if(cCurrentChar == '"'){
				// ������ַ����Ŀ�ʼ��������ַ�״̬��ǰ���Ų���Ҫ��ӵ�Token������
				iAddCurrentChar = FALSE;
				iCurrentLexState = LEX_STATE_STRING;
			}else if(IsCharOperator(cCurrentChar, 0)){
				// �ڿ�ʼ״̬��ֻ��Ҫ���ҵ�һ����Ϳ����ˡ�
				iCurrentLexState = LEX_STATE_OP;
				// ��g_iOpChars1�еõ�op�ĳ���״̬����
				// �ڿ�ʼ״̬��ֻ��Ҫ����һ��table�Ϳ�����
				iCurrOpStateIndex = GetOpStateIndex(cCurrentChar, 0, 0, 0);
				// ���û�ҵ���������˳�
				if( iCurrOpStateIndex == -1)
					ExitOnInvalidInputError(cCurrentChar);
				CurrOpState = GetOpState(0, iCurrOpStateIndex);
				// �����ƶ�����һ���������
				iCurrentOpTableIndex = 1;
				g_iCurrentOpType = CurrOpState.iOpIndex;
			}else
			{
				ExitOnInvalidInputError(cCurrentChar);
			}
			break;
		case LEX_STATE_INT:
			if(IsCharNumeric(cCurrentChar))
			{
				iCurrentLexState = LEX_STATE_INT;
			}else if(IsCharWhiteSpace(cCurrentChar) || IsCharDelim(cCurrentChar))
			{
				iAddCurrentChar = FALSE;
				iLexemeDone = TRUE;
			}else if(cCurrentChar == '.')
			{
				iCurrentLexState = LEX_STATE_FLOAT;
			}else
			{
				ExitOnInvalidInputError(cCurrentChar);
			}
			break;
		case LEX_STATE_FLOAT:
			if(IsCharNumeric(cCurrentChar))
			{
				iCurrentLexState = LEX_STATE_FLOAT;
			}else if(IsCharWhiteSpace(cCurrentChar) || IsCharDelim(cCurrentChar))
			{
				iAddCurrentChar = FALSE;
				iLexemeDone = TRUE;
			}else
			{
				ExitOnInvalidInputError(cCurrentChar);
			}
			break;
		case LEX_STATE_IDENT:
			if(IsCharIdent(cCurrentChar)){
				iCurrentLexState = LEX_STATE_IDENT;
			}else if(IsCharWhiteSpace(cCurrentChar) || IsCharDelim(cCurrentChar)){
				iAddCurrentChar = FALSE;
				iLexemeDone = TRUE;
			}else
			{
				ExitOnInvalidInputError(cCurrentChar);
			}
			break;
		case LEX_STATE_DELIMITER:
			iAddCurrentChar = FALSE;
			iLexemeDone = TRUE;
			break;
		case LEX_STATE_STRING:
			if (cCurrentChar == '"')
			{
				// �ַ��������ķ��Ų���Ҫ��ӵ�Token������
				iAddCurrentChar = FALSE;
				iCurrentLexState = LEX_STATE_STRING_END;
			}else if (cCurrentChar == '\\')
			{
				// ת���б��ͬ������Ҫ��ӵ�Token������
				iAddCurrentChar = FALSE;
				iCurrentLexState = LEX_STATE_STRING_ESCAPE;
			}
			// �����κ��ַ���Ҫ��ӵ��ַ���������
			else{
				iAddCurrentChar = TRUE;
				iCurrentLexState = LEX_STATE_STRING;
			}
			break;
		case LEX_STATE_STRING_END:
			// �����Ѿ��������ַ���������˫���ţ���ʱ�������ַ���˫���ź�һ���ַ�
			// ����Ӧ����ӵ�Token������������������Token����ʱ���������ǰ�ƶ�һλ
			// �������˫���ź�����ַ�Ҳ���ᶪʧ��
			iAddCurrentChar = FALSE;
			iLexemeDone = TRUE;
			break;
		case LEX_STATE_STRING_ESCAPE:
			// ���������ˣ�������ת��string״̬
			iCurrentLexState = LEX_STATE_STRING;
			break;
		case LEX_STATE_OP:
			// ����Ƿ��к�̵�op״̬
			if(CurrOpState.iSubStateCount == 0)
			{
				iAddCurrentChar = FALSE;
				iLexemeDone = TRUE;
				break;
			}
			if(IsCharOperator(cCurrentChar, iCurrentOpTableIndex)){
				iCurrOpStateIndex = GetOpStateIndex(cCurrentChar, iCurrentOpTableIndex, CurrOpState.iSubStateIndex, CurrOpState.iSubStateCount);
				if(iCurrOpStateIndex == -1){
					ExitOnInvalidInputError(cCurrentChar);
				}
				CurrOpState = GetOpState(iCurrentOpTableIndex, iCurrOpStateIndex);
				iCurrentOpTableIndex += 1;
				g_iCurrentOpType = CurrOpState.iOpIndex;
			}else{
				iAddCurrentChar = FALSE;
				iLexemeDone = TRUE;
			}
			break;
		default:
			break;
		}
		// ���һ��ѭ�������õ����ַ���ӵ�Token������
		if(iAddCurrentChar)
		{
			g_pstrCurrentToken[iNextLexemeCharIndex++] = cCurrentChar;
		}
		if(iLexemeDone)
		{
			break;
		}
	}
	g_pstrCurrentToken[iNextLexemeCharIndex] = '\0';
	-- g_iCurrentTokenEnd; // �������λ����Ҫ����һλ����Ϊ���Ѿ�ָ���˿ո�ĺ�һλ
	Token iTokenType;
	switch(iCurrentLexState)
	{
	case LEX_STATE_OP:
		iTokenType = TOKEN_OP;
		break;
	case LEX_STATE_INT:
		iTokenType = TOKEN_INT;
		break;
	case LEX_STATE_FLOAT:
		iTokenType = TOKEN_FLOAT;
		break;
	case LEX_STATE_IDENT:
		iTokenType = TOKEN_IDENT;
		// ��Ҫ�ж������ʶ���Ƿ���keywords
		if (_stricmp(g_pstrCurrentToken, "var") == 0)
		{
			iTokenType = TOKEN_KEYWORD_VAR;
		} 
		else if (_stricmp(g_pstrCurrentToken, "true") == 0)
		{
			iTokenType = TOKEN_KEYWORD_TRUE;
		}
		else if (_stricmp(g_pstrCurrentToken, "false") == 0)
		{
			iTokenType = TOKEN_KEYWORD_FALSE;
		}
		else if (_stricmp(g_pstrCurrentToken, "for") == 0)
		{
			iTokenType = TOKEN_KEYWORD_FOR;
		}
		else if (_stricmp(g_pstrCurrentToken, "func") == 0)
		{
			iTokenType = TOKEN_KEYWORD_FUNC;
		}
		else if (_stricmp(g_pstrCurrentToken, "break") == 0)
		{
			iTokenType = TOKEN_KEYWORD_BREAK;
		}
		else if (_stricmp(g_pstrCurrentToken, "continue") == 0)
		{
			iTokenType = TOKEN_KEYWORD_CONTINUE;
		}
		else if (_stricmp(g_pstrCurrentToken, "if") == 0)
		{
			iTokenType = TOKEN_KEYWORD_IF;
		}
		else if (_stricmp(g_pstrCurrentToken, "else") == 0)
		{
			iTokenType = TOKEN_KEYWORD_ELSE;
		}
		else if (_stricmp(g_pstrCurrentToken, "return") == 0)
		{
			iTokenType = TOKEN_KEYWORD_RETURN;
		}
		else if(_stricmp(g_pstrCurrentToken, "while") == 0)
		{
			iTokenType = TOKEN_KEYWORD_WHILE;
		}
		break;
	case LEX_STATE_DELIMITER:
		switch(g_pstrCurrentToken[0]){
		case ',':
			iTokenType = TOKEN_DELIM_COMMA;
			break;
		case '(':
			iTokenType = TOKEN_DELIM_OPEN_PAREN;
			break;
		case ')':
			iTokenType = TOKEN_DELIM_CLOSE_PAREN;
			break;
		case '[':
			iTokenType = TOKEN_DELIM_OPEN_BRACE;
			break;
		case ']':
			iTokenType = TOKEN_DELIM_CLOSE_BRACE;
			break;
		case '{':
			iTokenType = TOKEN_DELIM_OPEN_CURLY_BRACE;
			break;
		case '}':
			iTokenType = TOKEN_DELIM_CLOSE_CURLY_BRACE;
			break;
		case ';':
			iTokenType = TOKEN_DELIM_SEMICOLON;
			break;
		case ':':
			iTokenType = TOKEN_DELIM_COLON;
			break;
		default:
			break;
		}
		break;
	case LEX_STATE_STRING_END:
		iTokenType = TOKEN_STRING;
		break;
	default:
		iTokenType = TOKEN_EOF;
		break;
	}
	return iTokenType;
}