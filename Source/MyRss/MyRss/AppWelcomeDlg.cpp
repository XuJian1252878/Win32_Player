/*
*ѧ�ţ�1252878
*��������
*��ϵ�绰��18817870106
*���ʵ�ַ��1252878xujian@tongji.edu.cn
*�ļ���������ӭ����ľ���ʵ��
*/

#include"stdafx.h"
#include"AppWelcomeDlg.h"

HBRUSH hAppWelcomeDlgBrush = NULL;//��ӭ����ı�����ˢ��
HWND hMainDlgWnd = NULL;//������Ĵ��ھ����
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
			MessageBox(hwnd, TEXT("���ػ�ӭ����ͼ��ʧ�ܣ���"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			return FALSE;
		}
		RECT welcomeDlgRect;
		GetClientRect(hwnd, &welcomeDlgRect);
		hAppWelcomeDlgBrush = GetBkgndImage(hwnd, hBitmap, welcomeDlgRect);
		DeleteObject(hBitmap);
		//������ӭ���ڵ����塣
		LOGFONT logfont;
		ZeroMemory(&logfont, sizeof(logfont));
		logfont.lfCharSet = GB2312_CHARSET;
		logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		logfont.lfHeight = 20;
		lstrcpy(logfont.lfFaceName, TEXT("����"));
		logfont.lfQuality = DEFAULT_QUALITY;
		hWelcomeMsgFont = CreateFontIndirect(&logfont);
		//��������洰�ھ����
		hMainDlgWnd = (HWND)lParam;
		//��ʾ����Ļ�ӭ��Ϣ��
		TCHAR szWelcomeMsg[1024] = TEXT("��ӭʹ��MyRssPlayer!\r\n�����ߣ���_1252878\r\n���ڣ�2014��12��26��");
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
		case ID_EnterMyRssPlayer://�û�����˽��������ť��
		{
			ShowWindow(hMainDlgWnd, SW_SHOW);
			//֮��֪ͨ�������ػ汳����
			SendMessage(hMainDlgWnd, WM_CTLCOLORDLG, (WPARAM)0, (LPARAM)0);
			DeleteObject(hAppWelcomeDlgBrush);
			DeleteObject(hWelcomeMsgFont);
			hMainDlgWnd = NULL;
			EndDialog(hwnd,0);//��ӭ����رա������������潫��ʾ(�ں���InitMyRssPlayer��ʵ��)��
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
		//֮��֪ͨ�������ػ汳����
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