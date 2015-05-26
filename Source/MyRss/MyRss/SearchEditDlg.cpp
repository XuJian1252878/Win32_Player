/*
*文件描述：音乐播放器搜索框的实现
*/

#include"stdafx.h"//千万记得这个头文件。
#include"resource.h"
#include"SearchEditDlg.h"

int iDlgWidth = 0;//窗口的宽度。
int iDlgHeight = 0;//窗口的高度。

//窗口过程函数。
BOOL CALLBACK SearchEditDlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		//设置窗口半透明属性。
		DWORD dwExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
		dwExStyle |= WS_EX_LAYERED;
		SetWindowLong(hwnd, GWL_EXSTYLE, dwExStyle);
		SetLayeredWindowAttributes(hwnd, NULL, 180, LWA_ALPHA);
		//接下来设置窗口的位置。
		RECT searchItemDlgSRect;//获取它的屏幕坐标。
		GetWindowRect(hwnd, &searchItemDlgSRect);
		//算出窗口的长度与宽度。
		iDlgWidth = searchItemDlgSRect.right - searchItemDlgSRect.left;
		iDlgHeight = searchItemDlgSRect.bottom - searchItemDlgSRect.top;//全局变量。
		RECT* lpSearchItemBtnRect = (RECT*)lParam;
		//放置窗口的位置。
		SetWindowPos(hwnd, GetParent(hwnd), lpSearchItemBtnRect->right - iDlgWidth, lpSearchItemBtnRect->top - iDlgHeight,
			iDlgWidth, iDlgHeight, SWP_NOSIZE | SWP_NOZORDER);
	}
		return TRUE;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_CloseSearchItemDlgBtn:
		{
			//关闭当前搜索窗口，并且不返回搜索值。
			DestroyWindow(MusicMessage::GetSearchMusicItemWnd());
			//设置窗口句柄为空。
			MusicMessage::SetSearchMusicItemWnd(NULL);
		}
			return TRUE;
		case IDC_StartSearchItemBtn:
		{
			//将输入的消息返回给父窗口。
			TCHAR szSearchItemMsg[MAX_PATH] = { NULL };
			GetDlgItemText(hwnd, IDC_SearchItemMsgEdit, szSearchItemMsg, sizeof(szSearchItemMsg));
			//向他的父窗口发送消息。音乐播放列表窗口。
			SendMessage(MusicMessage::GetMusicWindowHwnd(), WM_SEARCHITEMMSG, (WPARAM)szSearchItemMsg, (LPARAM)0);
		}
			return TRUE;
		default:
			return FALSE;
		}
	}
		return TRUE;
	case WM_MAINDLGMOVE_SearchItemDlg://主窗口移动所发出的的消息。
	{
		//获得搜索按钮右上角的位置。
		int iXRightPixel = (int)wParam;//获得的都是屏幕坐标。
		int iYTopPixel = (int)lParam;
		SetWindowPos(hwnd, GetParent(hwnd), iXRightPixel - iDlgWidth, iYTopPixel - iDlgHeight,
			iDlgWidth, iDlgHeight, SWP_NOSIZE | SWP_NOZORDER);//屏幕坐标。
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
			break;
		}
	}
	default:
		return FALSE;
	}
	return FALSE;
}