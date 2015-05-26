/*
*学号：1252878
*姓名：许舰
*联系电话：18817870106
*电邮地址：1252878xujian@tongji.edu.cn
*文件描述：Rss阅读器相关内容写入本地磁盘的实现
*/


#include"resource.h"
#include<Windows.h>
#include<windowsx.h>
#include<CommCtrl.h>
#include"MusicFunction.h"
#include"MusicMessage.h"
#include"MusicTabDlg.h"
#include<stdlib.h>
#include<io.h>
#include"FileEncode.h"


//初始化rssXML文件的暂时保存路径，确保该暂时文件存在并可以解析。HWND 父窗口的句柄。
//该函数将暂存的RssXml的文件全路径返回（通过传参数的方式）。\\RssReader\\src\\TempRssXml.txt
bool InitRssXMLTempSavePath(HWND hwnd, TCHAR* szTempRssXmlPath);

//保存Rss阅读器已保存的信息。
//extern vector<string> RssFeedList;//保存父结点。
//extern vector< map<string, RssContent> > RssItemList;//保存Rss下每一个子节点的内容。
bool SaveRssMsg(HWND hwnd);

//读取Rss阅读器已保存的信息，初始化Rss阅读器。
//extern vector<string> RssFeedList;//保存父结点。
//extern vector< map<string, RssContent> > RssItemList;//保存Rss下每一个子节点的内容。
bool InitRssMsg(HWND hwnd);