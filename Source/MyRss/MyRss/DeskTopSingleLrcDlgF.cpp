/*
*文件描述：程序逻辑中暂未使用
*/

#include"stdafx.h"
#include"DeskTopSingleLrcDlgF.h"

bool bMouseTrackF = false;//记录鼠标是否在歌词控件上停留。
HWND DeskTopSingleLrcDlgWnd = NULL;//记录歌词窗口的句柄。
//extern HWND hDeskTopLrcDlgF;//显示歌词控件窗口的句柄。

//显示桌面歌词窗口的控件。
BOOL CALLBACK DeskTopSingleLrcDlgF_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		//获得传递过来的屏幕坐标参数。
		RECT deskTopLrcRect = *(RECT*)lParam;
		//相对应的显示当前窗口。
		deskTopLrcRect.left -= 15;
		deskTopLrcRect.right += 15;
		deskTopLrcRect.bottom += 15;
		deskTopLrcRect.top -= 45;
		//获得歌词显示窗口句柄。
		DeskTopSingleLrcDlgWnd = (HWND)GetWindowLong(hwnd, GWL_USERDATA);
		//设置窗口的位置。
		SetWindowPos(hwnd, DeskTopSingleLrcDlgWnd, deskTopLrcRect.left, deskTopLrcRect.top,
			deskTopLrcRect.right - deskTopLrcRect.left, deskTopLrcRect.bottom - deskTopLrcRect.top,
			SWP_NOSIZE);
	}
		return TRUE;
	case WM_MOUSEMOVE:
	{
		if (!bMouseTrackF)//鼠标没有停留在桌面歌词上。
		{
			TRACKMOUSEEVENT trackMouseEvent;//注册WM_MOUSEHOVER WM_MOUSELEAVE时间
			ZeroMemory(&trackMouseEvent, sizeof(trackMouseEvent));
			trackMouseEvent.cbSize = sizeof(trackMouseEvent);
			trackMouseEvent.dwFlags = TME_HOVER | TME_LEAVE;//要跟踪的鼠标消息。
			trackMouseEvent.hwndTrack = hwnd;//要跟踪的窗口句柄。
			trackMouseEvent.dwHoverTime = 10;// 若不设此参数，则无法触发mouseHover//定义hover事件耗尽时间
			if (_TrackMouseEvent(&trackMouseEvent))//MOUSELEAVE|MOUSEHOVER消息由此函数触发
			{
				bMouseTrackF = true;
			}
		}
	}
		return TRUE;
	case WM_MOUSEHOVER:
	{
		bMouseTrackF = true;//鼠标停留在窗口上。
	}
		return TRUE;
	case WM_MOUSELEAVE:
	{
		bMouseTrackF = false;//鼠标没有停留在窗口上。
		DeskTopSingleLrcDlgWnd = NULL;
//		hDeskTopLrcDlgF = NULL;//这样各次窗口就可以得知控件窗口被关闭了。
		DestroyWindow(hwnd);
	}
		return TRUE;
	case WM_DESTROY:
	{
		bMouseTrackF = false;
		DeskTopSingleLrcDlgWnd = NULL;
	}
		return TRUE;
	default:
		return FALSE;
	}
	return FALSE;
}