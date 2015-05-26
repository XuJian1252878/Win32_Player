/*
*文件描述：音乐播放子窗口界面相关处理
*/

#include"stdafx.h"
#include"MusicTabDlg.h"


//包含音乐列表中每一首音乐的信息。
vector<MusicMessage> musicMessageList;
//存放音乐播放列表的全局变量。
vector<Lyric> musicLyric;
//音乐主界面中显示当前歌词的字体，上下列表的字体。
HFONT hCurMusicLrcFont, hNotCurMusicLrcFont;
HBRUSH hMusicTabBkgndBrush = NULL;//音乐主界面画刷。
HBRUSH hNULLBrush = (HBRUSH)GetStockObject(NULL_BRUSH);//创建透明画刷。


BOOL CALLBACK MainDlg_MusicPlayer_Proc(HWND hwnd, UINT uMeg, WPARAM wParam, LPARAM lParam)
{
	switch (uMeg)
	{
	case WM_INITDIALOG:
	{
		//这一句非常重要，初始化MusicMessage的句柄和控件。
		MusicMessage::InitMusicMessage(hwnd, IDC_LISTMUSIC);
		//初始化音乐播放模式复选框。
		InitPlayModeMessage(hwnd);
		//读取上一次音乐播放器关闭后保存的列表数据，该函数必须在MusicMessage::InitMusicMessage(hwnd, IDC_LISTMUSIC);后调用。
		InitMusicList(hwnd);
		//创建当前音乐播放器主界面中当前显示歌词的字体。
		hCurMusicLrcFont = CreateMusicLrcFont(hwnd, GB2312_CHARSET, OUT_OUTLINE_PRECIS, TEXT("楷体"), 27, DEFAULT_QUALITY);
		hNotCurMusicLrcFont = CreateMusicLrcFont(hwnd, GB2312_CHARSET, CLIP_DEFAULT_PRECIS, TEXT("楷体"), 17, DEFAULT_QUALITY);
		//获得音乐界面的背景。
		RECT musicTabRect;
		GetClientRect(hwnd, &musicTabRect);
		HBITMAP hMusicTabBkgndBmp = (HBITMAP)LoadImage((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			MAKEINTRESOURCE(IDB_MusicTab_Bkgnd), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_CREATEDIBSECTION | LR_SHARED);
		//获得主界面背景画刷。
		hMusicTabBkgndBrush = GetBkgndImage(hwnd, hMusicTabBkgndBmp, musicTabRect);
		//初始化歌词列表控件，确保背景能够出现。
		SendMessage(GetDlgItem(hwnd, IDC_MusicLrcPrevious), LB_INSERTSTRING, (WPARAM)-1, (LPARAM)TEXT(""));
		SendMessage(GetDlgItem(hwnd, IDC_CurMusicLrc), LB_INSERTSTRING, (WPARAM)-1, (LPARAM)TEXT(""));
		SendMessage(GetDlgItem(hwnd, IDC_MusicLrcNext), LB_INSERTSTRING, (WPARAM)-1, (LPARAM)TEXT(""));
		//释放资源。
		DeleteObject(hMusicTabBkgndBmp);
		//设置控件的tip提示信息。
//		InitMusicTabDlgCtrolTip(hwnd);
		//2注册音乐播放列表窗口支持拖拽文件。
		DragAcceptFiles(GetDlgItem(MusicMessage::GetMusicWindowHwnd(), MusicMessage::GetMusicListControlID()), TRUE);
	}
		return TRUE;
	case WM_DROPFILES://文件拖拽第三步。
	{
		HDROP hDrop = (HDROP)wParam;
		//If the index value is 0xFFFFFFFF, the return value is a count of 
		//the dropped files. Note that the index variable itself returns unchanged, and therefore remains 0xFFFFFFFF.
		vector<wstring> dropFileNameVector;
		UINT nDropFileCount = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);//获得拖拽文件的总数。
		TCHAR szDropFileName[MAX_PATH] = { NULL };
		for (int index = 0; index < nDropFileCount; index++)
		{
			DragQueryFile(hDrop, index, szDropFileName, MAX_PATH);//获得拖拽的文件名。
			dropFileNameVector.push_back(szDropFileName);
		}
		DragFinish(hDrop);
		AddMusicMessageFromVector(dropFileNameVector);
	}
		return TRUE;
	case WM_LBUTTONDOWN:
	{
		//鼠标能够拖动界面移动。
//		HWND hPWND = GetParent(hwnd);
//		SendMessage(GetParent(hwnd), WM_LBUTTONDOWN, (WPARAM)0, (LPARAM)0);//无效，为什么？
		if (MusicMessage::GetMusicVolumeWnd())//检测音量窗口是否存在。
		{
			DestroyWindow(MusicMessage::GetMusicVolumeWnd());//关闭窗口。
			MusicMessage::SetMusicVolumeWnd(NULL);
		}
		if (MusicMessage::GetSearchMusicItemWnd())//检测搜索条目窗口是否存在。
		{
			DestroyWindow(MusicMessage::GetSearchMusicItemWnd());
			MusicMessage::SetSearchMusicItemWnd(NULL);
		}
	}
		return TRUE;
	case WM_MOVE:
	{//tab父窗口移动的时候，子窗口无法接收WM_MOVE消息，相对位置不变的缘故？
	}
		return TRUE;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_MusicVolume:
		{
			if (!MusicMessage::GetMusicVolumeWnd())//音量窗口未打开的时候。
			{
				//获得音量按钮的位置。
				RECT volBtnScreenRect;
				GetWindowRect(GetDlgItem(hwnd, IDC_MusicVolume), &volBtnScreenRect);
				HWND hMusicVolWnd = CreateDialogParam((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_MusicVolume),
					hwnd, MusicVolumeDlg_Proc, (LPARAM)&volBtnScreenRect);
				MusicMessage::SetMusicVolumeWnd(hMusicVolWnd);
			}
			else//此时要关闭音量窗口。
			{
				DestroyWindow(MusicMessage::GetMusicVolumeWnd());
				//关闭之后还要将MusicMessage中音量窗口的句柄设成零。
				MusicMessage::SetMusicVolumeWnd(NULL);
			}
		}
			return TRUE;
		case IDC_Operate_File_BTN:
			LoadOperateFileMenu(hwnd);
			return TRUE;
		case IDC_MusicPlayMode://改变音乐播放模式的时候。
		{
			int iPlayMode = SendMessage(GetDlgItem(hwnd, IDC_MusicPlayMode),CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
			if (iPlayMode == CB_ERR)
			{
				return TRUE;//直接返回不做处理。
			}
			MusicMessage::SetMusicPlayMode(iPlayMode);
		}
			return TRUE;
		case IDC_LISTMUSIC:
		{
			switch (HIWORD(wParam))
			{
			case LBN_DBLCLK://当该音乐播放列表被双击的时候。
				MusicMessage::PlayMusic();
				return TRUE;
			default:
				return FALSE;
			}
		}
			return TRUE;
		//菜单选项。
		case ID_Add_LocalMusic_File:
		case ID_AddMusicFileToList:
		{
			AddMusicFile(hwnd);
		}
			return TRUE;
		case IDC_PlayPauseMusicButton://按下音乐播放按钮。
		case ID_PlayMusicItem:
		{
			MusicMessage::PlayMusic();
		}
			return TRUE;
		case ID_DeleteMusicListItem://从音乐播放器列表中删除一个曲目。
		{
			DeleteMusicItemFromList(MusicMessage::GetCurSelMusicFromMusicListControl());
		}
			return TRUE;
		case ID_DeleteMusicListItem_Storage:
		{
			DeleteMusicItemFromDisk(MusicMessage::GetCurSelMusicFromMusicListControl());
		}
			return TRUE;
		case ID_EmptyMusicList://清空音乐播放列表中的所有条目
		case ID_Clear_MusicList:
		{
			ClearAllMusicItemFromList();
		}
			return TRUE;
		case ID_Add_LocalMusic_Folder:
		case ID_Add_LocalMusic_Folder_RC:
		{
			AddLocalMusicFolder(hwnd,false);
		}
			return TRUE;
		case IDC_StopMusicButton://停止音乐的播放。
		{
			MusicMessage::CloseMusic();
		}
			return TRUE;
		case IDC_Go_KT_Music://快退播放音乐。
		{
			MusicMessage::PlayMusicFrom(MusicMessage::GetCurMusicPos() - 5000);
		}
			return TRUE;
		case IDC_Go_KJ_Music://快进播放音乐。
		{
			MusicMessage::PlayMusicFrom(MusicMessage::GetCurMusicPos() + 5000);
		}
			return TRUE;
		case IDC_Go_Last_Music://播放上一曲。
		{
			MusicMessage::PlayNextOrLast(-1);
		}
			return TRUE;
		case IDC_Go_Next_Music://播放下一曲。
		{
			MusicMessage::PlayNextOrLast(1);
		}
			return TRUE;
		case IDC_DesktopLrc:
		{
			if (!MusicMessage::GetDeskTopLrcState())//把歌词窗口打开。
			{
				HWND hDTLWnd = CreateDialog((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_DeskTopSingleLrcDlg),
					NULL, DeskTop_SingleLrcDlg_Proc);
				MusicMessage::SetDeskTopLrcState(true);
				MusicMessage::SetDeskTopLrcWnd(hDTLWnd);
			}
			else
			{
				MusicMessage::SetDeskTopLrcState(false);
				DestroyWindow(MusicMessage::GetDeskTopLrcWnd());//关闭歌词窗口。
			}
		}
			return TRUE;
		case IDC_SearchMusicItem://弹出搜索窗口。
		{
			if (!MusicMessage::GetSearchMusicItemWnd())//当搜索窗口还没有打开的时候。
			{
				//打开搜索窗口，并提供搜索窗口显示的位置。
				RECT searchItemBtnRect;
				GetWindowRect(GetDlgItem(hwnd, IDC_SearchMusicItem), &searchItemBtnRect);//屏幕坐标。
				HWND hSearchItemDlgWnd = CreateDialogParam((HINSTANCE)GetWindowLong(hwnd, GWLP_HINSTANCE),
					MAKEINTRESOURCE(IDD_SearchItemDlg), hwnd, SearchEditDlg_Proc, (LPARAM)&searchItemBtnRect);
				MusicMessage::SetSearchMusicItemWnd(hSearchItemDlgWnd);
			}
			else
			{
				//获得文本框内的数据。
				//获取数据后关闭窗口。
				DestroyWindow(MusicMessage::GetSearchMusicItemWnd());
				MusicMessage::SetSearchMusicItemWnd(NULL);
			}
		}
			return TRUE;
/*		case ID_QuitApp://该菜单项无意义。
		{
			EndDialog(GetParent(hwnd), 0);//关闭主窗口。
		}
			return TRUE;*/
		default:
			return FALSE;
		}
	}
		return TRUE;//这里TRUE or FALSE已经无所谓了。
	case WM_MUSICEND://一首音乐播放完成之后，定时器函数中会发送这个消息给窗口，窗口响应该消息后调用函数播放下一首音乐。
	{
		MusicMessage::AutoPlayNextMusic();
	}
		return TRUE;
	case WM_SEARCHITEMMSG://接受搜索框传过来的搜索消息。
	{
		TCHAR szSearchItemMsg[MAX_PATH] = { NULL };
		lstrcpy(szSearchItemMsg, (TCHAR*)wParam);//拷贝搜索框传过来的搜索消息。
//		MessageBox(hwnd, szSearchItemMsg, TEXT("Message"), MB_OK);
		//开始音乐列表的搜索操作。
		MusicMessage::FindMusicItemFromMusicList(szSearchItemMsg);
	}
		return TRUE;
	case WM_CONTEXTMENU://The WM_CONTEXTMENU message notifies a window that the user clicked the right mouse button (right-clicked) in the window.
	{
		HWND hClickWnd = (HWND)wParam;
		int iXcoordinate = (int)LOWORD(lParam);//这两个都是点击时的屏幕坐标。
		int iYcoordinate = (int)HIWORD(lParam);
		//看是来自哪个控件的右键点击操作。
		switch (GetDlgCtrlID(hClickWnd))
		{
		case IDC_LISTMUSIC:
		{
			HWND hMusicListWnd = GetDlgItem(hwnd, IDC_LISTMUSIC);
			HMENU hMenu = LoadMenu((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
				MAKEINTRESOURCE(IDR_MusicList_RightClick_Menu));
			hMenu = GetSubMenu(hMenu, 0);
			RECT musicListScreenRect;
			GetWindowRect(hMusicListWnd, &musicListScreenRect);//获得此时音乐列表的屏幕坐标。
			int iTopIndex = SendMessage(hMusicListWnd, LB_GETTOPINDEX, (WPARAM)0, (LPARAM)0);
			int iMusicListItemHeight = SendMessage(hMusicListWnd, LB_GETITEMHEIGHT, (WPARAM)iTopIndex, (LPARAM)0);//就先取第零项的高度试一下。
			if (iMusicListItemHeight == LB_ERR)
			{
				MessageBox(hwnd, TEXT("在获取音乐列表的每一项的高度时发生错误！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			}
			int iCurIndex = iTopIndex + (int)((iYcoordinate - musicListScreenRect.top) / iMusicListItemHeight);
			SendMessage(hMusicListWnd, LB_SETCURSEL, (WPARAM)iCurIndex, (LPARAM)0);
			TrackPopupMenuEx(hMenu, TPM_LEFTALIGN, iXcoordinate, iYcoordinate, hwnd, NULL);
		}
			return TRUE;
		default:
			return FALSE;
		}
	}
		return TRUE;
	case WM_HSCROLL:
	{
		switch (GetDlgCtrlID((HWND)lParam))
		{
		case IDC_MusicSlider:
			PlayingSliderChange(hwnd, (HWND)lParam, wParam);
			return TRUE;
		default:
			return FALSE;
		}
	}
		return TRUE;
	case WM_CTLCOLORDLG:
	{
		HDC hDlgDC = (HDC)wParam;
		HWND hDlgWnd = (HWND)lParam;
	}
		return (BOOL)hMusicTabBkgndBrush;
	case WM_CTLCOLORSTATIC:
	{
		HDC hStaticDC = (HDC)wParam;
		HWND hStaticWnd = (HWND)lParam;
		SetBkMode(hStaticDC, TRANSPARENT);
	}
		return (BOOL)hMusicTabBkgndBrush;
	case WM_CTLCOLOREDIT:
	{
		HDC hEditDC = (HDC)wParam;
		HWND hEditWnd = (HWND)lParam;
		SetBkMode(hEditDC, TRANSPARENT);
	}
		return (BOOL)hMusicTabBkgndBrush;
	case WM_CTLCOLORLISTBOX:
	{
		HDC hListDC = (HDC)wParam;
		HWND hListWnd = (HWND)lParam;
		SetBkMode(hListDC, TRANSPARENT);//设置背景透明。
		switch (GetDlgCtrlID(hListWnd))
		{
		//lParam  The low - order word of lParam specifies whether the control should be redrawn immediately upon 
		//	setting the font.If this parameter is TRUE, the control redraws itself.   不断重画   就会不断闪烁。
		case IDC_CurMusicLrc:
		{
			SendMessage(hListWnd, WM_SETFONT, (WPARAM)hCurMusicLrcFont, (LPARAM)0);
		}
			return (BOOL)hMusicTabBkgndBrush;
//			return (BOOL)hCurMusicLrcListBrush;
		case IDC_MusicLrcPrevious:
		{
			SendMessage(hListWnd, WM_SETFONT, (WPARAM)hNotCurMusicLrcFont, (LPARAM)0);
		}
			return (BOOL)hMusicTabBkgndBrush;
//			return (BOOL)hMusicLrcPreviousListBrush;
		case IDC_MusicLrcNext:
		{
			SendMessage(hListWnd, WM_SETFONT, (WPARAM)hNotCurMusicLrcFont, (LPARAM)0);
		}
			return (BOOL)hMusicTabBkgndBrush;
//			return (BOOL)hMusicLrcNextListBrush;
		case IDC_LISTMUSIC:
		{
		}
			return (BOOL)hMusicTabBkgndBrush;
//			return (BOOL)hMusicListBrush;
		default:
			break;
		}
	}
//		return (BOOL)hMusicTabBkgndBrush;
		return TRUE;
//		return FALSE;
	case WM_CTLCOLORSCROLLBAR:
	{
		HDC hScrollBarDC = (HDC)wParam;
		HWND hScrollBarWnd = (HWND)lParam;
		SetBkMode(hScrollBarDC, TRANSPARENT);
	}
		return (BOOL)hMusicTabBkgndBrush;
	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT lpDrawItemStruct = (LPDRAWITEMSTRUCT)lParam;
		//根据自绘控件的不同类型进行不同的操作。
		switch (lpDrawItemStruct->CtlType)
		{
		case ODT_BUTTON:
		{
			DrawBkgndToButton(hwnd, wParam, (LPDRAWITEMSTRUCT)lParam);
		}
			return TRUE;
		case ODT_LISTBOX:
		{
			switch (wParam)//Specifies the identifier of the control that sent the WM_DRAWITEM message.
			{
			case IDC_LISTMUSIC:
			{
				RedrawMusicList(hwnd, (LPDRAWITEMSTRUCT)lParam);
			}
				return TRUE;
			case IDC_CurMusicLrc://主界面当前行的歌词。
			{
				RedrawCurMusicLrc(hwnd, (LPDRAWITEMSTRUCT)lParam);
			}
				return FALSE;
			case IDC_MusicLrcPrevious://前向歌词。
			{
				RedrawPreviousMusicLrc(hwnd, (LPDRAWITEMSTRUCT)lParam);
			}
				return TRUE;
			case IDC_MusicLrcNext:
			{
				RedrawNextMusicLrc(hwnd, (LPDRAWITEMSTRUCT)lParam);
			}
				return TRUE;
			default:
				break;
			}
		}
			return TRUE;
		case ODT_COMBOBOX:
		{
			switch (wParam)
			{
			case IDC_MusicPlayMode:
			{
				SetBkMode(lpDrawItemStruct->hDC, TRANSPARENT);
				FillRect(lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, hMusicTabBkgndBrush);
				RedrawPlayModeComboBox(hwnd, (LPDRAWITEMSTRUCT)lParam);
			}
				return TRUE;
			default:
				break;
			}
		}
			return TRUE;
		default:
			return FALSE;
		}
	}
		return TRUE;//看一下 return TRUE会不会异常。
	case WM_CLOSE:
	{
		//释放资源。
		DeleteObject(hCurMusicLrcFont);
		DeleteObject(hNotCurMusicLrcFont);
		//保存播放器的一些信息。
//		SaveMusicPlayerMessage(hwnd);关闭的时候这个WM_CLOSE没有被调用，怎么回事？
		DestroyWindow(hwnd);
	}
		return TRUE;
	}
	return FALSE;
}


