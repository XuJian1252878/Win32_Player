/*
*�ļ�������Rssվ��������ʵ��
*/

#include"stdafx.h"
#include"RssSearchMsgDlg.h"

int iWindowWidth = 0;
int iWindowHeight = 0;

BOOL CALLBACK RssSearchMsgDlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		//����͸�����ԡ�
		DWORD dwExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
		dwExStyle |= GWL_EXSTYLE;
		SetWindowLong(hwnd, GWL_EXSTYLE, dwExStyle);
		SetLayeredWindowAttributes(hwnd, NULL, 200, LWA_ALPHA);
		//���RssTab�д����ô��ڵİ�ť��
		RECT* lpRssSearchBtnRect = (RECT*)lParam;
		RECT RssSearchMsgDlg;
		GetClientRect(hwnd, &RssSearchMsgDlg);
		RssSearchMsgDlg.left = lpRssSearchBtnRect->right;
		RssSearchMsgDlg.top = lpRssSearchBtnRect->top;
		//��¼���ڵĸ߶ȺͿ�ȡ�
		iWindowWidth = RssSearchMsgDlg.right;
		iWindowHeight = RssSearchMsgDlg.bottom;
		SetWindowPos(hwnd, NULL, RssSearchMsgDlg.left, RssSearchMsgDlg.top,
			RssSearchMsgDlg.right, RssSearchMsgDlg.bottom, SWP_NOSIZE | SWP_NOZORDER);
	}
		return TRUE;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_RssStartSearchBtn:
		{
			//��RssTab������Ϣ��
			HWND hRssTabWnd = (HWND)GetWindowLong(hwnd, GWL_USERDATA);
			TCHAR szRssSearchMsg[500] = { NULL };
			GetDlgItemText(hwnd, IDC_RssSearchMsgEdit, szRssSearchMsg, sizeof(szRssSearchMsg));
			SendMessage(hRssTabWnd, WM_SEARCHITEMMSG, (WPARAM)szRssSearchMsg, (LPARAM)IDC_AddRssFeedBtn);
			DestroyWindow(hwnd);
		}
			return TRUE;
		default:
			return FALSE;
		}
	}
		return FALSE;
	case WM_MAINDLGMOVE_RssDlg:
	{
		//����������Ϣ���൱�����Ͻ����ꡣ
		int iXLeftPixel = (int)wParam;
		int iYTopPixel = (int)lParam;
		SetWindowPos(hwnd, NULL, iXLeftPixel, iYTopPixel, iWindowWidth, iWindowHeight, SWP_NOSIZE | SWP_NOZORDER);
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