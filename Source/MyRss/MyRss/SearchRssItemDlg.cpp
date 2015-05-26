/*
*ѧ�ţ�1252878
*��������
*��ϵ�绰��18817870106
*���ʵ�ַ��1252878xujian@tongji.edu.cn
*�ļ�������Rss��ϸ������Ŀ�������ʵ��
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
		//���ô��ڵ�͸�����ԡ�
		DWORD dwExStyle = GetWindowLong(hwnd, GWL_USERDATA);
		dwExStyle |= WS_EX_LAYERED;
		SetWindowLong(hwnd, GWL_USERDATA, dwExStyle);
		SetLayeredWindowAttributes(hwnd, NULL, 200, LWA_ALPHA);
		//���ݴ�������λ�ò������ô��ڵ�λ�á�
		//Ҫ�õ����ǰ�ť�����Ͻ����꣬��Ϊ���ڵ����Ͻ����ꡣ
		RECT rssSearchBtnRect = *(RECT*)lParam;
		RECT searchRssItemDlgRect;
		GetClientRect(hwnd, &searchRssItemDlgRect);
		iWinWidth = searchRssItemDlgRect.right - searchRssItemDlgRect.left;
		iWinHeight = searchRssItemDlgRect.bottom - searchRssItemDlgRect.top;
		//���ô��ڵ�λ�á�
		SetWindowPos(hwnd, NULL, rssSearchBtnRect.left - iWinWidth, rssSearchBtnRect.top,
			iWinWidth, iWinHeight, SWP_NOSIZE | SWP_NOZORDER);
		//��ʼ�������༭��
		SetDlgItemText(hwnd, IDC_SearchRssItemMsgEdit, TEXT(""));
	}
		return TRUE;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_StartSearchRssItemBtn:
		{
			//����ı�������������ݡ�
			TCHAR szSearchRssItemMsg[256] = { NULL };
			GetDlgItemText(hwnd, IDC_SearchRssItemMsgEdit, szSearchRssItemMsg, sizeof(szSearchRssItemMsg));
			//������Ϣ��RssTab���ڡ�
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
		//���յ���ť�����Ͻ����ꡣ
		int iXRightPixel = (int)wParam;
		int iYTopPixel = (int)lParam;
		//���ô��ڵ�λ�á�
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