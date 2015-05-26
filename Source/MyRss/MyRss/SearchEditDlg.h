/*
*学号：1252878
*姓名：许舰
*联系电话：18817870106
*电邮地址：1252878xujian@tongji.edu.cn
*文件描述：音乐播放器搜索框的实现
*/

#include"resource.h"
#include<Windows.h>
#include<windowsx.h>
#include"MusicFunction.h"
#include"MusicMessage.h"
#include"MainDlg.h"
#include"RssTabDlg.h"
#include"RssFunction.h"
#include"FileEncode.h"


BOOL CALLBACK SearchEditDlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);