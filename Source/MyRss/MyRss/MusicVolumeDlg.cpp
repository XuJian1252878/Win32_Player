/*
*文件描述：音乐播放音量控制子窗口界面相关处理
*/

#include"stdafx.h"
#include"MusicVolumeDlg.h"


HBRUSH hVolumeSliderBrush = NULL,hCurVolumeEditBrush = NULL;

BOOL CALLBACK MusicVolumeDlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		hVolumeSliderBrush = CreateSolidBrush(RGB(0, 0, 0));
		hCurVolumeEditBrush = CreateSolidBrush(RGB(200, 100, 100));
		//设置窗口的透明属性。
		DWORD dwExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
		dwExStyle |= WS_EX_LAYERED;
		SetWindowLong(hwnd, GWL_EXSTYLE, dwExStyle);
		SetLayeredWindowAttributes(hwnd, NULL, 130, LWA_ALPHA);
		//初始化显示的位置。
		RECT volBtnScreenRect = *((RECT*)(lParam));
		//设置音量条的初始化位置。volBtnScreenRect是音量button的屏幕坐标。
		SetWindowPos(hwnd, GetParent(hwnd), volBtnScreenRect.right, volBtnScreenRect.top + 7, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		//初始化音乐音量条。
		InitVolumeSlider(hwnd);
	}
		return FALSE;
/*	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		default:
			return FALSE;
		}
	}
		return TRUE;*/
	case WM_CTLCOLORSTATIC:
	{
		HDC hStaticDC = (HDC)wParam;
		HWND hStaticWnd = (HWND)lParam;
		SetBkMode(hStaticDC, TRANSPARENT);
		SetTextColor(hStaticDC, RGB(255, 0, 0));
		switch (GetDlgCtrlID(hStaticWnd))
		{
		case IDC_VolumeSlider:
		{
			SetBkColor(hStaticDC, RGB(0, 0, 0));
		}
			return (long)hVolumeSliderBrush;
		case IDC_CurVolume:
		{
			SetBkColor(hStaticDC, RGB(200, 100, 100));
		}
			return (long)hCurVolumeEditBrush;
		default:
			return NULL;
		}
	}
		return TRUE;
	case WM_MAINDLGMOVE_VolDlg://主窗口移动时给子窗口发送该消息。窗口间传递消息就是这样。
	{
		int iLeftPixel = (int)wParam;
		int iTopPixel = (int)lParam;
		SetWindowPos(hwnd, NULL, iLeftPixel, iTopPixel, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}
		return TRUE;
	case WM_HSCROLL:
	{
		switch (GetDlgCtrlID((HWND)lParam))
		{
		case IDC_VolumeSlider:
			VolumeSliderChange(hwnd, (HWND)lParam, wParam);
			return TRUE;
		default:
			return FALSE;
		}
	}
		return TRUE;
	case WM_LBUTTONDOWN:
	{
		DeleteObject(hVolumeSliderBrush);
		DeleteObject(hCurVolumeEditBrush);
		DestroyWindow(hwnd);
	}
		return TRUE;
	default:
		return FALSE;
	}
	return FALSE;
}


//当音量滑动条改变的时候。
void VolumeSliderChange(HWND hwnd, HWND hwndCtrol, WPARAM wParam)
{
	TCHAR szVolume[10] = { NULL };
	/* error C2360: initialization of 'iWillPlayVol' is skipped by 'case' label
	这个case语句可能没有被执行到（执行其他case了）导致变量没有初始化而导致接下来的错误(在case里面定义变量的时候)
	1.放到switch前面声明初始化
	2.在swith中定义内部变量加括号*/
	int iWillPlayVol = MusicMessage::GetCurMusicVol();
	switch (LOWORD(wParam))
	{
		//音量变小的操作。
	case SB_LEFT:
	case SB_LINELEFT:
	case SB_PAGELEFT:
		iWillPlayVol -= 5;
		if (iWillPlayVol < 0)
		{
			iWillPlayVol = 0;
		}
		SendMessage(hwndCtrol, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)iWillPlayVol);
		wsprintf(szVolume, TEXT("%d"), iWillPlayVol);
		SetDlgItemText(hwnd, IDC_CurVolume, szVolume);
		MusicMessage::SetCurMusicVol(iWillPlayVol);
		break;
		//音量变大的时候。
	case SB_RIGHT:
	case SB_LINERIGHT:
	case SB_PAGERIGHT:
		iWillPlayVol += 5;
		if (iWillPlayVol > 100)
		{
			iWillPlayVol = 100;
		}
		SendMessage(hwndCtrol, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)iWillPlayVol);
		wsprintf(szVolume, TEXT("%d"), iWillPlayVol);
		SetDlgItemText(hwnd, IDC_CurVolume, szVolume);
		MusicMessage::SetCurMusicVol(iWillPlayVol);
		break;
		//拖动音量条的时候。
	case SB_THUMBTRACK:
	{
		int iCurVolSliderPos = HIWORD(wParam);
		SendMessage(hwndCtrol, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)iCurVolSliderPos);
		wsprintf(szVolume, TEXT("%d"), iCurVolSliderPos);
		SetDlgItemText(hwnd, IDC_CurVolume, szVolume);
		MusicMessage::SetCurMusicVol(iCurVolSliderPos);
	}
		break;
	case SB_THUMBPOSITION:
		SendMessage(hwndCtrol, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(HIWORD(wParam)));
		wsprintf(szVolume, TEXT("%d"), (int)(HIWORD(wParam)));
		SetDlgItemText(hwnd, IDC_CurVolume, szVolume);
		MusicMessage::SetCurMusicVol((int)(HIWORD(wParam)));
		break;
	default:
		break;
	}
}


//当音量按钮被点击的时候，音量条的初始状态,HWND 音量条窗口的句柄。
bool InitVolumeSlider(HWND hwnd)
{
	//设置音量条范围0,100.
	SendMessage(GetDlgItem(hwnd, IDC_VolumeSlider), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 100));
	//显示当前音乐正在播放的音量。
	int iCurMusicVolume = MusicMessage::GetCurMusicVol();
	SendMessage(GetDlgItem(hwnd, IDC_VolumeSlider), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)iCurMusicVolume);
	TCHAR szCurVolume[20] = { NULL };
	wsprintf(szCurVolume, TEXT("%d"), iCurMusicVolume);
	SetDlgItemText(hwnd, IDC_CurVolume, szCurVolume);
	return true;
}