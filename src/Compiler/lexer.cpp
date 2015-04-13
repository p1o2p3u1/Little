# include "lexer.h"
char* g_pstrSource;         // �洢Ҫ������Դ�ļ��ַ���
int g_iCurrentTokenStart;  // ��ǰToken��ʼλ��
int g_iCurrentTokenEnd;    // ��ǰToken����λ��
char g_pstrCurrentToken[MAX_TOKEN_SIZE];  // ��ǰToken�Ļ�����

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
	default:
		iTokenType = TOKEN_EOF;
		break;
	}
	return iTokenType;
}