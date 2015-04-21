# include "lexer.h"


int g_iCurrentOpType;								 // 用来记录最终的op类型
char* g_pstrSource;									 // 存储要解析的源文件字符串
int g_iCurrentTokenStart;							// 当前Token开始位置
int g_iCurrentTokenEnd;							// 当前Token结束位置
char g_pstrCurrentToken[MAX_TOKEN_SIZE];  // 当前Token的缓冲区
char cDelims[MAX_DELIM_SIZE] = {',', '(', ')', '[', ']', '{', '}', ';' }; // 保存所有已知的分隔符
// 运算符只用一个字符来表示的
/*
现在来看一下这三个表格是怎么工作的：
1. 假设新读入的字符是`<`，从`g_OpChars1`中找到`{'<', 15, 2, 12}`，现在我们知道需要开始分析一个运算符了。
2. 如果下一个字符不是子状态字符，则直接返回`12`，即表示`<`。
3. 如果下一个字符是子状态符，那么根据`{'<', 15, 2, 12}`我们知道需要查找`g_OpChars2`中索引从`15`开始的`2`个字符。如果读到的字符是`=`，根据表格里的`{ '=', 0, 0, 29 }`，可以直接返回`29`，表示`<=`。如果读到的字符是`<`，则根据表格里的`{ '<', 0, 1, 30 }`，我们还需要查找`g_OpChars3`中索引从`0`开始的`1`个字符。
4. 如果下一个字符不是子状态，那么根据`{ '<', 0, 1, 30 }`直接返回`30`，表示`<<`。
5. 如果下一个字符是子状态，那么根据`{'=', 0, 0, 33}`，就可以直接返回`33`，表示`<<=`。
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
// 运算符用两个字符来表示的
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
// 运算符用三个字符来表示的
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
// 需要指定要查的是哪个op表。
// 比如对于<!这种非法操作符，查找!的时候就应该在g_OpChars2中找，会发现找不到而直接抛出词法解析错误。
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

int GetOpStateIndex ( char cChar, int iCharIndex, int iSubStateIndex, int iSubStateCount )
{
	int iStartStateIndex;
	int iEndStateIndex;
	// 字符索引是否为0
	if(iCharIndex == 0){
		iStartStateIndex = 0;
		iEndStateIndex = MAX_OP_SIZE;
	}else{
		iStartStateIndex = iSubStateIndex;
		iEndStateIndex = iStartStateIndex + iSubStateCount;
	}
	// 循环查找每个可能的后继状态
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
// 从流中读入一个字符
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
	int iCurrOpStateIndex = 0; // op state的索引
	OpState CurrOpState;
	int iCurrentOpTableIndex = 0; // 取值范围0~2
	// 下面需要开始分词循环
	while(TRUE)
	{
		cCurrentChar = GetNextChar();
		if (cCurrentChar == '\0')
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
			}else if(cCurrentChar == '.')
			{
				iCurrentLexState = LEX_STATE_FLOAT;
			}else if(IsCharIdent(cCurrentChar)){
				// 把它放在检测数字的后面，可以防止出现以数字开头的情况
				iCurrentLexState = LEX_STATE_IDENT;
			}else if(IsCharDelim(cCurrentChar)){
				iCurrentLexState = LEX_STATE_DELIMITER;
			}else if(cCurrentChar == '"'){
				// 如果是字符串的开始，则进入字符状态，前引号不需要添加到Token缓冲区
				iAddCurrentChar = FALSE;
				iCurrentLexState = LEX_STATE_STRING;
			}else if(IsCharOperator(cCurrentChar, 0)){
				// 在开始状态下只需要查找第一个表就可以了。
				iCurrentLexState = LEX_STATE_OP;
				// 从g_iOpChars1中得到op的初试状态索引
				// 在开始状态下只需要读第一个table就可以了
				iCurrOpStateIndex = GetOpStateIndex(cCurrentChar, 0, 0, 0);
				// 如果没找到，则出错退出
				if( iCurrOpStateIndex == -1)
					ExitOnInvalidInputError(cCurrentChar);
				CurrOpState = GetOpState(0, iCurrOpStateIndex);
				// 尝试移动到下一个运算符。
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
				// 字符串结束的符号不需要添加到Token缓冲区
				iAddCurrentChar = FALSE;
				iCurrentLexState = LEX_STATE_STRING_END;
			}else if (cCurrentChar == '\\')
			{
				// 转义的斜线同样不需要添加到Token缓冲区
				iAddCurrentChar = FALSE;
				iCurrentLexState = LEX_STATE_STRING_ESCAPE;
			}
			// 其他任何字符都要添加到字符缓冲区。
			else{
				iAddCurrentChar = TRUE;
				iCurrentLexState = LEX_STATE_STRING;
			}
			break;
		case LEX_STATE_STRING_END:
			// 我们已经跳过了字符串结束的双引号，此时读到的字符是双引号后一个字符
			// 它不应该添加到Token缓冲区，好在我们在Token结束时会把索引往前移动一位
			// 所以这个双引号后面的字符也不会丢失。
			iAddCurrentChar = FALSE;
			iLexemeDone = TRUE;
			break;
		case LEX_STATE_STRING_ESCAPE:
			// 我们跳过了，重新跳转回string状态
			iCurrentLexState = LEX_STATE_STRING;
			break;
		case LEX_STATE_OP:
			// 检查是否有后继的op状态
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