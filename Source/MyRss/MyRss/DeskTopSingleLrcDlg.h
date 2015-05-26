/*
*学号：1252878
*姓名：许舰
*联系电话：18817870106
*电邮地址：1252878xujian@tongji.edu.cn
*文件描述：桌面歌词显示界面头文件
*/

#include"resource.h"
#include"MusicTabDlg.h"
#include<Windows.h>
#include<windowsx.h>
#include<CommCtrl.h>
#include"DeskTopSingleLrcDlgF.h"


BOOL CALLBACK DeskTop_SingleLrcDlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//显示歌词的计时器。
VOID CALLBACK DesktopLrcSingle_TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);