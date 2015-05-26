/*
*学号：1252878
*姓名：许舰
*联系电话：18817870106
*电邮地址：1252878xujian@tongji.edu.cn
*文件描述：Rss站点信息、条目信息、及相关操作
*/


#pragma once
#include"resource.h"
#include"RssTabDlg.h"
#include"FileEncode.h"
#include"InitMusicPlayer.h"
#include"MainDlg.h"
#include"MainDlgFunc.h"
#include"RssFunction.h"

class RssMessage
{
public:
	//得到的是正常的下标，就是不包括空行的。
	static bool SetCurRssFeedIndex(int index);
	static int GetCurRssFeedIndex();
	static bool SetCurRssItemIndex(int index);
	static int GetCurRssItemIndex();
	//设置初始化RssMessage的相关信息。
	static bool InitRssMessage(HWND hwnd,int rssFeedListId,int rssItemListID);
	//设置RssTab窗口的句柄。
	static bool SetRssWindowHwnd(HWND hwnd);
	//获得RssTab窗口的句柄。
	static HWND GetRssWindowHwnd();
	//设置RssItem列表的句柄。
	static bool SetRssItemListWnd(HWND hwnd);
	//取出RssItem列表的句柄。
	static HWND GetRssItemListWnd();
	//设置增加RssFeed窗口的句柄。
	static bool SetRssFeedSearchWnd(HWND hwnd);
	//获得增加RssFeed窗口的句柄。
	static HWND GetRssFeedSearchWnd();
	//设置查找RssItem窗口的句柄。
	static bool SetRssItemSearchWnd(HWND hwnd);
	//获得查找RssItem窗口的句柄。
	static HWND GetRssItemSearchWnd();
	//从RssItem的列表中查找特定的RssItem。
	//返回值是查找到的下标，没有查找到，那么返回-1。
	//输入的是要搜索的信息。
	static int FindRssItemFromList(TCHAR* szSearchItemMsg);
private:
	static int iCurselRssfeed;
	static int iCurselRssItem;
	static HWND hRssWindowWnd;
	static HWND hRssFeedSearchWnd;
	static HWND hRssItemSearchWnd;
	static HWND hRssFeedListWnd;
	static HWND hRssItemListWnd;
};