# include <string.h>
# include <stdio.h>
# include <stdlib.h>
//  ����״̬
#define LEX_STATE_START									0  // ��ʼ״̬
#define LEX_STATE_INT											1  // ����״̬
#define LEX_STATE_FLOAT									2  // ������״̬
#define LEX_STATE_IDENT									5  // ��ʶ��״̬
#define LEX_STATE_OP											6  // ��ʾ������״̬
#define LEX_STATE_DELIMITER							7  // �ָ���״̬
#define LEX_STATE_STRING									8  // �ַ���״̬
#define LEX_STATE_STRING_ESCAPE					9	// �ַ����е�ת���ַ�״̬
#define LEX_STATE_STRING_END							10 // �ַ�������״̬
// ����Token����
#define TOKEN_EOF												0
#define TOKEN_INT												1
#define TOKEN_FLOAT											2
#define TOKEN_IDENT											3 // ��ʶ��
//--------------------------------------------------------------------------------------
#define TOKEN_KEYWORD_VAR							4 // �����ǹؼ���
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
#define TOKEN_OP												15	//Token��һ��������
#define TOKEN_DELIM_COLON							16    // :
#define TOKEN_DELIM_COMMA							17    // ,    
#define TOKEN_DELIM_OPEN_PAREN					18	// (
#define TOKEN_DELIM_CLOSE_PAREN				19	// )
#define TOKEN_DELIM_OPEN_BRACE					20   // [
#define TOKEN_DELIM_CLOSE_BRACE				21	// ]
#define TOKEN_DELIM_OPEN_CURLY_BRACE	22	// {
#define TOKEN_DELIM_CLOSE_CURLY_BRACE	23	// }
#define TOKEN_DELIM_SEMICOLON					24    // ;
#define TOKEN_STRING										25   // Token���ַ���
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

#define MAX_TOKEN_SIZE								1024	// һ��Token����󳤶�
#define MAX_DELIM_SIZE								24		// �ָ����������Ŀ
#define MAX_OP_SIZE										32      // ������������Ŀ
/*
���ǿ��Զ���һ��״̬ת�Ʊ�������ʾ
|�ַ�	|��״̬��Ŀ	|������״̬��Ҫ������ַ�	|������	|
|--------|-----------------|------------------------------------------	|-----------|
|+		|2					|+,=													|+			|
|+		|0					|														|++			|
|+		|0					|														|+=			|
1. �����һ���ַ�`+`
2. ����ڶ����ַ����������ַ�����`+`��`=`�е�����һ������ʶ�����`+`�������
3. �������ĵڶ����ַ���`+`��`=`�е�����һ������ô�Ϳ��������������һ���ַ���ʲô�ˣ���Ϊ�Ѿ�û����״̬�ˡ�
4. ��ˣ��ʷ����������ַ������ԣ���ǰ��״̬�Ͷ�����ַ�������һ������ô����
*/
typedef struct _OpState
{
	char cChar; // ״̬�ַ�
	int iSubStateIndex; // ��״̬��������
	int iSubStateCount; // ��״̬��Ŀ
	int iOpIndex;   // ���������������0��ʼ���±�
} OpState;
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
	�ж϶�����ַ��Ƿ��Ǳ�ʶ��
*/
int IsCharIdent(char c);
/*
	�ж϶�����ַ��Ƿ��Ƿָ���
*/
int IsCharDelim(char cChar);
/*
	�����˷Ƿ��ַ���ֱ���˳���
*/
void ExitOnInvalidInputError(char c);
/*
	�����ж���һ���ַ�
*/
char GetNextChar();
/*
	�����ж��ַ��Ƿ���һ����������
*/
int IsCharOperator(char cChar, int iOpTableIndex);
/*
	��������Ĳ����������ظò�������������������Ҫָ�������Ǹ�op��table
*/
int GetOpStateIndex ( char cChar, int iCharIndex, int iSubStateIndex, int iSubStateCount );
/*
	��ȡ��ǰOP״̬
*/
OpState GetOpState(int iCharIndex, int iStateIndex);
/*
	��ȡ��ǰOP����
*/
int GetCurrentOp();
char * GetCurrLexeme ();
/*
	��������һ��token����
*/
Token GetNextToken();
