# include <string.h>
# include <stdio.h>
# include <stdlib.h>
//  定义状态
#define LEX_STATE_START									0  // 开始状态
#define LEX_STATE_INT											1  // 整形状态
#define LEX_STATE_FLOAT									2  // 浮点型状态
#define LEX_STATE_IDENT									5  // 标识符状态
#define LEX_STATE_OP											6  // 表示操作符状态
#define LEX_STATE_DELIMITER							7  // 分隔符状态
#define LEX_STATE_STRING									8  // 字符串状态
#define LEX_STATE_STRING_ESCAPE					9	// 字符串中的转义字符状态
#define LEX_STATE_STRING_END							10 // 字符串结束状态
// 定义Token类型
#define TOKEN_EOF												0
#define TOKEN_INT												1
#define TOKEN_FLOAT											2
#define TOKEN_IDENT											3 // 标识符
//--------------------------------------------------------------------------------------
#define TOKEN_KEYWORD_VAR							4 // 以下是关键字
#define TOKEN_KEYWORD_TRUE						5
#define TOKEN_KEYWORD_FALSE						6
#define TOKEN_KEYWORD_IF								7
#define TOKEN_KEYWORD_ELSE							8
#define TOKEN_KEYWORD_BREAK						9
#define TOKEN_KEYWORD_CONTINUE				10
#define TOKEN_KEYWORD_FOR							11
#define TOKEN_KEYWORD_WHILE						12
#define TOKEN_KEYWORD_FUNC						13
#define TOKEN_KEYWORD_RETURN					14
//-------------------------------------------------------------------------------------------
#define TOKEN_OP												15	//Token是一个操作符
#define TOKEN_DELIM_COLON							16    // :
#define TOKEN_DELIM_COMMA							17    // ,    
#define TOKEN_DELIM_OPEN_PAREN					18	// (
#define TOKEN_DELIM_CLOSE_PAREN				19	// )
#define TOKEN_DELIM_OPEN_BRACE					20   // [
#define TOKEN_DELIM_CLOSE_BRACE				21	// ]
#define TOKEN_DELIM_OPEN_CURLY_BRACE	22	// {
#define TOKEN_DELIM_CLOSE_CURLY_BRACE	23	// }
#define TOKEN_DELIM_SEMICOLON					24    // ;
#define TOKEN_STRING										25   // Token是字符串
//----------------------------------------------------------------------
#define OP_TYPE_ADD											0           // +
#define OP_TYPE_SUB											1           // -
#define OP_TYPE_MUL											2           // *
#define OP_TYPE_DIV											3           // /
#define OP_TYPE_MOD										4           // %
#define OP_TYPE_EXP											5           // ^
#define OP_TYPE_BITWISE_AND							6           // &
#define OP_TYPE_BITWISE_OR							7           // |
#define OP_TYPE_BITWISE_XOR							8           // #
#define OP_TYPE_BITWISE_NOT							9           // ~
#define OP_TYPE_LOGICAL_NOT							10          // !
#define OP_TYPE_ASSIGN									11          // =
#define OP_TYPE_LESS											12          // <
#define OP_TYPE_GREATER									13          // >
#define OP_TYPE_ASSIGN_ADD							14          // +=
#define OP_TYPE_INC											15          // ++
#define OP_TYPE_ASSIGN_SUB							16          // -= 
#define OP_TYPE_DEC											17          // --
#define OP_TYPE_ASSIGN_MUL							18          // *=
#define OP_TYPE_ASSIGN_DIV							19          // /=
#define OP_TYPE_ASSIGN_MOD							20          // %=
#define OP_TYPE_ASSIGN_EXP							21          // ^=
#define OP_TYPE_ASSIGN_AND							22          // &=
#define OP_TYPE_LOGICAL_AND							23          // &&
#define OP_TYPE_ASSIGN_OR								24          // |=
#define OP_TYPE_LOGICAL_OR							25          // ||
#define OP_TYPE_ASSIGN_XOR							26          // #=
#define OP_TYPE_NOT_EQUAL							27          // !=
#define OP_TYPE_EQUAL										28          // ==
#define OP_TYPE_LESS_EQUAL							29          // <=
#define OP_TYPE_BITWISE_SHIFT_LEFT				30          // <<
#define OP_TYPE_GREATER_EQUAL					31          // >=
#define OP_TYPE_BITWISE_SHIFT_RIGHT			32          // >>
#define OP_TYPE_ASSIGN_SHIFT_LEFT				33          // <<=
#define OP_TYPE_ASSIGN_SHIFT_RIGHT			34          // >>=     
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
typedef int Token;
typedef int State;

#define MAX_TOKEN_SIZE								1024	// 一个Token的最大长度
#define MAX_DELIM_SIZE								24		// 分隔符的最大数目
#define MAX_OP_SIZE										32      // 运算符的最大数目
/*
我们可以定义一个状态转移表，如下所示
|字符	|子状态数目	|到达子状态需要读入的字符	|操作符	|
|--------|-----------------|------------------------------------------	|-----------|
|+		|2					|+,=													|+			|
|+		|0					|														|++			|
|+		|0					|														|+=			|
1. 读入第一个字符`+`
2. 读入第二个字符，如果这个字符不是`+`和`=`中的任意一个，则识别出了`+`运算符。
3. 如果读入的第二个字符是`+`和`=`中的任意一个，那么就可以无需关心再下一个字符是什么了，因为已经没有子状态了。
4. 因此，词法分析根据字符的属性，当前的状态和读入的字符决定下一步该怎么做。
*/
typedef struct _OpState
{
	char cChar; // 状态字符
	int iSubStateIndex; // 子状态数组索引
	int iSubStateCount; // 子状态数目
	int iOpIndex;   // 运算符索引，即从0开始的下标
} OpState;
/*
	初始化全局变量
*/
void InitLexer();
/*
	判断一个字符是否是空字符。
	空字符包括空格，制表符，换行符
*/
int IsCharWhiteSpace(char c);
/*
	判断一个字符是否是数字
*/
int IsCharNumeric(char c);
/*
	判断读入的字符是否是标识符
*/
int IsCharIdent(char c);
/*
	判断读入的字符是否是分隔符
*/
int IsCharDelim(char cChar);
/*
	读入了非法字符，直接退出。
*/
void ExitOnInvalidInputError(char c);
/*
	从流中读入一个字符
*/
char GetNextChar();
/*
	用来判断字符是否是一个操作符。
*/
int IsCharOperator(char cChar, int iOpTableIndex);
/*
	根据输入的操作符，返回该操作符的类型索引，需要指定搜索那个op的table
*/
int GetOpStateIndex ( char cChar, int iCharIndex, int iSubStateIndex, int iSubStateCount );
/*
	获取当前OP状态
*/
OpState GetOpState(int iCharIndex, int iStateIndex);
/*
	获取当前OP类型
*/
int GetCurrentOp();
char * GetCurrLexeme ();
/*
	解析出下一个token类型
*/
Token GetNextToken();
