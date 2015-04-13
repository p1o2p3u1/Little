# include "lexer.h"
char* g_pstrSource;         // 存储要解析的源文件字符串
int g_iCurrentTokenStart;  // 当前Token开始位置
int g_iCurrentTokenEnd;    // 当前Token结束位置
char g_pstrCurrentToken[MAX_TOKEN_SIZE];  // 当前Token的缓冲区

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
// 从流中读入一个字符
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
	// 下面需要开始分词循环
	while(TRUE)
	{
		cCurrentChar = GetNextChar();
		if (cCurrentChar = '\0')
		{
			// 读到文件末尾，退出
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
		// 完成一轮循环，将得到到字符添加到Token缓冲区
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
	-- g_iCurrentTokenEnd; // 这儿索引位置需要回退一位，因为它已经指向了空格的后一位
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