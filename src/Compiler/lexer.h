# include <string.h>
# include <stdio.h>
# include <stdlib.h>
//  ����״̬
#define LEX_STATE_START								0
#define LEX_STATE_INT										1
#define LEX_STATE_FLOAT								2
#define LEX_STATE_IDENT								5
// ����Token����
#define TOKEN_EOF											0
#define TOKEN_INT											1
#define TOKEN_FLOAT										2
#define TOKEN_IDENT										3 // ��ʶ��
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

#define MAX_TOKEN_SIZE								1024 // һ��Token����󳤶�


/*
	��ʼ��ȫ�ֱ���
*/
void InitLexer();
/*
	�ж�һ���ַ��Ƿ��ǿ��ַ���
	���ַ������ո��Ʊ�������з�
*/
int IsCharWhiteSpace(char c);
/*
	�ж�һ���ַ��Ƿ�������
*/
int IsCharNumeric(char c);
/*
*/
int IsCharIdent(char c);
/*
	�����˷Ƿ��ַ���ֱ���˳���
*/
void ExitOnInvalidInputError(char c);
/*
	�����ж���һ���ַ�
*/
char GetNextChar();
/*
	��������һ��token����
*/
Token GetNextToken();
