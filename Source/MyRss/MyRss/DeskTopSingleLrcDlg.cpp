/*
*ѧ�ţ�1252878
*��������
*��ϵ�绰��18817870106
*���ʵ�ַ��1252878xujian@tongji.edu.cn
*�ļ���������������ʾ���߼�ʵ��
*/

#include"stdafx.h"
#include"DeskTopSingleLrcDlg.h"

extern vector<Lyric> musicLyric;
HDC hDstDC,hMemDC;//�����ڴ�DC��
RECT windowRect,lrcClientRect;//���ڵ�ʵ�ʴ�С(��Ļ����),���ڵ�ʵ�ʴ�С(�ͻ�������)
HBRUSH hMemBrush;//�ڴ�DC�ı�����ˢ��
HBITMAP hMemBitmap;//ѡ���ڴ�DC���봰�ڴ�С��ͬ��λͼ��
BLENDFUNCTION blendFunction;//UpdateLayeredWindow���õ��Ĳ�����
int iCurLyricIndex = 0;//�ڴ��ļ��и����ش�ʱ�����ʾ���˵ڼ��С�
bool bMouseTrack = false;//��¼����Ƿ��ڸ����ͣ����
//HWND hDeskTopLrcDlgF = NULL;//��ʾ��ʿؼ����ڵľ����

BOOL CALLBACK DeskTop_SingleLrcDlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		hDstDC = GetDC(hwnd);
		hMemDC = CreateCompatibleDC(hDstDC);
		//��ø�����ʾ���ڵĴ�С��
		GetWindowRect(hwnd, &windowRect);
		lrcClientRect.left = 0;
		lrcClientRect.top = 0;
		lrcClientRect.right = windowRect.right - windowRect.left;
		lrcClientRect.bottom = windowRect.bottom - windowRect.top;
		hMemBitmap = CreateCompatibleBitmap(hDstDC, windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top);
		SelectObject(hMemDC, hMemBitmap);
		//����ڴ�DC�ı�����ˢ��
		hMemBrush = CreateSolidBrush(RGB(0, 0, 0));//��ɫ��ˢ��
		//Font is never antialiased, that is, font smoothing is not done.
		HFONT hFont = CreateFont(53, 25, 0, 0, 700, FALSE, FALSE, FALSE, GB2312_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("����_GB2312"));
		//�������Լ���ˢ�����ڴ�DC�С�
		SelectObject(hMemDC, hMemBrush);
		SelectObject(hMemDC, hFont);
		//�����ڴ�DC����͸����
		SetBkMode(hMemDC, TRANSPARENT);
		//UpdateLayeredWindow���õ��Ĳ�����
		blendFunction.BlendOp = AC_SRC_OVER;
		blendFunction.BlendFlags = 0;
		blendFunction.SourceConstantAlpha = 150;
		blendFunction.AlphaFormat = AC_SRC_ALPHA;
		//���ô���͸����
		DWORD dwExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
		dwExStyle |= WS_EX_LAYERED;
		dwExStyle |= WS_EX_TOPMOST;
		SetWindowLong(hwnd, GWL_EXSTYLE, dwExStyle);
		//ʹ��ʴ��ھӵײ�������ʾ��
		int cxScreen = GetSystemMetrics(SM_CXSCREEN);
		int cyScreen = GetSystemMetrics(SM_CYSCREEN);
		windowRect.left = (cxScreen - lrcClientRect.right) / 2;
		windowRect.right = (cxScreen + lrcClientRect.left) / 2;
		windowRect.top = cyScreen - lrcClientRect.bottom - 10;
		windowRect.bottom = cyScreen - 10;
		//��������ʾ������ 
		ShowWindow(hwnd, SW_SHOW);
		//ʹ��ʴ��ڳ�ʼ��ʱ��ʾ�ڵײ����С�
		SetWindowPos(hwnd, HWND_TOPMOST, windowRect.left, windowRect.top,
			lrcClientRect.right, lrcClientRect.bottom, SWP_NOSIZE);
		//��ʴ���һ�򿪾Ϳ�����ʱ����
		SetTimer(hwnd, TIMER_DISPLAYDESKTOPSINGLELRC, 10, DesktopLrcSingle_TimerProc);
	}
		return TRUE;
/*	case WM_MOUSEMOVE:
	{
		if (!bMouseTrack)//���û��ͣ�����������ϡ�
		{
			TRACKMOUSEEVENT trackMouseEvent;//ע��WM_MOUSEHOVER WM_MOUSELEAVEʱ��
			ZeroMemory(&trackMouseEvent, sizeof(trackMouseEvent));
			trackMouseEvent.cbSize = sizeof(trackMouseEvent);
			trackMouseEvent.dwFlags = TME_HOVER | TME_LEAVE;//Ҫ���ٵ������Ϣ��
			trackMouseEvent.hwndTrack = hwnd;//Ҫ���ٵĴ��ھ����
			trackMouseEvent.dwHoverTime = 10;// ������˲��������޷�����mouseHover//����hover�¼��ľ�ʱ��
			if (_TrackMouseEvent(&trackMouseEvent))//MOUSELEAVE|MOUSEHOVER��Ϣ�ɴ˺�������
			{
				bMouseTrack = true;
			}
		}
	}
		return TRUE;
	case WM_MOUSEHOVER:
	{
		bMouseTrack = true;//��ʱ���ͣ���ڸ���ϣ���ʾ�ؼ����ڡ�
		RECT deskTopLrcRect;//��ǰ�����ʾ���ڵ���Ļ���ꡣ
		GetWindowRect(hwnd, &deskTopLrcRect);
		if (hDeskTopLrcDlgF == NULL)
		{
			hDeskTopLrcDlgF = CreateDialogParam((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
				MAKEINTRESOURCE(IDD_DeskTopSingleLrcDlgF), NULL, DeskTopSingleLrcDlgF_Proc, (LPARAM)&deskTopLrcRect);
			SetWindowLong(hDeskTopLrcDlgF, GWL_USERDATA, (LONG)hwnd);
		}
	}
		return TRUE;
	case WM_MOUSELEAVE:
	{
		//����뿪��
		bMouseTrack = false;
		//�رտؼ����ڡ�Ҫ�ڿؼ����ڵĴ��ڹ����йرա�
//		DestroyWindow(hDeskTopLrcDlgF);

	}
		return TRUE;*/
	case WM_LBUTTONDOWN:
	{
		SendMessage(hwnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, (LPARAM)0);
	}
		return TRUE;
	case WM_CLOSE:
	{
		bMouseTrack = false;//��¼����Ƿ��ڸ����ͣ����
		//�ͷ���Դ��
		ReleaseDC(hwnd, hDstDC);
		DeleteObject(hMemDC);
		DeleteObject(hMemBitmap);
		DeleteObject(hMemBrush);
		iCurLyricIndex = 0;//��ʾ��ʵ�������λ��
		DestroyWindow(hwnd);
	}
		return TRUE;
	default:
		break;
	}
	return FALSE;
}




//��ʾ��ʵļ�ʱ��
VOID CALLBACK DesktopLrcSingle_TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	GetWindowRect(hwnd, &windowRect);//��ֹ�û��϶���ʴ��ڵ���UpdateLayeredWindow��Ч��
	POINT screenPoint,memDCPoint;//��ǰ��ʴ�������Ļ�ϵ�λ�ã���Ļ���꣩���Լ����ڴ�DC���Ǹ��꿪ʼץȡλͼ��
	SIZE curWindowSize;//��ǰ��ʴ��ڵĴ�С��
	RECT sangLrcRect;//�Ѿ������ĸ�ʵľ�������
	lrcClientRect.left = 0;
	lrcClientRect.top = 0;
	lrcClientRect.right = windowRect.right - windowRect.left;
	lrcClientRect.bottom = windowRect.bottom - windowRect.top;
	//���ڴ�DC���Ϻ�ɫ�ı���ɫ�����ں����ܽ����������ʾ��ǰ̨��
	FillRect(hMemDC, &lrcClientRect, hMemBrush);
	//��õ�ǰ���ֲ��ŵ���λ�á�
	long lCurMusicPos = MusicMessage::GetCurMusicPos();
	if (!(MusicMessage::GetHasLyric()) || (MusicMessage::GetCurPlayingMusic() == -1)
		|| (lCurMusicPos / 1000 >= MusicMessage::GetPlayingMusicLen() / 1000))//�����������֮��
	{
		iCurLyricIndex = 0;//��ʾ��ʵ�������λ��
		SetTextColor(hMemDC, RGB(100, 255, 0));
		DrawText(hMemDC, TEXT("Simple Player Desktop Lyric"),
			lstrlen(TEXT("Simple Player Desktop Lyric")), &lrcClientRect, DT_CENTER | DT_SINGLELINE);
		screenPoint.x = windowRect.left;
		screenPoint.y = windowRect.top;
		curWindowSize.cx = windowRect.right - windowRect.left;
		curWindowSize.cy = windowRect.bottom - windowRect.top;
		memDCPoint.x = 0;
		memDCPoint.y = 0;
		int iResult = UpdateLayeredWindow(hwnd, hDstDC, &screenPoint, &curWindowSize, hMemDC, &memDCPoint, RGB(0, 0, 0),
			&blendFunction, ULW_COLORKEY);
		if (iResult == 0)
			ShowError();
		return;
	}
	//���������и�ʵ������
	int iLrclen = 0, iLrcSang = 0;//һ�����ʵĳ��ȣ��Լ�������ʵĳ��ȡ�
	SIZE curLrcLineSZ;//�洢��ǰһ�����ʵĳߴ硣
	GetTextExtentPoint32(hMemDC, musicLyric[MusicMessage::GetCurLyricIndex()].szMusicLrc,
		lstrlen(musicLyric[MusicMessage::GetCurLyricIndex()].szMusicLrc), &curLrcLineSZ);
	iLrclen = curLrcLineSZ.cx;//��õ�ǰ�������ܳ��ȡ�
	iCurLyricIndex = MusicMessage::GetCurLyricIndex();
	if (lCurMusicPos < musicLyric[iCurLyricIndex].lTime)//�п����ǿ��˵������
	{
		while (lCurMusicPos < musicLyric[iCurLyricIndex].lTime)
		{
			iCurLyricIndex--;//�ҵ�����ʱ���±ꡣ
			if (iCurLyricIndex < 0)  { iCurLyricIndex = 0; break; }
			if (iCurLyricIndex >= musicLyric.size() - 2)  { iCurLyricIndex = musicLyric.size() - 1 -2; break; }
		}
	}
	else//�п����ǿ���������
	{
		while (lCurMusicPos > musicLyric[iCurLyricIndex].lTime)
		{
			iCurLyricIndex++;
			if (iCurLyricIndex < 0)  { iCurLyricIndex = 0; break; }
			if (iCurLyricIndex >= musicLyric.size() - 2)  { iCurLyricIndex = musicLyric.size() - 2; break; }//����֮����Զ���һ��
		}
		iCurLyricIndex--;//�ҵ�����ʱ���±ꡣ
	}
	if ((iCurLyricIndex >= 0) && (iCurLyricIndex < ( musicLyric.size() - 2 )))//���ϸ������������
	{
		lrcClientRect.left = (windowRect.right - windowRect.left - iLrclen) >> 1;
		lrcClientRect.right = (windowRect.right - windowRect.left + iLrclen) >> 1;
		lrcClientRect.top = 0;
		lrcClientRect.bottom = (windowRect.bottom - windowRect.top);
		//����δ������ʵ���ɫ��
		SetTextColor(hMemDC, RGB(100, 255, 0));
		TextOut(hMemDC, lrcClientRect.left, lrcClientRect.top, musicLyric[iCurLyricIndex].szMusicLrc,
			lstrlen(musicLyric[iCurLyricIndex].szMusicLrc));
		//���ó�����ʵ���ɫ��
		iLrcSang = (int)( ( (lCurMusicPos - musicLyric[iCurLyricIndex].lTime)*1.0 /
			(musicLyric[iCurLyricIndex + 1].lTime - musicLyric[iCurLyricIndex].lTime) ) * iLrclen);
		sangLrcRect.left = lrcClientRect.left;
		sangLrcRect.right = lrcClientRect.left + iLrcSang;
		sangLrcRect.top = lrcClientRect.top;
		sangLrcRect.bottom = lrcClientRect.bottom;
		SetTextColor(hMemDC, RGB(255, 0, 0));
		DrawText(hMemDC, musicLyric[iCurLyricIndex].szMusicLrc,
			lstrlen(musicLyric[iCurLyricIndex].szMusicLrc),&sangLrcRect,DT_LEFT);
		//ΪUpdateLayeredWindow��׼����
		screenPoint.x = windowRect.left;//��������Ļ�е�λ�á�
		screenPoint.y = windowRect.top;
		curWindowSize.cx = windowRect.right - windowRect.left;//��������Ļ�еĳߴ���Ϣ��
		curWindowSize.cy = windowRect.bottom - windowRect.top;
		memDCPoint.x = 0;//���ڴ�DC����һ�����꿪ʼ������
		memDCPoint.y = 0;
		int iResult = UpdateLayeredWindow(hwnd, hDstDC, &screenPoint, &curWindowSize, hMemDC, &memDCPoint,
			RGB(0, 0, 0), &blendFunction, ULW_COLORKEY);
		if (iResult == 0)
			ShowError();
	}
}