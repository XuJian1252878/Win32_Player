/*
*�ļ������������߼�����δʹ��
*/

#include"stdafx.h"
#include"DeskTopSingleLrcDlgF.h"

bool bMouseTrackF = false;//��¼����Ƿ��ڸ�ʿؼ���ͣ����
HWND DeskTopSingleLrcDlgWnd = NULL;//��¼��ʴ��ڵľ����
//extern HWND hDeskTopLrcDlgF;//��ʾ��ʿؼ����ڵľ����

//��ʾ�����ʴ��ڵĿؼ���
BOOL CALLBACK DeskTopSingleLrcDlgF_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		//��ô��ݹ�������Ļ���������
		RECT deskTopLrcRect = *(RECT*)lParam;
		//���Ӧ����ʾ��ǰ���ڡ�
		deskTopLrcRect.left -= 15;
		deskTopLrcRect.right += 15;
		deskTopLrcRect.bottom += 15;
		deskTopLrcRect.top -= 45;
		//��ø����ʾ���ھ����
		DeskTopSingleLrcDlgWnd = (HWND)GetWindowLong(hwnd, GWL_USERDATA);
		//���ô��ڵ�λ�á�
		SetWindowPos(hwnd, DeskTopSingleLrcDlgWnd, deskTopLrcRect.left, deskTopLrcRect.top,
			deskTopLrcRect.right - deskTopLrcRect.left, deskTopLrcRect.bottom - deskTopLrcRect.top,
			SWP_NOSIZE);
	}
		return TRUE;
	case WM_MOUSEMOVE:
	{
		if (!bMouseTrackF)//���û��ͣ�����������ϡ�
		{
			TRACKMOUSEEVENT trackMouseEvent;//ע��WM_MOUSEHOVER WM_MOUSELEAVEʱ��
			ZeroMemory(&trackMouseEvent, sizeof(trackMouseEvent));
			trackMouseEvent.cbSize = sizeof(trackMouseEvent);
			trackMouseEvent.dwFlags = TME_HOVER | TME_LEAVE;//Ҫ���ٵ������Ϣ��
			trackMouseEvent.hwndTrack = hwnd;//Ҫ���ٵĴ��ھ����
			trackMouseEvent.dwHoverTime = 10;// ������˲��������޷�����mouseHover//����hover�¼��ľ�ʱ��
			if (_TrackMouseEvent(&trackMouseEvent))//MOUSELEAVE|MOUSEHOVER��Ϣ�ɴ˺�������
			{
				bMouseTrackF = true;
			}
		}
	}
		return TRUE;
	case WM_MOUSEHOVER:
	{
		bMouseTrackF = true;//���ͣ���ڴ����ϡ�
	}
		return TRUE;
	case WM_MOUSELEAVE:
	{
		bMouseTrackF = false;//���û��ͣ���ڴ����ϡ�
		DeskTopSingleLrcDlgWnd = NULL;
//		hDeskTopLrcDlgF = NULL;//�������δ��ھͿ��Ե�֪�ؼ����ڱ��ر��ˡ�
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