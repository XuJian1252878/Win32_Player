/*
*�ļ����������ֲ����Ӵ��ڽ�����ش���
*/

#include"stdafx.h"
#include"MusicTabDlg.h"


//���������б���ÿһ�����ֵ���Ϣ��
vector<MusicMessage> musicMessageList;
//������ֲ����б��ȫ�ֱ�����
vector<Lyric> musicLyric;
//��������������ʾ��ǰ��ʵ����壬�����б�����塣
HFONT hCurMusicLrcFont, hNotCurMusicLrcFont;
HBRUSH hMusicTabBkgndBrush = NULL;//���������滭ˢ��
HBRUSH hNULLBrush = (HBRUSH)GetStockObject(NULL_BRUSH);//����͸����ˢ��


BOOL CALLBACK MainDlg_MusicPlayer_Proc(HWND hwnd, UINT uMeg, WPARAM wParam, LPARAM lParam)
{
	switch (uMeg)
	{
	case WM_INITDIALOG:
	{
		//��һ��ǳ���Ҫ����ʼ��MusicMessage�ľ���Ϳؼ���
		MusicMessage::InitMusicMessage(hwnd, IDC_LISTMUSIC);
		//��ʼ�����ֲ���ģʽ��ѡ��
		InitPlayModeMessage(hwnd);
		//��ȡ��һ�����ֲ������رպ󱣴���б����ݣ��ú���������MusicMessage::InitMusicMessage(hwnd, IDC_LISTMUSIC);����á�
		InitMusicList(hwnd);
		//������ǰ���ֲ������������е�ǰ��ʾ��ʵ����塣
		hCurMusicLrcFont = CreateMusicLrcFont(hwnd, GB2312_CHARSET, OUT_OUTLINE_PRECIS, TEXT("����"), 27, DEFAULT_QUALITY);
		hNotCurMusicLrcFont = CreateMusicLrcFont(hwnd, GB2312_CHARSET, CLIP_DEFAULT_PRECIS, TEXT("����"), 17, DEFAULT_QUALITY);
		//������ֽ���ı�����
		RECT musicTabRect;
		GetClientRect(hwnd, &musicTabRect);
		HBITMAP hMusicTabBkgndBmp = (HBITMAP)LoadImage((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			MAKEINTRESOURCE(IDB_MusicTab_Bkgnd), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_CREATEDIBSECTION | LR_SHARED);
		//��������汳����ˢ��
		hMusicTabBkgndBrush = GetBkgndImage(hwnd, hMusicTabBkgndBmp, musicTabRect);
		//��ʼ������б�ؼ���ȷ�������ܹ����֡�
		SendMessage(GetDlgItem(hwnd, IDC_MusicLrcPrevious), LB_INSERTSTRING, (WPARAM)-1, (LPARAM)TEXT(""));
		SendMessage(GetDlgItem(hwnd, IDC_CurMusicLrc), LB_INSERTSTRING, (WPARAM)-1, (LPARAM)TEXT(""));
		SendMessage(GetDlgItem(hwnd, IDC_MusicLrcNext), LB_INSERTSTRING, (WPARAM)-1, (LPARAM)TEXT(""));
		//�ͷ���Դ��
		DeleteObject(hMusicTabBkgndBmp);
		//���ÿؼ���tip��ʾ��Ϣ��
//		InitMusicTabDlgCtrolTip(hwnd);
		//2ע�����ֲ����б���֧����ק�ļ���
		DragAcceptFiles(GetDlgItem(MusicMessage::GetMusicWindowHwnd(), MusicMessage::GetMusicListControlID()), TRUE);
	}
		return TRUE;
	case WM_DROPFILES://�ļ���ק��������
	{
		HDROP hDrop = (HDROP)wParam;
		//If the index value is 0xFFFFFFFF, the return value is a count of 
		//the dropped files. Note that the index variable itself returns unchanged, and therefore remains 0xFFFFFFFF.
		vector<wstring> dropFileNameVector;
		UINT nDropFileCount = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);//�����ק�ļ���������
		TCHAR szDropFileName[MAX_PATH] = { NULL };
		for (int index = 0; index < nDropFileCount; index++)
		{
			DragQueryFile(hDrop, index, szDropFileName, MAX_PATH);//�����ק���ļ�����
			dropFileNameVector.push_back(szDropFileName);
		}
		DragFinish(hDrop);
		AddMusicMessageFromVector(dropFileNameVector);
	}
		return TRUE;
	case WM_LBUTTONDOWN:
	{
		//����ܹ��϶������ƶ���
//		HWND hPWND = GetParent(hwnd);
//		SendMessage(GetParent(hwnd), WM_LBUTTONDOWN, (WPARAM)0, (LPARAM)0);//��Ч��Ϊʲô��
		if (MusicMessage::GetMusicVolumeWnd())//������������Ƿ���ڡ�
		{
			DestroyWindow(MusicMessage::GetMusicVolumeWnd());//�رմ��ڡ�
			MusicMessage::SetMusicVolumeWnd(NULL);
		}
		if (MusicMessage::GetSearchMusicItemWnd())//���������Ŀ�����Ƿ���ڡ�
		{
			DestroyWindow(MusicMessage::GetSearchMusicItemWnd());
			MusicMessage::SetSearchMusicItemWnd(NULL);
		}
	}
		return TRUE;
	case WM_MOVE:
	{//tab�������ƶ���ʱ���Ӵ����޷�����WM_MOVE��Ϣ�����λ�ò����Ե�ʣ�
	}
		return TRUE;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_MusicVolume:
		{
			if (!MusicMessage::GetMusicVolumeWnd())//��������δ�򿪵�ʱ��
			{
				//���������ť��λ�á�
				RECT volBtnScreenRect;
				GetWindowRect(GetDlgItem(hwnd, IDC_MusicVolume), &volBtnScreenRect);
				HWND hMusicVolWnd = CreateDialogParam((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_MusicVolume),
					hwnd, MusicVolumeDlg_Proc, (LPARAM)&volBtnScreenRect);
				MusicMessage::SetMusicVolumeWnd(hMusicVolWnd);
			}
			else//��ʱҪ�ر��������ڡ�
			{
				DestroyWindow(MusicMessage::GetMusicVolumeWnd());
				//�ر�֮��Ҫ��MusicMessage���������ڵľ������㡣
				MusicMessage::SetMusicVolumeWnd(NULL);
			}
		}
			return TRUE;
		case IDC_Operate_File_BTN:
			LoadOperateFileMenu(hwnd);
			return TRUE;
		case IDC_MusicPlayMode://�ı����ֲ���ģʽ��ʱ��
		{
			int iPlayMode = SendMessage(GetDlgItem(hwnd, IDC_MusicPlayMode),CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
			if (iPlayMode == CB_ERR)
			{
				return TRUE;//ֱ�ӷ��ز�������
			}
			MusicMessage::SetMusicPlayMode(iPlayMode);
		}
			return TRUE;
		case IDC_LISTMUSIC:
		{
			switch (HIWORD(wParam))
			{
			case LBN_DBLCLK://�������ֲ����б�˫����ʱ��
				MusicMessage::PlayMusic();
				return TRUE;
			default:
				return FALSE;
			}
		}
			return TRUE;
		//�˵�ѡ�
		case ID_Add_LocalMusic_File:
		case ID_AddMusicFileToList:
		{
			AddMusicFile(hwnd);
		}
			return TRUE;
		case IDC_PlayPauseMusicButton://�������ֲ��Ű�ť��
		case ID_PlayMusicItem:
		{
			MusicMessage::PlayMusic();
		}
			return TRUE;
		case ID_DeleteMusicListItem://�����ֲ������б���ɾ��һ����Ŀ��
		{
			DeleteMusicItemFromList(MusicMessage::GetCurSelMusicFromMusicListControl());
		}
			return TRUE;
		case ID_DeleteMusicListItem_Storage:
		{
			DeleteMusicItemFromDisk(MusicMessage::GetCurSelMusicFromMusicListControl());
		}
			return TRUE;
		case ID_EmptyMusicList://������ֲ����б��е�������Ŀ
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
		case IDC_StopMusicButton://ֹͣ���ֵĲ��š�
		{
			MusicMessage::CloseMusic();
		}
			return TRUE;
		case IDC_Go_KT_Music://���˲������֡�
		{
			MusicMessage::PlayMusicFrom(MusicMessage::GetCurMusicPos() - 5000);
		}
			return TRUE;
		case IDC_Go_KJ_Music://����������֡�
		{
			MusicMessage::PlayMusicFrom(MusicMessage::GetCurMusicPos() + 5000);
		}
			return TRUE;
		case IDC_Go_Last_Music://������һ����
		{
			MusicMessage::PlayNextOrLast(-1);
		}
			return TRUE;
		case IDC_Go_Next_Music://������һ����
		{
			MusicMessage::PlayNextOrLast(1);
		}
			return TRUE;
		case IDC_DesktopLrc:
		{
			if (!MusicMessage::GetDeskTopLrcState())//�Ѹ�ʴ��ڴ򿪡�
			{
				HWND hDTLWnd = CreateDialog((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_DeskTopSingleLrcDlg),
					NULL, DeskTop_SingleLrcDlg_Proc);
				MusicMessage::SetDeskTopLrcState(true);
				MusicMessage::SetDeskTopLrcWnd(hDTLWnd);
			}
			else
			{
				MusicMessage::SetDeskTopLrcState(false);
				DestroyWindow(MusicMessage::GetDeskTopLrcWnd());//�رո�ʴ��ڡ�
			}
		}
			return TRUE;
		case IDC_SearchMusicItem://�����������ڡ�
		{
			if (!MusicMessage::GetSearchMusicItemWnd())//���������ڻ�û�д򿪵�ʱ��
			{
				//���������ڣ����ṩ����������ʾ��λ�á�
				RECT searchItemBtnRect;
				GetWindowRect(GetDlgItem(hwnd, IDC_SearchMusicItem), &searchItemBtnRect);//��Ļ���ꡣ
				HWND hSearchItemDlgWnd = CreateDialogParam((HINSTANCE)GetWindowLong(hwnd, GWLP_HINSTANCE),
					MAKEINTRESOURCE(IDD_SearchItemDlg), hwnd, SearchEditDlg_Proc, (LPARAM)&searchItemBtnRect);
				MusicMessage::SetSearchMusicItemWnd(hSearchItemDlgWnd);
			}
			else
			{
				//����ı����ڵ����ݡ�
				//��ȡ���ݺ�رմ��ڡ�
				DestroyWindow(MusicMessage::GetSearchMusicItemWnd());
				MusicMessage::SetSearchMusicItemWnd(NULL);
			}
		}
			return TRUE;
/*		case ID_QuitApp://�ò˵��������塣
		{
			EndDialog(GetParent(hwnd), 0);//�ر������ڡ�
		}
			return TRUE;*/
		default:
			return FALSE;
		}
	}
		return TRUE;//����TRUE or FALSE�Ѿ�����ν�ˡ�
	case WM_MUSICEND://һ�����ֲ������֮�󣬶�ʱ�������лᷢ�������Ϣ�����ڣ�������Ӧ����Ϣ����ú���������һ�����֡�
	{
		MusicMessage::AutoPlayNextMusic();
	}
		return TRUE;
	case WM_SEARCHITEMMSG://���������򴫹�����������Ϣ��
	{
		TCHAR szSearchItemMsg[MAX_PATH] = { NULL };
		lstrcpy(szSearchItemMsg, (TCHAR*)wParam);//���������򴫹�����������Ϣ��
//		MessageBox(hwnd, szSearchItemMsg, TEXT("Message"), MB_OK);
		//��ʼ�����б������������
		MusicMessage::FindMusicItemFromMusicList(szSearchItemMsg);
	}
		return TRUE;
	case WM_CONTEXTMENU://The WM_CONTEXTMENU message notifies a window that the user clicked the right mouse button (right-clicked) in the window.
	{
		HWND hClickWnd = (HWND)wParam;
		int iXcoordinate = (int)LOWORD(lParam);//���������ǵ��ʱ����Ļ���ꡣ
		int iYcoordinate = (int)HIWORD(lParam);
		//���������ĸ��ؼ����Ҽ����������
		switch (GetDlgCtrlID(hClickWnd))
		{
		case IDC_LISTMUSIC:
		{
			HWND hMusicListWnd = GetDlgItem(hwnd, IDC_LISTMUSIC);
			HMENU hMenu = LoadMenu((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
				MAKEINTRESOURCE(IDR_MusicList_RightClick_Menu));
			hMenu = GetSubMenu(hMenu, 0);
			RECT musicListScreenRect;
			GetWindowRect(hMusicListWnd, &musicListScreenRect);//��ô�ʱ�����б����Ļ���ꡣ
			int iTopIndex = SendMessage(hMusicListWnd, LB_GETTOPINDEX, (WPARAM)0, (LPARAM)0);
			int iMusicListItemHeight = SendMessage(hMusicListWnd, LB_GETITEMHEIGHT, (WPARAM)iTopIndex, (LPARAM)0);//����ȡ������ĸ߶���һ�¡�
			if (iMusicListItemHeight == LB_ERR)
			{
				MessageBox(hwnd, TEXT("�ڻ�ȡ�����б��ÿһ��ĸ߶�ʱ��������"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
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
		SetBkMode(hListDC, TRANSPARENT);//���ñ���͸����
		switch (GetDlgCtrlID(hListWnd))
		{
		//lParam  The low - order word of lParam specifies whether the control should be redrawn immediately upon 
		//	setting the font.If this parameter is TRUE, the control redraws itself.   �����ػ�   �ͻ᲻����˸��
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
		//�����Ի�ؼ��Ĳ�ͬ���ͽ��в�ͬ�Ĳ�����
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
			case IDC_CurMusicLrc://�����浱ǰ�еĸ�ʡ�
			{
				RedrawCurMusicLrc(hwnd, (LPDRAWITEMSTRUCT)lParam);
			}
				return FALSE;
			case IDC_MusicLrcPrevious://ǰ���ʡ�
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
		return TRUE;//��һ�� return TRUE�᲻���쳣��
	case WM_CLOSE:
	{
		//�ͷ���Դ��
		DeleteObject(hCurMusicLrcFont);
		DeleteObject(hNotCurMusicLrcFont);
		//���沥������һЩ��Ϣ��
//		SaveMusicPlayerMessage(hwnd);�رյ�ʱ�����WM_CLOSEû�б����ã���ô���£�
		DestroyWindow(hwnd);
	}
		return TRUE;
	}
	return FALSE;
}


