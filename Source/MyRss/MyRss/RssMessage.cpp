/*
*学号：1252878
*姓名：许舰
*联系电话：18817870106
*电邮地址：1252878xujian@tongji.edu.cn
*文件描述：Rss站点信息、条目信息、及相关操作
*/

#include"stdafx.h"
#include"RssMessage.h"

extern vector<string> RssFeedList;//保存父结点。
extern vector< map<string, RssContent> > RssItemList;//保存Rss下每一个子节点的内容。

int RssMessage::iCurselRssfeed = -1;
int RssMessage::iCurselRssItem = -1;
HWND RssMessage::hRssFeedSearchWnd = NULL;
HWND RssMessage::hRssItemSearchWnd = NULL;
HWND RssMessage::hRssWindowWnd = NULL;
HWND RssMessage::hRssFeedListWnd;
HWND RssMessage::hRssItemListWnd;

//得到的是正常的下标，
bool RssMessage::SetCurRssFeedIndex(int index)
{
	RssMessage::iCurselRssfeed = index;
	return true;
}
//得到的是正常的下标，
int RssMessage::GetCurRssFeedIndex()
{
	return RssMessage::iCurselRssfeed;
}
//得到的是正常的下标，
bool RssMessage::SetCurRssItemIndex(int index)
{
	RssMessage::iCurselRssItem = index;
	return true;
}
//得到的是正常的下标，
int RssMessage::GetCurRssItemIndex()
{
	return RssMessage::iCurselRssItem;
}

//设置增加RssFeed窗口的句柄。
bool RssMessage::SetRssFeedSearchWnd(HWND hwnd)
{
	RssMessage::hRssFeedSearchWnd = hwnd;
	return true;
}
//获得增加RssFeed窗口的句柄。
HWND RssMessage::GetRssFeedSearchWnd()
{
	return RssMessage::hRssFeedSearchWnd;
}
//设置查找RssItem窗口的句柄。
bool RssMessage::SetRssItemSearchWnd(HWND hwnd)
{
	RssMessage::hRssItemSearchWnd = hwnd;
	return true;
}
//获得查找RssItem窗口的句柄。
HWND RssMessage::GetRssItemSearchWnd()
{
	return RssMessage::hRssItemSearchWnd;
}


//设置初始化RssMessage的相关信息。
bool RssMessage::InitRssMessage(HWND hwnd, int rssFeedListId, int rssItemListID)
{
	RssMessage::hRssWindowWnd = hwnd;
	RssMessage::hRssFeedListWnd = GetDlgItem(hwnd, rssFeedListId);
	RssMessage::hRssItemListWnd = GetDlgItem(hwnd, rssItemListID);
	return true;
}

//设置RssTab窗口的句柄。
bool RssMessage::SetRssWindowHwnd(HWND hwnd)
{
	RssMessage::hRssWindowWnd = hwnd;
	return true;
}
//获得RssTab窗口的句柄。
HWND RssMessage::GetRssWindowHwnd()
{
	return RssMessage::hRssWindowWnd;
}

//设置RssItem列表的句柄。
bool RssMessage::SetRssItemListWnd(HWND hwnd)
{
//	RssMessage::hRssItemSearchWnd = hwnd;
	RssMessage::hRssItemListWnd = hwnd;
	return true;
}
//取出RssItem列表的句柄。
HWND RssMessage::GetRssItemListWnd()
{
	return RssMessage::hRssItemListWnd;
//	return RssMessage::hRssItemSearchWnd;
}

//从RssItem的列表中查找特定的RssItem。
//返回值是查找到的下标，没有查找到，那么返回-1。
//输入的是要搜索的信息。
int RssMessage::FindRssItemFromList(TCHAR* szSearchItemMsg)
{
	int iResult = -1;
	char* cszSearchItemMsg = FileEncode::UnicodeToAnsi((char*)szSearchItemMsg);
	//从RssItemList的map里面寻找字符串。
	if (RssMessage::GetCurRssFeedIndex() == -1)
	{
		return -1;
	}
	map<string,RssContent> rssItemMap = RssItemList[RssMessage::GetCurRssFeedIndex()];
	//由rssItemMap 遍历整个RssItem的Title。
	int index = 0;//记录查找到了哪一个下标。
	bool bFlag = false;
	for (map<string, RssContent>::iterator iterator = rssItemMap.begin();
		iterator != rssItemMap.end(); iterator++,++index)//前加和后加的区别？
	{
		char* cszRssItemTitle = FileEncode::UTF8ToAnsi((char*)(iterator->first.c_str()));
		//开始比较每一个取出的RssItem的Title。
		if (strstr(cszRssItemTitle, cszSearchItemMsg))
		{
			bFlag = true;
			free(cszRssItemTitle);
			break;
		}
		free(cszRssItemTitle);
	}
	if (bFlag)//找到特定的Title，那么指定下标。
	{
		iResult = index;
	}

	free(cszSearchItemMsg);
	SendMessage(GetDlgItem(RssMessage::GetRssWindowHwnd(), GetDlgCtrlID(RssMessage::GetRssItemListWnd())),
		LB_SETCURSEL, (WPARAM)iResult, (LPARAM)0);
	return iResult;
}