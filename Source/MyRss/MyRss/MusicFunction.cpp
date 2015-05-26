/*
*ѧ�ţ�1252878
*��������
*��ϵ�绰��18817870106
*���ʵ�ַ��1252878xujian@tongji.edu.cn
*�ļ����������ֲ����߼������ʵ��
*/

#include"stdafx.h"
#include"MusicFunction.h"

extern vector<MusicMessage> musicMessageList;
extern vector<Lyric> musicLyric;


//��ʼ�����ֽ���Ŀؼ�tip��ʾ��Ϣ��
bool InitMusicTabDlgCtrolTip(HWND hwnd)
{
	int iControlID[12] = { IDC_CurMusicEdit, IDC_Go_Last_Music, IDC_Go_KT_Music, IDC_PlayPauseMusicButton, IDC_StopMusicButton,
		IDC_Go_KJ_Music, IDC_Go_Next_Music, IDC_Operate_File_BTN, IDC_SearchMusicItem, IDC_MusicVolume, IDC_MusicPlayMode,
		IDC_DesktopLrc };
	TCHAR szControlTip[12][50] = { TEXT("���ڲ���"), TEXT("��һ��"), TEXT("����"), TEXT("����/��ͣ"), TEXT("ֹͣ"),
		TEXT("���"), TEXT("��һ��"), TEXT("���������ļ�"), TEXT("��������"), TEXT("��������"), TEXT("���ֲ���ģʽ"), TEXT("������") };

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

//�������ֵļ�ʱ����
VOID CALLBACK MusicPlay_TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	int iCurMusicPos = (int)(MusicMessage::GetCurMusicPos() / 1000);//��õ����ֲ��ŵ�ǰλ���Ǻ��룬��Ҫת��Ϊ�롣
	TCHAR szCurMusicPos[100] = { NULL };
	wsprintf(szCurMusicPos, TEXT("%02d:%02d"), (int)iCurMusicPos / 60, (int)iCurMusicPos % 60);
	SetDlgItemText(hwnd, IDC_CurMusicPos, szCurMusicPos);//���õ�ǰ�Ѳ��ŵ���ʱ�䡣

	//������ֲ����ѽ�������ô������һ����Ŀ��ͣ�����뼶���Ƚϼ��ٺ����޷���ת�����⡣
	if (iCurMusicPos >= MusicMessage::GetPlayingMusicLen()/1000)
	{
		SendMessage(hwnd, WM_MUSICEND, (WPARAM)0, (LPARAM)0);
	}

	//���û���ĵ�ǰλ�á�
	SendMessage(GetDlgItem(hwnd, IDC_MusicSlider), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)iCurMusicPos);
}


//��ʾ��ʵļ�ʱ����
VOID CALLBACK RealTimeLrc_TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	HWND hLrcPreviousWnd = GetDlgItem(hwnd, IDC_MusicLrcPrevious);
	HWND hLrcCurWnd = GetDlgItem(hwnd, IDC_CurMusicLrc);
	HWND hLrcNextWnd = GetDlgItem(hwnd, IDC_MusicLrcNext);
	MusicMessage::ClearMusicLrcList();
	//Ȼ���ٽ�����ʾ��ʵĲ�����
	if (MusicMessage::GetHasLyric())
	{
		long lCurMusicPos = MusicMessage::GetCurMusicPos();
		int iCurLyricIndex = MusicMessage::GetCurLyricIndex();
		//��ʾ��ʡ�
		if (lCurMusicPos < musicLyric[iCurLyricIndex].lTime)//�п����ǿ��˵������
		{
			while (lCurMusicPos < musicLyric[iCurLyricIndex].lTime)
			{
				iCurLyricIndex--;//�ҵ�����ʱ���±ꡣ
				if (iCurLyricIndex < 0)  { iCurLyricIndex = 0; break; }
				if (iCurLyricIndex >= musicLyric.size() - 2 )  { iCurLyricIndex = musicLyric.size() - 1 - 2; break; }
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
		//����ʱ���±�Ҫ����������
		//�ҵ�����ʱ���±�����õ�ǰ�ĸ���±ꡣ
		MusicMessage::SetCurLyricIndex(iCurLyricIndex);
		for (int index = SendMessage(hLrcCurWnd, LB_GETCOUNT, (WPARAM)0, (LPARAM)0);
			index >= 0; --index)
		{
			SendMessage(hLrcCurWnd, LB_DELETESTRING, (WPARAM)index, (LPARAM)0);
		}
		SendMessage(hLrcCurWnd, LB_SETITEMHEIGHT, (WPARAM)0,
			(LPARAM)25);
		SendMessage(hLrcCurWnd, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)musicLyric[iCurLyricIndex].szMusicLrc);
		//���·��б�ĸ�ʡ�
		int iLrcCount = musicLyric.size();
		int iLrcLine = 7;//���·��б���ʾ�ĸ�ʵ�������
		//���Ϸ��б�����������iLrcLine����ʡ�
		for (int indexPrevious = iCurLyricIndex - 1, indexPreviousFlag = indexPrevious;
			(indexPrevious >= -1) && (indexPrevious >= iCurLyricIndex - iLrcLine); --indexPrevious)
		{
			if (indexPrevious == -1)
			{
				//��ô����Ϊ�˱�������֡�
				SendMessage(hLrcPreviousWnd, LB_INSERTSTRING, (WPARAM)0, (LPARAM)TEXT(""));
				break;
			}
			if (indexPreviousFlag < iLrcLine)
			{
				for (int index1 = 0; index1 < iLrcLine - indexPreviousFlag - 1; index1++)
				{
					//Ҫ������ô��ո�
					SendMessage(hLrcPreviousWnd, LB_INSERTSTRING, (WPARAM)0, (LPARAM)TEXT(""));
				}
				for (int index2 = 0; index2 <= indexPreviousFlag; index2++)
				{
					SendMessage(hLrcPreviousWnd, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)musicLyric[index2].szMusicLrc);
				}
				break;//�������֮���Ƴ���
			}
			SendMessage(hLrcPreviousWnd, LB_INSERTSTRING, (WPARAM)0, (LPARAM)musicLyric[indexPrevious].szMusicLrc);
		}
		//���·��б�����������iLrcLine����ʡ�
		for (int indexNext = iCurLyricIndex + 1;
			(indexNext < iLrcCount) && (indexNext <= iCurLyricIndex + iLrcLine);
			++indexNext)
		{
			SendMessage(hLrcNextWnd, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)musicLyric[indexNext].szMusicLrc);
		}
	}
}



//�����ֲ����б���ɾ��һ����Ŀ������ɾ�������ļ�
//HWND �����ھ��,iCurSelIndex �б��е�ǰѡ�е��±ꡣ
//musicMessageList�ж�Ӧ����ĿҲ��ɾ����
bool DeleteMusicItemFromList(int iCurSelIndex)
{
	if (iCurSelIndex == -1)
	{
		MessageBox(MusicMessage::GetMusicWindowHwnd(), TEXT("DeleteMusicItemFromList(HWND hwnd,int iCurSelIndex)  ��ǰû��ѡ�е����֣���"),
			TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return false;
	}
	HWND hMusicListWnd = GetDlgItem(MusicMessage::GetMusicWindowHwnd(), MusicMessage::GetMusicListControlID());
	//���жϱ�ɾ����Ŀ�Ƿ����ڲ��š�
	if ((MusicMessage::GetMusicPlayState() != PLAYSTATEMENT_CLOSE)&&
		(MusicMessage::GetCurSelMusicFromMusicListControl() != iCurSelIndex))
	{
		MusicMessage::CloseMusic();
	}
	//���Ƚ��б�iCurSelIndex(����iCurSelIndex)�������ȫ����ա�
	for (int index = musicMessageList.size(); index >= iCurSelIndex; --index)
	{
		SendMessage(hMusicListWnd, LB_DELETESTRING, (WPARAM)index, (LPARAM)0);
	}
	//Ȼ����musicMessageList��ɾ����һ��Ŀ��
	musicMessageList.erase(musicMessageList.begin() + iCurSelIndex);
	//����һ�ʼ��Ҫ�޸����ֲ����б����ʾ��Ϣ��
	for (int index = iCurSelIndex; index < musicMessageList.size(); index++)
	{
		TCHAR szMusicItemName[MAX_PATH] = { NULL };
		wsprintf(szMusicItemName, TEXT("%d.  %s"), index + 1, musicMessageList[index].GetMusicFileName());
		MusicMessage::AddMusicToList(-1, szMusicItemName);
	}
	//��Ҫ���ô�ʱ��Ŀ��������
	TCHAR szMusicCount[20] = { NULL };
	wsprintf(szMusicCount, TEXT("%d"), musicMessageList.size());
	SetDlgItemText(MusicMessage::GetMusicWindowHwnd(), IDC_MusicCount, szMusicCount);
	return true;
}


//�����ֲ����б���ɾ��һ����Ŀ��Ҳɾ�������ļ�
//HWND �����ھ��,iCurSelIndex �б��е�ǰѡ�е��±ꡣ
//musicMessageList�ж�Ӧ����ĿҲ��ɾ����
bool DeleteMusicItemFromDisk(int iCurSelIndex)
{
	if (iCurSelIndex == -1)
	{
		MessageBox(MusicMessage::GetMusicWindowHwnd(), TEXT("DeleteMusicItemFromList(HWND hwnd,int iCurSelIndex)  ��ǰû��ѡ�е����֣���"),
			TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return false;
	}
	HWND hMusicListWnd = GetDlgItem(MusicMessage::GetMusicWindowHwnd(), MusicMessage::GetMusicListControlID());
	//������������������ڲ��ţ���ô�͹ر��������֣����������Ĳ�����
	if ((MusicMessage::GetMusicPlayState() != PLAYSTATEMENT_CLOSE)&&
		(MusicMessage::GetCurSelMusicFromMusicListControl() != iCurSelIndex))
	{
		MusicMessage::CloseMusic();
	}
	//���Ƚ��б�iCurSelIndex(����iCurSelIndex)�������ȫ����ա�
	for (int index = musicMessageList.size(); index >= iCurSelIndex; --index)
	{
		SendMessage(hMusicListWnd, LB_DELETESTRING, (WPARAM)index, (LPARAM)0);
	}
	//�ڴ�����ɾ����һ�ļ���
	DeleteFile(musicMessageList[iCurSelIndex].GetMusicFileFullPath());
	//Ȼ����musicMessageList��ɾ����һ��Ŀ��
	musicMessageList.erase(musicMessageList.begin() + iCurSelIndex);
	//����һ�ʼ��Ҫ�޸����ֲ����б����ʾ��Ϣ��
	for (int index = iCurSelIndex; index < musicMessageList.size(); index++)
	{
		TCHAR szMusicItemName[MAX_PATH] = { NULL };
		wsprintf(szMusicItemName, TEXT("%d.  %s"), index + 1, musicMessageList[index].GetMusicFileName());
		MusicMessage::AddMusicToList(-1, szMusicItemName);
	}
	//��Ҫ���ô�ʱ��Ŀ��������
	TCHAR szMusicCount[20] = { NULL };
	wsprintf(szMusicCount, TEXT("%d"), musicMessageList.size());
	SetDlgItemText(MusicMessage::GetMusicWindowHwnd(), IDC_MusicCount, szMusicCount);
	return true;
}


//������ֲ����б��е�������Ŀ��HWND �����ھ����
//musicMessageList�ж�Ӧ����ĿҲ��ɾ����
bool ClearAllMusicItemFromList()
{
	HWND hMusicListWnd = GetDlgItem(MusicMessage::GetMusicWindowHwnd(), MusicMessage::GetMusicListControlID());
	//�����ǰ���������ڲ��ţ���ô��ֹͣ���ŵ�ǰ���֣�������б�
	if (MusicMessage::GetMusicPlayState() != PLAYSTATEMENT_CLOSE)
	{
		MusicMessage::CloseMusic();
	}
	//����б���ȫ�����ݡ�
	for (int index = musicMessageList.size(); index >= 0; --index)
	{
		SendMessage(hMusicListWnd, LB_DELETESTRING, (WPARAM)index, (LPARAM)0);
	}
	//���vector��ȫ�����ݡ�
	musicMessageList.erase(musicMessageList.begin(), musicMessageList.end());
	SetDlgItemText(MusicMessage::GetMusicWindowHwnd(), IDC_MusicCount, TEXT("0"));
	return true;
}


//���û��϶��������Ž�������ʱ����Ӧ�ĺ�����
void PlayingSliderChange(HWND hwnd, HWND hwndCtrol, WPARAM wParam)
{
	//��������ֽ������Ĵ�������
	//����ֹͣ��ʱ����
	KillTimer(hwnd, TIMER_PLAYMUSIC);
	switch (LOWORD(wParam))
	{
		//��ʱ�൱�ڿ��ˣ����������ӡ�
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
		//��ʱ�൱�ڿ��ˡ�
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
		//����ط����Ժ���Ϊ��λ�ġ�
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
		//ʵʱ�任������Ϣ��iWillPlayPos�������������Ϣ��
//		TCHAR szCurMusicPos[50] = { NULL };
//		wsprintf(szCurMusicPos, TEXT("%d:%d"), iWillPlayPos / 60, iWillPlayPos % 60);
//		SetDlgItemText(hwnd, IDC_CurMusicPos, szCurMusicPos);
//		MusicMessage::SeekMusicTo(iWillPlayPos * 1000);//PlayFrom֮ǰҪ��SeekTo�����϶���ʱ�򽫻Ῠ�١�
//		MusicMessage::PlayMusicFrom(iWillPlayPos * 1000);
	}
		break;
	default:
		break;
	}
	//�����껬��λ��֮�����¿�����ʱ����
	SetTimer(hwnd, TIMER_PLAYMUSIC, 100, MusicPlay_TimerProc);
}


//��ʼ�����ֲ���ģʽComboBox������������ֲ��Ŵ��ھ��HWND��
bool InitPlayModeMessage(HWND hwnd)
{
	HWND hCBModeWnd = GetDlgItem(hwnd, IDC_MusicPlayMode);
	SendMessage(hCBModeWnd, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)TEXT("˳�򲥷�"));
	SendMessage(hCBModeWnd, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)TEXT("�������"));
	SendMessage(hCBModeWnd, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)TEXT("����ѭ��"));
	SendMessage(hCBModeWnd, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)TEXT("�б�ѭ��"));
	//ѡ��Ĭ�ϵ����ֲ���ģʽ��
	SendMessage(hCBModeWnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	return true;
}


//���Ի�ؼ���Button�ؼ����뱳����
/*hwnd �����ڵľ�� wParam lParam WM_DRAWITEM��Ϣ�Ĳ���*/
void DrawBkgndToButton(HWND hwnd, WPARAM odBtnID, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	HBITMAP hBtnBitmap = NULL;
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
	switch (odBtnID)//���ݲ�ͬ�İ�ť���벻ͬ��ͼƬ��
	{
	case IDC_PlayPauseMusicButton:
	{
		//�����ǰ���ڲ������֡�
		if (MusicMessage::GetMusicPlayState() == PLAYSTATEMENT_PLAY)
		{
			if (lpDrawItemStruct->itemState & ODS_SELECTED)
				hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_PlayMusic_Press));
			else if (!(lpDrawItemStruct->itemState & ODS_SELECTED))
				hBtnBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_PauseMusic_Light));
		}
		else//��ǰ������ͣ����ֹͣ״̬��
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
	//���ͼ���Ƿ�˳�����ء�
	if (hBtnBitmap == NULL)
	{
		MessageBox(hwnd, TEXT("�Ի水ťͼ�����ʧ�ܣ�"), TEXT("����"), MB_OK | MB_ICONERROR);
		return;
	}
	//���ð�ťDC��͸�����ԡ�
	SetCtrolHbitmapTransparent(lpDrawItemStruct->hwndItem, RGB(255, 255, 255));
	//ͼ�������سɹ�����ô��ͼ�����ϰ�ť��
	//�����ڴ�DC���ڴ�DCֻ����ѡ��λͼ֮����ܽ���λͼ����ͼ�ȸ��ֲ�����
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
		hTempMemDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);//����ͼ��С����ͬ��ʱ��
//	StretchBlt(lpDrawItemStruct->hDC, 0, 0, lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom,
//		hMemDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);//����ͼ��С����ͬ��ʱ��

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
	hMemDC, 0, 0, SRCCOPY);//����ͼ��С��ͬ��ʱ��*/
	//�ͷ���Դ��
	DeleteObject(hBtnBitmap);
	DeleteObject(hTempMemDC);
	DeleteObject(hTempBtnBitmap);
	DeleteObject(hMemDC);
}

//������ǰ���ֲ������������е�ǰ��ʾ��ʵ����塣
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

/*������������ļ��Ĳ˵���
������������ھ��HWND �ɴ˻��Ӧ�ó���ʵ�����,�û�ѡ��˵���ҲҪ�õ�����*/
bool LoadOperateFileMenu(HWND hwnd)
{
	HMENU hMenu = LoadMenu((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDR_Operate_File_Menu));
	hMenu = GetSubMenu(hMenu, 0);
	if (hMenu == NULL)
	{
		return false;
	}
	POINT point;//�洢��ǰ������Ļ���ꡣ
	GetCursorPos(&point);//Retrieves the cursor's position, in screen coordinates.
	//�ú�����Ҫ������Ļ���ꡣ
	TrackPopupMenuEx(hMenu, TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_LEFTBUTTON | TPM_VERNEGANIMATION | TPM_HORIZONTAL,
		point.x, point.y - 10, hwnd, NULL);
	return true;
}

/*�Բ���ģʽ��ComboBox�����ػ�
����������hwnd �����ھ��
(LPDRAWITEMSTRUCT)lParam �ػ���ص���Ϣ
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

/*�����ֲ����б�����ػ�,�Լ���ʾ��ʵ��б�
����������hwnd �����ھ��
(LPDRAWITEMSTRUCT)lParam �ػ���ص���Ϣ*/
bool RedrawMusicList(HWND hwnd, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct->itemID == -1)// For an empty list box or combo box, this member can be -1.
	{
		return false;
	}
	if ((lpDrawItemStruct->itemState&ODS_SELECTED) && (lpDrawItemStruct->itemAction&(ODA_DRAWENTIRE | ODA_SELECT)))
	{
		DrawFocusRect(lpDrawItemStruct->hDC, &(lpDrawItemStruct->rcItem));//���߿�
//		InvertRect(lpDrawItemStruct->hDC, &(lpDrawItemStruct->rcItem));//��ɫ��
	}
	else if (!(lpDrawItemStruct->itemState&ODS_SELECTED) && (lpDrawItemStruct->itemAction& (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		DrawFocusRect(lpDrawItemStruct->hDC, &(lpDrawItemStruct->rcItem));
//		InvertRect(lpDrawItemStruct->hDC, &(lpDrawItemStruct->rcItem));
	}
	TCHAR szMusicName[MAX_PATH] = { NULL };
	MusicMessage::GetMusicNameInList(lpDrawItemStruct->itemID, szMusicName);
	//ѡ�к�δѡ�������ֵ���ɫ��
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
	//�ػ����֡�
	DrawText(lpDrawItemStruct->hDC, szMusicName, lstrlen(szMusicName), &(lpDrawItemStruct->rcItem), DT_LEFT | DT_SINGLELINE);
	return true;
}

/*�Ե�ǰ���ָ���н����ػ�,�����ʾ�б�
����������hwnd �����ھ��
(LPDRAWITEMSTRUCT)lParam �ػ���ص���Ϣ*/
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

/*���ѳ������ָ���н����ػ�,�����ʾ�б�
����������hwnd �����ھ��
(LPDRAWITEMSTRUCT)lParam �ػ���ص���Ϣ*/
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

/*��δ�������ָ���н����ػ�,�����ʾ�б�
����������hwnd �����ھ��
(LPDRAWITEMSTRUCT)lParam �ػ���ص���Ϣ*/
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
//��ʾ���������Ϣ��
void ShowError()
{
	TCHAR* lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | //�Զ�������Ϣ������
		FORMAT_MESSAGE_FROM_SYSTEM, //��ϵͳ��ȡ��Ϣ
		NULL, GetLastError(), //��ȡ������Ϣ��ʶ
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),//ʹ��ϵͳȱʡ����
		(LPTSTR)&lpMsgBuf, //��Ϣ������
		0,
		NULL);
	MessageBox(NULL, lpMsgBuf, TEXT("��������Ϣ"), MB_ICONERROR);
}

//�ж���ӵ������ļ��Ƿ��ظ���
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

//��÷ָ�Ϊ//��·����
TCHAR* GetPrePath(TCHAR *sMusicName)
{
	for (int iIndex = 0; iIndex<lstrlen(sMusicName); iIndex++)
	{
		if ('\\' == sMusicName[iIndex])
		{
			for (int i = lstrlen(sMusicName); i >= iIndex; --i)
			{
				sMusicName[i + 1] = sMusicName[i];//�����п��ܷ�������������
			}
			iIndex++;
		}
	}
	return sMusicName;
}

//�ӵ�������������ļ��ĺ�����
void AddMusicFile(HWND hwnd)
{
	TCHAR szFile[100 * MAX_PATH] = { NULL };//�洢ѡ�е��ļ�·������
	TCHAR szPath[MAX_PATH] = { NULL };
	TCHAR *pFileName = NULL;

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	//һ�㲽�裬���ṹ�塣
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;//szFile�ĵ�һ���ֶα���Ϊ�գ���GetOpenFileName���ص�ʱ�򣬸��ֶ������ɵ����û���ѡ����ļ�·������
	ofn.lpstrFile[0] = TEXT('\0');//The first character of this buffer must be NULL if initialization is not necessary.
	ofn.nMaxFile = sizeof(szFile);//Specifies the size, in TCHARs, of the buffer pointed to by lpstrFile.
	ofn.lpstrFilter = TEXT("ALL\0*.*\0MIDI\0*.midi\0MP3\0*.mp3\0WMA\0*.wma\0\0");//Pointer to a buffer containing pairs of null-terminated filter strings. The last string in the buffer must be terminated by two NULL characters. 
	ofn.nFilterIndex = 3;//Specifies the index of the currently selected filter in the File Types control. 
	//The first pair of strings has an index value of 1, the second pair 2, and so on. An index of zero indicates the custom filter specified by lpstrCustomFilter.
	ofn.lpstrFileTitle = NULL;//Pointer to a buffer that receives the file name and extension (without path information) of the selected file. This member can be NULL.
	ofn.nMaxFileTitle = 0;//Specifies the size, in TCHARs, of the buffer pointed to by lpstrFileTitle.
	ofn.lpstrInitialDir = NULL;//Pointer to a NULL terminated string that can specify the initial directory. ��������㷨��
	ofn.lpstrTitle = TEXT("����������ļ�");//Pointer to a string to be placed in the title bar of the dialog box. If this member is NULL, the system uses the default title (that is, Save As or Open).
	ofn.hwndOwner = hwnd;
	ofn.Flags = OFN_ALLOWMULTISELECT | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	//��ʼ���ļ���Ѱ�������ļ��Ĳ�����
	if (GetOpenFileName(&ofn))
	{
		lstrcpyn(szPath, szFile, ofn.nFileOffset);//lstrcpyn���Ƶ�ʱ�����ַ�����ĩβ�Զ�����\n�����Ҳ���븴�������С�
		if ('\\' != szPath[lstrlen(szPath) - 1])
		{
			lstrcat(szPath, TEXT("\\"));
		}
		szPath[lstrlen(szPath)] = '\0';
		//ָ���һ���ļ�����ͷһ��λ�á�
		pFileName = szFile + ofn.nFileOffset;
		//�洢�ļ���Ϣ����ʱ������
		TCHAR szMusicFileName[MAX_PATH] = { NULL };
		TCHAR szMusicFileFullPath[MAX_PATH] = { NULL };
		TCHAR szMusicFileShortPath[MAX_PATH] = { NULL };
		TCHAR szMusicFileType[20] = { NULL };
		TCHAR szRepeatMessage[30 * MAX_PATH] = { NULL };

		int iMusicItemCount = musicMessageList.size();//��õ�ǰ�����б������ֵ�������
		while (*pFileName)
		{
			//����Explorer���Ի���Ŀ¼���ļ����Ǳ�NULL�ָ��ģ��������ļ�������ж����NULL��
			MusicMessage tempMusicMessage;
			ZeroMemory(&tempMusicMessage, sizeof(tempMusicMessage));
			if (!IsRepeat(pFileName))
			{
				iMusicItemCount++;//�����ļ�������һ�������ظ���·����
				//��ʼƴ��ȫ·����
				lstrcat(szMusicFileFullPath, szPath);
				wsprintf(szMusicFileName, TEXT("%s"), pFileName);
				lstrcat(szMusicFileFullPath, szMusicFileName);//�õ��к�׺����ȫ·������ΪGetShortPathName��Ҫ��ȫ·������ת����
				//��·����Ϣд����ʱ��MusicMessage������
				tempMusicMessage.SetMusicFileFullPath(szMusicFileFullPath);
				//��øø����Ķ�·������
				int result = GetShortPathName(GetPrePath(szMusicFileFullPath), szMusicFileShortPath, sizeof(szMusicFileShortPath));
				DWORD error = GetLastError();
				tempMusicMessage.SetMusicFileShortPath(szMusicFileShortPath);
				//�ٽ����������еĺ�׺��ȥ����
				int index = lstrlen(szMusicFileName);
				for (; index >= 0; --index)
				{
					if (szMusicFileName[index] == '.')
					{
						break;//��ø����ļ��������һ��'.'��λ�á�
					}
				}
				memset(szMusicFileName, 0, sizeof(szMusicFileName));
				lstrcpyn(szMusicFileName, pFileName, index + 1);//�õ���������׺�����ļ�����
				tempMusicMessage.SetMusicFileName(szMusicFileName);
				//��ø����ļ��ĺ�׺��,������׺��ǰ��һ�㡣
				lstrcpyn(szMusicFileType, pFileName + index, lstrlen(pFileName) - index + 1);
				tempMusicMessage.SetMusicFileType(szMusicFileType);

				//���õ��ĸ����ļ���Ϣ�����б��С�
				musicMessageList.push_back(tempMusicMessage);
				//���ü���������ֲ����б��е����֡�
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
			//��ָ��ת�Ƶ���һ���ļ�����λ�á�
			pFileName += (lstrlen(pFileName) + 1);
		}
		//�������ֲ�������ǰ���ܸ�������
		TCHAR szMusicCount[20] = { NULL };
		wsprintf(szMusicCount, TEXT("%d"), iMusicItemCount);
		SetDlgItemText(hwnd, IDC_MusicCount, szMusicCount);
		//�������ظ������Ϣ�����
		if ( (*szRepeatMessage) != NULL)
		{
			TCHAR szMessage[31 * MAX_PATH] = { NULL };
			wsprintf(szMessage, TEXT("����\r\n %s �Ѵ��ڣ��޷��ٴ���ӣ�"), szRepeatMessage);
			MessageBox(hwnd, szMessage, TEXT("��ʾ"), MB_OK);
		}
	}
}


//����ļ���Ŀ¼�ĺ�����HWND �����ھ����TCHAR* szSelFolderNameѡ���ļ��е����֡�
bool BrowseForFolder(HWND hwnd, TCHAR* szSelFolderPath)
{
	TCHAR szTempFolderPath[MAX_PATH] = { NULL };
	BROWSEINFO browseinfo;
	ZeroMemory(&browseinfo, sizeof(browseinfo));
	//hwndOwner������ļ��жԻ���ĸ���������
	browseinfo.hwndOwner = hwnd;
	//lpszTitle��������ļ��жԻ���Ի������ʾ�ı���������ʾ������ļ��жԻ���Ĺ��ܡ����ú�Ŀ�ġ�
	browseinfo.lpszTitle = TEXT("��ѡ�������ļ���");
	//pidlRoot��ITEMIDLIST�ṹ�ĵ�ַ���������ʱ�ĳ�ʼ��Ŀ¼������ֻ�б�ָ����Ŀ¼������Ŀ¼����ʾ������ļ��жԻ����С��ó�Ա����������NULL���ڴ�ʱ����Ŀ¼����ʹ�á�
	browseinfo.pidlRoot = NULL;
	//pszDisplayName�����������û�ѡ�е�Ŀ¼�ַ������ڴ��ַ���û������Ĵ�Сȱʡ�Ƕ����MAX_PATH�����ꡣ
	browseinfo.pszDisplayName = szTempFolderPath;
	//BIF_RETURNONLYFSDIRS�����������ļ�ϵͳ��Ŀ¼�����磺������ļ��жԻ����У���ѡ������һ��Ŀ¼ʱ���á�OK����ť���ã�����ѡ�С��ҵĵ��ԡ��������ھӡ��ȷ�������Ľڵ�ʱ����OK����ťΪ��ɫ��
	//BIF_BROWSEINCLUDEFILES�����������ʾĿ¼��ͬʱҲ��ʾ�ļ���
	//BIF_RETURNFSANCESTORS�������ļ�ϵͳ��һ���ڵ㡣������ѡ�е���������Ľڵ�ʱ����OK����ť�ſ���ʹ�á�
	//To specify a dialog box using the newer style, set the BIF_USENEWUI flag in the ulFlags member of the BROWSEINFO structure.
	browseinfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_RETURNFSANCESTORS | BIF_USENEWUI;
	//��ѡ��Ի���
	//Ŀ¼�ķ�����Ϣ����LPITEMIDLIST ����ṹ���� LPITEMIDLIST ΪSHBrowseForFolder�ķ���ֵ��
	LPITEMIDLIST lpItemIDList = SHBrowseForFolder(&browseinfo);
	//�����������������LPITEMIDLIST �ṹ�е�·��ȡ���ŵ�CString m_path��
	SHGetPathFromIDList(lpItemIDList, szSelFolderPath);
	//�Եõ����ļ�����·�����д���
	if (lstrlen(szSelFolderPath) != 0)
	{
		//D:\\...   D:\\123\\456  ��Ϊ�������������������\\�е�\���ᱻʡ�ԡ�
		if ((szSelFolderPath[2] == '\\') && (szSelFolderPath[3] != '\0'))
		{
			lstrcat(szSelFolderPath, TEXT("\\"));
		}
	}
	return true;
}

//��ӱ��������ļ����е������ļ�,HWND �����ھ����
//�����ļ������������ļ��е������Ҫ���ǡ�
bool AddLocalMusicFolder(HWND hwnd, bool bIsDragFolder, TCHAR* szDragFolderName)
{
	TCHAR szSelFolderPath[MAX_PATH] = { NULL };
	if (bIsDragFolder)//�ļ����Ǳ���ק�����ġ�
	{
		//����û���ק���ļ���·����
		lstrcpy(szSelFolderPath, szDragFolderName);
	}
	else
	{
		//����û�ѡ����ļ���·����
		BrowseForFolder(hwnd, szSelFolderPath);
	}
	//�õ�·��֮��ȷ��Ҫ���˵��������͡�
	TCHAR szSearchCommand[MAX_PATH] = { NULL };
	wsprintf(szSearchCommand, TEXT("%s*.mp3"), szSelFolderPath);
	//��ʼ���Ҳ�����
	long lSearchHandle = 0;
	struct _finddata_t stFileInfo;
	ZeroMemory(&stFileInfo, sizeof(stFileInfo));
	//������ҳɹ��Ļ���������һ��long�͵�Ψһ�Ĳ����õľ��
	//������һ��Ψһ��ţ�������������_findnext�����б�ʹ�á���ʧ�ܣ��򷵻�-1.
	char* cszSearchCommand = NULL;
	cszSearchCommand = FileEncode::UnicodeToAnsi((char*)szSearchCommand);
	lSearchHandle = _findfirst(cszSearchCommand, &stFileInfo);
	if (lSearchHandle == -1)
	{
		MessageBox(hwnd, TEXT("AddLocalMusicFolder _findfirst����ʧ�ܣ���"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return false;
	}
	TCHAR szFindFilePath[MAX_PATH] = { NULL };
	TCHAR* szFindFileName = NULL;
	szFindFileName = FileEncode::AnsiToUnicode(stFileInfo.name);
	wsprintf(szFindFilePath, TEXT("%s%s"), szSelFolderPath, szFindFileName);//szSelFolderPath������Ѿ���\\
	//������������ļ��Ĳ�����
	AddMusicMessageFromPath(szFindFilePath);
	free(szFindFileName);
	//ѭ��Ѱ���ļ���
	while ((_findnext(lSearchHandle, &stFileInfo)) == 0)//���ɹ�����0�����򷵻�-1.
	{
		memset(szFindFilePath, 0, sizeof(szFindFilePath));
		szFindFileName = FileEncode::AnsiToUnicode(stFileInfo.name);
		wsprintf(szFindFilePath, TEXT("%s%s"), szSelFolderPath, szFindFileName);
		AddMusicMessageFromPath(szFindFilePath);
		free(szFindFileName);
	}
	//������ɾ͹رվ����
	_findclose(lSearchHandle);
	free(cszSearchCommand);//FileEncode������Ҫ�ֶ�free��
	return true;
}

//��һ�������ļ�·����vector��������������ļ���
bool AddMusicMessageFromVector(vector<wstring>& szMusicFileFullPathVector)
{
	int iDropFileCount = szMusicFileFullPathVector.size();
	for (int index = 0; index < iDropFileCount; index++)
	{
		TCHAR szDropFileName[MAX_PATH] = { NULL };//����ץȡ���ļ�����
		lstrcpy(szDropFileName, szMusicFileFullPathVector[index].c_str());
		//����ק�����������п������ļ��С�Ҫ�������������
		AddMusicMessageFromPath(szDropFileName);
	}
	return true;
}

//��ö��������ļ���·����Ϣ��szMusicFileFullPath�Ѿ��Ǻ���\\��ȫ·��
//szMusicFileFullPath �����Ǳ��Ͻ������ļ���·�����������ļ�·����
//�ú�����·����Ϣ��������musicMessageList�б�
bool AddMusicMessageFromPath(TCHAR* szMusicFileFullPath)
{
	int iCount = musicMessageList.size() + 1;//�����б�����ʾ�����ݡ�
/*	TCHAR szMusicFilePathMsg[MAX_PATH] = { NULL };
	MusicMessage MM;
	ZeroMemory(&MM, sizeof(MM));
	//��ø�����ȫ·�����ơ�
	MM.SetMusicFileFullPath(szMusicFileFullPath);
	//��ø����Ķ�·������
	GetShortPathName(szMusicFileFullPath, szMusicFilePathMsg, sizeof(szMusicFilePathMsg));
	MM.SetMusicFileShortPath(szMusicFilePathMsg);
	memset(szMusicFilePathMsg, 0, sizeof(szMusicFilePathMsg));
	//��ȡ�����ļ������͡�
	int index1 = 0;
	for (index1  = lstrlen(szMusicFileFullPath); index1 >= 0; --index1)
	{
		if (szMusicFileFullPath[index1] == '.')//���ﲻ��TEXTҲ�У�Ϊʲô��
		{
			break;
		}
	}*/

	//��ʼ�ж��Ͻ������ļ��Ƿ����ļ��У�ͨ����׺�����жϡ�
	bool bIsFolder = true;//����ļ����Ƿ�Ϊ�ļ��е�·����
	bool bRefirstDivide = false;//�Ƿ������˵�һ��б�ܡ�
	int index1 = 0;
	for (index1 = lstrlen(szMusicFileFullPath) - 1; index1 >= 0; index1--)
	{
		if (szMusicFileFullPath[index1] == '\\')//�����˵�����һ��б�ܡ�
		{
			break;
		}
		if (szMusicFileFullPath[index1] == '.')//����������һ����
		{
			bIsFolder = false;
			break;
		}
	}
	if (!bIsFolder)//���·����һ���ļ���ȫ·����
	{
		TCHAR szMusicFilePathMsg[MAX_PATH] = { NULL };
		MusicMessage MM;
		ZeroMemory(&MM, sizeof(MM));
		//��ø�����ȫ·�����ơ�
		MM.SetMusicFileFullPath(szMusicFileFullPath);
		//��ø����Ķ�·������
		GetShortPathName(szMusicFileFullPath, szMusicFilePathMsg, sizeof(szMusicFilePathMsg));
		MM.SetMusicFileShortPath(szMusicFilePathMsg);
		memset(szMusicFilePathMsg, 0, sizeof(szMusicFilePathMsg));
//		ZeroMemory(&MM, sizeof(MM));
//		memset(szMusicFilePathMsg, 0, sizeof(szMusicFilePathMsg));
//		lstrcpyn(szMusicFilePathMsg, &(szMusicFileFullPath[index1]), lstrlen(szMusicFileFullPath) - index1 + 1);
//		lstrcpy(szMusicFilePathMsg, &(szMusicFileFullPath[index1]));//Ϊʲô����ֿ������󣬣�����һ�м�������
		for (int index = index1, i = 0; index < lstrlen(szMusicFileFullPath); index++, i++)
		{
			szMusicFilePathMsg[i] = szMusicFileFullPath[index];
		}
		//�ж��ļ��������Ƿ�Ϊ���֡��������ļ�������½������²�����
		if (lstrcmp(szMusicFilePathMsg, TEXT(".mp3")) == 0 || lstrcmp(szMusicFilePathMsg, TEXT(".wav")) == 0)
		{
			MM.SetMusicFileType(szMusicFilePathMsg);
			memset(szMusicFilePathMsg, 0, sizeof(szMusicFilePathMsg));
			//��ø����ļ����ļ�����
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
			//�����ֲ����б�����ʾ��Ϣ��
			wsprintf(szMusicFilePathMsg, TEXT("%d.  %s"), iCount, MM.GetMusicFileName());
			MusicMessage::AddMusicToList(-1, szMusicFilePathMsg);
			musicMessageList.push_back(MM);
			//ͬʱҲҪʵʱ��ʾ������Ŀ��������
			TCHAR szMusicCount[20] = { NULL };
			wsprintf(szMusicCount, TEXT("%d"), musicMessageList.size());
			SetDlgItemText(MusicMessage::GetMusicWindowHwnd(), IDC_MusicCount, szMusicCount);
			return true;
		}
		else
		{
			//��ӵ��ļ������������͵��ļ���
			TCHAR szErrorMsg[MAX_PATH] = { NULL };
			wsprintf(szErrorMsg, TEXT("%s �����������͵��ļ�������"), szMusicFilePathMsg);
			MessageBox(MusicMessage::GetMusicWindowHwnd(), szErrorMsg, TEXT("Message"), MB_OK);
			return false;
		}
	}
	else//������·����һ���ļ��е�·������ô�ݹ����AddLocalMusicFolder������������ļ���
	{
		//�Եõ����ļ�����·�����д���
		if (lstrlen(szMusicFileFullPath) != 0)
		{
			//D:\\...   D:\\123\\456  ��Ϊ�������������������\\�е�\���ᱻʡ�ԡ�
			if ((szMusicFileFullPath[2] == '\\') && (szMusicFileFullPath[3] != '\0'))
			{
				lstrcat(szMusicFileFullPath, TEXT("\\"));
			}
		}
		AddLocalMusicFolder(MusicMessage::GetMusicWindowHwnd(), true, szMusicFileFullPath);
	}
	return true;
}