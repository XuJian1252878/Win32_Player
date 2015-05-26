/*
*ѧ�ţ�1252878
*��������
*��ϵ�绰��18817870106
*���ʵ�ַ��1252878xujian@tongji.edu.cn
*�ļ������������ļ������Ϣ�Լ����ֲ����߼���ʵ��
*/

#include"stdafx.h"
#include"MusicMessage.h"
#include"MusicTabDlg.h"

extern vector<MusicMessage> musicMessageList;
extern vector<Lyric> musicLyric;

int MusicMessage::iCurLyricIndex = 0;
int MusicMessage::musicPlayState = PLAYSTATEMENT_CLOSE;
int MusicMessage::curPlayingMusic = -1;
int MusicMessage::musicPlayMode = PLAYMODE_ORDER;
int MusicMessage::musicListControlID = MM_LIST_CONTROL_ID_NOT_FOUND;
int MusicMessage::curMusicPlayingVolume = 50;
bool MusicMessage::bHasLyric = false;
HWND MusicMessage::musicWindowHwnd = NULL;
bool MusicMessage::bDeskTopLrcState = false;//�����ʳ�ʼ��ʱδ��ʾ��
HWND MusicMessage::hDeskTopLrcWnd = NULL;
HWND MusicMessage::hMusicVolumeWnd = NULL;
HWND MusicMessage::hSearchMusicItemWnd = NULL;

//��ʼ��MusicMessage��
void MusicMessage::InitMusicMessage(HWND hwnd, int musicListControlID)
{
	SetMusicWindowHwnd(hwnd);
	SetMusicListControlID(musicListControlID);
}

//��������ļ��������Ϣ
HWND MusicMessage::GetMusicWindowHwnd()
{
	return MusicMessage::musicWindowHwnd;
}
int MusicMessage::GetMusicListControlID()
{
	return musicListControlID;
}
TCHAR* MusicMessage::GetMusicFileName()
{
	return musicFileName;
}
TCHAR* MusicMessage::GetMusicFileFullPath()
{
	return musicFileFullPath;
}
TCHAR* MusicMessage::GetMusicFileShortPath()
{
	return musicFileShortPath;
}
TCHAR* MusicMessage::GetMusicFileType()
{
	return musicFileType;
}

//���������ļ��������Ϣ��
void MusicMessage::SetMusicWindowHwnd(HWND hwnd)
{
	MusicMessage::musicWindowHwnd = hwnd;
}
void MusicMessage::SetMusicListControlID(int musicListControlID)
{
	MusicMessage::musicListControlID = musicListControlID;
}
void MusicMessage::SetMusicFileName(TCHAR* musicFileName)
{
	wsprintf(this->musicFileName, TEXT("%s"), musicFileName);
}
void MusicMessage::SetMusicFileFullPath(TCHAR* musicFileFullPath)
{
	wsprintf(this->musicFileFullPath, TEXT("%s"), musicFileFullPath);
}
void MusicMessage::SetMusicFileShortPath(TCHAR* musicFileShortPath)
{
	wsprintf(this->musicFileShortPath, TEXT("%s"), musicFileShortPath);
}
void MusicMessage::SetMusicFileType(TCHAR* musicFileType)
{
	wsprintf(this->musicFileType, TEXT("%s"), musicFileType);
}

//���õ�ǰ���ŵ������Ƿ��и���ļ���
bool MusicMessage::SetHasLyric(bool b)
{
	MusicMessage::bHasLyric = b;
	return true;
}

//���õ�ǰ���ŵ������Ƿ��и���ļ���
bool MusicMessage::GetHasLyric()
{
	return MusicMessage::bHasLyric;
}

//�������ֲ��ŵĺ�����
//�õ���ǰ���ڲ������ֵ��±ꡣ
int MusicMessage::GetCurPlayingMusic()
{
	return MusicMessage::curPlayingMusic;
}
//�����ֲ����б�ؼ��У���õ�ǰѡ�������ļ����±ꡣ
int MusicMessage::GetCurSelMusicFromMusicListControl()
{
	//hwnd�����ֲ����б����ڴ��ڵľ����
	//musicListControlID �������б�ؼ���ID�����������б�ؼ��Ĳ�ͬ����ȡ�����ļ��±�ķ�ʽҲ�Ͳ�ͬ��
	//��������б���ListBox�ؼ�����ô��ȡ�����±����¡�
	if (MusicMessage::musicListControlID == MM_LIST_CONTROL_ID_NOT_FOUND
		|| MusicMessage::musicWindowHwnd == NULL)
	{
		MessageBox(NULL, TEXT("int MusicMessage::GetCurSelMusicFromMusicListControl(HWND hwnd)����\r\n���ֲ����б�δ��ʼ��"), TEXT("ERROR"),
			MB_OK | MB_ICONERROR);
		return -1;
	}
	return SendMessage(GetDlgItem(MusicMessage::musicWindowHwnd, MusicMessage::musicListControlID),
		LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
}
//���õ�ǰ���ڲ��ŵ����֡�
void MusicMessage::SetCurPlayingMusic(int iCurPlayingMusic)
{
	MusicMessage::curPlayingMusic = iCurPlayingMusic;
}
//�������ֲ��ŵ�״̬��
void MusicMessage::SetMusicPlayState(int palyState = PLAYSTATEMENT_CLOSE)//�趨Ĭ�ϳ�ʼ������
{
	MusicMessage::musicPlayState = palyState;
}
//������ֵĲ���״̬��
int MusicMessage::GetMusicPlayState()
{
	return MusicMessage::musicPlayState;
}

//�������ļ��Ĳ�������PlayMusic��ʹ�á�
void MusicMessage::OpenMusic(int iCurSelMusic)
{
	//�����ǰ�������±곬��������������ô���ء�
	if (iCurSelMusic >= musicMessageList.size())
	{
		MessageBox(MusicMessage::musicWindowHwnd, TEXT("������Ŀ�±�Խ��"), TEXT("����"), MB_OK);
		return;
	}
	//һ��Ҫ���õ�ǰ���ֵĲ���״̬�Լ���ǰ���ڲ������ֵ��б��±ꡣ
	int iLastVol = MusicMessage::GetCurMusicVol();
	MusicMessage::SetCurPlayingMusic(iCurSelMusic);
	MusicMessage::SetMusicPlayState(PLAYSTATEMENT_PLAY);

	MusicMessage mm = musicMessageList[iCurSelMusic];
	TCHAR szOpenMusic[MAX_PATH + 10] = { NULL };
	wsprintf(szOpenMusic, TEXT("open %s alias music"), mm.GetMusicFileShortPath());
	mciSendString(szOpenMusic, NULL, 0, NULL);
	//����ǰ���ŵ�������ʾ�����ڲ������С�
	SetDlgItemText(MusicMessage::musicWindowHwnd, IDC_CurMusicEdit,
		musicMessageList[MusicMessage::GetCurPlayingMusic()].GetMusicFileName());
	//�������ֵ���������һ�׵�������ͬ��
	MusicMessage::SetCurMusicVol(iLastVol);
	//����ǰ���ڲ���������ѡ�С�
	SendMessage(GetDlgItem(MusicMessage::musicWindowHwnd, MusicMessage::musicListControlID), LB_SETCURSEL,
		(WPARAM)iCurSelMusic, (LPARAM)0);
}

//�������֣����ŵ�ʱ�����,ͬʱҲ�趨��ǰ���ڲ��ŵ������±ꡣ
void MusicMessage::PlayMusic(int iSelMusicIndex)
{
	//���Ȼ�õ�ǰ�б���ѡ�е������ļ����±ꡣ
	int iCurSelMusic = -1;
	if (iSelMusicIndex <= -1)//�ǳ�ʼѡ�����Զ�ѡ��
	{
		iCurSelMusic = MusicMessage::GetCurSelMusicFromMusicListControl();
	}
	else
	{
		iCurSelMusic = iSelMusicIndex;
	}

	if (iCurSelMusic == -1)
	{
		MessageBox(MusicMessage::musicWindowHwnd, TEXT("��ǰû���κ���Ŀ��ѡ�У�"), TEXT("��Ϣ"), MB_OK);
		return;
	}
	//�����ǰ���������ڲ��ţ����һ�ѡ������ͬ�����֣���ôִ����ͣ������
	if ((PLAYSTATEMENT_PLAY == MusicMessage::GetMusicPlayState()) && (iCurSelMusic == MusicMessage::GetCurPlayingMusic()))
	{
		PauseMusic();
	}
	else
	{
		//ѡ��������뵱ǰ���ڲ��ŵ����ֲ�ͬ�����ҵ�ǰ���ŵ����ֲ�Ϊ�ա���ô���ŵ�ǰѡ������֡�
		if ((MusicMessage::GetCurPlayingMusic() != iCurSelMusic) && (-1 != MusicMessage::GetCurPlayingMusic())
			&& (PLAYSTATEMENT_CLOSE != MusicMessage::GetMusicPlayState()))
		{
			//����ֹͣ��ǰ���ڲ��ŵ����֡�
			CloseMusic();
			//Ȼ���ѡ������֣����ֵĲ���״̬�Լ���ǰ���ڲ������ֵ��б��±��������Ѿ����á�
			OpenMusic(iCurSelMusic);
		}
		else
		{
			OpenMusic(iCurSelMusic);
		}
		//��ʼ�����µ�����ǰ��׼��������------
		TCHAR szMusicLen[100] = { NULL };
		int iMusicLen = MusicMessage::GetPlayingMusicLen() / 1000;//��OpenMusic���Ѿ����ù���ǰ���ŵ���Ŀ�ˡ�
		wsprintf(szMusicLen, TEXT("%02d:%02d"), (int)iMusicLen / 60, (int)iMusicLen % 60);
		SetDlgItemText(MusicMessage::musicWindowHwnd, IDC_MusicLen, szMusicLen);//�������ֵ��ܳ��ȡ�
		//�������ֽ������ķ�Χ�����õķ�Χ������Ϊ��λ��
		SendMessage( GetDlgItem( MusicMessage::musicWindowHwnd, IDC_MusicSlider ),
			TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, iMusicLen));
		//����ǰ��������λ������Ϊ0����ʼ̬��
		SendMessage(GetDlgItem(MusicMessage::musicWindowHwnd,IDC_MusicSlider),
			TBM_SETPOS,(WPARAM)TRUE,(LPARAM)0);
		//��ʼ���������ļ��ĸ�ʡ��������ø��ļ��Ƿ��и�ʵı�ǡ�
		bool bHasLyric = MusicMessage::GetPlayingMusicLrc();
		if (bHasLyric)
		{
			SetTimer(MusicMessage::musicWindowHwnd, TIMER_DISPLAYMUSICLRC, 100, RealTimeLrc_TimerProc);
		}
		MusicMessage::SetHasLyric(bHasLyric);
		//���ø�ʽ�����ʵʱ�±ꡣ
		MusicMessage::SetCurLyricIndex(0);
		//��ʼ�������֡�
		mciSendString(TEXT("play music"), NULL, 0, NULL);
		//��ʼ��ʱ����
		SetTimer(MusicMessage::musicWindowHwnd, TIMER_PLAYMUSIC, 100, MusicPlay_TimerProc);
	}
}

//��ͣ�������֡�
void MusicMessage::PauseMusic()
{
	mciSendString(TEXT("pause music"), NULL, 0, NULL);
	MusicMessage::SetMusicPlayState(PLAYSTATEMENT_PAUSE);
	//����ֹͣ���ֲ��ż�ʱ����
	KillTimer(MusicMessage::musicWindowHwnd, TIMER_PLAYMUSIC);
}

//ֹͣ�������֡��ر�ע��MusicMessage::SetCurPlayingMusic(-1);��
void MusicMessage::CloseMusic()
{
	mciSendString(TEXT("close music"), NULL, 0, NULL);
	//�ر�����֮�����β������
	MusicMessage::SetMusicPlayState(PLAYSTATEMENT_CLOSE);
	//ֹ֮ͣ��
	MusicMessage::SetCurPlayingMusic(-1);
	//��ʾ���ֲ��Ž��ȵ��ı�����㡣
	SetDlgItemText(MusicMessage::musicWindowHwnd, IDC_CurMusicPos, TEXT("00:00"));
	//���ֲ��Ż����λ��
	SendMessage(GetDlgItem(MusicMessage::musicWindowHwnd, IDC_MusicSlider), TBM_SETPOS, TRUE, 0);
	//ʹ�����б���δѡ��״̬��
	SendMessage(GetDlgItem(MusicMessage::musicWindowHwnd, IDC_LISTMUSIC), LB_SETCURSEL, (WPARAM)-1, (LPARAM)NULL);
	//���õ�ǰ����±�Ϊ0��
	MusicMessage::SetCurLyricIndex(0);
	//ֹͣ��ʱ����----
	KillTimer(MusicMessage::musicWindowHwnd, TIMER_PLAYMUSIC);
	if (MusicMessage::GetHasLyric())
	{
		KillTimer(MusicMessage::musicWindowHwnd, TIMER_DISPLAYMUSICLRC);
		//������ո���б�
		MusicMessage::ClearMusicLrcList();
		SendMessage(GetDlgItem(MusicMessage::musicWindowHwnd, IDC_MusicLrcPrevious), LB_INSERTSTRING, (WPARAM)-1, (LPARAM)TEXT(""));
		SendMessage(GetDlgItem(MusicMessage::musicWindowHwnd, IDC_CurMusicLrc), LB_INSERTSTRING, (WPARAM)-1, (LPARAM)TEXT(""));
		SendMessage(GetDlgItem(MusicMessage::musicWindowHwnd, IDC_MusicLrcNext), LB_INSERTSTRING, (WPARAM)-1, (LPARAM)TEXT(""));
	}
}

//��õ�ǰ���ڲ������ֵĳ���,��ǰ�ĵ�λ�Ǻ��롣
long MusicMessage::GetPlayingMusicLen()
{
	TCHAR szMusicLen[MAX_PATH] = { NULL };
	long lMusicLen = 0;
	mciSendString(TEXT("status music length"), szMusicLen, sizeof(szMusicLen), NULL);
	lMusicLen = _tcstol(szMusicLen, NULL, lMusicLen);//��õ������ļ����ȵ�λΪ���롣
	return lMusicLen;
}

//�������ִӵ�ǰλ�ÿ�ʼ����,��ǰ�ĵ�λ�Ǻ��롣
void MusicMessage::PlayMusicFrom(int pos)
{
	//�ж����ֲ��ŵ�λ���Ƿ�Ϸ���
	if (pos < 0)
	{
		pos = 0;
	}
	if (pos > MusicMessage::GetPlayingMusicLen())
	{
		pos = MusicMessage::GetPlayingMusicLen();
	}
	//������Ҫ�ж������Ƿ���ֹͣ״̬������ֹͣ״̬�����ܽ������²�����
	if (PLAYSTATEMENT_CLOSE != MusicMessage::GetMusicPlayState())
	{
		TCHAR szCmdPlayMusic[MAX_PATH] = { NULL };
		wsprintf(szCmdPlayMusic, TEXT("play music from %d"), pos);
		mciSendString(szCmdPlayMusic, NULL, 0, NULL);
	}
}

//��õ�ǰ���ֵĲ���λ��,��ǰ�ĵ�λ�Ǻ��롣
long MusicMessage::GetCurMusicPos()
{
	long lPos = 0;
	TCHAR sPos[MAX_PATH] = { NULL };
	mciSendString(TEXT("status music position"), sPos, sizeof(sPos), NULL);
	lPos = _tcstol(sPos, NULL, 10);
	return lPos;
}

//��õ�ǰ���ֲ��ŵ���������õĲ������Ѿ�/10�ġ�
int MusicMessage::GetCurMusicVol()
{
/*	TCHAR szMusicVol[MAX_PATH] = { NULL };
	long lMusicVol = 0;
	mciSendString(TEXT("status music volume"), szMusicVol, sizeof(szMusicVol), NULL);
	lMusicVol = _tcstol(szMusicVol, NULL, 10);
	return lMusicVol / 10;//������������Ϊ1000�ݡ������������õ��ǰٷ��ơ�*/
	return MusicMessage::curMusicPlayingVolume;
}

//���õ�ǰ���ֵĲ�������������Ĳ������Ѿ�/10�ġ�
void MusicMessage::SetCurMusicVol(int vol)
{
	if (vol < 0) vol = 0;
	if (vol >100) vol = 100;
	TCHAR szCmdMusicVol[MAX_PATH] = { NULL };
	wsprintf(szCmdMusicVol, TEXT("setaudio music volume to %d"), vol * 10);
	mciSendString(szCmdMusicVol, NULL, 0, NULL);
	MusicMessage::curMusicPlayingVolume = vol;
}

//����������ת��ĳһ����,��ǰ�ĵ�λ�Ǻ��롣
void MusicMessage::SeekMusicTo(int iMusicPos)
{
	TCHAR szCmdSeekMusicTo[MAX_PATH] = { NULL };
	wsprintf(szCmdSeekMusicTo, TEXT("seek music to %d"), iMusicPos);
	mciSendString(szCmdSeekMusicTo, NULL, 0, NULL);
}

//��õ�ǰ���ֵĲ���ģʽ��
int MusicMessage::GetMusicPlayMode()
{
	return MusicMessage::musicPlayMode;
}

//���õ�ǰ���ֵĲ���ģʽ��
bool MusicMessage::SetMusicPlayMode(int iMusicPlayMode)
{
	if ((iMusicPlayMode >= (int)SendMessage(GetDlgItem(MusicMessage::musicWindowHwnd, IDC_MusicPlayMode),CB_GETCOUNT,(WPARAM)0,(LPARAM)0) )
		|| (iMusicPlayMode < 0))
	{
		return false;
	}
	musicPlayMode = iMusicPlayMode;
	return true;
}

//������һ������һ��������������ź���������order��ƫ������
void MusicMessage::PlayNextOrLast(int order)
{
	int iMusicMode = MusicMessage::GetMusicPlayMode();
	//��õ�ǰ�б������ֵ�����Ŀ������Ҫע�⣬vectorһ�����list���������ݱ�����һ��һ���ġ�
	int iMusicItemCount = musicMessageList.size();
	int iTempSelMusic = MusicMessage::GetCurPlayingMusic();//����ʱ�洢��һ��Ҫ���Ÿ������±ꡣ
	switch (iMusicMode)
	{
		case PLAYMODE_ORDER:
		{
			if ((iTempSelMusic + order < iMusicItemCount) && (iTempSelMusic + order >= 0))
			{
				do
				{
					MusicMessage::CloseMusic();//��ֹͣ���ŵ�ǰ���֡�
					if ((iTempSelMusic + order > iMusicItemCount ) || (iTempSelMusic + order < 0))
					{
						break;
					}
					iTempSelMusic += order;//Ӧ�÷���if֮�󣬷���if�л�����ж��쳣��
					OpenMusic(iTempSelMusic);//�ٴ򿪱仯������֡�OpenMusic�л����õ�ǰ���������֡�
				} while ((MusicMessage::GetPlayingMusicLen() == 0) &&
					(PLAYSTATEMENT_CLOSE != MusicMessage::GetMusicPlayState()));//��Щ�����ļ�����û��ʱ�䣬Ӧ�ü�顣
				//��������������ѡ�����֡�
				MusicMessage::CloseMusic();
				//while���ж�������Ҫ����(CLOSE!=MusicClass::GetMusicState())  ��Ϊmusic��close״̬��ʱ������õĵĳ��ȶ����㡣
				if ((iTempSelMusic < iMusicItemCount) && (iTempSelMusic >= 0))
				{
					MusicMessage::PlayMusic(iTempSelMusic);
				}
			}
			else
			{
				//ȫ�����ֶ��𻵵������
				MusicMessage::CloseMusic();
			}
		}
			break;
		case PLAYMODE_REPEATONE:
		{
			int iFlag = order;//��¼ԭ��������orderֵ��
			order = 0;//����ѭ��ģʽ��
			do
			{
				MusicMessage::CloseMusic();
				if ((iTempSelMusic + order >= iMusicItemCount) || (iTempSelMusic + order<0))
				{
					break;
				}
				iTempSelMusic += iFlag;
				OpenMusic(iTempSelMusic);
				if (iFlag == -1) order--;
				if (iFlag == 1) order++;//�����ļ�����ʱ�Ĳ�����
			} while ((MusicMessage::GetPlayingMusicLen() == 0) &&
				(PLAYSTATEMENT_CLOSE != MusicMessage::GetMusicPlayState()));
			MusicMessage::CloseMusic();
			if ((iTempSelMusic < iMusicItemCount) && (iTempSelMusic >= 0))
			{
				MusicMessage::PlayMusic(iTempSelMusic);
			}
		}
			break;
		case PLAYMODE_REPEATALL:
		{
			if ((iTempSelMusic + order >= iMusicItemCount))
			{
				iTempSelMusic = 0;
			}
			else if ((iTempSelMusic + order <= 0))
			{
				iTempSelMusic = iMusicItemCount - 1;
			}
			else
			{
				do
				{
					MusicMessage::CloseMusic();
					if ((iTempSelMusic + order >= iMusicItemCount) || (iTempSelMusic + order<0))
					{
						break;
					}
					iTempSelMusic += order;
					OpenMusic(iTempSelMusic);
				} while ((MusicMessage::GetPlayingMusicLen() == 0) &&
					(PLAYSTATEMENT_CLOSE != MusicMessage::GetMusicPlayState()));
			}
			MusicMessage::CloseMusic();
			if ((iTempSelMusic < iMusicItemCount) && (iTempSelMusic >= 0))
			{
				MusicMessage::PlayMusic(iTempSelMusic);
			}
		}
			break;
		case PLAYMODE_RAND:
		{
			int iFlag = iTempSelMusic;//�������������Ƚϡ�
			do
			{
				srand((unsigned int)time(NULL));//��ʼ���������������
				//����Ҫ�ṩһ�����ӣ�������ӻ��Ӧһ������������ʹ����ͬ�����Ӻ����rand()���������һ�����������
				//��Time�����õ���ϵͳʱ����Ϊ����,�������Ա���ÿ�β������������ͬ��
				if (0 == iMusicItemCount)
				{
					MessageBox(MusicMessage::musicWindowHwnd, TEXT("��ǰ�����б�Ϊ�գ��������Ŀ��"), 
						TEXT("PlayNextOrLast RAND"), MB_OK);
					return;
				}
				//��������������±ꡣ
				int iNextMusic = -1;
				do//��ֹ�����뵱ǰ���ŵ������±���ͬ���������
				{
					iNextMusic = rand() % iMusicItemCount;//���� 0-iCount ���ڵ��������
					iTempSelMusic = iNextMusic;
				} while (iNextMusic == iFlag);
				//�����֣��������ܼ�����ֵĳ��ȡ�
				OpenMusic(iTempSelMusic);
			} while ((MusicMessage::GetPlayingMusicLen() == 0) &&
				(PLAYSTATEMENT_CLOSE != MusicMessage::GetMusicPlayState()));
			MusicMessage::CloseMusic();
			if ((iTempSelMusic < iMusicItemCount) && (iTempSelMusic >= 0))
			{
				MusicMessage::PlayMusic(iTempSelMusic);
			}
		}
			break;
		default:
			break;
	}
}

//�Զ�������һ�����֡�
void MusicMessage::AutoPlayNextMusic()
{
	//�ĳ���Windows����WM_MUSICEND��Ϣ���жϡ�
	switch (MusicMessage::GetMusicPlayMode())
	{
	case PLAYMODE_ORDER:
	case PLAYMODE_REPEATALL:
	{
		MusicMessage::PlayNextOrLast(1);
	}
		break;
	case PLAYMODE_REPEATONE:
	{
		MusicMessage::PlayNextOrLast();
	}
		break;
	case PLAYMODE_RAND:
	{
		MusicMessage::PlayNextOrLast();
	}
		break;
	default:
		break;
	}
}


/*�����ֲ������б��������Ŀ��hwnd �� �����б��ID��MusicMessage�ж��Ѿ�����
���Ըú�����Ҫ�������� insertIndex ������ļ����б��е���һ��
�����ļ����б��е����� szName�����Ҳ���ı�musicMessageList�е�ֵ*/
bool MusicMessage::AddMusicToList(int insertIndex,TCHAR* szName)
{
	int iResult = SendMessage(GetDlgItem(MusicMessage::GetMusicWindowHwnd(), MusicMessage::GetMusicListControlID()),
		LB_INSERTSTRING, (WPARAM)-1, (LPARAM)szName);
	if (iResult == LB_ERR || iResult == LB_ERRSPACE)
	{
		MessageBox(MusicMessage::GetMusicWindowHwnd(), TEXT("�������б�����������ļ�ʧ�ܣ���"), TEXT("����!"), MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}

/*�õ����ֲ����б��е�index������֡�*/
bool MusicMessage::GetMusicNameInList(int index,TCHAR* szMusicName)
{
	HWND hwnd = MusicMessage::GetMusicWindowHwnd();
	int ctrolID = MusicMessage::GetMusicListControlID();
	int iStrLen = SendMessage(GetDlgItem(MusicMessage::GetMusicWindowHwnd(), MusicMessage::GetMusicListControlID()),
		LB_GETTEXTLEN, (WPARAM)index, (LPARAM)0);
	SendMessage(GetDlgItem(MusicMessage::GetMusicWindowHwnd(), MusicMessage::GetMusicListControlID()),
		LB_GETTEXT, (WPARAM)index, (LPARAM)szMusicName);
	return true;
}


//��õ�ǰ���ڲ������ֵĸ�ʡ��������Ӧ��ʱ��Ҳ�Ǻ��뼶�ġ�
bool MusicMessage::GetPlayingMusicLrc()
{
	//�������ԭ���ĸ�ʡ�
	musicLyric.erase(musicLyric.begin(), musicLyric.end());
	int iOffset = 0;//��¼��ʵ��Ӻ���ƽ���Ϣ��
	TCHAR szCurMusicPath[MAX_PATH] = { NULL };
	TCHAR szCurMusicLrcPath[MAX_PATH] = { NULL };
	memset(szCurMusicPath, 0, MAX_PATH*sizeof(TCHAR));
	memset(szCurMusicLrcPath, 0, MAX_PATH*sizeof(TCHAR));
	wsprintf(szCurMusicPath, TEXT("%s"), musicMessageList[MusicMessage::GetCurPlayingMusic()].GetMusicFileFullPath());
	lstrcpyn(szCurMusicLrcPath, szCurMusicPath, _tcschr(szCurMusicPath, '.') - szCurMusicPath + 1);
	lstrcat(szCurMusicLrcPath, _T(".lrc"));

	//�������򿪸���ļ���
	FileEncode feFile(szCurMusicLrcPath);
	if (!(feFile.GetFILEState()))
	{
		MusicMessage::ClearMusicLrcList();//clear��ǰ������������С�
		SendMessage(GetDlgItem(MusicMessage::musicWindowHwnd,
			IDC_MusicLrcPrevious), LB_INSERTSTRING, (WPARAM)-1, (LPARAM)TEXT(""));
		SendMessage(GetDlgItem(MusicMessage::musicWindowHwnd,
			IDC_MusicLrcNext), LB_INSERTSTRING, (WPARAM)-1, (LPARAM)TEXT(""));
		SendMessage(GetDlgItem(MusicMessage::musicWindowHwnd, IDC_CurMusicLrc),
			LB_INSERTSTRING, (WPARAM)0, (LPARAM)TEXT("δ�ҵ�����ļ�"));
		return false;
	}
	//���۸ø���ļ���ʲô��ʽ���룬��ת��ΪUNICODE��ʽ��ȡ����ת��Ϊ���ַ��ķ�ʽ��
	TCHAR* szLrcFileContent = NULL;//���ֵ���Ҫ�ͷš�calloc�õ���
	if (feFile.IsUTF8())
	{
		szLrcFileContent = feFile.UTF8ToUnicode(feFile.ReadFileContent());
	}
	else
	{
		szLrcFileContent = feFile.AnsiToUnicode(feFile.ReadFileContent());
	}

	//�õ�����ļ���ȫ������֮��ʼ�������ļ���
	TCHAR* pLrcCurLine = szLrcFileContent, *pLrcNextLine = NULL, *pCtemp = szLrcFileContent, *pLrcContent = NULL, *pCtemp1 = NULL;
	TCHAR szNum[10] = { NULL };
	int iWcharCount = wcslen(szLrcFileContent);
	Lyric stCurLrcLine = { NULL };
	//�õ�һ�и�ʡ�
	while (pCtemp - szLrcFileContent<iWcharCount)
	{
		ZeroMemory(&stCurLrcLine, sizeof(stCurLrcLine));//���Ƚ���ʽṹ�����㣬װ����һ����ʡ�
		pCtemp = pLrcCurLine;
		while ((*pCtemp) != '\0')
		{
			if (((*pCtemp) == '\n') || ((*pCtemp) == '\r'))
			{
				break;
			}
			pCtemp++;
		}
		(*pCtemp) = '\0';
		pCtemp += 1;
		pLrcNextLine = pCtemp;
		//ȡ��һ��֮��ʼ������
		pLrcContent = pLrcCurLine;
		while ((pLrcContent - szLrcFileContent<iWcharCount - 1) && (!((*pLrcContent == ']') && (*(pLrcContent + 1) != '['))))
		{
			pLrcContent++;
		}
		pLrcContent++;//����ַ�������ʼ��ַ��

		//�������и�ʵ�ʱ�䡣
		pCtemp1 = pLrcCurLine;
		int iNumIndex = 0, iMinute = 0, iSecond = 0;
		while (*pCtemp1 != '\0')
		{
			if (*pCtemp1 == '[')
			{
				pCtemp1++;
				if ((*pCtemp1) >= '0' && (*pCtemp1) <= '9')
				{
					while ((*pCtemp1) != ':')
					{
						szNum[iNumIndex] = (*pCtemp1);
						iNumIndex++;
						pCtemp1++;
					}
				}
				if ((*pCtemp1) == ':')
				{
					iMinute = _ttoi(szNum);//��ø��ζ�Ӧ�ķ�����
					iNumIndex = 0;
					memset(szNum, 0, 10 * sizeof(TCHAR));
					//����ơ�
					pCtemp1++;
					while ((*pCtemp1) != ']')
					{
						szNum[iNumIndex] = (*pCtemp1);
						iNumIndex++;
						pCtemp1++;
					}
					if ((*pCtemp1) == ']')
					{
						iSecond = (int)(_ttof(szNum) * 1000);
						iNumIndex = 0;
						memset(szNum, 0, 10);
						//������д�뵽MusicLrc�С�
						stCurLrcLine.lTime = iMinute * 60000 + iSecond + iOffset;
						lstrcpy(stCurLrcLine.szMusicLrc, pLrcContent);
						musicLyric.push_back(stCurLrcLine);
					}
				}
				else//������[ti:��ʹ][ar:������][al:241910][offset:0]  ֮��ĸ����Ϣ��
				{
					TCHAR szLrcMeg[MAX_PATH] = { NULL };
					lstrcpyn(szLrcMeg, pCtemp1, _tcschr(pCtemp1, ':') - pCtemp1 + 1);
					if ((lstrcmp(_T("ti"), szLrcMeg) == 0) || (lstrcmp(_T("ar"), szLrcMeg) == 0) || (lstrcmp(_T("al"), szLrcMeg) == 0))
					{
/*						stCurLrcLine.lTime = 0;
						memset(szLrcMeg, 0, MAX_PATH * sizeof(TCHAR));
						lstrcpyn(szLrcMeg, pCtemp1 + 3, _tcschr(pCtemp1 + 3, ']') - pCtemp1 - 2);
						lstrcat(stCurLrcLine.szMusicLrc, szLrcMeg);
						lstrcat(stCurLrcLine.szMusicLrc, _T("	"));
						musicLyric.push_back(stCurLrcLine);*/
						memset(szLrcMeg, 0, MAX_PATH * sizeof(TCHAR));
					}
					else if ((lstrcmp(_T("offset"), szLrcMeg) == 0))
					{
						memset(szLrcMeg, 0, MAX_PATH * sizeof(TCHAR));
						lstrcpyn(szLrcMeg, pCtemp1 + 7, _tcschr(pCtemp1 + 7, ']') - pCtemp1 - 6);
						iOffset = _ttoi(szLrcMeg);//iOffect�����Ժ���Ϊ��λ�ġ�
					}
				}
			}
			pCtemp1++;//?[]��Ϊ������������������õ��ֽ���Ϣ��[���][���]�������
		}
		pLrcCurLine = pLrcNextLine;//׼��������һ�С�
	}
	free(szLrcFileContent);

	//��������
	for (int index1 = 0; index1 < musicLyric.size() - 1; index1 ++)
	{
		for (int index2 = index1 + 1; index2 < musicLyric.size(); index2 ++)
		{
			struct Lyric stTempLyric;
			if (musicLyric[index2].lTime < musicLyric[index1].lTime)
			{
				stTempLyric.lTime = musicLyric[index1].lTime;
				musicLyric[index1].lTime = musicLyric[index2].lTime;
				musicLyric[index2].lTime = stTempLyric.lTime;
				lstrcpy(stTempLyric.szMusicLrc, musicLyric[index2].szMusicLrc);
				memset(musicLyric[index2].szMusicLrc, 0, sizeof(musicLyric[index2].szMusicLrc));
				lstrcpy(musicLyric[index2].szMusicLrc, musicLyric[index1].szMusicLrc);
				memset(musicLyric[index1].szMusicLrc, 0, sizeof(musicLyric[index1].szMusicLrc));
				lstrcpy(musicLyric[index1].szMusicLrc, stTempLyric.szMusicLrc);
			}
		}
	}
	//�ڲ�������ʱ��ֵΪ���ֳ��ȵĸ��,��ֹ��ȡ��ʵ�ʱ�������
	stCurLrcLine.lTime = MusicMessage::GetPlayingMusicLen();
	lstrcpy(stCurLrcLine.szMusicLrc, TEXT(""));
	musicLyric.push_back(stCurLrcLine);
	musicLyric.push_back(stCurLrcLine);
	return true;
}

//���õ�ǰ���ֲ�������ʵ��±�����
bool MusicMessage::SetCurLyricIndex(int index)
{
	MusicMessage::iCurLyricIndex = index;
	return true;
}

//��õ�ǰ���ֲ�������ʵ��±�����
int MusicMessage::GetCurLyricIndex()
{
	return MusicMessage::iCurLyricIndex;
}

//���ò�������ǰ�Ƿ�������ʾ�����ʡ�
bool MusicMessage::SetDeskTopLrcState(bool b)
{
	MusicMessage::bDeskTopLrcState = b;
	return true;
}

//��ò�������ǰ�Ƿ�������ʾ�����ʡ�
bool MusicMessage::GetDeskTopLrcState()
{
	return MusicMessage::bDeskTopLrcState;
}

//���õ�ǰ���ֲ����������ʴ��ڵľ����
bool MusicMessage::SetDeskTopLrcWnd(HWND hwnd)
{
	MusicMessage::hDeskTopLrcWnd = hwnd;
	return true;
}

//��õ�ǰ���ֲ����������ʴ��ڵľ����
HWND MusicMessage::GetDeskTopLrcWnd()
{
	return MusicMessage::hDeskTopLrcWnd;
}

//���ñ����������ڵľ����
bool MusicMessage::SetMusicVolumeWnd(HWND hwnd)
{
	MusicMessage::hMusicVolumeWnd = hwnd;
	return true;
}
//����������ڵľ����
HWND MusicMessage::GetMusicVolumeWnd()
{
	return MusicMessage::hMusicVolumeWnd;
}

//����������������ʾ��ʴ��ڣ�HWND��ʴ��ڵľ����
bool MusicMessage::ClearMusicLrcList()
{
	HWND hLrcPreviousWnd = GetDlgItem(MusicMessage::musicWindowHwnd, IDC_MusicLrcPrevious);
	HWND hLrcNextWnd = GetDlgItem(MusicMessage::musicWindowHwnd, IDC_MusicLrcNext);
	HWND hLrcCurWnd = GetDlgItem(MusicMessage::musicWindowHwnd, IDC_CurMusicLrc);
	//��������·������ʾ�б�ĸ�ʡ�
	for (int index = SendMessage(hLrcPreviousWnd, LB_GETCOUNT, (WPARAM)0, (LPARAM)0);
		index >= 0; --index)
	{
		SendMessage(hLrcPreviousWnd, LB_DELETESTRING, (WPARAM)index, (LPARAM)0);
	}
	for (int index = SendMessage(hLrcNextWnd, LB_GETCOUNT, (WPARAM)0, (LPARAM)0);
		index >= 0; --index)
	{
		SendMessage(hLrcNextWnd, LB_DELETESTRING, (WPARAM)index, (LPARAM)0);
	}
	SendMessage(hLrcCurWnd, LB_DELETESTRING, (WPARAM)0, (LPARAM)0);
	return true;
}

//�������������б��ڵľ����
bool MusicMessage::SetSearchMusicItemWnd(HWND hwnd)
{
	MusicMessage::hSearchMusicItemWnd = hwnd;
	return true;
}

//������������б��ڵľ����
HWND MusicMessage::GetSearchMusicItemWnd()
{
	return MusicMessage::hSearchMusicItemWnd;
}

//�������ֲ����б����������������Ŀ������һ���ַ�������ѡ�з����ҵ��ĵ�һ�
int MusicMessage::FindMusicItemFromMusicList(TCHAR* szSearchItemMsg)
{
//	int iResult = SendMessage(GetDlgItem(MusicMessage::GetMusicWindowHwnd(), MusicMessage::GetMusicListControlID()),
//		LB_SELECTSTRING, (WPARAM)-1, (LPARAM)szSearchItemMsg);
	int iResult = -1;//������ʾ���ҵ���ѡ�
	char* cszSearchItemMsg = FileEncode::UnicodeToAnsi((char*)szSearchItemMsg);
	//��������musicMessageList�б�
	for (vector<MusicMessage>::iterator iterator = musicMessageList.begin();
		iterator != musicMessageList.end(); iterator++)
	{
		char* cszMusicFileName = FileEncode::UnicodeToAnsi((char*)(iterator->GetMusicFileName()));
		//��ʼȡ��ÿһ�������ļ������֡�
		if (strstr(cszMusicFileName,cszSearchItemMsg))
		{
			iResult = iterator - musicMessageList.begin();
			free(cszMusicFileName);
			break;
		}
		free(cszMusicFileName);
	}
	free(cszSearchItemMsg);
	if (iResult != -1)//������ҵ������ļ��������ҵ��������ļ��ö�����ѡ�С�
	{
		SendMessage(GetDlgItem(MusicMessage::GetMusicWindowHwnd(), MusicMessage::GetMusicListControlID()),
			LB_SETCURSEL, (WPARAM)iResult, (LPARAM)0);
		//�ö�
		SendMessage(GetDlgItem(MusicMessage::GetMusicWindowHwnd(), MusicMessage::GetMusicListControlID()),
			LB_SETTOPINDEX, (WPARAM)iResult, (LPARAM)0);
	}
	else//δ���ҵ�����ʾ��Ϣ��ʾ�򣬸�֪�û�δ��ѯ����Ӧ��Ϣ��
	{
		TCHAR szSearchMusicMsg[MAX_PATH] = { NULL };
		//%sǰ���ת���ַ��Ǳ����
		wsprintf(szSearchMusicMsg, TEXT("��Ǹ��δ�������б�������������\r\n\t\"%s\"\r\n\t����Ŀ����"), szSearchItemMsg);
		MessageBox(MusicMessage::GetMusicWindowHwnd(), szSearchMusicMsg, TEXT("Message"), MB_OK);
	}
	return iResult;
}