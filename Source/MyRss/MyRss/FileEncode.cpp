/*
*ѧ�ţ�1252878
*��������
*��ϵ�绰��18817870106
*���ʵ�ַ��1252878xujian@tongji.edu.cn
*�ļ�����������ļ����봦��ľ���ʵ��
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
//		MessageBox(NULL, TEXT("���ļ�ʧ�ܣ�"), TEXT("ERROR!"), MB_OK | MB_ICONERROR);
	}
}

FileEncode::FileEncode(wchar_t* szFileName)
{
	szFileContent = NULL;
	_wfopen_s(&fpFile, szFileName, L"rb+");
	if (fpFile == NULL)
	{
//		MessageBox(NULL, TEXT("���ļ�ʧ�ܣ�"), TEXT("ERROR!"), MB_OK | MB_ICONERROR);
	}
}

FileEncode::~FileEncode()
{
	free(szFileContent);
	if (fpFile)
	{
		fclose(fpFile);//fclose  NULL ��FILE*��ʱ�� ����ִ���   stream != NULL��
	}
}


char* FileEncode::UTF8ToAnsi(const char* szStrIn)
{
	WCHAR* wszAnsi = NULL;
	char* szAnsi = NULL;
	//�ú���ӳ��һ���ַ�����һ�����ַ���unicode�����ַ������ɸú���ӳ����ַ���û��Ҫ�Ƕ��ֽ��ַ��顣
	// cchWideChar��ָ���ɲ���lpWideCharStrָ��Ļ������Ŀ��ַ�����������ֵΪ�㣬�������ػ�����������Ŀ��ַ���������������£�lpWideCharStr�еĻ���������ʹ�á�
	int iCount = MultiByteToWideChar(CP_UTF8, 0, szStrIn, -1, NULL, 0);//����ֻ��Ҫ��MultiByteToWideChar()�ĵ��������β���ΪNULL,���ɷ�������Ŀ��ַ�����ռ�ĸ���:
	wszAnsi = new WCHAR[iCount + 1];
	memset(wszAnsi, 0, 2 * (iCount + 1));//��WCHAR�����㡣
	//char �洢��UTF-8 ת��Ϊwchart �洢�����ʵ�ֵ���UTF-8��UNICODE��ת����
	MultiByteToWideChar(CP_UTF8, 0, szStrIn, -1, wszAnsi, iCount);
	//��������UNICODE��ת��ΪAnsi��
	//cchWideChar��ָ���ɲ���lpWideCharStrָ��Ļ��������ַ�������������ֵΪ-1���ַ��������趨Ϊ��NULLΪ���������ַ����������Զ����㳤�ȡ�
	//cchMultiByte��ָ���ɲ���lpMultiByteStrָ��Ļ��������ֵ�����ֽ���������������ֵΪ�㣬��������lpMultiByteStrָ���Ŀ�껺������������ֽ���������������£�lpMultiByteStr����ͨ��ΪNULL��
	iCount = WideCharToMultiByte(CP_ACP, 0, wszAnsi, -1, NULL, 0, NULL, NULL);
	szAnsi = new char[iCount + 1];//Ansi�������������ַ�����
	memset(szAnsi, 0, iCount + 1);
	WideCharToMultiByte(CP_ACP, 0, wszAnsi, -1, szAnsi, iCount, NULL, NULL);

	delete[] wszAnsi;
	return szAnsi;
}

//Ansiת��ΪUTF-8�롣
char* FileEncode::AnsiToUTF8(const char* szStrIn)
{
	WCHAR* wszUtf8 = NULL;
	char* szUtf8 = NULL;
	//��Ansi�ı���ת��ΪUNICODE�ı��룬��Ҫ���ٸ����ֽڡ�
	int iCount = MultiByteToWideChar(CP_ACP, 0, szStrIn, -1, NULL, 0);
	wszUtf8 = new WCHAR[iCount + 1];
	memset(wszUtf8, 0, 2 * (iCount + 1));
	//���Ƚ�Ansiת��ΪUNICODE��
	MultiByteToWideChar(CP_ACP, 0, szStrIn, -1, wszUtf8, iCount);
	//���UNICODEת��ΪUTF-8��ʱ������Ҫ���ֽ�����
	iCount = WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, NULL, 0, NULL, false);
	szUtf8 = new char[iCount + 1];
	memset(szUtf8, 0, iCount + 1);
	//��UNICODEת��ΪUTF-8�롣
	WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, szUtf8, iCount, NULL, false);

	delete[] wszUtf8;
	return szUtf8;
}

//UTF-8ת��ΪUNICODE�롣
TCHAR* FileEncode::UTF8ToUnicode(const char* szStrIn)
{
	WCHAR* wszUnicode = NULL;
	//��UTF-8 ת��ΪUNICODEʱ����Ҫ�Ŀ��ֽ�����
	int iCount = MultiByteToWideChar(CP_UTF8, 0, szStrIn, -1, NULL, 0);
	wszUnicode = new WCHAR[iCount + 1];
	memset(wszUnicode, 0, 2 * (iCount + 1));
	//��ʼ��UTF-8ת��ΪUNICODE���롣
	MultiByteToWideChar(CP_UTF8, 0, szStrIn, -1, wszUnicode, iCount);
	return wszUnicode;
}

//UNICODEת��ΪUTF-8���롣
char* FileEncode::UnicodeToUTF8(const char* szStrIn)
{
	char* szUtf8 = NULL;
	//��UNICODEת��ΪUTF-8�����ʱ������Ҫ����ͨ�ֽ�����
	int iCount = WideCharToMultiByte(CP_UTF8, 0, (TCHAR*)szStrIn, -1, NULL, 0, NULL, false);
	szUtf8 = new char[iCount + 1];
	memset(szUtf8, 0, iCount);
	//��UNICODEת��ΪUTF-8���롣
	WideCharToMultiByte(CP_UTF8, 0, (TCHAR*)szStrIn, -1, szUtf8, iCount, NULL, false);
	return szUtf8;
}

//UNICODEת��ΪAnsi��
char* FileEncode::UnicodeToAnsi(const char* szStrIn)
{
	char* szAnsi = NULL;
	int iCount = WideCharToMultiByte(CP_ACP, 0, (TCHAR*)szStrIn, -1, NULL, 0, NULL, false);
	szAnsi = new char[iCount + 1];
	memset(szAnsi, 0, iCount + 1);
	WideCharToMultiByte(CP_ACP, 0, (TCHAR*)szStrIn, -1, szAnsi, iCount, NULL, false);
	return szAnsi;
}

//Ansiת��ΪUnicode��
TCHAR* FileEncode::AnsiToUnicode(const char* szStrIn)
{
	TCHAR* wszUnicode = NULL;
	int iCount = MultiByteToWideChar(CP_ACP, 0, szStrIn, -1, NULL, 0);
	wszUnicode = new WCHAR[iCount + 1];
	memset(wszUnicode, 0, 2 * (iCount + 1));
	MultiByteToWideChar(CP_ACP, 0, szStrIn, -1, wszUnicode, iCount);
	return wszUnicode;
}

//UNICODEת��ΪUNICODE Big Endian��
TCHAR* FileEncode::UnicodeToUnicodeBigEndian(const char* szStrIn)
{
	//��Big Endianת��ΪLittle Endian��
	unsigned char cHighByte = NULL, cLowByte = NULL;
	TCHAR* wszUnicodeBigEndian = NULL;
	//���±��в���ucs-2��
	int iStrLen = wcslen((wchar_t*)szStrIn) << 1;
	wszUnicodeBigEndian = (TCHAR*)calloc((iStrLen >> 1) + 1, sizeof(TCHAR));
	int index = 0;
	while (index < iStrLen)
	{
		/*		//��szStrInΪwchar_tʱ���ʵ�֡�
		cHighByte = ((szStrIn[index] & 0xFF00) >> 8);//�������ơ�
		cLowByte = (szStrIn[index] & 0x00FF);
		szStrIn[index] = ((cLowByte << 8) | cHighByte);*/
		cLowByte = szStrIn[index + 1];
		cHighByte = szStrIn[index];
		wszUnicodeBigEndian[index >> 1] = ((cHighByte << 8) | cLowByte);
		index += 2;
	}
	return wszUnicodeBigEndian;
}

//UNICODE Big Endianת��ΪUNICODE��
TCHAR* FileEncode::UnicodeBigEndianToUnicode(const char* szStrIn)//��ʱ��szStrIn�����Դ�˴洢�ġ�
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

//UNICODE Big Endianת��ΪAnsi��
char* FileEncode::UnicodeBigEndianToAnsi(const char* szStrIn)
{
	TCHAR* wszUnicode = UnicodeBigEndianToUnicode(szStrIn);
	//char* wszUnicode = (char*)UnicodeBigEndianToUnicode(szStrIn);//������ֱ��ת����������
	char* szAnsi = UnicodeToAnsi((char*)wszUnicode);
	free(wszUnicode);
	return szAnsi;
}

//��Ansiת��ΪUNICODE Big Endian��
TCHAR* FileEncode::AnsiToUnicodeBigEndian(const char* szStrIn)
{
	TCHAR* wszUnicode = AnsiToUnicode(szStrIn);
	TCHAR*wszUnicodeBigEndian = UnicodeToUnicodeBigEndian((char*)wszUnicode);
	free(wszUnicode);
	return wszUnicodeBigEndian;
}

//��Unicode big endianת��Ϊ UTF-8��
char* FileEncode::UnicodeBigEndianToUTF8(const char* szStrIn)
{
	TCHAR* wszUnicode = UnicodeBigEndianToUnicode(szStrIn);
	char* szUtf8 = UnicodeToUTF8((char*)wszUnicode);
	free(wszUnicode);
	return szUtf8;
}

//��UTF-8ת��ΪUnicode Big Endian��
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
		MessageBox(NULL, TEXT("bool FileEncode::ContentIsUTF8(FILE *fpFile) fpFileΪ�գ�"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return false;
	}
	char szStrTest[MAX_PATH] = { NULL };
	if ((fgets(szStrTest, MAX_PATH, fpFile)) == 0)
	{
		MessageBox(NULL, TEXT("fgets(szStrTest, MAX_PATH, fpFile) fgets����ʧ�ܣ�bool FileEncode::ContentIsUTF8(FILE *fpFile)"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
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
		int step = 0;//��¼��UTF-8���������£�һ���ַ�������ֽ�����
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
		//ʹiCount����һ��UTF-8���ֽڡ�
		iCount += step;
	}
	//����¼�Ĳ������⵽������ĳ��ȣ������ȼ�ΪUTF-8�ı��뷽ʽ��
	if (iCount == cSize)
		return true;
	return false;
}

//��һ���ж�UTF-8�ַ����ķ����������UTF-8�ַ������Բ�������
bool FileEncode::IsUTF8Byte(const char* szStr)
{
	//�����жϷ���������0xxxxxxx�������
	int iCharCount = 1;
	int iStrLen = strlen(szStr);
	unsigned char cCurChar;//��ǰ���ڷ������ֽڡ�
	if (iStrLen == 0)
	{
		MessageBox(NULL, TEXT("bool FileEncode::IsUTF8Byte(const char* szStr) ������ַ���Ϊ�գ�"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return false;
	}
	for (int index = 0; index < iStrLen; index++)
	{
		cCurChar = szStr[index];
		if (iCharCount == 1)//δ��ʼͳ��UTF-8������ַ�ʵ����ռ���˶��ٸ��ֽ�ʱ�������
		{
			//�����0xxxxxxx���������ô���Ե���
			if (cCurChar >= 0x80)
			{
				//��ʼ�߼������жϣ��߼����Ƽ������λ�����λ���㣬�������Ƽ������λ�����λ���Ϸ���λ��
				while (((cCurChar <<= 1) & 0x80) != 0x00)//(cCurChar <<1)���ִ���
				{
					iCharCount++;//ͳ��ʵ����һ����UTF-8������ַ�ʵ����ռ���˶��ٸ��ֽڡ�
				}
				if ((iCharCount == 1) || (iCharCount > 6))//UTF-8���������λ���롣
				{
					return false;
				}
			}
		}
		else//ͳ��UTF-8������ַ�ʵ����ռ���˶��ٸ��ֽں�������
		{
			//���˱������λ��������0xxxxxxx����ô��ʱ�ֽڵĵ�һλ�ض���һ��
			if (((cCurChar << 1) & 0x80) != 0)
			{
				return false;
			}
			iCharCount--;
		}
	}
	return true;
}

//����ı��ļ���ȫ�ġ�
char* FileEncode::ReadFileContent()
{
	fseek(fpFile, 0L, 2);//���ļ�ָ���Ƶ��ļ��Ľ�β��
	int iFCharCount = ftell(fpFile);
	rewind(fpFile);//ʹ�ļ�ָ�����»ص���ͷ��
	szFileContent = (char*)calloc(iFCharCount + 1, sizeof(char));//calloc���Զ�����Ĺ��ܡ�
	if ((fread(szFileContent, sizeof(char), iFCharCount, fpFile) == 0))
	{
		MessageBox(NULL, TEXT("bool FileEncode::ReadFileContent()  fread����ʧ�ܣ�"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return NULL;
	}
	szFileContent[iFCharCount] = '\0';//���ӽ�������
	return szFileContent;
}

//�ж��ļ��Ƿ񱻳ɹ��򿪡�
bool FileEncode::GetFILEState()
{
	return (fpFile == NULL) ? false : true;
}
