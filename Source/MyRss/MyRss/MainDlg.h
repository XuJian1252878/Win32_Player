/*
*文件描述：主窗口功能实现（Rss和Musicplayer界面切换的实现）
*/


#pragma once

#include"resource.h"
#include<Windows.h>
#include<windowsx.h>
#include<CommCtrl.h>//与Tab控件的显示有关。
#include"MainDlgFunc.h"
#include"InitMusicPlayer.h"
#include<locale.h>//跟setlocale有关。

//通知音量条显示窗口主窗口移动了，它也应该跟着移动。主界面发送给音量界面的消息。
//wparam 音量按钮窗口left坐标  lparam音量按钮窗口top坐标。
#define WM_MAINDLGMOVE_VolDlg WM_USER+2
#define WM_SHOWTRAY WM_USER+3//软件的系统托盘显示，以及托盘的右键菜单。
//跟WM_MAINDLGMOVE_VolDlg类似，提供给搜索条目窗口。
//wparam 音量窗口right坐标  lparam音量窗口top坐标。
#define WM_MAINDLGMOVE_SearchItemDlg WM_USER+4//跟WM_MAINDLGMOVE_VolDlg类似，提供给搜索条目窗口。
#define WM_SEARCHITEMMSG WM_USER+5//返回在搜索框输入的文字信息。wParam是字符串的头指针。lParam是该消息要返回给的控件的ID。
////wparam Rss按钮窗口right left坐标  lparamRss按钮窗口top坐标。
#define WM_MAINDLGMOVE_RssDlg WM_USER+6//跟WM_MAINDLGMOVE_VolDlg类似，提供给搜索条目窗口。
//提供的是主窗口的左上角坐标。wParam lParam
#define WM_MAINDLGMOVE_HideTab WM_USER+7//跟WM_MAINDLGMOVE_VolDlg类似,提供坐标给隐藏TAB的窗口移动

//主界面的主窗口过程函数。
BOOL CALLBACK MainDlg_Proc(HWND hwnd, UINT uMeg, WPARAM wParam, LPARAM lParam);
//隐藏TAB界面。
BOOL CALLBACK HideMainTab_Proc(HWND hwnd, UINT uMeg, WPARAM wParam, LPARAM lParam);





