/*
*学号：1252878
*姓名：许舰
*联系电话：18817870106
*电邮地址：1252878xujian@tongji.edu.cn
*文件描述：歌词文件编码处理的具体实现
*/

#include"StdAfx.h"
#include"FileEncode.h"
#include<vector>
using namespace std;


FileEncode::FileEncode(char* szFileName)
{
	szFileContent = NULL;
	fopen_s(&fpFile, szFileName, "rb+");
	if (fpFile == NULL)
	{
//		MessageBox(NULL, TEXT("打开文件失败！"), TEXT("ERROR!"), MB_OK | MB_ICONERROR);
	}
}

FileEncode::FileEncode(wchar_t* szFileName)
{
	szFileContent = NULL;
	_wfopen_s(&fpFile, szFileName, L"rb+");
	if (fpFile == NULL)
	{
//		MessageBox(NULL, TEXT("打开文件失败！"), TEXT("ERROR!"), MB_OK | MB_ICONERROR);
	}
}

FileEncode::~FileEncode()
{
	free(szFileContent);
	if (fpFile)
	{
		fclose(fpFile);//fclose  NULL 的FILE*的时候 会出现错误   stream != NULL。
	}
}


char* FileEncode::UTF8ToAnsi(const char* szStrIn)
{
	WCHAR* wszAnsi = NULL;
	char* szAnsi = NULL;
	//该函数映射一个字符串到一个宽字符（unicode）的字符串。由该函数映射的字符串没必要是多字节字符组。
	// cchWideChar：指定由参数lpWideCharStr指向的缓冲区的宽字符个数。若此值为零，函数返回缓冲区所必需的宽字符数，在这种情况下，lpWideCharStr中的缓冲区不被使用。
	int iCount = MultiByteToWideChar(CP_UTF8, 0, szStrIn, -1, NULL, 0);//我们只需要将MultiByteToWideChar()的第五六个形参设为NULL,即可返回所需的宽字符数组空间的个数:
	wszAnsi = new WCHAR[iCount + 1];
	memset(wszAnsi, 0, 2 * (iCount + 1));//对WCHAR的清零。
	//char 存储的UTF-8 转化为wchart 存储。这个实现的是UTF-8到UNICODE的转换。
	MultiByteToWideChar(CP_UTF8, 0, szStrIn, -1, wszAnsi, iCount);
	//接下来将UNICODE其转化为Ansi。
	//cchWideChar：指定由参数lpWideCharStr指向的缓冲区的字符个数。如果这个值为-1，字符串将被设定为以NULL为结束符的字符串，并且自动计算长度。
	//cchMultiByte：指定由参数lpMultiByteStr指向的缓冲区最大值（用字节来计量）。若此值为零，函数返回lpMultiByteStr指向的目标缓冲区所必需的字节数，在这种情况下，lpMultiByteStr参数通常为NULL。
	iCount = WideCharToMultiByte(CP_ACP, 0, wszAnsi, -1, NULL, 0, NULL, NULL);
	szAnsi = new char[iCount + 1];//Ansi编码所需最大的字符数。
	memset(szAnsi, 0, iCount + 1);
	WideCharToMultiByte(CP_ACP, 0, wszAnsi, -1, szAnsi, iCount, NULL, NULL);

	delete[] wszAnsi;
	return szAnsi;
}

//Ansi转化为UTF-8码。
char* FileEncode::AnsiToUTF8(const char* szStrIn)
{
	WCHAR* wszUtf8 = NULL;
	char* szUtf8 = NULL;
	//将Ansi的编码转化为UNICODE的编码，需要多少个宽字节。
	int iCount = MultiByteToWideChar(CP_ACP, 0, szStrIn, -1, NULL, 0);
	wszUtf8 = new WCHAR[iCount + 1];
	memset(wszUtf8, 0, 2 * (iCount + 1));
	//首先将Ansi转化为UNICODE。
	MultiByteToWideChar(CP_ACP, 0, szStrIn, -1, wszUtf8, iCount);
	//获得UNICODE转化为UTF-8的时候所需要的字节数。
	iCount = WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, NULL, 0, NULL, false);
	szUtf8 = new char[iCount + 1];
	memset(szUtf8, 0, iCount + 1);
	//将UNICODE转化为UTF-8码。
	WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, szUtf8, iCount, NULL, false);

	delete[] wszUtf8;
	return szUtf8;
}

//UTF-8转化为UNICODE码。
TCHAR* FileEncode::UTF8ToUnicode(const char* szStrIn)
{
	WCHAR* wszUnicode = NULL;
	//将UTF-8 转化为UNICODE时所需要的宽字节数。
	int iCount = MultiByteToWideChar(CP_UTF8, 0, szStrIn, -1, NULL, 0);
	wszUnicode = new WCHAR[iCount + 1];
	memset(wszUnicode, 0, 2 * (iCount + 1));
	//开始将UTF-8转化为UNICODE编码。
	MultiByteToWideChar(CP_UTF8, 0, szStrIn, -1, wszUnicode, iCount);
	return wszUnicode;
}

//UNICODE转化为UTF-8编码。
char* FileEncode::UnicodeToUTF8(const char* szStrIn)
{
	char* szUtf8 = NULL;
	//将UNICODE转化为UTF-8编码的时候所需要的普通字节数。
	int iCount = WideCharToMultiByte(CP_UTF8, 0, (TCHAR*)szStrIn, -1, NULL, 0, NULL, false);
	szUtf8 = new char[iCount + 1];
	memset(szUtf8, 0, iCount);
	//将UNICODE转化为UTF-8编码。
	WideCharToMultiByte(CP_UTF8, 0, (TCHAR*)szStrIn, -1, szUtf8, iCount, NULL, false);
	return szUtf8;
}

//UNICODE转化为Ansi。
char* FileEncode::UnicodeToAnsi(const char* szStrIn)
{
	char* szAnsi = NULL;
	int iCount = WideCharToMultiByte(CP_ACP, 0, (TCHAR*)szStrIn, -1, NULL, 0, NULL, false);
	szAnsi = new char[iCount + 1];
	memset(szAnsi, 0, iCount + 1);
	WideCharToMultiByte(CP_ACP, 0, (TCHAR*)szStrIn, -1, szAnsi, iCount, NULL, false);
	return szAnsi;
}

//Ansi转化为Unicode。
TCHAR* FileEncode::AnsiToUnicode(const char* szStrIn)
{
	TCHAR* wszUnicode = NULL;
	int iCount = MultiByteToWideChar(CP_ACP, 0, szStrIn, -1, NULL, 0);
	wszUnicode = new WCHAR[iCount + 1];
	memset(wszUnicode, 0, 2 * (iCount + 1));
	MultiByteToWideChar(CP_ACP, 0, szStrIn, -1, wszUnicode, iCount);
	return wszUnicode;
}

//UNICODE转化为UNICODE Big Endian。
TCHAR* FileEncode::UnicodeToUnicodeBigEndian(const char* szStrIn)
{
	//将Big Endian转化为Little Endian。
	unsigned char cHighByte = NULL, cLowByte = NULL;
	TCHAR* wszUnicodeBigEndian = NULL;
	//记事本中采用ucs-2。
	int iStrLen = wcslen((wchar_t*)szStrIn) << 1;
	wszUnicodeBigEndian = (TCHAR*)calloc((iStrLen >> 1) + 1, sizeof(TCHAR));
	int index = 0;
	while (index < iStrLen)
	{
		/*		//当szStrIn为wchar_t时候的实现。
		cHighByte = ((szStrIn[index] & 0xFF00) >> 8);//算数右移。
		cLowByte = (szStrIn[index] & 0x00FF);
		szStrIn[index] = ((cLowByte << 8) | cHighByte);*/
		cLowByte = szStrIn[index + 1];
		cHighByte = szStrIn[index];
		wszUnicodeBigEndian[index >> 1] = ((cHighByte << 8) | cLowByte);
		index += 2;
	}
	return wszUnicodeBigEndian;
}

//UNICODE Big Endian转化为UNICODE。
TCHAR* FileEncode::UnicodeBigEndianToUnicode(const char* szStrIn)//此时的szStrIn就是以大端存储的。
{
	unsigned char cHighByte = NULL, cLowByte = NULL;
	TCHAR* wszUnicode = NULL;
	int iStrLen = wcslen((wchar_t*)szStrIn) << 1;
	wszUnicode = (wchar_t*)calloc((iStrLen >> 1) + 1, sizeof(TCHAR));
	int index = 0;
	while (index < iStrLen)
	{
		cHighByte = szStrIn[index + 1];
		cLowByte = szStrIn[index];
		wszUnicode[index >> 1] = ((cLowByte << 8) | cHighByte);
		index += 2;
	}
	return wszUnicode;
}

//UNICODE Big Endian转化为Ansi。
char* FileEncode::UnicodeBigEndianToAnsi(const char* szStrIn)
{
	TCHAR* wszUnicode = UnicodeBigEndianToUnicode(szStrIn);
	//char* wszUnicode = (char*)UnicodeBigEndianToUnicode(szStrIn);//这样会直接转化不出来。
	char* szAnsi = UnicodeToAnsi((char*)wszUnicode);
	free(wszUnicode);
	return szAnsi;
}

//将Ansi转化为UNICODE Big Endian。
TCHAR* FileEncode::AnsiToUnicodeBigEndian(const char* szStrIn)
{
	TCHAR* wszUnicode = AnsiToUnicode(szStrIn);
	TCHAR*wszUnicodeBigEndian = UnicodeToUnicodeBigEndian((char*)wszUnicode);
	free(wszUnicode);
	return wszUnicodeBigEndian;
}

//将Unicode big endian转化为 UTF-8。
char* FileEncode::UnicodeBigEndianToUTF8(const char* szStrIn)
{
	TCHAR* wszUnicode = UnicodeBigEndianToUnicode(szStrIn);
	char* szUtf8 = UnicodeToUTF8((char*)wszUnicode);
	free(wszUnicode);
	return szUtf8;
}

//将UTF-8转化为Unicode Big Endian。
TCHAR* FileEncode::UTF8ToUnicodeBigEndian(const char* szStrIn)
{
	TCHAR* wszUnicode = UTF8ToUnicode(szStrIn);
	TCHAR* wszUnicodeBigEndian = UnicodeToUnicodeBigEndian((char*)wszUnicode);
	free(wszUnicode);
	return wszUnicodeBigEndian;
}

bool FileEncode::IsUTF8()
{
	return FileEncode::IsUTF8(fpFile);
}

bool FileEncode::IsUTF8(FILE* fpFile)
{
	if (fpFile == NULL)
	{
		MessageBox(NULL, TEXT("bool FileEncode::ContentIsUTF8(FILE *fpFile) fpFile为空！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return false;
	}
	char szStrTest[MAX_PATH] = { NULL };
	if ((fgets(szStrTest, MAX_PATH, fpFile)) == 0)
	{
		MessageBox(NULL, TEXT("fgets(szStrTest, MAX_PATH, fpFile) fgets读入失败！bool FileEncode::ContentIsUTF8(FILE *fpFile)"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return false;
	}
	rewind(fpFile);
	return IsUTF8Byte(szStrTest);
}

bool FileEncode::ContentIsUTF8(const char*szStr)
{
	int iCount = 0;
	int cSize = strlen(szStr);
	while (iCount < cSize)
	{
		int step = 0;//记录在UTF-8编码的情况下，一个字符编码的字节数。
		if ((szStr[iCount] & 0x80) == 0x00)
		{
			step = 1;
		}
		else if ((szStr[iCount] & 0xE0) == 0xC0)
		{
			if ((iCount + 1) >= cSize)
				return false;
			if ((szStr[iCount + 1] & 0xC0) != 0x80)
				return false;
			step = 2;
		}
		else if ((szStr[iCount] & 0xF0) == 0xE0)
		{
			if (iCount + 2 >= cSize)
				return false;
			if ((szStr[iCount + 1] & 0xC0) != 0x80)
				return false;
			if ((szStr[iCount + 2] & 0xC0) != 0x80)
				return false;
			step = 3;
		}
		else
			return false;
		//使iCount跳过一个UTF-8的字节。
		iCount += step;
	}
	//检测记录的步长与检测到的数组的长度，如果相等即为UTF-8的编码方式。
	if (iCount == cSize)
		return true;
	return false;
}

//另一种判断UTF-8字符串的方法。传入的UTF-8字符串可以不完整。
bool FileEncode::IsUTF8Byte(const char* szStr)
{
	//这种判断方法忽略了0xxxxxxx的情况。
	int iCharCount = 1;
	int iStrLen = strlen(szStr);
	unsigned char cCurChar;//当前正在分析的字节。
	if (iStrLen == 0)
	{
		MessageBox(NULL, TEXT("bool FileEncode::IsUTF8Byte(const char* szStr) 传入的字符串为空！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return false;
	}
	for (int index = 0; index < iStrLen; index++)
	{
		cCurChar = szStr[index];
		if (iCharCount == 1)//未开始统计UTF-8编码的字符实际上占有了多少个字节时的情况。
		{
			//如果是0xxxxxxx的情况，那么忽略掉。
			if (cCurChar >= 0x80)
			{
				//开始逻辑左移判断，逻辑左移挤掉最高位，最低位补零，算数右移挤掉最低位，最高位补上符号位。
				while (((cCurChar <<= 1) & 0x80) != 0x00)//(cCurChar <<1)出现错误。
				{
					iCharCount++;//统计实际上一个以UTF-8编码的字符实际上占有了多少个字节。
				}
				if ((iCharCount == 1) || (iCharCount > 6))//UTF-8编码最多六位编码。
				{
					return false;
				}
			}
		}
		else//统计UTF-8编码的字符实际上占有了多少个字节后的情况。
		{
			//过了编码最高位，忽略了0xxxxxxx，那么此时字节的第一位必定是一。
			if (((cCurChar << 1) & 0x80) != 0)
			{
				return false;
			}
			iCharCount--;
		}
	}
	return true;
}

//获得文本文件的全文。
char* FileEncode::ReadFileContent()
{
	fseek(fpFile, 0L, 2);//把文件指针移到文件的结尾。
	int iFCharCount = ftell(fpFile);
	rewind(fpFile);//使文件指针重新回到开头。
	szFileContent = (char*)calloc(iFCharCount + 1, sizeof(char));//calloc有自动清零的功能。
	if ((fread(szFileContent, sizeof(char), iFCharCount, fpFile) == 0))
	{
		MessageBox(NULL, TEXT("bool FileEncode::ReadFileContent()  fread函数失败！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return NULL;
	}
	szFileContent[iFCharCount] = '\0';//增加结束符。
	return szFileContent;
}

//判断文件是否被成功打开。
bool FileEncode::GetFILEState()
{
	return (fpFile == NULL) ? false : true;
}
