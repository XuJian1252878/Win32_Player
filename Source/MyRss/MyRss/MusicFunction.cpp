/*
*学号：1252878
*姓名：许舰
*联系电话：18817870106
*电邮地址：1252878xujian@tongji.edu.cn
*文件描述：音乐播放逻辑的相关实现
*/

#include"stdafx.h"
#include"MusicFunction.h"

extern vector<MusicMessage> musicMessageList;
extern vector<Lyric> musicLyric;


//初始化音乐界面的控件tip提示消息。
bool InitMusicTabDlgCtrolTip(HWND hwnd)
{
	int iControlID[12] = { IDC_CurMusicEdit, IDC_Go_Last_Music, IDC_Go_KT_Music, IDC_PlayPauseMusicButton, IDC_StopMusicButton,
		IDC_Go_KJ_Music, IDC_Go_Next_Music, IDC_Operate_File_BTN, IDC_SearchMusicItem, IDC_MusicVolume, IDC_MusicPlayMode,
		IDC_DesktopLrc };
	TCHAR szControlTip[12][50] = { TEXT("正在播放"), TEXT("上一曲"), TEXT("快退"), TEXT("播放/暂停"), TEXT("停止"),
		TEXT("快进"), TEXT("下一曲"), TEXT("操作音乐文件"), TEXT("搜索音乐"), TEXT("播放音量"), TEXT("音乐播放模式"), TEXT("桌面歌词") };

	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
	TOOLINFO toolinfo[12];
	ZeroMemory(toolinfo, sizeof(toolinfo));
	HWND hToolTipWnd = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
		0, 0, 0, 0, hwnd, NULL, hInstance, NULL);
	for (int index = 0; index < 12; index++)
	{
		toolinfo[index].cbSize = sizeof(TOOLINFO);
		toolinfo[index].hinst = hInstance;
		toolinfo[index].lpszText = szControlTip[index];
		toolinfo[index].uId = (UINT)GetDlgItem(hwnd, iControlID[index]);
		toolinfo[index].uFlags = TTF_IDISHWND | TTF_SUBCLASS;
		SendMessage(hToolTipWnd, TTM_ADDTOOL, (WPARAM)TRUE, (LPARAM)&toolinfo[index]);
	}
	return true;
}

//播放音乐的计时器。
VOID CALLBACK MusicPlay_TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	int iCurMusicPos = (int)(MusicMessage::GetCurMusicPos() / 1000);//获得的音乐播放当前位置是毫秒，需要转化为秒。
	TCHAR szCurMusicPos[100] = { NULL };
	wsprintf(szCurMusicPos, TEXT("%02d:%02d"), (int)iCurMusicPos / 60, (int)iCurMusicPos % 60);
	SetDlgItemText(hwnd, IDC_CurMusicPos, szCurMusicPos);//设置当前已播放到的时间。

	//如果音乐播放已结束，那么播放下一首曲目。停留在秒级来比较减少毫秒无法跳转的问题。
	if (iCurMusicPos >= MusicMessage::GetPlayingMusicLen()/1000)
	{
		SendMessage(hwnd, WM_MUSICEND, (WPARAM)0, (LPARAM)0);
	}

	//设置滑块的当前位置。
	SendMessage(GetDlgItem(hwnd, IDC_MusicSlider), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)iCurMusicPos);
}


//显示歌词的计时器。
VOID CALLBACK RealTimeLrc_TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	HWND hLrcPreviousWnd = GetDlgItem(hwnd, IDC_MusicLrcPrevious);
	HWND hLrcCurWnd = GetDlgItem(hwnd, IDC_CurMusicLrc);
	HWND hLrcNextWnd = GetDlgItem(hwnd, IDC_MusicLrcNext);
	MusicMessage::ClearMusicLrcList();
	//然后再进行显示歌词的操作。
	if (MusicMessage::GetHasLyric())
	{
		long lCurMusicPos = MusicMessage::GetCurMusicPos();
		int iCurLyricIndex = MusicMessage::GetCurLyricIndex();
		//显示歌词。
		if (lCurMusicPos < musicLyric[iCurLyricIndex].lTime)//有可能是快退的情况。
		{
			while (lCurMusicPos < musicLyric[iCurLyricIndex].lTime)
			{
				iCurLyricIndex--;//找到下限时间下标。
				if (iCurLyricIndex < 0)  { iCurLyricIndex = 0; break; }
				if (iCurLyricIndex >= musicLyric.size() - 2 )  { iCurLyricIndex = musicLyric.size() - 1 - 2; break; }
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
		//下限时间下标要满足条件。
		//找到下限时间下标后，设置当前的歌词下标。
		MusicMessage::SetCurLyricIndex(iCurLyricIndex);
		for (int index = SendMessage(hLrcCurWnd, LB_GETCOUNT, (WPARAM)0, (LPARAM)0);
			index >= 0; --index)
		{
			SendMessage(hLrcCurWnd, LB_DELETESTRING, (WPARAM)index, (LPARAM)0);
		}
		SendMessage(hLrcCurWnd, LB_SETITEMHEIGHT, (WPARAM)0,
			(LPARAM)25);
		SendMessage(hLrcCurWnd, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)musicLyric[iCurLyricIndex].szMusicLrc);
		//上下方列表的歌词。
		int iLrcCount = musicLyric.size();
		int iLrcLine = 7;//上下方列表显示的歌词的条数。
		//向上方列表插入接下来的iLrcLine条歌词。
		for (int indexPrevious = iCurLyricIndex - 1, indexPreviousFlag = indexPrevious;
			(indexPrevious >= -1) && (indexPrevious >= iCurLyricIndex - iLrcLine); --indexPrevious)
		{
			if (indexPrevious == -1)
			{
				//这么做是为了背景会出现。
				SendMessage(hLrcPreviousWnd, LB_INSERTSTRING, (WPARAM)0, (LPARAM)TEXT(""));
				break;
			}
			if (indexPreviousFlag < iLrcLine)
			{
				for (int index1 = 0; index1 < iLrcLine - indexPreviousFlag - 1; index1++)
				{
					//要插入这么多空格。
					SendMessage(hLrcPreviousWnd, LB_INSERTSTRING, (WPARAM)0, (LPARAM)TEXT(""));
				}
				for (int index2 = 0; index2 <= indexPreviousFlag; index2++)
				{
					SendMessage(hLrcPreviousWnd, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)musicLyric[index2].szMusicLrc);
				}
				break;//插入完成之后推出。
			}
			SendMessage(hLrcPreviousWnd, LB_INSERTSTRING, (WPARAM)0, (LPARAM)musicLyric[indexPrevious].szMusicLrc);
		}
		//向下方列表插入接下来的iLrcLine条歌词。
		for (int indexNext = iCurLyricIndex + 1;
			(indexNext < iLrcCount) && (indexNext <= iCurLyricIndex + iLrcLine);
			++indexNext)
		{
			SendMessage(hLrcNextWnd, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)musicLyric[indexNext].szMusicLrc);
		}
	}
}



//从音乐播放列表中删除一个曲目，但不删除本地文件
//HWND 父窗口句柄,iCurSelIndex 列表中当前选中的下标。
//musicMessageList中对应的项目也会删除。
bool DeleteMusicItemFromList(int iCurSelIndex)
{
	if (iCurSelIndex == -1)
	{
		MessageBox(MusicMessage::GetMusicWindowHwnd(), TEXT("DeleteMusicItemFromList(HWND hwnd,int iCurSelIndex)  当前没有选中的音乐！！"),
			TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return false;
	}
	HWND hMusicListWnd = GetDlgItem(MusicMessage::GetMusicWindowHwnd(), MusicMessage::GetMusicListControlID());
	//先判断被删除曲目是否正在播放。
	if ((MusicMessage::GetMusicPlayState() != PLAYSTATEMENT_CLOSE)&&
		(MusicMessage::GetCurSelMusicFromMusicListControl() != iCurSelIndex))
	{
		MusicMessage::CloseMusic();
	}
	//首先将列表iCurSelIndex(包括iCurSelIndex)后面的项全部清空。
	for (int index = musicMessageList.size(); index >= iCurSelIndex; --index)
	{
		SendMessage(hMusicListWnd, LB_DELETESTRING, (WPARAM)index, (LPARAM)0);
	}
	//然后在musicMessageList中删除这一条目。
	musicMessageList.erase(musicMessageList.begin() + iCurSelIndex);
	//从这一项开始，要修改音乐播放列表的显示信息。
	for (int index = iCurSelIndex; index < musicMessageList.size(); index++)
	{
		TCHAR szMusicItemName[MAX_PATH] = { NULL };
		wsprintf(szMusicItemName, TEXT("%d.  %s"), index + 1, musicMessageList[index].GetMusicFileName());
		MusicMessage::AddMusicToList(-1, szMusicItemName);
	}
	//还要设置此时曲目的总数。
	TCHAR szMusicCount[20] = { NULL };
	wsprintf(szMusicCount, TEXT("%d"), musicMessageList.size());
	SetDlgItemText(MusicMessage::GetMusicWindowHwnd(), IDC_MusicCount, szMusicCount);
	return true;
}


//从音乐播放列表中删除一个曲目，也删除本地文件
//HWND 父窗口句柄,iCurSelIndex 列表中当前选中的下标。
//musicMessageList中对应的项目也会删除。
bool DeleteMusicItemFromDisk(int iCurSelIndex)
{
	if (iCurSelIndex == -1)
	{
		MessageBox(MusicMessage::GetMusicWindowHwnd(), TEXT("DeleteMusicItemFromList(HWND hwnd,int iCurSelIndex)  当前没有选中的音乐！！"),
			TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return false;
	}
	HWND hMusicListWnd = GetDlgItem(MusicMessage::GetMusicWindowHwnd(), MusicMessage::GetMusicListControlID());
	//首先如果这首音乐正在播放，那么就关闭这首音乐，再做后续的操作。
	if ((MusicMessage::GetMusicPlayState() != PLAYSTATEMENT_CLOSE)&&
		(MusicMessage::GetCurSelMusicFromMusicListControl() != iCurSelIndex))
	{
		MusicMessage::CloseMusic();
	}
	//首先将列表iCurSelIndex(包括iCurSelIndex)后面的项全部清空。
	for (int index = musicMessageList.size(); index >= iCurSelIndex; --index)
	{
		SendMessage(hMusicListWnd, LB_DELETESTRING, (WPARAM)index, (LPARAM)0);
	}
	//在磁盘中删除这一文件。
	DeleteFile(musicMessageList[iCurSelIndex].GetMusicFileFullPath());
	//然后在musicMessageList中删除这一条目。
	musicMessageList.erase(musicMessageList.begin() + iCurSelIndex);
	//从这一项开始，要修改音乐播放列表的显示信息。
	for (int index = iCurSelIndex; index < musicMessageList.size(); index++)
	{
		TCHAR szMusicItemName[MAX_PATH] = { NULL };
		wsprintf(szMusicItemName, TEXT("%d.  %s"), index + 1, musicMessageList[index].GetMusicFileName());
		MusicMessage::AddMusicToList(-1, szMusicItemName);
	}
	//还要设置此时曲目的总数。
	TCHAR szMusicCount[20] = { NULL };
	wsprintf(szMusicCount, TEXT("%d"), musicMessageList.size());
	SetDlgItemText(MusicMessage::GetMusicWindowHwnd(), IDC_MusicCount, szMusicCount);
	return true;
}


//清空音乐播放列表中的所有条目，HWND 父窗口句柄。
//musicMessageList中对应的项目也会删除。
bool ClearAllMusicItemFromList()
{
	HWND hMusicListWnd = GetDlgItem(MusicMessage::GetMusicWindowHwnd(), MusicMessage::GetMusicListControlID());
	//如果当前的音乐正在播放，那么先停止播放当前音乐，再清除列表。
	if (MusicMessage::GetMusicPlayState() != PLAYSTATEMENT_CLOSE)
	{
		MusicMessage::CloseMusic();
	}
	//清空列表中全部内容。
	for (int index = musicMessageList.size(); index >= 0; --index)
	{
		SendMessage(hMusicListWnd, LB_DELETESTRING, (WPARAM)index, (LPARAM)0);
	}
	//清空vector中全部内容。
	musicMessageList.erase(musicMessageList.begin(), musicMessageList.end());
	SetDlgItemText(MusicMessage::GetMusicWindowHwnd(), IDC_MusicCount, TEXT("0"));
	return true;
}


//当用户拖动歌曲播放进度条的时候响应的函数。
void PlayingSliderChange(HWND hwnd, HWND hwndCtrol, WPARAM wParam)
{
	//针对于音乐进度条的处理函数。
	//首先停止计时器。
	KillTimer(hwnd, TIMER_PLAYMUSIC);
	switch (LOWORD(wParam))
	{
		//此时相当于快退，快退五秒钟。
	case SB_LEFT:
	case SB_LINELEFT:
	case SB_PAGELEFT:
	{
		int iWillPlayPos = MusicMessage::GetCurMusicPos() - 5000;
		if (iWillPlayPos < 0) iWillPlayPos = 0;
		SendMessage(hwndCtrol, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)iWillPlayPos);
		MusicMessage::PlayMusicFrom(iWillPlayPos);
	}
		break;
		//此时相当于快退。
	case SB_RIGHT:
	case SB_LINERIGHT:
	case SB_PAGERIGHT:
	{
		int iWillPlayPos = MusicMessage::GetCurMusicPos() + 5000;
		if (iWillPlayPos >= MusicMessage::GetPlayingMusicLen()) iWillPlayPos = MusicMessage::GetPlayingMusicLen();
		SendMessage(hwndCtrol, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)iWillPlayPos);
		MusicMessage::PlayMusicFrom(iWillPlayPos);
	}
		break;
		//The user is dragging the scroll box. This message is sent repeatedly until the user releases the mouse button.
		//The high-order word indicates the position that the scroll box has been dragged to.
	case SB_THUMBTRACK:
	{
		//这个地方是以毫秒为单位的。
		int iWillPlayPos = HIWORD(wParam) * 1000 ;
		MusicMessage::PlayMusicFrom(iWillPlayPos);
	}
		break;
		//The user is dragging the scroll box. This message is sent repeatedly until the user releases the mouse button. 
		//The high-order word indicates the position that the scroll box has been dragged to.
	case SB_THUMBPOSITION:
	{
		int iWillPlayPos = HIWORD(wParam);
		SendMessage(hwndCtrol, TBM_SETPOS, (WPARAM)wParam, (LPARAM)iWillPlayPos);
		//实时变换秒数信息。iWillPlayPos本身就是秒数信息。
//		TCHAR szCurMusicPos[50] = { NULL };
//		wsprintf(szCurMusicPos, TEXT("%d:%d"), iWillPlayPos / 60, iWillPlayPos % 60);
//		SetDlgItemText(hwnd, IDC_CurMusicPos, szCurMusicPos);
//		MusicMessage::SeekMusicTo(iWillPlayPos * 1000);//PlayFrom之前要先SeekTo否则拖动的时候将会卡顿。
//		MusicMessage::PlayMusicFrom(iWillPlayPos * 1000);
	}
		break;
	default:
		break;
	}
	//处理完滑动位置之后，重新开启定时器。
	SetTimer(hwnd, TIMER_PLAYMUSIC, 100, MusicPlay_TimerProc);
}


//初始化音乐播放模式ComboBox。传入参数音乐播放窗口句柄HWND。
bool InitPlayModeMessage(HWND hwnd)
{
	HWND hCBModeWnd = GetDlgItem(hwnd, IDC_MusicPlayMode);
	SendMessage(hCBModeWnd, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)TEXT("顺序播放"));
	SendMessage(hCBModeWnd, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)TEXT("随机播放"));
	SendMessage(hCBModeWnd, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)TEXT("单曲循环"));
	SendMessage(hCBModeWnd, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)TEXT("列表循环"));
	//选择默认的音乐播放模式。
	SendMessage(hCBModeWnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	return true;
}


//对自绘控件的Button控件载入背景。
/*hwnd 父窗口的句柄 wParam lParam WM_DRAWITEM消息的参数*/
void DrawBkgndToButton(HWND hwnd, WPARAM odBtnID, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	HBITMAP hBtnBitmap = NULL;
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
	switch (odBtnID)//根据不同的按钮载入不同的图片。
	{
	case IDC_PlayPauseMusicButton:
	{
		//如果当前正在播放音乐。
		if (MusicMessage::GetMusicPlayState() == PLAYSTATEMENT_PLAY)
		{
			if (lpDrawItemStruct->itemState & ODS_SELECTED)
				hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_PlayMusic_Press));
			else if (!(lpDrawItemStruct->itemState & ODS_SELECTED))
				hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_PauseMusic_Light));
		}
		else//当前处于暂停或者停止状态。
		{
			if (lpDrawItemStruct->itemState & ODS_SELECTED)
				hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_PauseMusic_Press));
			else if (!(lpDrawItemStruct->itemState & ODS_SELECTED))
				hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_PlayMusic_Light));
		}
	}
		break;
	case IDC_StopMusicButton:
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_StopMusic_Press));
		else if (!(lpDrawItemStruct->itemState & ODS_SELECTED))
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_StopMusic_light));
	}
		break;
	case IDC_MusicVolume:
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_MusicVolume_Press));
		else if (!(lpDrawItemStruct->itemState & ODS_SELECTED))
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_MusicVolume_Light));
	}
		break;
	case IDC_Go_Next_Music:
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_GoNextMusic_Press));
		else if (!(lpDrawItemStruct->itemState & ODS_SELECTED))
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_GoNextMusic_Light));
	}
	case IDC_Go_Last_Music:
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_GoLastMusic_Press));
		else if (!(lpDrawItemStruct->itemState & ODS_SELECTED))
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_GoLastMusic_Light));
	}
		break;
	case IDC_Go_KT_Music:
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_GoKTMusic_Press));
		else if (!(lpDrawItemStruct->itemState & ODS_SELECTED))
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_GoKTMusic_Light));
	}
		break;
	case IDC_Go_KJ_Music:
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_GoKJMusic_Press));
		else if (!(lpDrawItemStruct->itemState & ODS_SELECTED))
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_GoKJMusic_Light));
	}
		break;
	case IDC_Operate_File_BTN:
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_AddFile_Press));
		else if (!(lpDrawItemStruct->itemState & ODS_SELECTED))
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_AddFile_Light));
	}
		break;
	case IDC_SearchMusicItem:
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_Search_Press));
		else if (!(lpDrawItemStruct->itemState & ODS_SELECTED))
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_Search_Light));
	}
		break;
	case IDC_ToTrayBTN:
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_Min_To_Tray_Press));
		else if (!(lpDrawItemStruct->itemState & ODS_SELECTED))
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_Min_To_Tray_Light));
	}
		break;
	case IDC_MinMainDlgBTN:
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_MinToTaskBar_Press));
		else if (!(lpDrawItemStruct->itemState & ODS_SELECTED))
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_MinToTaskBar_Light));
	}
		break;
	case IDC_AppSettingMenuBTN:
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_AppSettingMenu_Press));
		else if (!(lpDrawItemStruct->itemState & ODS_SELECTED))
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_AppSettingMenu_Light));
	}
		break;
	case IDC_StartSearchItemBtn:
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_SearchItem_Start));
		else if (!(lpDrawItemStruct->itemState & ODS_SELECTED))
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_SearchItem_Start));
	}
		break;
	case IDC_CloseSearchItemDlgBtn:
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_SearchItem_Quit_Press));
		else if (!(lpDrawItemStruct->itemState & ODS_SELECTED))
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_SearchItem_Quit_Light));
	}
		break;
	case IDC_AddRssFeedBtn:
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_AddRssFeed_Press));
		else if (!(lpDrawItemStruct->itemState & ODS_SELECTED))
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_AddRssFeed_Light));
	}
		break;
	case IDC_RssStartSearchBtn:
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_RssStartSearch_Press));
		else if (!(lpDrawItemStruct->itemState & ODS_SELECTED))
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_RssStartSearch_Light));
	}
		break;
	case IDC_RssSeeDescriptionBTN:
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_RssViewDescription_Press));
		else if (!(lpDrawItemStruct->itemState & ODS_SELECTED))
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_RssViewDescription_Light));
	}
		break;
	case IDC_SearchRssItemBtn:
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_RssItemSearch_Press));
		else if (!(lpDrawItemStruct->itemState & ODS_SELECTED))
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_RssItemSearch_Light));
	}
		break;
	case IDC_StartSearchRssItemBtn:
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_StartSearchRssItem));
		else if (!(lpDrawItemStruct->itemState & ODS_SELECTED))
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_StartSearchRssItem));
	}
		break;
	case IDC_DesktopLrc:
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_DeskTopLrc_Press));
		else if (!(lpDrawItemStruct->itemState & ODS_SELECTED))
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_DeskTopLrc_Light));
	}
		break;
	case ID_EnterMyRssPlayer:
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_EnterMainDlg_Press));
		else if (!(lpDrawItemStruct->itemState & ODS_SELECTED))
			hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_EnterMainDlg_Light));
	}
		break;
	default:
		break;
	}
	//检查图标是否顺利加载。
	if (hBtnBitmap == NULL)
	{
		MessageBox(hwnd, TEXT("自绘按钮图标加载失败！"), TEXT("错误"), MB_OK | MB_ICONERROR);
		return;
	}
	//设置按钮DC的透明属性。
	SetCtrolHbitmapTransparent(lpDrawItemStruct->hwndItem, RGB(255, 255, 255));
	//图标若加载成功，那么将图标贴上按钮。
	//建立内存DC。内存DC只有在选入位图之后才能进行位图的贴图等各种操作。
	HDC hMemDC = CreateCompatibleDC(lpDrawItemStruct->hDC);
	HBITMAP hTempBtnBitmap = CreateCompatibleBitmap(lpDrawItemStruct->hDC,
		lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom);
	SelectObject(hMemDC, hTempBtnBitmap);

	HDC hTempMemDC = CreateCompatibleDC(lpDrawItemStruct->hDC);
	SelectObject(hTempMemDC, hBtnBitmap);

	BITMAP bitmap;
	GetObject(hBtnBitmap, sizeof(bitmap), &bitmap);
	SetStretchBltMode(hMemDC, HALFTONE);
	StretchBlt(hMemDC, 0, 0, lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom,
		hTempMemDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);//两张图大小不相同的时候。
//	StretchBlt(lpDrawItemStruct->hDC, 0, 0, lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom,
//		hMemDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);//两张图大小不相同的时候。

	BitBlt(lpDrawItemStruct->hDC, 0, 0, lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom,
		hMemDC, 0, 0, SRCCOPY);
/*	POINT memDCPoint = {0,0},btnScreenPoint;
	RECT btnScreenRect;
	GetWindowRect(lpDrawItemStruct->hwndItem, &btnScreenRect);
	btnScreenPoint.x = btnScreenRect.left; btnScreenPoint.y = btnScreenRect.top;
	SIZE btnSize;
	btnSize.cx = btnScreenRect.right - btnScreenRect.left;
	btnSize.cy = btnScreenRect.bottom - btnScreenRect.top;

	int iResult = UpdateLayeredWindow(lpDrawItemStruct->hwndItem, lpDrawItemStruct->hDC, &btnScreenPoint, &btnSize, hMemDC,
		&memDCPoint, RGB(255, 255, 255), NULL, ULW_COLORKEY);
	/*	BitBlt(lpDrawItemStruct->hDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight,
	hMemDC, 0, 0, SRCCOPY);//两张图大小相同的时候。*/
	//释放资源。
	DeleteObject(hBtnBitmap);
	DeleteObject(hTempMemDC);
	DeleteObject(hTempBtnBitmap);
	DeleteObject(hMemDC);
}

//创建当前音乐播放器主界面中当前显示歌词的字体。
HFONT CreateMusicLrcFont(HWND hwnd, BYTE lfCharSet, BYTE lfClipPrecision, TCHAR* lfFaceName, LONG lfHeight, BYTE lfQuality)
{
	LOGFONT logFont;
	ZeroMemory(&logFont, sizeof(logFont));
	logFont.lfCharSet = lfCharSet;
	logFont.lfClipPrecision = lfClipPrecision;
	lstrcpy(logFont.lfFaceName, lfFaceName);
	logFont.lfHeight = lfHeight;
	logFont.lfQuality = lfQuality;
	HFONT hFont = CreateFontIndirect(&logFont);
	return hFont;
}

/*载入操作音乐文件的菜单。
传入参数，窗口句柄HWND 由此获得应用程序实例句柄,用户选择菜单项也要用到该项*/
bool LoadOperateFileMenu(HWND hwnd)
{
	HMENU hMenu = LoadMenu((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDR_Operate_File_Menu));
	hMenu = GetSubMenu(hMenu, 0);
	if (hMenu == NULL)
	{
		return false;
	}
	POINT point;//存储当前鼠标的屏幕坐标。
	GetCursorPos(&point);//Retrieves the cursor's position, in screen coordinates.
	//该函数需要的是屏幕坐标。
	TrackPopupMenuEx(hMenu, TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_LEFTBUTTON | TPM_VERNEGANIMATION | TPM_HORIZONTAL,
		point.x, point.y - 10, hwnd, NULL);
	return true;
}

/*对播放模式的ComboBox进行重绘
两个参数：hwnd 父窗口句柄
(LPDRAWITEMSTRUCT)lParam 重绘相关的信息
*/
bool RedrawPlayModeComboBox(HWND hwnd, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct->itemID == -1)
	{
		return false;
	}
	TCHAR szPlayMode[50] = { NULL };
	SendMessage(GetDlgItem(hwnd, IDC_MusicPlayMode), CB_GETLBTEXT,
		(WPARAM)lpDrawItemStruct->itemID, (LPARAM)szPlayMode);
	if (lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		SetBkMode(lpDrawItemStruct->hDC, TRANSPARENT);
		SetTextColor(lpDrawItemStruct->hDC, RGB(255, 0, 0));
	}
	else if (!(lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		SetBkMode(lpDrawItemStruct->hDC, TRANSPARENT);
		SetTextColor(lpDrawItemStruct->hDC, RGB(0, 0, 0));
	}
	DrawText(lpDrawItemStruct->hDC, szPlayMode, lstrlen(szPlayMode),
		&(lpDrawItemStruct->rcItem), DT_CENTER | DT_SINGLELINE);
	return true;
}

/*对音乐播放列表进行重绘,以及显示歌词的列表。
两个参数：hwnd 父窗口句柄
(LPDRAWITEMSTRUCT)lParam 重绘相关的信息*/
bool RedrawMusicList(HWND hwnd, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct->itemID == -1)// For an empty list box or combo box, this member can be -1.
	{
		return false;
	}
	if ((lpDrawItemStruct->itemState&ODS_SELECTED) && (lpDrawItemStruct->itemAction&(ODA_DRAWENTIRE | ODA_SELECT)))
	{
		DrawFocusRect(lpDrawItemStruct->hDC, &(lpDrawItemStruct->rcItem));//画边框。
//		InvertRect(lpDrawItemStruct->hDC, &(lpDrawItemStruct->rcItem));//反色。
	}
	else if (!(lpDrawItemStruct->itemState&ODS_SELECTED) && (lpDrawItemStruct->itemAction& (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		DrawFocusRect(lpDrawItemStruct->hDC, &(lpDrawItemStruct->rcItem));
//		InvertRect(lpDrawItemStruct->hDC, &(lpDrawItemStruct->rcItem));
	}
	TCHAR szMusicName[MAX_PATH] = { NULL };
	MusicMessage::GetMusicNameInList(lpDrawItemStruct->itemID, szMusicName);
	//选中和未选中项文字的颜色。
	if (lpDrawItemStruct->itemState&ODS_SELECTED)
	{
		SetBkMode(lpDrawItemStruct->hDC, TRANSPARENT);
		SetTextColor(lpDrawItemStruct->hDC, RGB(255, 80, 20));
	}
	else if (!(lpDrawItemStruct->itemState&ODS_SELECTED))
	{
		SetBkMode(lpDrawItemStruct->hDC, TRANSPARENT);
		SetTextColor(lpDrawItemStruct->hDC, RGB(0, 30, 20));
	}
	//重绘文字。
	DrawText(lpDrawItemStruct->hDC, szMusicName, lstrlen(szMusicName), &(lpDrawItemStruct->rcItem), DT_LEFT | DT_SINGLELINE);
	return true;
}

/*对当前音乐歌词行进行重绘,歌词显示列表
两个参数：hwnd 父窗口句柄
(LPDRAWITEMSTRUCT)lParam 重绘相关的信息*/
bool RedrawCurMusicLrc(HWND hwnd, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct->itemID == -1)
	{
		return false;
	}
	SetBkMode(lpDrawItemStruct->hDC, TRANSPARENT);
	SetTextColor(lpDrawItemStruct->hDC, RGB(255, 0, 0));
	TCHAR szCurMusicLrc[MAX_PATH] = { NULL };
	SendMessage(GetDlgItem(hwnd, IDC_CurMusicLrc), LB_GETTEXT, (WPARAM)lpDrawItemStruct->itemID, (LPARAM)szCurMusicLrc);
	SendMessage(GetDlgItem(hwnd, IDC_CurMusicLrc), LB_SETITEMHEIGHT, (WPARAM)lpDrawItemStruct->itemID,
		(LPARAM)25);
	DrawText(lpDrawItemStruct->hDC, szCurMusicLrc, lstrlen(szCurMusicLrc),
		&(lpDrawItemStruct->rcItem), DT_CENTER | DT_SINGLELINE);
	return true;
}

/*对已唱过音乐歌词行进行重绘,歌词显示列表
两个参数：hwnd 父窗口句柄
(LPDRAWITEMSTRUCT)lParam 重绘相关的信息*/
bool RedrawPreviousMusicLrc(HWND hwnd, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct->itemID == -1)
	{
		return false;
	}
	SetBkMode(lpDrawItemStruct->hDC, TRANSPARENT);
	SetTextColor(lpDrawItemStruct->hDC, RGB(100, 255, 80));
	TCHAR szPreviousMusicLrc[MAX_PATH] = { NULL };
	SendMessage(GetDlgItem(hwnd, IDC_MusicLrcPrevious), LB_GETTEXT,
		(WPARAM)lpDrawItemStruct->itemID, (LPARAM)szPreviousMusicLrc);
	SendMessage(GetDlgItem(hwnd, IDC_MusicLrcPrevious), LB_SETITEMHEIGHT, (WPARAM)lpDrawItemStruct->itemID,
		(LPARAM)30);
	DrawText(lpDrawItemStruct->hDC, szPreviousMusicLrc, lstrlen(szPreviousMusicLrc),
		&(lpDrawItemStruct->rcItem), DT_CENTER | DT_SINGLELINE);
	return true;
}

/*对未唱过音乐歌词行进行重绘,歌词显示列表
两个参数：hwnd 父窗口句柄
(LPDRAWITEMSTRUCT)lParam 重绘相关的信息*/
bool RedrawNextMusicLrc(HWND hwnd, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct->itemID == -1)
	{
		return false;
	}
	SetBkMode(lpDrawItemStruct->hDC, TRANSPARENT);
	SetTextColor(lpDrawItemStruct->hDC, RGB(10, 255, 80));
	TCHAR szPreviousMusicLrc[MAX_PATH] = { NULL };
	SendMessage(GetDlgItem(hwnd, IDC_MusicLrcNext), LB_GETTEXT,
		(WPARAM)lpDrawItemStruct->itemID, (LPARAM)szPreviousMusicLrc);
	SendMessage(GetDlgItem(hwnd, IDC_MusicLrcNext), LB_SETITEMHEIGHT, (WPARAM)lpDrawItemStruct->itemID,
		(LPARAM)30);
	DrawText(lpDrawItemStruct->hDC, szPreviousMusicLrc, lstrlen(szPreviousMusicLrc),
		&(lpDrawItemStruct->rcItem), DT_CENTER | DT_SINGLELINE);
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//显示程序错误消息。
void ShowError()
{
	TCHAR* lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | //自动分配消息缓冲区
		FORMAT_MESSAGE_FROM_SYSTEM, //从系统获取信息
		NULL, GetLastError(), //获取错误信息标识
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),//使用系统缺省语言
		(LPTSTR)&lpMsgBuf, //消息缓冲区
		0,
		NULL);
	MessageBox(NULL, lpMsgBuf, TEXT("错误码消息"), MB_ICONERROR);
}

//判断添加的音乐文件是否重复。
bool IsRepeat(TCHAR* musicFileName)
{
	for (vector<MusicMessage>::iterator iter = musicMessageList.begin(); iter != musicMessageList.end(); iter++)
	{
		if (0 == lstrcmp(iter->GetMusicFileName(), musicFileName))
		{
			return true;
		}
	}
	return false;
}

//获得分割为//的路径。
TCHAR* GetPrePath(TCHAR *sMusicName)
{
	for (int iIndex = 0; iIndex<lstrlen(sMusicName); iIndex++)
	{
		if ('\\' == sMusicName[iIndex])
		{
			for (int i = lstrlen(sMusicName); i >= iIndex; --i)
			{
				sMusicName[i + 1] = sMusicName[i];//这里有可能发生数组的溢出。
			}
			iIndex++;
		}
	}
	return sMusicName;
}

//从电脑中添加音乐文件的函数。
void AddMusicFile(HWND hwnd)
{
	TCHAR szFile[100 * MAX_PATH] = { NULL };//存储选中的文件路径名。
	TCHAR szPath[MAX_PATH] = { NULL };
	TCHAR *pFileName = NULL;

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	//一般步骤，填充结构体。
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;//szFile的第一个字段必须为空，当GetOpenFileName返回的时候，该字段中容纳的是用户所选择的文件路径名。
	ofn.lpstrFile[0] = TEXT('\0');//The first character of this buffer must be NULL if initialization is not necessary.
	ofn.nMaxFile = sizeof(szFile);//Specifies the size, in TCHARs, of the buffer pointed to by lpstrFile.
	ofn.lpstrFilter = TEXT("ALL\0*.*\0MIDI\0*.midi\0MP3\0*.mp3\0WMA\0*.wma\0\0");//Pointer to a buffer containing pairs of null-terminated filter strings. The last string in the buffer must be terminated by two NULL characters. 
	ofn.nFilterIndex = 3;//Specifies the index of the currently selected filter in the File Types control. 
	//The first pair of strings has an index value of 1, the second pair 2, and so on. An index of zero indicates the custom filter specified by lpstrCustomFilter.
	ofn.lpstrFileTitle = NULL;//Pointer to a buffer that receives the file name and extension (without path information) of the selected file. This member can be NULL.
	ofn.nMaxFileTitle = 0;//Specifies the size, in TCHARs, of the buffer pointed to by lpstrFileTitle.
	ofn.lpstrInitialDir = NULL;//Pointer to a NULL terminated string that can specify the initial directory. 有许多种算法。
	ofn.lpstrTitle = TEXT("请添加音乐文件");//Pointer to a string to be placed in the title bar of the dialog box. If this member is NULL, the system uses the default title (that is, Save As or Open).
	ofn.hwndOwner = hwnd;
	ofn.Flags = OFN_ALLOWMULTISELECT | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	//开始打开文件夹寻找音乐文件的操作。
	if (GetOpenFileName(&ofn))
	{
		lstrcpyn(szPath, szFile, ofn.nFileOffset);//lstrcpyn复制的时候在字符串的末尾自动加上\n，这个也计入复制数量中。
		if ('\\' != szPath[lstrlen(szPath) - 1])
		{
			lstrcat(szPath, TEXT("\\"));
		}
		szPath[lstrlen(szPath)] = '\0';
		//指向第一个文件名的头一个位置。
		pFileName = szFile + ofn.nFileOffset;
		//存储文件信息的暂时变量。
		TCHAR szMusicFileName[MAX_PATH] = { NULL };
		TCHAR szMusicFileFullPath[MAX_PATH] = { NULL };
		TCHAR szMusicFileShortPath[MAX_PATH] = { NULL };
		TCHAR szMusicFileType[20] = { NULL };
		TCHAR szRepeatMessage[30 * MAX_PATH] = { NULL };

		int iMusicItemCount = musicMessageList.size();//获得当前播放列表中音乐的总数。
		while (*pFileName)
		{
			//对于Explorer风格对话框，目录和文件名是被NULL分隔的，在最后的文件名后带有额外的NULL。
			MusicMessage tempMusicMessage;
			ZeroMemory(&tempMusicMessage, sizeof(tempMusicMessage));
			if (!IsRepeat(pFileName))
			{
				iMusicItemCount++;//音乐文件数量加一。不算重复的路径。
				//开始拼凑全路径。
				lstrcat(szMusicFileFullPath, szPath);
				wsprintf(szMusicFileName, TEXT("%s"), pFileName);
				lstrcat(szMusicFileFullPath, szMusicFileName);//得到有后缀名的全路径，因为GetShortPathName需要用全路径进行转化。
				//将路径信息写入暂时的MusicMessage变量。
				tempMusicMessage.SetMusicFileFullPath(szMusicFileFullPath);
				//获得该歌曲的短路径名。
				int result = GetShortPathName(GetPrePath(szMusicFileFullPath), szMusicFileShortPath, sizeof(szMusicFileShortPath));
				DWORD error = GetLastError();
				tempMusicMessage.SetMusicFileShortPath(szMusicFileShortPath);
				//再将歌曲名称中的后缀名去掉。
				int index = lstrlen(szMusicFileName);
				for (; index >= 0; --index)
				{
					if (szMusicFileName[index] == '.')
					{
						break;//获得歌曲文件名中最后一个'.'的位置。
					}
				}
				memset(szMusicFileName, 0, sizeof(szMusicFileName));
				lstrcpyn(szMusicFileName, pFileName, index + 1);//得到不包含后缀名的文件名。
				tempMusicMessage.SetMusicFileName(szMusicFileName);
				//获得歌曲文件的后缀名,包括后缀名前的一点。
				lstrcpyn(szMusicFileType, pFileName + index, lstrlen(pFileName) - index + 1);
				tempMusicMessage.SetMusicFileType(szMusicFileType);

				//将得到的歌曲文件信息放入列表中。
				musicMessageList.push_back(tempMusicMessage);
				//设置加入界面音乐播放列表中的文字。
				wsprintf(szMusicFileName, TEXT("%d.  %s"), iMusicItemCount, tempMusicMessage.GetMusicFileName());
				MusicMessage::AddMusicToList(-1, szMusicFileName);
			}
			else
			{
				lstrcat(szRepeatMessage, pFileName);
				lstrcat(szRepeatMessage, TEXT("\r\n"));
			}
			memset(szMusicFileFullPath, 0, sizeof(szMusicFileFullPath));
			memset(szMusicFileName, 0, sizeof(szMusicFileName));
			memset(szMusicFileShortPath, 0, sizeof(szMusicFileShortPath));
			memset(szMusicFileType, 0, sizeof(szMusicFileType));
			//将指针转移到下一个文件名的位置。
			pFileName += (lstrlen(pFileName) + 1);
		}
		//设置音乐播放器当前的总歌曲数。
		TCHAR szMusicCount[20] = { NULL };
		wsprintf(szMusicCount, TEXT("%d"), iMusicItemCount);
		SetDlgItemText(hwnd, IDC_MusicCount, szMusicCount);
		//将音乐重复添加信息输出。
		if ( (*szRepeatMessage) != NULL)
		{
			TCHAR szMessage[31 * MAX_PATH] = { NULL };
			wsprintf(szMessage, TEXT("歌曲\r\n %s 已存在，无法再次添加！"), szRepeatMessage);
			MessageBox(hwnd, szMessage, TEXT("提示"), MB_OK);
		}
	}
}


//浏览文件夹目录的函数，HWND 父窗口句柄，TCHAR* szSelFolderName选择文件夹的名字。
bool BrowseForFolder(HWND hwnd, TCHAR* szSelFolderPath)
{
	TCHAR szTempFolderPath[MAX_PATH] = { NULL };
	BROWSEINFO browseinfo;
	ZeroMemory(&browseinfo, sizeof(browseinfo));
	//hwndOwner：浏览文件夹对话框的父窗体句柄。
	browseinfo.hwndOwner = hwnd;
	//lpszTitle：该浏览文件夹对话框对话框的显示文本，用来提示该浏览文件夹对话框的功能、作用和目的。
	browseinfo.lpszTitle = TEXT("请选择音乐文件夹");
	//pidlRoot：ITEMIDLIST结构的地址，包含浏览时的初始根目录，而且只有被指定的目录和其子目录才显示在浏览文件夹对话框中。该成员变量可以是NULL，在此时桌面目录将被使用。
	browseinfo.pidlRoot = NULL;
	//pszDisplayName：用来保存用户选中的目录字符串的内存地址。该缓冲区的大小缺省是定义的MAX_PATH常量宏。
	browseinfo.pszDisplayName = szTempFolderPath;
	//BIF_RETURNONLYFSDIRS：仅仅返回文件系统的目录。例如：在浏览文件夹对话框中，当选中任意一个目录时，该“OK”按钮可用，而当选中“我的电脑”或“网上邻居”等非有意义的节点时，“OK”按钮为灰色。
	//BIF_BROWSEINCLUDEFILES：浏览器将显示目录，同时也显示文件。
	//BIF_RETURNFSANCESTORS：返回文件系统的一个节点。仅仅当选中的是有意义的节点时，“OK”按钮才可以使用。
	//To specify a dialog box using the newer style, set the BIF_USENEWUI flag in the ulFlags member of the BROWSEINFO structure.
	browseinfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_RETURNFSANCESTORS | BIF_USENEWUI;
	//打开选择对话框。
	//目录的返回信息就在LPITEMIDLIST 这个结构里了 LPITEMIDLIST 为SHBrowseForFolder的返回值。
	LPITEMIDLIST lpItemIDList = SHBrowseForFolder(&browseinfo);
	//用下面面这个方法把LPITEMIDLIST 结构中的路径取出放到CString m_path中
	SHGetPathFromIDList(lpItemIDList, szSelFolderPath);
	//对得到的文件保存路径进行处理。
	if (lstrlen(szSelFolderPath) != 0)
	{
		//D:\\...   D:\\123\\456  因为有这种情况。在数组里\\中的\将会被省略。
		if ((szSelFolderPath[2] == '\\') && (szSelFolderPath[3] != '\0'))
		{
			lstrcat(szSelFolderPath, TEXT("\\"));
		}
	}
	return true;
}

//添加本地音乐文件夹中的音乐文件,HWND 父窗口句柄。
//但是文件夹里面又有文件夹的情况需要考虑。
bool AddLocalMusicFolder(HWND hwnd, bool bIsDragFolder, TCHAR* szDragFolderName)
{
	TCHAR szSelFolderPath[MAX_PATH] = { NULL };
	if (bIsDragFolder)//文件夹是被拖拽进来的。
	{
		//获得用户拖拽的文件夹路径。
		lstrcpy(szSelFolderPath, szDragFolderName);
	}
	else
	{
		//获得用户选择的文件夹路径。
		BrowseForFolder(hwnd, szSelFolderPath);
	}
	//得到路径之后，确定要过滤的音乐类型。
	TCHAR szSearchCommand[MAX_PATH] = { NULL };
	wsprintf(szSearchCommand, TEXT("%s*.mp3"), szSelFolderPath);
	//开始查找操作。
	long lSearchHandle = 0;
	struct _finddata_t stFileInfo;
	ZeroMemory(&stFileInfo, sizeof(stFileInfo));
	//如果查找成功的话，将返回一个long型的唯一的查找用的句柄
	//（就是一个唯一编号）。这个句柄将在_findnext函数中被使用。若失败，则返回-1.
	char* cszSearchCommand = NULL;
	cszSearchCommand = FileEncode::UnicodeToAnsi((char*)szSearchCommand);
	lSearchHandle = _findfirst(cszSearchCommand, &stFileInfo);
	if (lSearchHandle == -1)
	{
		MessageBox(hwnd, TEXT("AddLocalMusicFolder _findfirst操作失败！！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return false;
	}
	TCHAR szFindFilePath[MAX_PATH] = { NULL };
	TCHAR* szFindFileName = NULL;
	szFindFileName = FileEncode::AnsiToUnicode(stFileInfo.name);
	wsprintf(szFindFilePath, TEXT("%s%s"), szSelFolderPath, szFindFileName);//szSelFolderPath的最后已经有\\
	//进行添加音乐文件的操作。
	AddMusicMessageFromPath(szFindFilePath);
	free(szFindFileName);
	//循环寻找文件。
	while ((_findnext(lSearchHandle, &stFileInfo)) == 0)//若成功返回0，否则返回-1.
	{
		memset(szFindFilePath, 0, sizeof(szFindFilePath));
		szFindFileName = FileEncode::AnsiToUnicode(stFileInfo.name);
		wsprintf(szFindFilePath, TEXT("%s%s"), szSelFolderPath, szFindFileName);
		AddMusicMessageFromPath(szFindFilePath);
		free(szFindFileName);
	}
	//查找完成就关闭句柄。
	_findclose(lSearchHandle);
	free(cszSearchCommand);//FileEncode操作都要手动free。
	return true;
}

//从一个包含文件路径的vector容器中添加音乐文件。
bool AddMusicMessageFromVector(vector<wstring>& szMusicFileFullPathVector)
{
	int iDropFileCount = szMusicFileFullPathVector.size();
	for (int index = 0; index < iDropFileCount; index++)
	{
		TCHAR szDropFileName[MAX_PATH] = { NULL };//保存抓取的文件名。
		lstrcpy(szDropFileName, szMusicFileFullPathVector[index].c_str());
		//被拖拽进来的内容有可能是文件夹。要考虑这种情况。
		AddMusicMessageFromPath(szDropFileName);
	}
	return true;
}

//获得读入音乐文件的路径信息。szMusicFileFullPath已经是含有\\的全路径
//szMusicFileFullPath 可能是被拖进来的文件夹路径，而不是文件路径。
//该函数中路径信息将被加入musicMessageList列表。
bool AddMusicMessageFromPath(TCHAR* szMusicFileFullPath)
{
	int iCount = musicMessageList.size() + 1;//是在列表上显示的数据。
/*	TCHAR szMusicFilePathMsg[MAX_PATH] = { NULL };
	MusicMessage MM;
	ZeroMemory(&MM, sizeof(MM));
	//获得歌曲的全路径名称。
	MM.SetMusicFileFullPath(szMusicFileFullPath);
	//获得歌曲的短路径名。
	GetShortPathName(szMusicFileFullPath, szMusicFilePathMsg, sizeof(szMusicFilePathMsg));
	MM.SetMusicFileShortPath(szMusicFilePathMsg);
	memset(szMusicFilePathMsg, 0, sizeof(szMusicFilePathMsg));
	//获取歌曲文件的类型。
	int index1 = 0;
	for (index1  = lstrlen(szMusicFileFullPath); index1 >= 0; --index1)
	{
		if (szMusicFileFullPath[index1] == '.')//这里不用TEXT也行，为什么？
		{
			break;
		}
	}*/

	//开始判断拖进来的文件是否是文件夹，通过后缀名来判断。
	bool bIsFolder = true;//标记文件名是否为文件夹的路径。
	bool bRefirstDivide = false;//是否遇到了第一个斜杠。
	int index1 = 0;
	for (index1 = lstrlen(szMusicFileFullPath) - 1; index1 >= 0; index1--)
	{
		if (szMusicFileFullPath[index1] == '\\')//遇到了倒数第一个斜杠。
		{
			break;
		}
		if (szMusicFileFullPath[index1] == '.')//遇到倒数第一个点
		{
			bIsFolder = false;
			break;
		}
	}
	if (!bIsFolder)//如果路径是一个文件的全路径。
	{
		TCHAR szMusicFilePathMsg[MAX_PATH] = { NULL };
		MusicMessage MM;
		ZeroMemory(&MM, sizeof(MM));
		//获得歌曲的全路径名称。
		MM.SetMusicFileFullPath(szMusicFileFullPath);
		//获得歌曲的短路径名。
		GetShortPathName(szMusicFileFullPath, szMusicFilePathMsg, sizeof(szMusicFilePathMsg));
		MM.SetMusicFileShortPath(szMusicFilePathMsg);
		memset(szMusicFilePathMsg, 0, sizeof(szMusicFilePathMsg));
//		ZeroMemory(&MM, sizeof(MM));
//		memset(szMusicFilePathMsg, 0, sizeof(szMusicFilePathMsg));
//		lstrcpyn(szMusicFilePathMsg, &(szMusicFileFullPath[index1]), lstrlen(szMusicFileFullPath) - index1 + 1);
//		lstrcpy(szMusicFilePathMsg, &(szMusicFileFullPath[index1]));//为什么会出现拷贝错误，？？？一切计算正常
		for (int index = index1, i = 0; index < lstrlen(szMusicFileFullPath); index++, i++)
		{
			szMusicFilePathMsg[i] = szMusicFileFullPath[index];
		}
		//判断文件的类型是否为音乐。是音乐文件的情况下进行以下操作。
		if (lstrcmp(szMusicFilePathMsg, TEXT(".mp3")) == 0 || lstrcmp(szMusicFilePathMsg, TEXT(".wav")) == 0)
		{
			MM.SetMusicFileType(szMusicFilePathMsg);
			memset(szMusicFilePathMsg, 0, sizeof(szMusicFilePathMsg));
			//获得歌曲文件的文件名。
			int index2 = 0;
			for (index2 = index1; index2 >= 0; --index2)
			{
				if (szMusicFileFullPath[index2] == '\\')
				{
					break;
				}
			}
			lstrcpyn(szMusicFilePathMsg, &(szMusicFileFullPath[index2 + 1]), index1 - index2);
			MM.SetMusicFileName(szMusicFilePathMsg);
			memset(szMusicFilePathMsg, 0, sizeof(szMusicFilePathMsg));
			//向音乐播放列表中显示信息。
			wsprintf(szMusicFilePathMsg, TEXT("%d.  %s"), iCount, MM.GetMusicFileName());
			MusicMessage::AddMusicToList(-1, szMusicFilePathMsg);
			musicMessageList.push_back(MM);
			//同时也要实时显示音乐曲目的总数。
			TCHAR szMusicCount[20] = { NULL };
			wsprintf(szMusicCount, TEXT("%d"), musicMessageList.size());
			SetDlgItemText(MusicMessage::GetMusicWindowHwnd(), IDC_MusicCount, szMusicCount);
			return true;
		}
		else
		{
			//添加的文件不是音乐类型的文件。
			TCHAR szErrorMsg[MAX_PATH] = { NULL };
			wsprintf(szErrorMsg, TEXT("%s 不是音乐类型的文件！！！"), szMusicFilePathMsg);
			MessageBox(MusicMessage::GetMusicWindowHwnd(), szErrorMsg, TEXT("Message"), MB_OK);
			return false;
		}
	}
	else//如果这个路径是一个文件夹的路径，那么递归调用AddLocalMusicFolder函数添加音乐文件。
	{
		//对得到的文件保存路径进行处理。
		if (lstrlen(szMusicFileFullPath) != 0)
		{
			//D:\\...   D:\\123\\456  因为有这种情况。在数组里\\中的\将会被省略。
			if ((szMusicFileFullPath[2] == '\\') && (szMusicFileFullPath[3] != '\0'))
			{
				lstrcat(szMusicFileFullPath, TEXT("\\"));
			}
		}
		AddLocalMusicFolder(MusicMessage::GetMusicWindowHwnd(), true, szMusicFileFullPath);
	}
	return true;
}