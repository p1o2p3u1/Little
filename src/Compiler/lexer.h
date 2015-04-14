# include <string.h>
# include <stdio.h>
# include <stdlib.h>
//  定义状态
#define LEX_STATE_START									0  // 开始状态
#define LEX_STATE_INT											1  // 整形状态
#define LEX_STATE_FLOAT									2  // 浮点型状态
#define LEX_STATE_IDENT									5  // 标识符状态
#define LEX_STATE_DELIMITER							7  // 分隔符状态
// 定义Token类型
#define TOKEN_EOF												0
#define TOKEN_INT												1
#define TOKEN_FLOAT											2
#define TOKEN_IDENT											3 // 标识符
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
#define TOKEN_DELIM_COLON							15    // :
#define TOKEN_DELIM_COMMA							16    // ,    
#define TOKEN_DELIM_OPEN_PAREN					17	// (
#define TOKEN_DELIM_CLOSE_PAREN				18	// )
#define TOKEN_DELIM_OPEN_BRACE					19   // [
#define TOKEN_DELIM_CLOSE_BRACE				20	// ]
#define TOKEN_DELIM_OPEN_CURLY_BRACE	21	// {
#define TOKEN_DELIM_CLOSE_CURLY_BRACE	22	// }
#define TOKEN_DELIM_SEMICOLON					23    // ;
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
typedef int Token;
typedef int State;

#define MAX_TOKEN_SIZE								1024 // 一个Token的最大长度
#define MAX_DELIM_SIZE								24 

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
*/
int IsCharIdent(char c);
/*
	读入了非法字符，直接退出。
*/
void ExitOnInvalidInputError(char c);
/*
	从流中读入一个字符
*/
char GetNextChar();
/*
	解析出下一个token类型
*/
Token GetNextToken();
