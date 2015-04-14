# include <string.h>
# include <stdio.h>
# include <stdlib.h>
//  定义状态
#define LEX_STATE_START								0
#define LEX_STATE_INT										1
#define LEX_STATE_FLOAT								2
#define LEX_STATE_IDENT								5
// 定义Token类型
#define TOKEN_EOF											0
#define TOKEN_INT											1
#define TOKEN_FLOAT										2
#define TOKEN_IDENT										3 // 标识符
#define TOKEN_KEYWORD_VAR						4
#define TOKEN_KEYWORD_TRUE					5
#define TOKEN_KEYWORD_FALSE					6
#define TOKEN_KEYWORD_IF							7
#define TOKEN_KEYWORD_ELSE						8
#define TOKEN_KEYWORD_BREAK					 9
#define TOKEN_KEYWORD_CONTINUE			10
#define TOKEN_KEYWORD_FOR						11
#define TOKEN_KEYWORD_WHILE					12
#define TOKEN_KEYWORD_FUNC					13
#define TOKEN_KEYWORD_RETURN				14

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
typedef int Token;
typedef int State;

#define MAX_TOKEN_SIZE								1024 // 一个Token的最大长度


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
