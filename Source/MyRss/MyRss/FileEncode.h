/*
*学号：1252878
*姓名：许舰
*联系电话：18817870106
*电邮地址：1252878xujian@tongji.edu.cn
*文件描述：歌词文件编码处理的头文件
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
	//UTF-8码转化为Ansi码。
	static char* UTF8ToAnsi(const char* szStrIn);
	//Ansi转化为UTF-8码。
	static char* AnsiToUTF8(const char* szStrIn);
	//UTF-8转化为UNICODE。
	static TCHAR* UTF8ToUnicode(const char* szStrIn);
	//UNICODE转化为UTF-8。
	static char* UnicodeToUTF8(const char* szStrIn);
	//UNICODE转化为Ansi。
	static char* UnicodeToAnsi(const char* szStrIn);
	//Ansi转化为Unicode。
	static TCHAR* AnsiToUnicode(const char* szStrIn);
	//UNICODE转化为UNICODE Big Endian。
	static TCHAR* UnicodeToUnicodeBigEndian(const char* szStrIn);
	//UNICODE Big Endian转化为UNICODE。
	static TCHAR* UnicodeBigEndianToUnicode(const char* szStrIn);
	//UNICODE Big Endian转化为Ansi。
	static char* UnicodeBigEndianToAnsi(const char* szStrIn);
	//将Ansi转化为UNICODE Big Endian。
	static TCHAR* AnsiToUnicodeBigEndian(const char* szStrIn);
	//将Unicode big endian转化为 UTF-8。
	static char* UnicodeBigEndianToUTF8(const char* szStrIn);
	//将UTF-8转化为Unicode Big Endian。
	static TCHAR* UTF8ToUnicodeBigEndian(const char* szStrIn);
	//文本文件无bom头的情况下判断文本文件的编码方式。
	bool IsUTF8();
	static bool IsUTF8(FILE* fpFile);
	//判断UTF - 8字符串的方法。传入的UTF - 8字符串可以不完整。
	static bool IsUTF8Byte(const char* szStr);
	//该判断方法传入的必须是完整的UTF-8字符。
	static bool ContentIsUTF8(const char* szStr);
	//判断文件的编码类型。
	//	static TextCode GetFileType();
	//获得文本文件的全文。
	char* ReadFileContent();
	//判断文件是否被成功打开。
	bool GetFILEState();
private:
	FILE* fpFile;
	char* szFileContent;
};