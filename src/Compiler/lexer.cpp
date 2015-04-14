# include "lexer.h"
char* g_pstrSource;         // �洢Ҫ������Դ�ļ��ַ���
int g_iCurrentTokenStart;  // ��ǰToken��ʼλ��
int g_iCurrentTokenEnd;    // ��ǰToken����λ��
char g_pstrCurrentToken[MAX_TOKEN_SIZE];  // ��ǰToken�Ļ�����
char cDelims[MAX_DELIM_SIZE] = {',', '(', ')', '[', ']', '{', '}', ';' }; // ����������֪�ķָ���
void InitLexer()
{
	g_iCurrentTokenStart = 0;
	g_iCurrentTokenEnd = 0;
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

void ExitOnInvalidInputError(char c){

}
// �����ж���һ���ַ�
char GetNextChar()
{
	return g_pstrSource[g_iCurrentTokenEnd++];
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
	// ������Ҫ��ʼ�ִ�ѭ��
	while(TRUE)
	{
		cCurrentChar = GetNextChar();
		if (cCurrentChar = '\0')
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
			}else if(cCurrentChar = '.')
			{
				iCurrentLexState = LEX_STATE_FLOAT;
			}else if(IsCharIdent(cCurrentChar)){
				// �������ڼ�����ֵĺ��棬���Է�ֹ���������ֿ�ͷ�����
				iCurrentLexState = LEX_STATE_IDENT;
			}else if(IsCharDelim(cCurrentChar)){
				iCurrentLexState = LEX_STATE_DELIMITER;
			}else
			{
				ExitOnInvalidInputError(cCurrentChar);
			}
			break;
		case LEX_STATE_INT:
			if(IsCharNumeric(cCurrentChar))
			{
				iCurrentLexState = LEX_STATE_INT;
			}else if(IsCharWhiteSpace(cCurrentChar))
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
			}else if(IsCharWhiteSpace(cCurrentChar))
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
			}else if(IsCharWhiteSpace(cCurrentChar)){
				iAddCurrentChar = FALSE;
				iLexemeDone = TRUE;
			}else
			{
				ExitOnInvalidInputError(cCurrentChar);
			}
			break;
		case LEX_STATE_DELIMITER:
			iAddCurrentChar = TRUE;
			iLexemeDone = TRUE;
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
	case LEX_STATE_INT:
		iTokenType = TOKEN_INT;
		break;
	case LEX_STATE_FLOAT:
		iTokenType = TOKEN_FLOAT;
		break;
	case LEX_STATE_IDENT:
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
		else if (_stricmp(g_pstrCurrentToken, "while") == 0)
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
	default:
		iTokenType = TOKEN_EOF;
		break;
	}
	return iTokenType;
}