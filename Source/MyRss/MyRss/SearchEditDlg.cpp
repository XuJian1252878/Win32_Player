/*
*�ļ����������ֲ������������ʵ��
*/

#include"stdafx.h"//ǧ��ǵ����ͷ�ļ���
#include"resource.h"
#include"SearchEditDlg.h"

int iDlgWidth = 0;//���ڵĿ�ȡ�
int iDlgHeight = 0;//���ڵĸ߶ȡ�

//���ڹ��̺�����
BOOL CALLBACK SearchEditDlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		//���ô��ڰ�͸�����ԡ�
		DWORD dwExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
		dwExStyle |= WS_EX_LAYERED;
		SetWindowLong(hwnd, GWL_EXSTYLE, dwExStyle);
		SetLayeredWindowAttributes(hwnd, NULL, 180, LWA_ALPHA);
		//���������ô��ڵ�λ�á�
		RECT searchItemDlgSRect;//��ȡ������Ļ���ꡣ
		GetWindowRect(hwnd, &searchItemDlgSRect);
		//������ڵĳ������ȡ�
		iDlgWidth = searchItemDlgSRect.right - searchItemDlgSRect.left;
		iDlgHeight = searchItemDlgSRect.bottom - searchItemDlgSRect.top;//ȫ�ֱ�����
		RECT* lpSearchItemBtnRect = (RECT*)lParam;
		//���ô��ڵ�λ�á�
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
			//�رյ�ǰ�������ڣ����Ҳ���������ֵ��
			DestroyWindow(MusicMessage::GetSearchMusicItemWnd());
			//���ô��ھ��Ϊ�ա�
			MusicMessage::SetSearchMusicItemWnd(NULL);
		}
			return TRUE;
		case IDC_StartSearchItemBtn:
		{
			//���������Ϣ���ظ������ڡ�
			TCHAR szSearchItemMsg[MAX_PATH] = { NULL };
			GetDlgItemText(hwnd, IDC_SearchItemMsgEdit, szSearchItemMsg, sizeof(szSearchItemMsg));
			//�����ĸ����ڷ�����Ϣ�����ֲ����б��ڡ�
			SendMessage(MusicMessage::GetMusicWindowHwnd(), WM_SEARCHITEMMSG, (WPARAM)szSearchItemMsg, (LPARAM)0);
		}
			return TRUE;
		default:
			return FALSE;
		}
	}
		return TRUE;
	case WM_MAINDLGMOVE_SearchItemDlg://�������ƶ��������ĵ���Ϣ��
	{
		//���������ť���Ͻǵ�λ�á�
		int iXRightPixel = (int)wParam;//��õĶ�����Ļ���ꡣ
		int iYTopPixel = (int)lParam;
		SetWindowPos(hwnd, GetParent(hwnd), iXRightPixel - iDlgWidth, iYTopPixel - iDlgHeight,
			iDlgWidth, iDlgHeight, SWP_NOSIZE | SWP_NOZORDER);//��Ļ���ꡣ
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