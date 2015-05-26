/*
*ѧ�ţ�1252878
*��������
*��ϵ�绰��18817870106
*���ʵ�ַ��1252878xujian@tongji.edu.cn
*�ļ�����������ļ����봦���ͷ�ļ�
*/

#pragma once
#include<Windows.h>
#include<windowsx.h>
#include"resource.h"
#include<stdlib.h>

typedef enum TextCodeType
{
	FILEERROR = 0,
	UTF8 = 1,
	UNICODEBIGENDIAN = 2,
	ANSI = 3,
	UNKNOW = -1,
}TextCode;

class FileEncode
{
public:
	FileEncode(char* szFileName);
	FileEncode(wchar_t* szFileName);
	~FileEncode();
	//UTF-8��ת��ΪAnsi�롣
	static char* UTF8ToAnsi(const char* szStrIn);
	//Ansiת��ΪUTF-8�롣
	static char* AnsiToUTF8(const char* szStrIn);
	//UTF-8ת��ΪUNICODE��
	static TCHAR* UTF8ToUnicode(const char* szStrIn);
	//UNICODEת��ΪUTF-8��
	static char* UnicodeToUTF8(const char* szStrIn);
	//UNICODEת��ΪAnsi��
	static char* UnicodeToAnsi(const char* szStrIn);
	//Ansiת��ΪUnicode��
	static TCHAR* AnsiToUnicode(const char* szStrIn);
	//UNICODEת��ΪUNICODE Big Endian��
	static TCHAR* UnicodeToUnicodeBigEndian(const char* szStrIn);
	//UNICODE Big Endianת��ΪUNICODE��
	static TCHAR* UnicodeBigEndianToUnicode(const char* szStrIn);
	//UNICODE Big Endianת��ΪAnsi��
	static char* UnicodeBigEndianToAnsi(const char* szStrIn);
	//��Ansiת��ΪUNICODE Big Endian��
	static TCHAR* AnsiToUnicodeBigEndian(const char* szStrIn);
	//��Unicode big endianת��Ϊ UTF-8��
	static char* UnicodeBigEndianToUTF8(const char* szStrIn);
	//��UTF-8ת��ΪUnicode Big Endian��
	static TCHAR* UTF8ToUnicodeBigEndian(const char* szStrIn);
	//�ı��ļ���bomͷ��������ж��ı��ļ��ı��뷽ʽ��
	bool IsUTF8();
	static bool IsUTF8(FILE* fpFile);
	//�ж�UTF - 8�ַ����ķ����������UTF - 8�ַ������Բ�������
	static bool IsUTF8Byte(const char* szStr);
	//���жϷ�������ı�����������UTF-8�ַ���
	static bool ContentIsUTF8(const char* szStr);
	//�ж��ļ��ı������͡�
	//	static TextCode GetFileType();
	//����ı��ļ���ȫ�ġ�
	char* ReadFileContent();
	//�ж��ļ��Ƿ񱻳ɹ��򿪡�
	bool GetFILEState();
private:
	FILE* fpFile;
	char* szFileContent;
};