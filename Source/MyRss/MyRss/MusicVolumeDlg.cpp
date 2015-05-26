/*
*�ļ����������ֲ������������Ӵ��ڽ�����ش���
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
		//���ô��ڵ�͸�����ԡ�
		DWORD dwExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
		dwExStyle |= WS_EX_LAYERED;
		SetWindowLong(hwnd, GWL_EXSTYLE, dwExStyle);
		SetLayeredWindowAttributes(hwnd, NULL, 130, LWA_ALPHA);
		//��ʼ����ʾ��λ�á�
		RECT volBtnScreenRect = *((RECT*)(lParam));
		//�����������ĳ�ʼ��λ�á�volBtnScreenRect������button����Ļ���ꡣ
		SetWindowPos(hwnd, GetParent(hwnd), volBtnScreenRect.right, volBtnScreenRect.top + 7, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		//��ʼ��������������
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
	case WM_MAINDLGMOVE_VolDlg://�������ƶ�ʱ���Ӵ��ڷ��͸���Ϣ�����ڼ䴫����Ϣ����������
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


//�������������ı��ʱ��
void VolumeSliderChange(HWND hwnd, HWND hwndCtrol, WPARAM wParam)
{
	TCHAR szVolume[10] = { NULL };
	/* error C2360: initialization of 'iWillPlayVol' is skipped by 'case' label
	���case������û�б�ִ�е���ִ������case�ˣ����±���û�г�ʼ�������½������Ĵ���(��case���涨�������ʱ��)
	1.�ŵ�switchǰ��������ʼ��
	2.��swith�ж����ڲ�����������*/
	int iWillPlayVol = MusicMessage::GetCurMusicVol();
	switch (LOWORD(wParam))
	{
		//������С�Ĳ�����
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
		//��������ʱ��
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
		//�϶���������ʱ��
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


//��������ť�������ʱ���������ĳ�ʼ״̬,HWND ���������ڵľ����
bool InitVolumeSlider(HWND hwnd)
{
	//������������Χ0,100.
	SendMessage(GetDlgItem(hwnd, IDC_VolumeSlider), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 100));
	//��ʾ��ǰ�������ڲ��ŵ�������
	int iCurMusicVolume = MusicMessage::GetCurMusicVol();
	SendMessage(GetDlgItem(hwnd, IDC_VolumeSlider), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)iCurMusicVolume);
	TCHAR szCurVolume[20] = { NULL };
	wsprintf(szCurVolume, TEXT("%d"), iCurMusicVolume);
	SetDlgItemText(hwnd, IDC_CurVolume, szCurVolume);
	return true;
}