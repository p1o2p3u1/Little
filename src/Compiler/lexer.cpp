# include "lexer.h"
char* g_pstrSource;         // 存储要解析的源文件字符串
int g_iCurrentTokenStart;  // 当前Token开始位置
int g_iCurrentTokenEnd;    // 当前Token结束位置
char g_pstrCurrentToken[MAX_TOKEN_SIZE];  // 当前Token的缓冲区
char cDelims[MAX_DELIM_SIZE] = {',', '(', ')', '[', ']', '{', '}', ';' }; // 保存所有已知的分隔符
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
	// 与数组中的每一个元素进行比较来确定是否是一个分隔符
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
			}else if(IsCharIdent(cCurrentChar)){
				// 把它放在检测数字的后面，可以防止出现以数字开头的情况
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
	case LEX_STATE_IDENT:
		// 需要判断这个标识符是否是keywords
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