/*
*学号：1252878
*姓名：许舰
*联系电话：18817870106
*电邮地址：1252878xujian@tongji.edu.cn
*文件描述：桌面歌词显示的逻辑实现
*/

#include"stdafx.h"
#include"DeskTopSingleLrcDlg.h"

extern vector<Lyric> musicLyric;
HDC hDstDC,hMemDC;//窗口内存DC。
RECT windowRect,lrcClientRect;//窗口的实际大小(屏幕坐标),窗口的实际大小(客户区坐标)
HBRUSH hMemBrush;//内存DC的背景画刷。
HBITMAP hMemBitmap;//选入内存DC的与窗口大小相同的位图。
BLENDFUNCTION blendFunction;//UpdateLayeredWindow所用到的参数。
int iCurLyricIndex = 0;//在此文件中负责监控此时歌词显示到了第几行。
bool bMouseTrack = false;//记录鼠标是否在歌词上停留。
//HWND hDeskTopLrcDlgF = NULL;//显示歌词控件窗口的句柄。

BOOL CALLBACK DeskTop_SingleLrcDlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		hDstDC = GetDC(hwnd);
		hMemDC = CreateCompatibleDC(hDstDC);
		//获得各次显示窗口的大小。
		GetWindowRect(hwnd, &windowRect);
		lrcClientRect.left = 0;
		lrcClientRect.top = 0;
		lrcClientRect.right = windowRect.right - windowRect.left;
		lrcClientRect.bottom = windowRect.bottom - windowRect.top;
		hMemBitmap = CreateCompatibleBitmap(hDstDC, windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top);
		SelectObject(hMemDC, hMemBitmap);
		//获得内存DC的背景画刷。
		hMemBrush = CreateSolidBrush(RGB(0, 0, 0));//黑色画刷。
		//Font is never antialiased, that is, font smoothing is not done.
		HFONT hFont = CreateFont(53, 25, 0, 0, 700, FALSE, FALSE, FALSE, GB2312_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("楷体_GB2312"));
		//将字体以及画刷载入内存DC中。
		SelectObject(hMemDC, hMemBrush);
		SelectObject(hMemDC, hFont);
		//设置内存DC背景透明。
		SetBkMode(hMemDC, TRANSPARENT);
		//UpdateLayeredWindow所用到的参数。
		blendFunction.BlendOp = AC_SRC_OVER;
		blendFunction.BlendFlags = 0;
		blendFunction.SourceConstantAlpha = 150;
		blendFunction.AlphaFormat = AC_SRC_ALPHA;
		//设置窗口透明。
		DWORD dwExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
		dwExStyle |= WS_EX_LAYERED;
		dwExStyle |= WS_EX_TOPMOST;
		SetWindowLong(hwnd, GWL_EXSTYLE, dwExStyle);
		//使歌词窗口居底部居中显示。
		int cxScreen = GetSystemMetrics(SM_CXSCREEN);
		int cyScreen = GetSystemMetrics(SM_CYSCREEN);
		windowRect.left = (cxScreen - lrcClientRect.right) / 2;
		windowRect.right = (cxScreen + lrcClientRect.left) / 2;
		windowRect.top = cyScreen - lrcClientRect.bottom - 10;
		windowRect.bottom = cyScreen - 10;
		//将窗口显示出来。 
		ShowWindow(hwnd, SW_SHOW);
		//使歌词窗口初始化时显示在底部居中。
		SetWindowPos(hwnd, HWND_TOPMOST, windowRect.left, windowRect.top,
			lrcClientRect.right, lrcClientRect.bottom, SWP_NOSIZE);
		//歌词窗口一打开就开启定时器。
		SetTimer(hwnd, TIMER_DISPLAYDESKTOPSINGLELRC, 10, DesktopLrcSingle_TimerProc);
	}
		return TRUE;
/*	case WM_MOUSEMOVE:
	{
		if (!bMouseTrack)//鼠标没有停留在桌面歌词上。
		{
			TRACKMOUSEEVENT trackMouseEvent;//注册WM_MOUSEHOVER WM_MOUSELEAVE时间
			ZeroMemory(&trackMouseEvent, sizeof(trackMouseEvent));
			trackMouseEvent.cbSize = sizeof(trackMouseEvent);
			trackMouseEvent.dwFlags = TME_HOVER | TME_LEAVE;//要跟踪的鼠标消息。
			trackMouseEvent.hwndTrack = hwnd;//要跟踪的窗口句柄。
			trackMouseEvent.dwHoverTime = 10;// 若不设此参数，则无法触发mouseHover//定义hover事件耗尽时间
			if (_TrackMouseEvent(&trackMouseEvent))//MOUSELEAVE|MOUSEHOVER消息由此函数触发
			{
				bMouseTrack = true;
			}
		}
	}
		return TRUE;
	case WM_MOUSEHOVER:
	{
		bMouseTrack = true;//此时鼠标停留在歌词上，显示控件窗口。
		RECT deskTopLrcRect;//当前歌词显示窗口的屏幕坐标。
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
		//鼠标离开。
		bMouseTrack = false;
		//关闭控件窗口。要在控件窗口的窗口过程中关闭。
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
		bMouseTrack = false;//记录鼠标是否在歌词上停留。
		//释放资源。
		ReleaseDC(hwnd, hDstDC);
		DeleteObject(hMemDC);
		DeleteObject(hMemBitmap);
		DeleteObject(hMemBrush);
		iCurLyricIndex = 0;//显示歌词的行数归位。
		DestroyWindow(hwnd);
	}
		return TRUE;
	default:
		break;
	}
	return FALSE;
}




//显示歌词的计时器
VOID CALLBACK DesktopLrcSingle_TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	GetWindowRect(hwnd, &windowRect);//防止用户拖动歌词窗口导致UpdateLayeredWindow无效。
	POINT screenPoint,memDCPoint;//当前歌词窗口在屏幕上的位置（屏幕坐标），以及从内存DC的那个店开始抓取位图。
	SIZE curWindowSize;//当前歌词窗口的大小。
	RECT sangLrcRect;//已经唱过的歌词的矩形区域。
	lrcClientRect.left = 0;
	lrcClientRect.top = 0;
	lrcClientRect.right = windowRect.right - windowRect.left;
	lrcClientRect.bottom = windowRect.bottom - windowRect.top;
	//对内存DC画上黑色的背景色，便于后面能将它方便的显示到前台。
	FillRect(hMemDC, &lrcClientRect, hMemBrush);
	//获得当前音乐播放到的位置。
	long lCurMusicPos = MusicMessage::GetCurMusicPos();
	if (!(MusicMessage::GetHasLyric()) || (MusicMessage::GetCurPlayingMusic() == -1)
		|| (lCurMusicPos / 1000 >= MusicMessage::GetPlayingMusicLen() / 1000))//歌曲播放完成之后。
	{
		iCurLyricIndex = 0;//显示歌词的行数归位。
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
	//接下来是有歌词的情况。
	int iLrclen = 0, iLrcSang = 0;//一整句歌词的长度，以及唱过歌词的长度。
	SIZE curLrcLineSZ;//存储当前一整句歌词的尺寸。
	GetTextExtentPoint32(hMemDC, musicLyric[MusicMessage::GetCurLyricIndex()].szMusicLrc,
		lstrlen(musicLyric[MusicMessage::GetCurLyricIndex()].szMusicLrc), &curLrcLineSZ);
	iLrclen = curLrcLineSZ.cx;//获得当前歌曲的总长度。
	iCurLyricIndex = MusicMessage::GetCurLyricIndex();
	if (lCurMusicPos < musicLyric[iCurLyricIndex].lTime)//有可能是快退的情况。
	{
		while (lCurMusicPos < musicLyric[iCurLyricIndex].lTime)
		{
			iCurLyricIndex--;//找到下限时间下标。
			if (iCurLyricIndex < 0)  { iCurLyricIndex = 0; break; }
			if (iCurLyricIndex >= musicLyric.size() - 2)  { iCurLyricIndex = musicLyric.size() - 1 -2; break; }
		}
	}
	else//有可能是快进的情况。
	{
		while (lCurMusicPos > musicLyric[iCurLyricIndex].lTime)
		{
			iCurLyricIndex++;
			if (iCurLyricIndex < 0)  { iCurLyricIndex = 0; break; }
			if (iCurLyricIndex >= musicLyric.size() - 2)  { iCurLyricIndex = musicLyric.size() - 2; break; }//跳出之后会自动减一。
		}
		iCurLyricIndex--;//找到下限时间下标。
	}
	if ((iCurLyricIndex >= 0) && (iCurLyricIndex < ( musicLyric.size() - 2 )))//符合歌词条数条件。
	{
		lrcClientRect.left = (windowRect.right - windowRect.left - iLrclen) >> 1;
		lrcClientRect.right = (windowRect.right - windowRect.left + iLrclen) >> 1;
		lrcClientRect.top = 0;
		lrcClientRect.bottom = (windowRect.bottom - windowRect.top);
		//设置未唱过歌词的颜色。
		SetTextColor(hMemDC, RGB(100, 255, 0));
		TextOut(hMemDC, lrcClientRect.left, lrcClientRect.top, musicLyric[iCurLyricIndex].szMusicLrc,
			lstrlen(musicLyric[iCurLyricIndex].szMusicLrc));
		//设置唱过歌词的颜色。
		iLrcSang = (int)( ( (lCurMusicPos - musicLyric[iCurLyricIndex].lTime)*1.0 /
			(musicLyric[iCurLyricIndex + 1].lTime - musicLyric[iCurLyricIndex].lTime) ) * iLrclen);
		sangLrcRect.left = lrcClientRect.left;
		sangLrcRect.right = lrcClientRect.left + iLrcSang;
		sangLrcRect.top = lrcClientRect.top;
		sangLrcRect.bottom = lrcClientRect.bottom;
		SetTextColor(hMemDC, RGB(255, 0, 0));
		DrawText(hMemDC, musicLyric[iCurLyricIndex].szMusicLrc,
			lstrlen(musicLyric[iCurLyricIndex].szMusicLrc),&sangLrcRect,DT_LEFT);
		//为UpdateLayeredWindow做准备。
		screenPoint.x = windowRect.left;//窗口在屏幕中的位置。
		screenPoint.y = windowRect.top;
		curWindowSize.cx = windowRect.right - windowRect.left;//窗口在屏幕中的尺寸信息。
		curWindowSize.cy = windowRect.bottom - windowRect.top;
		memDCPoint.x = 0;//从内存DC的哪一个坐标开始拷贝。
		memDCPoint.y = 0;
		int iResult = UpdateLayeredWindow(hwnd, hDstDC, &screenPoint, &curWindowSize, hMemDC, &memDCPoint,
			RGB(0, 0, 0), &blendFunction, ULW_COLORKEY);
		if (iResult == 0)
			ShowError();
	}
}