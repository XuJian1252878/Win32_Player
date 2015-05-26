/*
*ѧ�ţ�1252878
*��������
*��ϵ�绰��18817870106
*���ʵ�ַ��1252878xujian@tongji.edu.cn
*�ļ�������Rss�Ķ����������д�뱾�ش��̵�ʵ��
*/


#include"stdafx.h"
#include"InitRssReader.h"

extern vector<string> RssFeedList;//���游��㡣
extern vector<string> RssFeedLinks;//����RssFeed��������Ϣ��
extern vector< map<string, RssContent> > RssItemList;//����Rss��ÿһ���ӽڵ�����ݡ�
extern bool bIsAppFirseUse;

//��ʼ��rssXML�ļ�����ʱ����·����ȷ������ʱ�ļ����ڲ����Խ�����HWND �����ڵľ����
//�ú������ݴ��RssXml���ļ�ȫ·�����أ�ͨ���������ķ�ʽ����\\RssReader\\src\\TempRssXml.txt
bool InitRssXMLTempSavePath(HWND hwnd, TCHAR* szTempRssXmlPath)
{
	TCHAR szRssReaderPath[MAX_PATH] = { NULL };
	TCHAR szAppRunPath[MAX_PATH] = { NULL };
	GetAppRunPath(hwnd, szAppRunPath);
	lstrcpy(szRssReaderPath, szAppRunPath);
	//���첥������Ϣ�洢���ļ��С�
	lstrcat(szRssReaderPath, TEXT("RssReader"));
	//�����жϸ��ļ����Ƿ���ڡ�
	/*	When used with files, the _access function determines whether the specified file exists
	and can be accessed as specified by the value of mode. When used with directories, _access
	determines only whether the specified directory exists; in Windows NT, all directories have read and write access.
	mode Value            Checks File For
	00                              Existence only
	02                              Write permission
	04                              Read permission
	06                              Read and write permission*/
	char* cszRssReaderPath = NULL;//���ֵҪ�ֶ��ͷš�
	cszRssReaderPath = FileEncode::UnicodeToAnsi((char*)szRssReaderPath);
	//Rss�Ķ���������Ŀ¼��
	//ERROR_PATH_NOT_FOUND
	//One or more intermediate directories do not exist; this function will only create the final directory in the path. 
	if ((_access(cszRssReaderPath, 0) == -1))//������ļ��в����ڡ�
	{
		CreateDirectory(szRssReaderPath, NULL);
		if (GetLastError() == ERROR_PATH_NOT_FOUND)
		{
			MessageBox(hwnd, TEXT("RssReader���ļ����޷��������м��ļ��ж�ʧ����"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			free(cszRssReaderPath);
			return false;
		}
	}
	//RSS�Ķ�������ԴĿ¼��
	TCHAR szRssReaderSrcPath[MAX_PATH] = { NULL };
	lstrcat(szRssReaderSrcPath, szRssReaderPath);
	lstrcat(szRssReaderSrcPath, TEXT("\\src"));
	char* cszRssReaderSrcPath = FileEncode::UnicodeToAnsi((char*)szRssReaderSrcPath);
	if (_access(cszRssReaderSrcPath, 0) == -1)
	{
		CreateDirectory(szRssReaderSrcPath, NULL);
		if (GetLastError() == ERROR_PATH_NOT_FOUND)
		{
			MessageBox(hwnd, TEXT("RssReader\\src���ļ����޷��������м��ļ��ж�ʧ����"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			free(cszRssReaderSrcPath);
			free(szRssReaderPath);
			return false;
		}
	}
	//�洢��ʱRSSXML���ļ���
	lstrcat(szTempRssXmlPath, szRssReaderSrcPath);
	lstrcat(szTempRssXmlPath, TEXT("\\TempRssXml.txt"));
	return true;
}


//����Rss�Ķ����ѱ������Ϣ��
//extern vector<string> RssFeedList;//���游��㡣
//extern vector< map<string, RssContent> > RssItemList;//����Rss��ÿһ���ӽڵ�����ݡ�
bool SaveRssMsg(HWND hwnd)
{
	TCHAR szRssReaderPath[MAX_PATH] = { NULL };
	TCHAR szAppRunPath[MAX_PATH] = { NULL };
	GetAppRunPath(hwnd, szAppRunPath);
	lstrcpy(szRssReaderPath, szAppRunPath);
	//���첥������Ϣ�洢���ļ��С�
	lstrcat(szRssReaderPath, TEXT("RssReader"));
	//�����жϸ��ļ����Ƿ���ڡ�
	char* cszRssReaderPath = NULL;//���ֵҪ�ֶ��ͷš�
	cszRssReaderPath = FileEncode::UnicodeToAnsi((char*)szRssReaderPath);
	//Rss�Ķ���������Ŀ¼��
	//ERROR_PATH_NOT_FOUND
	//One or more intermediate directories do not exist; this function will only create the final directory in the path. 
	if ((_access(cszRssReaderPath, 0) == -1))//������ļ��в����ڡ�
	{
		CreateDirectory(szRssReaderPath, NULL);
		if (GetLastError() == ERROR_PATH_NOT_FOUND)
		{
			MessageBox(hwnd, TEXT("RssReader���ļ����޷��������м��ļ��ж�ʧ����"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			free(cszRssReaderPath);
			return false;
		}
	}
	//RSS�Ķ�������ԴĿ¼��
	TCHAR szRssReaderSrcPath[MAX_PATH] = { NULL };
	lstrcat(szRssReaderSrcPath, szRssReaderPath);
	lstrcat(szRssReaderSrcPath, TEXT("\\src"));
	char* cszRssReaderSrcPath = FileEncode::UnicodeToAnsi((char*)szRssReaderSrcPath);
	if (_access(cszRssReaderSrcPath, 0) == -1)
	{
		CreateDirectory(szRssReaderSrcPath, NULL);
		if (GetLastError() == ERROR_PATH_NOT_FOUND)
		{
			MessageBox(hwnd, TEXT("RssReader\\src���ļ����޷��������м��ļ��ж�ʧ����"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			free(cszRssReaderSrcPath);
			free(szRssReaderPath);
			return false;
		}
	}
	//Rss�Ķ�����ԴĿ¼���ں�ʼд��ֵ��
	//����д��RssFeedList��
	TCHAR szRssFeedPath[MAX_PATH] = { NULL };
	lstrcat(szRssFeedPath, szRssReaderSrcPath);
	lstrcat(szRssFeedPath, TEXT("\\RssFeed.txt"));
	//���ļ����в�����
	ofstream outputFileRssFeed;
	char* cszRssFeedFileName = FileEncode::UnicodeToAnsi((char*)szRssFeedPath);
	outputFileRssFeed.open(cszRssFeedFileName, ios::binary);
	free(cszRssFeedFileName);
	if (!outputFileRssFeed.is_open())
	{
		MessageBox(hwnd, TEXT("����RssFeed�ļ�ʧ�ܣ���"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return false;
	}
	//��ʼд��RssFeedList���ݡ�
	int iRssFeedSize = RssFeedList.size();
	for (int index = 0; index < iRssFeedSize; index++)
	{
		outputFileRssFeed << RssFeedList[index];
		if (index != iRssFeedSize - 1)
		{
			outputFileRssFeed << "\r\n";
		}
	}
	outputFileRssFeed.close();//RssFeed��Ϣд����ɡ�
	//������д��EssFeedLink����Ϣ��
	TCHAR szRssFeedLinkPath[MAX_PATH] = { NULL };
	lstrcat(szRssFeedLinkPath, szRssReaderSrcPath);
	lstrcat(szRssFeedLinkPath, TEXT("\\RssFeedLink.txt"));
	ofstream outputFileRssLink;
	char* cszRssLinkFileName = FileEncode::UnicodeToAnsi((char*)szRssFeedLinkPath);
	outputFileRssLink.open(cszRssLinkFileName, ios::binary);
	free(cszRssLinkFileName);
	if (!outputFileRssLink.is_open())
	{
		MessageBox(hwnd, TEXT("����RssFeedLink�ļ�ʧ�ܣ���"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return false;
	}
	int iRssLinkSize = RssFeedLinks.size();
	for (int index = 0; index < iRssLinkSize; index++)
	{
		outputFileRssLink << RssFeedLinks[index];
		if (index != iRssLinkSize - 1)
		{
			outputFileRssLink << "\r\n";
		}
	}
	outputFileRssLink.close();
	//������д��ÿ��RssFeed��Ӧ��RssItem��Ϣ ��RssItemList
	TCHAR szRssItemPath[MAX_PATH] = { NULL };
	lstrcat(szRssItemPath, szRssReaderSrcPath);
	lstrcat(szRssItemPath, TEXT("\\RssItem.txt"));
	ofstream outputFileRssItem;
	char* cszRssItemFileName = FileEncode::UnicodeToAnsi((char*)szRssItemPath);
	outputFileRssItem.open(cszRssItemFileName, ios::binary);
	if (!outputFileRssItem.is_open())
	{
		MessageBox(hwnd, TEXT("����RssItem�ļ�ʧ�ܣ���"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return false;
	}
	//��������ʼд�롣
	int iRssItemSize = RssItemList.size();
	for (int index = 0; index < iRssItemSize; index++)
	{
		//ȡ�����е�һ��map��
		map<string, RssContent> rssItemMap = RssItemList[index];
		//д���map��
		for (map<string, RssContent>::iterator iterator = rssItemMap.begin();
			iterator != rssItemMap.end(); iterator++)
		{
			outputFileRssItem << iterator->first;//д��RssItem�ı��⡣
			outputFileRssItem << " !@#$%^&*()";//��ʾ����
			outputFileRssItem << "\r\n";
			outputFileRssItem << iterator->second.Web;//д���RssItem����ϸ���ӵ�ַ��
			outputFileRssItem << " !@#$%^&*()";
			outputFileRssItem << "\r\n";
			outputFileRssItem << iterator->second.Date;
			outputFileRssItem << " !@#$%^&*()";
			outputFileRssItem << "\r\n";
			outputFileRssItem << iterator->second.Description;
			outputFileRssItem << " !@#$%^&*()";
			outputFileRssItem << "\r\n";
		}
		outputFileRssItem << "MAPRSSFEDD";//�ָ���־��
		outputFileRssItem << "\r\n";
	}
	outputFileRssItem.close();
	return true;//д��ɹ�����ʼ���롣
}

//��ȡRss�Ķ����ѱ������Ϣ����ʼ��Rss�Ķ�����
//extern vector<string> RssFeedList;//���游��㡣
//extern vector< map<string, RssContent> > RssItemList;//����Rss��ÿһ���ӽڵ�����ݡ�
bool InitRssMsg(HWND hwnd)
{
	TCHAR szRssReaderPath[MAX_PATH] = { NULL };
	TCHAR szAppRunPath[MAX_PATH] = { NULL };
	GetAppRunPath(hwnd, szAppRunPath);
	lstrcpy(szRssReaderPath, szAppRunPath);
	//�����Ķ�����Ϣ�洢���ļ��С�
	lstrcat(szRssReaderPath, TEXT("RssReader"));
	//�����жϸ��ļ����Ƿ���ڡ�
	char* cszRssReaderPath = NULL;//���ֵҪ�ֶ��ͷš�
	cszRssReaderPath = FileEncode::UnicodeToAnsi((char*)szRssReaderPath);
	//Rss�Ķ���������Ŀ¼��
	//ERROR_PATH_NOT_FOUND
	//One or more intermediate directories do not exist; this function will only create the final directory in the path. 
	if (!bIsAppFirseUse)//�û����ǵ�һ��ʹ�ø������ʱ�򣬽������²�����
	{
		if ((_access(cszRssReaderPath, 0) == -1))//������ļ��в����ڡ�
		{
			CreateDirectory(szRssReaderPath, NULL);
			if (GetLastError() == ERROR_PATH_NOT_FOUND)
			{
				MessageBox(hwnd, TEXT("RssReader���ļ����޷��������м��ļ��ж�ʧ����"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
				free(cszRssReaderPath);
				return false;
			}
		}
		//RSS�Ķ�������ԴĿ¼��
		TCHAR szRssReaderSrcPath[MAX_PATH] = { NULL };
		lstrcat(szRssReaderSrcPath, szRssReaderPath);
		lstrcat(szRssReaderSrcPath, TEXT("\\src"));
		char* cszRssReaderSrcPath = FileEncode::UnicodeToAnsi((char*)szRssReaderSrcPath);
		if (_access(cszRssReaderSrcPath, 0) == -1)
		{
			CreateDirectory(szRssReaderSrcPath, NULL);
			if (GetLastError() == ERROR_PATH_NOT_FOUND)
			{
				MessageBox(hwnd, TEXT("RssReader\\src���ļ����޷��������м��ļ��ж�ʧ����"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
				free(cszRssReaderSrcPath);
				free(szRssReaderPath);
				return false;
			}
		}
		//Rss�Ķ�����ԴĿ¼���ں�ʼ����ֵ��
		//���ȶ�ȡRssFeedList��
		TCHAR szRssFeedPath[MAX_PATH] = { NULL };
		lstrcat(szRssFeedPath, szRssReaderSrcPath);
		lstrcat(szRssFeedPath, TEXT("\\RssFeed.txt"));
		ifstream inputFileRssFeed;
		char* cszRssFeedFileName = FileEncode::UnicodeToAnsi((char*)szRssFeedPath);
		inputFileRssFeed.open(cszRssFeedFileName, ios::binary);
		free(cszRssFeedFileName);
		if (!inputFileRssFeed.is_open())
		{
			MessageBox(hwnd, TEXT("����RssFeed�ļ���ʧ���Ķ������ص���ʼ״̬����"), TEXT("Message"), MB_OK);
			return false;
		}
		//��ʼ��ȡ���ݡ�
		while (!inputFileRssFeed.eof())//δ���ļ�ĩβʱ��
		{
			string tempString;
			inputFileRssFeed >> tempString;
			RssFeedList.push_back(tempString);
		}
		inputFileRssFeed.close();
		//�������������Ӧ��RssFeedLink����Ϣ��
		TCHAR szRssLinkPath[MAX_PATH] = { NULL };
		lstrcat(szRssLinkPath, szRssReaderSrcPath);
		lstrcat(szRssLinkPath, TEXT("\\RssFeedLink.txt"));
		ifstream inputFileRssLink;
		char* cszRssLinkFileName = FileEncode::UnicodeToAnsi((char*)szRssLinkPath);
		inputFileRssLink.open(cszRssLinkFileName, ios::binary);
		free(cszRssLinkFileName);
		if (!inputFileRssLink.is_open())
		{
			MessageBox(hwnd, TEXT("����RssFeedLink�ļ���ʧ���Ķ������ص���ʼ״̬����"), TEXT("Message"), MB_OK);
			return false;
		}
		while (!inputFileRssLink.eof())
		{
			string tempString;
			inputFileRssLink >> tempString;
			RssFeedLinks.push_back(tempString);
		}
		inputFileRssLink.close();


		//��������ȡÿ��RssFeed��Ӧ��RssItem��Ϣ ��RssItemList
		TCHAR szRssItemPath[MAX_PATH] = { NULL };
		lstrcat(szRssItemPath, szRssReaderSrcPath);
		lstrcat(szRssItemPath, TEXT("\\RssItem.txt"));
		ifstream inputFileRssItem;
		char* cszRssItemPath = FileEncode::UnicodeToAnsi((char*)szRssItemPath);
		inputFileRssItem.open(cszRssItemPath, ios::binary);
		free(cszRssItemPath);
		if (!inputFileRssItem.is_open())
		{
			MessageBox(hwnd, TEXT("����RssItem�ļ���ʧ���Ķ������ص���ʼ״̬����"), TEXT("Message"), MB_OK);
			RssFeedList.clear();
			return false;
		}
		//��������ʼ��ȡ���ݡ�	//�����д�롣
		int indexFlag = 0;//�����ʱ����Ĳ���д�룬������ʱ��ҲҪ���Ĳ��ֶ�����
		string rssItemTitle;
		RssContent rssContent;
		//��ʱ�洢���ݵ�map��
		map<string, RssContent> rssItemMap;
		while (!inputFileRssItem.eof())//δ���ļ�ĩβ��ʱ��
		{
			//��ʱ�洢���ݵı�����
			string tempString, tempMsgString;
			//��ʼ���롣
			inputFileRssItem >> tempString;
			if (tempString == "MAPRSSFEDD")
			{
				indexFlag = 0;
				//�����map�ŵ�RssItemList���档
				RssItemList.push_back(rssItemMap);
				rssItemMap.clear();
				continue;//Ȼ��������롣
			}
			while ((tempString != "!@#$%^&*()") && (!inputFileRssItem.eof()))//��ʶ����
			{
				tempMsgString = tempMsgString + tempString + " ";
				inputFileRssItem >> tempString;//(!inputFileRssItem.eof())�����ʱ��Ҫ�������Ϊ
				//�ڶ������ļ����� ������������(tempString != "!@#$%^&*()")  ��ô���������ѭ����
			}
			indexFlag++;
			switch (indexFlag)
			{
			case 1:
			{
				rssItemTitle = tempMsgString;
				tempMsgString = "";
			}
				break;
			case 2:
			{
				rssContent.Web = tempMsgString;
				tempMsgString = "";
			}
				break;
			case 3:
			{
				rssContent.Date = tempMsgString;
				tempMsgString = "";
			}
				break;
			case 4:
			{
				rssContent.Description = tempMsgString;
				tempMsgString = "";
				//����һ�����ݲ���map��
				rssItemMap.insert(pair<string, RssContent>(rssItemTitle, rssContent));
				indexFlag = 0;
			}
				break;
			default:
				break;
			}
		}
		inputFileRssItem.close();//�����ļ��ɹ���
		//����RssFeed���б��м����Ӧ���
		//�ڴ�֮ǰ�ȼ����еĴ��ڡ�
		ClearRssListBlank(hwnd, IDC_RssFeedList);
		int iRssFeedCount = RssFeedList.size();
		for (int index = 0; index < iRssFeedCount; index++)
		{
			TCHAR* szRssFeedTitle = FileEncode::UTF8ToUnicode((char*)RssFeedList[index].c_str());
			SendMessage(GetDlgItem(hwnd, IDC_RssFeedList), LB_INSERTSTRING, (WPARAM)-1, (LPARAM)szRssFeedTitle);
			free(szRssFeedTitle);
		}
	}
	return true;
}