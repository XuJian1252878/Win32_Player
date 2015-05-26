/*
*学号：1252878
*姓名：许舰
*联系电话：18817870106
*电邮地址：1252878xujian@tongji.edu.cn
*文件描述：Rss阅读器Rss条目添加、删除等相关逻辑的实现
*/

#pragma once
#include"resource.h"
#include"MainDlg.h"
#include"MainDlgFunc.h"
#include"RssTabDlg.h"
#include<iostream>
#include<map>
#include<vector>
#include<string>
#include<fstream>
#include <algorithm>
#include"InitRssReader.h"
#include<Urlmon.h>//URLDownloadToFile
#include "tinyxml.h"
#include "tinystr.h"
#include <windows.h>
#include <atlstr.h>
#include"RssMessage.h"
using namespace std;

#define MAX_RSSMSGLEN 1024//定义rss消息字符的最大长度。
//结构体定义
struct RssContent
{
	string Web;
	string Date;
	string Description;
};

//向该界面的控件提供tip提示消息。
bool InitRssTabDlgCtrolTip(HWND hwnd);

//读取Rss文件的信息。
//传入参数 本地暂存文件的绝对路径。
bool ReadXmlFile(TCHAR* lpszTempRssXmlPath, string& rssFeedTitle, map<string, RssContent>& mXML);

//从链接中抓取Rss的XML数据。
bool GetRssMsgFromLink(HWND hwnd, TCHAR* szRssFeedLink);
//将RssTreeView与ImageList绑定,hwnd RssTreeView父窗口的句柄。
//bool InitRssTreeViewImageLists(HWND hwnd);
/*//向树列表中插入父结点，该父结点代表的是一个Rss链接，LPTSTR lpszItem是父结点表头的名字。
HTREEITEM AddRssFatherNode(HWND hwnd, LPTSTR lpszItem);
//把指定父结点包含的子节点插入树中。返回出入父结点的句柄。
bool AddRssChildNode(HWND hwnd, HTREEITEM hFatherNodeTreeItem, map<string, RssContent>& RssItemMsgMap);
//向树中加入一个RSS Feed那么父结点跟子节点一起加入。
//LPTSTR lpszItem 父结点（该Rss Feed项的名字），RssContent>& RssItemMsgMap 该父结点下的数据。
//内部调用了AddRssFatherNode AddRssChildNode
bool AddRssFeedToTree(HWND hwnd, LPTSTR lpszItem, map<string, RssContent>& RssItemMsgMap);*/


//重画RssList，HWND父窗口句柄，WPARAM 控件的ID，COLORREF notSelectItemRGB未选中的颜色，COLORREF selectItemRGB选中的颜色。
bool RedrawRssList(HWND hwnd, WPARAM iControlID, LPDRAWITEMSTRUCT lpDrawItemStruct,
	COLORREF notSelectItemRGB, COLORREF selectItemRGB);

//创建Rss内容的相关字体。
HFONT CreateRssMsgFont(HWND hwnd, BYTE lfCharSet, BYTE lfClipPrecision, TCHAR* lfFaceName, LONG lfHeight, BYTE lfQuality);

//初始化RssList的状态，使其背景能与主界面的背景一样。
//HWND Rss主界面的背景。
bool InitRssList(HWND hwnd);

//将选中的RssFeed向显示到RssItem的列表中。
//hwnd父窗口的句柄。iRssFeedIndex在RssFeed列表中的RssFeed的下标。
bool DisplayRssFeedToRssItemList(HWND hwnd, int iRssFeedIndex);

//将选中的RssItem详情显示出来。
//hwnd父窗口的句柄。iRssItemIndex在RssItem列表中的RssItem的下标。
bool DisplayRssItemMsg(HWND hwnd, int iRssItemIndex);

//清空RssList中的内容。
bool ClearRssListBlank(HWND hwnd, WPARAM listControlID);

//删除RssFeed列表里的一个选项。右键菜单，删除的是当前选中项。
bool DeleteRssFeedFromList(HWND hwnd);

//删除RssItem列表里的一个选项，右键菜单，删除的是当前选中项。
bool DeleteRssItemFromList(HWND hwnd);

//刷新RssFeed列表里面确定的某一项。
//HWND 父窗口的句柄，刷新RssFeed列表里面的某一项。
bool RefreshReeFeedInList(HWND hwnd);
