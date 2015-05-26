/*
*学号：1252878
*姓名：许舰
*联系电话：18817870106
*电邮地址：1252878xujian@tongji.edu.cn
*文件描述：Rss详细新闻条目搜索框的实现
*/

#include"stdafx.h"
#include"SearchRssItemDlg.h"

int iWinWidth = 0;
int iWinHeight = 0;

BOOL CALLBACK SearchRssItemDlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		//设置窗口的透明属性。
		DWORD dwExStyle = GetWindowLong(hwnd, GWL_USERDATA);
		dwExStyle |= WS_EX_LAYERED;
		SetWindowLong(hwnd, GWL_USERDATA, dwExStyle);
		SetLayeredWindowAttributes(hwnd, NULL, 200, LWA_ALPHA);
		//根据传过来的位置参数设置窗口的位置。
		//要得到的是按钮的左上角坐标，作为窗口的右上角坐标。
		RECT rssSearchBtnRect = *(RECT*)lParam;
		RECT searchRssItemDlgRect;
		GetClientRect(hwnd, &searchRssItemDlgRect);
		iWinWidth = searchRssItemDlgRect.right - searchRssItemDlgRect.left;
		iWinHeight = searchRssItemDlgRect.bottom - searchRssItemDlgRect.top;
		//设置窗口的位置。
		SetWindowPos(hwnd, NULL, rssSearchBtnRect.left - iWinWidth, rssSearchBtnRect.top,
			iWinWidth, iWinHeight, SWP_NOSIZE | SWP_NOZORDER);
		//初始化搜索编辑框。
		SetDlgItemText(hwnd, IDC_SearchRssItemMsgEdit, TEXT(""));
	}
		return TRUE;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_StartSearchRssItemBtn:
		{
			//获得文本框内输入的数据。
			TCHAR szSearchRssItemMsg[256] = { NULL };
			GetDlgItemText(hwnd, IDC_SearchRssItemMsgEdit, szSearchRssItemMsg, sizeof(szSearchRssItemMsg));
			//返回消息给RssTab窗口。
			HWND hParentWnd = (HWND)GetWindowLong(hwnd, GWL_USERDATA);
			SendMessage(hParentWnd, WM_SEARCHITEMMSG, (WPARAM)szSearchRssItemMsg, (LPARAM)IDC_SearchRssItemBtn);
			DestroyWindow(hwnd);
		}
			return TRUE;
		default:
			return FALSE;
		}
	}
		return TRUE;
	case WM_MAINDLGMOVE_RssDlg:
	{
		//接收到按钮的左上角坐标。
		int iXRightPixel = (int)wParam;
		int iYTopPixel = (int)lParam;
		//设置窗口的位置。
		SetWindowPos(hwnd, NULL, iXRightPixel - iWinWidth, iYTopPixel, iWinWidth,
			iWinHeight, SWP_NOSIZE | SWP_NOZORDER);
	}
		return TRUE;
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
	default:
		return FALSE;
	}
	return FALSE;
}