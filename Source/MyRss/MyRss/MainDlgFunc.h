/*
*文件描述：主窗口功能实现（Rss和Musicplayer界面切换的实现等）
*/

#pragma once

#include"MainDlg.h"
#include"MusicTabDlg.h"
#include"RssTabDlg.h"
#include"MusicFunction.h"
#include<shellapi.h>//系统托盘图标。
#include"AppWelcomeDlg.h"

const int MODE_RSS_TAB = 0x123;
const int MODE_MUSICPLAYER_TAB = 0x124;

//加载窗体的Tab控件。
VOID LoadTabFrom(HWND hwnd, HWND* hMainDlgTab);
//将小图标添加到Tabs上。返回包含贴图的内存DC。
//BOOL BltImageToTabs(HDC hdc,int TAB_MODE,HBITMAP hBitmap,RECT rect);

//设置主对话框的背景。
/*hdc 主窗口的HDC
hBitmap 主窗口背景图片
rect  主窗口的矩形区域*/
HBRUSH GetBkgndImage(HWND hwnd, HBITMAP hBitmap, RECT rect);
//创建主界面底部的状态栏。返回状态栏的句柄，传入父窗口的句柄。
HWND CreateStatusBar(HWND hwnd);
/*获得指定大小的位图句柄
HWND 窗口句柄，由此获得绘图的HDC
HBITMAP：原位图
iWidth:需要转化的位图的宽度。
iHeight:需要转化的位图的高度*/
HBITMAP GetSuitableHbitmap(HWND hwnd, HBITMAP oriHbitmap, int iWidth, int iHeight);
/*将自绘控件上位图的背景色设为透明。
HWND 该控件的句柄，注意不是窗口的句柄。
COLORREF 被选作透明色的颜色。即位图上需要变为透明的颜色
函数无返回值。*/
void SetCtrolHbitmapTransparent(HWND hwnd, COLORREF crKey);

//获得该程序的运行路径。
bool GetAppRunPath(HWND hwnd,TCHAR* szAppRunPath);

//将程序显示到系统托盘，HWND hwnd父主窗口的句柄。
void AppToTray(HWND hwnd, NOTIFYICONDATA& notifyicondata);

//获得控件对应的画刷，与主界面背景相一致。
//hwnd父窗口 iControlID父窗口里的子控件，不把源位图删除。
HBRUSH GetControlBkgndImage(HWND hwnd, HBITMAP oriHbitmap, int iControlID);

//初始化界面控件的tip提示信息。hwnd是rss窗口的句柄。
bool InitMainDlgCtrolTip(HWND hwnd);

//初始化软件，现在主要是检测应用程序是否第一次被启动。然后做相应的动作。
//HWND 应用程序主窗口的句柄。
bool InitMyRssPlayer(HWND hwnd);