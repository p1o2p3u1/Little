# include <string.h>
# include <stdio.h>
# include <stdlib.h>
//  ����״̬
#define LEX_STATE_START 0
#define LEX_STATE_INT 1
#define LEX_STATE_FLOAT 2

// ����Token����
#define TOKEN_EOF 0
#define TOKEN_INT 1
#define TOKEN_FLOAT 2

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
typedef int Token;
typedef int State;

#define MAX_TOKEN_SIZE  1024 // һ��Token����󳤶�


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
