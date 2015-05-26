/*
*学号：1252878
*姓名：许舰
*联系电话：18817870106
*电邮地址：1252878xujian@tongji.edu.cn
*文件描述：欢迎界面的具体实现
*/

#include"stdafx.h"
#include"AppWelcomeDlg.h"

HBRUSH hAppWelcomeDlgBrush = NULL;//欢迎界面的背景画刷。
HWND hMainDlgWnd = NULL;//主界面的窗口句柄。
HFONT hWelcomeMsgFont = NULL;

BOOL CALLBACK AppWelcomeDlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HBITMAP hBitmap = (HBITMAP)LoadImage((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDB_AppWelcomeDlg_Bkgnd),
			IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_CREATEDIBSECTION | LR_SHARED);
		if (hBitmap == NULL)
		{
			MessageBox(hwnd, TEXT("加载欢迎界面图标失败！！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			return FALSE;
		}
		RECT welcomeDlgRect;
		GetClientRect(hwnd, &welcomeDlgRect);
		hAppWelcomeDlgBrush = GetBkgndImage(hwnd, hBitmap, welcomeDlgRect);
		DeleteObject(hBitmap);
		//创建欢迎窗口的字体。
		LOGFONT logfont;
		ZeroMemory(&logfont, sizeof(logfont));
		logfont.lfCharSet = GB2312_CHARSET;
		logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		logfont.lfHeight = 20;
		lstrcpy(logfont.lfFaceName, TEXT("楷体"));
		logfont.lfQuality = DEFAULT_QUALITY;
		hWelcomeMsgFont = CreateFontIndirect(&logfont);
		//获得主界面窗口句柄。
		hMainDlgWnd = (HWND)lParam;
		//显示程序的欢迎信息。
		TCHAR szWelcomeMsg[1024] = TEXT("欢迎使用MyRssPlayer!\r\n制作者：许舰_1252878\r\n日期：2014年12月26号");
		SetDlgItemText(hwnd, IDC_WelcomeEdit, szWelcomeMsg);
	}
		return FALSE;
	case WM_CTLCOLORDLG:
	{
		HDC hDlgDC = (HDC)wParam;
		HWND hDlgWnd = (HWND)lParam;
		SetBkMode(hDlgDC, TRANSPARENT);
	}
		return (BOOL)hAppWelcomeDlgBrush;
	case WM_CTLCOLORSTATIC:
	{
		HDC hStaticDC = (HDC)wParam;
		HWND hStaticWnd = (HWND)lParam;
		SetBkMode(hStaticDC, TRANSPARENT);
		SetTextColor(hStaticDC, RGB(255, 255, 0));
		SendMessage(hwnd, WM_SETFONT, (WPARAM)hWelcomeMsgFont, (LPARAM)FALSE);
	}
		return (BOOL)hAppWelcomeDlgBrush;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_EnterMyRssPlayer://用户点击了进入软件按钮。
		{
			ShowWindow(hMainDlgWnd, SW_SHOW);
			//之后通知主窗口重绘背景。
			SendMessage(hMainDlgWnd, WM_CTLCOLORDLG, (WPARAM)0, (LPARAM)0);
			DeleteObject(hAppWelcomeDlgBrush);
			DeleteObject(hWelcomeMsgFont);
			hMainDlgWnd = NULL;
			EndDialog(hwnd,0);//欢迎界面关闭。接下来主界面将显示(在函数InitMyRssPlayer中实现)。
		}
			return TRUE;
		default:
			return FALSE;
		}
	}
		return FALSE;
	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT lpDrawItemStruct = (LPDRAWITEMSTRUCT)lParam;
		switch (lpDrawItemStruct->CtlType)
		{
		case ODT_BUTTON:
		{
			DrawBkgndToButton(hwnd, wParam, lpDrawItemStruct);
		}
			return TRUE;
		default:
			return FALSE;
		}
	}
		return FALSE;
	case WM_DESTROY:
	{
		ShowWindow(hMainDlgWnd, SW_SHOW);
		//之后通知主窗口重绘背景。
		SendMessage(hMainDlgWnd, WM_CTLCOLORDLG, (WPARAM)0, (LPARAM)0);
		DeleteObject(hAppWelcomeDlgBrush);
		DeleteObject(hWelcomeMsgFont);
		hMainDlgWnd = NULL;
		EndDialog(hwnd, 0);
	}
		return TRUE;
	}
	return FALSE;
}