/*
*文件描述：音乐播放子窗口界面相关处理
*/

#pragma once
#include"resource.h"
#include<Windows.h>
#include<windowsx.h>
#include<CommCtrl.h>//与Tab控件的显示有关。
#include"MusicFunction.h"
#include"MusicMessage.h"
#include"MainDlgFunc.h"
#include"DeskTopSingleLrcDlg.h"
#include"MusicMessage.h"
#include"InitMusicPlayer.h"
#include"MusicVolumeDlg.h"
#include"SearchEditDlg.h"
#include<shellapi.h>
#pragma comment(lib,"shell32.lib")

//这个是音乐播放完成之后发送给MusicTabDlg的消息。
#define WM_MUSICEND WM_USER+1

#define TIMER_PLAYMUSIC 1//播放音乐的定时器的ID。
#define TIMER_DISPLAYMUSICLRC 2//显示歌词定时器的ID。
#define TIMER_DISPLAYDESKTOPSINGLELRC 3//显示桌面的单行歌词计时器。

//主界面Tab选项卡中音乐播放器界面的窗口过程函数。
BOOL CALLBACK MainDlg_MusicPlayer_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
