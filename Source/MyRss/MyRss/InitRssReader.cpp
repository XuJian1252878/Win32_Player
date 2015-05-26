/*
*学号：1252878
*姓名：许舰
*联系电话：18817870106
*电邮地址：1252878xujian@tongji.edu.cn
*文件描述：Rss阅读器相关内容写入本地磁盘的实现
*/


#include"stdafx.h"
#include"InitRssReader.h"

extern vector<string> RssFeedList;//保存父结点。
extern vector<string> RssFeedLinks;//保存RssFeed的链接信息。
extern vector< map<string, RssContent> > RssItemList;//保存Rss下每一个子节点的内容。
extern bool bIsAppFirseUse;

//初始化rssXML文件的暂时保存路径，确保该暂时文件存在并可以解析。HWND 父窗口的句柄。
//该函数将暂存的RssXml的文件全路径返回（通过传参数的方式）。\\RssReader\\src\\TempRssXml.txt
bool InitRssXMLTempSavePath(HWND hwnd, TCHAR* szTempRssXmlPath)
{
	TCHAR szRssReaderPath[MAX_PATH] = { NULL };
	TCHAR szAppRunPath[MAX_PATH] = { NULL };
	GetAppRunPath(hwnd, szAppRunPath);
	lstrcpy(szRssReaderPath, szAppRunPath);
	//构造播放器信息存储的文件夹。
	lstrcat(szRssReaderPath, TEXT("RssReader"));
	//首先判断该文件夹是否存在。
	/*	When used with files, the _access function determines whether the specified file exists
	and can be accessed as specified by the value of mode. When used with directories, _access
	determines only whether the specified directory exists; in Windows NT, all directories have read and write access.
	mode Value            Checks File For
	00                              Existence only
	02                              Write permission
	04                              Read permission
	06                              Read and write permission*/
	char* cszRssReaderPath = NULL;//这个值要手动释放。
	cszRssReaderPath = FileEncode::UnicodeToAnsi((char*)szRssReaderPath);
	//Rss阅读器设置总目录。
	//ERROR_PATH_NOT_FOUND
	//One or more intermediate directories do not exist; this function will only create the final directory in the path. 
	if ((_access(cszRssReaderPath, 0) == -1))//如果该文件夹不存在。
	{
		CreateDirectory(szRssReaderPath, NULL);
		if (GetLastError() == ERROR_PATH_NOT_FOUND)
		{
			MessageBox(hwnd, TEXT("RssReader子文件夹无法创建，中间文件夹丢失！！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			free(cszRssReaderPath);
			return false;
		}
	}
	//RSS阅读器的资源目录。
	TCHAR szRssReaderSrcPath[MAX_PATH] = { NULL };
	lstrcat(szRssReaderSrcPath, szRssReaderPath);
	lstrcat(szRssReaderSrcPath, TEXT("\\src"));
	char* cszRssReaderSrcPath = FileEncode::UnicodeToAnsi((char*)szRssReaderSrcPath);
	if (_access(cszRssReaderSrcPath, 0) == -1)
	{
		CreateDirectory(szRssReaderSrcPath, NULL);
		if (GetLastError() == ERROR_PATH_NOT_FOUND)
		{
			MessageBox(hwnd, TEXT("RssReader\\src子文件夹无法创建，中间文件夹丢失！！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			free(cszRssReaderSrcPath);
			free(szRssReaderPath);
			return false;
		}
	}
	//存储暂时RSSXML的文件。
	lstrcat(szTempRssXmlPath, szRssReaderSrcPath);
	lstrcat(szTempRssXmlPath, TEXT("\\TempRssXml.txt"));
	return true;
}


//保存Rss阅读器已保存的信息。
//extern vector<string> RssFeedList;//保存父结点。
//extern vector< map<string, RssContent> > RssItemList;//保存Rss下每一个子节点的内容。
bool SaveRssMsg(HWND hwnd)
{
	TCHAR szRssReaderPath[MAX_PATH] = { NULL };
	TCHAR szAppRunPath[MAX_PATH] = { NULL };
	GetAppRunPath(hwnd, szAppRunPath);
	lstrcpy(szRssReaderPath, szAppRunPath);
	//构造播放器信息存储的文件夹。
	lstrcat(szRssReaderPath, TEXT("RssReader"));
	//首先判断该文件夹是否存在。
	char* cszRssReaderPath = NULL;//这个值要手动释放。
	cszRssReaderPath = FileEncode::UnicodeToAnsi((char*)szRssReaderPath);
	//Rss阅读器设置总目录。
	//ERROR_PATH_NOT_FOUND
	//One or more intermediate directories do not exist; this function will only create the final directory in the path. 
	if ((_access(cszRssReaderPath, 0) == -1))//如果该文件夹不存在。
	{
		CreateDirectory(szRssReaderPath, NULL);
		if (GetLastError() == ERROR_PATH_NOT_FOUND)
		{
			MessageBox(hwnd, TEXT("RssReader子文件夹无法创建，中间文件夹丢失！！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			free(cszRssReaderPath);
			return false;
		}
	}
	//RSS阅读器的资源目录。
	TCHAR szRssReaderSrcPath[MAX_PATH] = { NULL };
	lstrcat(szRssReaderSrcPath, szRssReaderPath);
	lstrcat(szRssReaderSrcPath, TEXT("\\src"));
	char* cszRssReaderSrcPath = FileEncode::UnicodeToAnsi((char*)szRssReaderSrcPath);
	if (_access(cszRssReaderSrcPath, 0) == -1)
	{
		CreateDirectory(szRssReaderSrcPath, NULL);
		if (GetLastError() == ERROR_PATH_NOT_FOUND)
		{
			MessageBox(hwnd, TEXT("RssReader\\src子文件夹无法创建，中间文件夹丢失！！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			free(cszRssReaderSrcPath);
			free(szRssReaderPath);
			return false;
		}
	}
	//Rss阅读器资源目录存在后开始写入值。
	//首先写入RssFeedList。
	TCHAR szRssFeedPath[MAX_PATH] = { NULL };
	lstrcat(szRssFeedPath, szRssReaderSrcPath);
	lstrcat(szRssFeedPath, TEXT("\\RssFeed.txt"));
	//打开文件进行操作。
	ofstream outputFileRssFeed;
	char* cszRssFeedFileName = FileEncode::UnicodeToAnsi((char*)szRssFeedPath);
	outputFileRssFeed.open(cszRssFeedFileName, ios::binary);
	free(cszRssFeedFileName);
	if (!outputFileRssFeed.is_open())
	{
		MessageBox(hwnd, TEXT("保存RssFeed文件失败！！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return false;
	}
	//开始写入RssFeedList数据。
	int iRssFeedSize = RssFeedList.size();
	for (int index = 0; index < iRssFeedSize; index++)
	{
		outputFileRssFeed << RssFeedList[index];
		if (index != iRssFeedSize - 1)
		{
			outputFileRssFeed << "\r\n";
		}
	}
	outputFileRssFeed.close();//RssFeed信息写入完成。
	//接下来写入EssFeedLink的信息。
	TCHAR szRssFeedLinkPath[MAX_PATH] = { NULL };
	lstrcat(szRssFeedLinkPath, szRssReaderSrcPath);
	lstrcat(szRssFeedLinkPath, TEXT("\\RssFeedLink.txt"));
	ofstream outputFileRssLink;
	char* cszRssLinkFileName = FileEncode::UnicodeToAnsi((char*)szRssFeedLinkPath);
	outputFileRssLink.open(cszRssLinkFileName, ios::binary);
	free(cszRssLinkFileName);
	if (!outputFileRssLink.is_open())
	{
		MessageBox(hwnd, TEXT("保存RssFeedLink文件失败！！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
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
	//接下来写入每个RssFeed对应的RssItem信息 即RssItemList
	TCHAR szRssItemPath[MAX_PATH] = { NULL };
	lstrcat(szRssItemPath, szRssReaderSrcPath);
	lstrcat(szRssItemPath, TEXT("\\RssItem.txt"));
	ofstream outputFileRssItem;
	char* cszRssItemFileName = FileEncode::UnicodeToAnsi((char*)szRssItemPath);
	outputFileRssItem.open(cszRssItemFileName, ios::binary);
	if (!outputFileRssItem.is_open())
	{
		MessageBox(hwnd, TEXT("保存RssItem文件失败！！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return false;
	}
	//接下来开始写入。
	int iRssItemSize = RssItemList.size();
	for (int index = 0; index < iRssItemSize; index++)
	{
		//取出其中的一个map。
		map<string, RssContent> rssItemMap = RssItemList[index];
		//写入该map。
		for (map<string, RssContent>::iterator iterator = rssItemMap.begin();
			iterator != rssItemMap.end(); iterator++)
		{
			outputFileRssItem << iterator->first;//写入RssItem的标题。
			outputFileRssItem << " !@#$%^&*()";//标示符。
			outputFileRssItem << "\r\n";
			outputFileRssItem << iterator->second.Web;//写入该RssItem的详细链接地址。
			outputFileRssItem << " !@#$%^&*()";
			outputFileRssItem << "\r\n";
			outputFileRssItem << iterator->second.Date;
			outputFileRssItem << " !@#$%^&*()";
			outputFileRssItem << "\r\n";
			outputFileRssItem << iterator->second.Description;
			outputFileRssItem << " !@#$%^&*()";
			outputFileRssItem << "\r\n";
		}
		outputFileRssItem << "MAPRSSFEDD";//分隔标志。
		outputFileRssItem << "\r\n";
	}
	outputFileRssItem.close();
	return true;//写入成功，开始读入。
}

//读取Rss阅读器已保存的信息，初始化Rss阅读器。
//extern vector<string> RssFeedList;//保存父结点。
//extern vector< map<string, RssContent> > RssItemList;//保存Rss下每一个子节点的内容。
bool InitRssMsg(HWND hwnd)
{
	TCHAR szRssReaderPath[MAX_PATH] = { NULL };
	TCHAR szAppRunPath[MAX_PATH] = { NULL };
	GetAppRunPath(hwnd, szAppRunPath);
	lstrcpy(szRssReaderPath, szAppRunPath);
	//构造阅读器信息存储的文件夹。
	lstrcat(szRssReaderPath, TEXT("RssReader"));
	//首先判断该文件夹是否存在。
	char* cszRssReaderPath = NULL;//这个值要手动释放。
	cszRssReaderPath = FileEncode::UnicodeToAnsi((char*)szRssReaderPath);
	//Rss阅读器设置总目录。
	//ERROR_PATH_NOT_FOUND
	//One or more intermediate directories do not exist; this function will only create the final directory in the path. 
	if (!bIsAppFirseUse)//用户不是第一次使用该软件的时候，进行如下操作。
	{
		if ((_access(cszRssReaderPath, 0) == -1))//如果该文件夹不存在。
		{
			CreateDirectory(szRssReaderPath, NULL);
			if (GetLastError() == ERROR_PATH_NOT_FOUND)
			{
				MessageBox(hwnd, TEXT("RssReader子文件夹无法创建，中间文件夹丢失！！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
				free(cszRssReaderPath);
				return false;
			}
		}
		//RSS阅读器的资源目录。
		TCHAR szRssReaderSrcPath[MAX_PATH] = { NULL };
		lstrcat(szRssReaderSrcPath, szRssReaderPath);
		lstrcat(szRssReaderSrcPath, TEXT("\\src"));
		char* cszRssReaderSrcPath = FileEncode::UnicodeToAnsi((char*)szRssReaderSrcPath);
		if (_access(cszRssReaderSrcPath, 0) == -1)
		{
			CreateDirectory(szRssReaderSrcPath, NULL);
			if (GetLastError() == ERROR_PATH_NOT_FOUND)
			{
				MessageBox(hwnd, TEXT("RssReader\\src子文件夹无法创建，中间文件夹丢失！！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
				free(cszRssReaderSrcPath);
				free(szRssReaderPath);
				return false;
			}
		}
		//Rss阅读器资源目录存在后开始读入值。
		//首先读取RssFeedList。
		TCHAR szRssFeedPath[MAX_PATH] = { NULL };
		lstrcat(szRssFeedPath, szRssReaderSrcPath);
		lstrcat(szRssFeedPath, TEXT("\\RssFeed.txt"));
		ifstream inputFileRssFeed;
		char* cszRssFeedFileName = FileEncode::UnicodeToAnsi((char*)szRssFeedPath);
		inputFileRssFeed.open(cszRssFeedFileName, ios::binary);
		free(cszRssFeedFileName);
		if (!inputFileRssFeed.is_open())
		{
			MessageBox(hwnd, TEXT("本地RssFeed文件丢失，阅读器将回到初始状态！！"), TEXT("Message"), MB_OK);
			return false;
		}
		//开始读取数据。
		while (!inputFileRssFeed.eof())//未到文件末尾时。
		{
			string tempString;
			inputFileRssFeed >> tempString;
			RssFeedList.push_back(tempString);
		}
		inputFileRssFeed.close();
		//接下来读入相对应的RssFeedLink的信息。
		TCHAR szRssLinkPath[MAX_PATH] = { NULL };
		lstrcat(szRssLinkPath, szRssReaderSrcPath);
		lstrcat(szRssLinkPath, TEXT("\\RssFeedLink.txt"));
		ifstream inputFileRssLink;
		char* cszRssLinkFileName = FileEncode::UnicodeToAnsi((char*)szRssLinkPath);
		inputFileRssLink.open(cszRssLinkFileName, ios::binary);
		free(cszRssLinkFileName);
		if (!inputFileRssLink.is_open())
		{
			MessageBox(hwnd, TEXT("本地RssFeedLink文件丢失，阅读器将回到初始状态！！"), TEXT("Message"), MB_OK);
			return false;
		}
		while (!inputFileRssLink.eof())
		{
			string tempString;
			inputFileRssLink >> tempString;
			RssFeedLinks.push_back(tempString);
		}
		inputFileRssLink.close();


		//接下来读取每个RssFeed对应的RssItem信息 即RssItemList
		TCHAR szRssItemPath[MAX_PATH] = { NULL };
		lstrcat(szRssItemPath, szRssReaderSrcPath);
		lstrcat(szRssItemPath, TEXT("\\RssItem.txt"));
		ifstream inputFileRssItem;
		char* cszRssItemPath = FileEncode::UnicodeToAnsi((char*)szRssItemPath);
		inputFileRssItem.open(cszRssItemPath, ios::binary);
		free(cszRssItemPath);
		if (!inputFileRssItem.is_open())
		{
			MessageBox(hwnd, TEXT("本地RssItem文件丢失，阅读器将回到初始状态！！"), TEXT("Message"), MB_OK);
			RssFeedList.clear();
			return false;
		}
		//接下来开始读取数据。	//分情况写入。
		int indexFlag = 0;//读入的时候分四部分写入，读出的时候也要分四部分读出。
		string rssItemTitle;
		RssContent rssContent;
		//暂时存储数据的map。
		map<string, RssContent> rssItemMap;
		while (!inputFileRssItem.eof())//未到文件末尾的时候。
		{
			//暂时存储数据的变量。
			string tempString, tempMsgString;
			//开始读入。
			inputFileRssItem >> tempString;
			if (tempString == "MAPRSSFEDD")
			{
				indexFlag = 0;
				//把这个map放到RssItemList里面。
				RssItemList.push_back(rssItemMap);
				rssItemMap.clear();
				continue;//然后继续读入。
			}
			while ((tempString != "!@#$%^&*()") && (!inputFileRssItem.eof()))//标识符。
			{
				tempMsgString = tempMsgString + tempString + " ";
				inputFileRssItem >> tempString;//(!inputFileRssItem.eof())读入的时候要加这个因为
				//在读入是文件结束 而条件仅仅是(tempString != "!@#$%^&*()")  那么将会出现死循环。
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
				//将这一对数据插入map。
				rssItemMap.insert(pair<string, RssContent>(rssItemTitle, rssContent));
				indexFlag = 0;
			}
				break;
			default:
				break;
			}
		}
		inputFileRssItem.close();//读入文件成功。
		//在向RssFeed的列表中加入对应的项。
		//在此之前先检查空行的存在。
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