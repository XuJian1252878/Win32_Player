/*
*学号：1252878
*姓名：许舰
*联系电话：18817870106
*电邮地址：1252878xujian@tongji.edu.cn
*文件描述：Rss阅读器界面的实现
*/

#pragma once

#include"resource.h"
#include<Windows.h>
#include<windowsx.h>
#include<CommCtrl.h>//与Tab控件的显示有关。
#include"InitRssReader.h"
#include"RssFunction.h"
#include"RssSearchMsgDlg.h"
#include"RssMessage.h"
#include"MainDlg.h"
#include"SearchRssItemDlg.h"


//主界面Tab选项卡中RSS新闻阅读的界面的窗口过程函数。
BOOL CALLBACK MainDlg_RSS_Proc(HWND hwnd, UINT uMeg, WPARAM wParam, LPARAM lParam);