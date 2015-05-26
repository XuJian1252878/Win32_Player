/*
*学号：1252878
*姓名：许舰
*联系电话：18817870106
*电邮地址：1252878xujian@tongji.edu.cn
*文件描述：音乐文件相关信息以及音乐播放逻辑的实现
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
bool MusicMessage::bDeskTopLrcState = false;//桌面歌词初始化时未显示。
HWND MusicMessage::hDeskTopLrcWnd = NULL;
HWND MusicMessage::hMusicVolumeWnd = NULL;
HWND MusicMessage::hSearchMusicItemWnd = NULL;

//初始化MusicMessage。
void MusicMessage::InitMusicMessage(HWND hwnd, int musicListControlID)
{
	SetMusicWindowHwnd(hwnd);
	SetMusicListControlID(musicListControlID);
}

//获得音乐文件的相关信息
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

//设置音乐文件的相关信息。
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

//设置当前播放的音乐是否有歌词文件。
bool MusicMessage::SetHasLyric(bool b)
{
	MusicMessage::bHasLyric = b;
	return true;
}

//设置当前播放的音乐是否有歌词文件。
bool MusicMessage::GetHasLyric()
{
	return MusicMessage::bHasLyric;
}

//控制音乐播放的函数。
//得到当前正在播放音乐的下标。
int MusicMessage::GetCurPlayingMusic()
{
	return MusicMessage::curPlayingMusic;
}
//从音乐播放列表控件中，获得当前选中音乐文件的下标。
int MusicMessage::GetCurSelMusicFromMusicListControl()
{
	//hwnd是音乐播放列表所在窗口的句柄。
	//musicListControlID 是音乐列表控件的ID，由于音乐列表控件的不同，获取音乐文件下标的方式也就不同。
	//如果播放列表是ListBox控件，那么获取音乐下标如下。
	if (MusicMessage::musicListControlID == MM_LIST_CONTROL_ID_NOT_FOUND
		|| MusicMessage::musicWindowHwnd == NULL)
	{
		MessageBox(NULL, TEXT("int MusicMessage::GetCurSelMusicFromMusicListControl(HWND hwnd)错误！\r\n音乐播放列表未初始化"), TEXT("ERROR"),
			MB_OK | MB_ICONERROR);
		return -1;
	}
	return SendMessage(GetDlgItem(MusicMessage::musicWindowHwnd, MusicMessage::musicListControlID),
		LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
}
//设置当前正在播放的音乐。
void MusicMessage::SetCurPlayingMusic(int iCurPlayingMusic)
{
	MusicMessage::curPlayingMusic = iCurPlayingMusic;
}
//设置音乐播放的状态。
void MusicMessage::SetMusicPlayState(int palyState = PLAYSTATEMENT_CLOSE)//设定默认初始参数。
{
	MusicMessage::musicPlayState = palyState;
}
//获得音乐的播放状态。
int MusicMessage::GetMusicPlayState()
{
	return MusicMessage::musicPlayState;
}

//打开音乐文件的操作，在PlayMusic中使用。
void MusicMessage::OpenMusic(int iCurSelMusic)
{
	//如果当前的音乐下标超过音乐总数，那么返回。
	if (iCurSelMusic >= musicMessageList.size())
	{
		MessageBox(MusicMessage::musicWindowHwnd, TEXT("音乐曲目下标越界"), TEXT("错误"), MB_OK);
		return;
	}
	//一定要设置当前音乐的播放状态以及当前正在播放音乐的列表下标。
	int iLastVol = MusicMessage::GetCurMusicVol();
	MusicMessage::SetCurPlayingMusic(iCurSelMusic);
	MusicMessage::SetMusicPlayState(PLAYSTATEMENT_PLAY);

	MusicMessage mm = musicMessageList[iCurSelMusic];
	TCHAR szOpenMusic[MAX_PATH + 10] = { NULL };
	wsprintf(szOpenMusic, TEXT("open %s alias music"), mm.GetMusicFileShortPath());
	mciSendString(szOpenMusic, NULL, 0, NULL);
	//将当前播放的音乐显示在正在播放栏中。
	SetDlgItemText(MusicMessage::musicWindowHwnd, IDC_CurMusicEdit,
		musicMessageList[MusicMessage::GetCurPlayingMusic()].GetMusicFileName());
	//设置音乐的音量与上一首的音量相同。
	MusicMessage::SetCurMusicVol(iLastVol);
	//将当前正在操作的音乐选中。
	SendMessage(GetDlgItem(MusicMessage::musicWindowHwnd, MusicMessage::musicListControlID), LB_SETCURSEL,
		(WPARAM)iCurSelMusic, (LPARAM)0);
}

//播放音乐，播放的时候调用,同时也设定当前正在播放的音乐下标。
void MusicMessage::PlayMusic(int iSelMusicIndex)
{
	//首先获得当前列表中选中的音乐文件的下标。
	int iCurSelMusic = -1;
	if (iSelMusicIndex <= -1)//是初始选择还是自动选择。
	{
		iCurSelMusic = MusicMessage::GetCurSelMusicFromMusicListControl();
	}
	else
	{
		iCurSelMusic = iSelMusicIndex;
	}

	if (iCurSelMusic == -1)
	{
		MessageBox(MusicMessage::musicWindowHwnd, TEXT("当前没有任何曲目被选中！"), TEXT("消息"), MB_OK);
		return;
	}
	//如果当前有音乐正在播放，并且还选中了相同的音乐，那么执行暂停操作。
	if ((PLAYSTATEMENT_PLAY == MusicMessage::GetMusicPlayState()) && (iCurSelMusic == MusicMessage::GetCurPlayingMusic()))
	{
		PauseMusic();
	}
	else
	{
		//选择的音乐与当前正在播放的音乐不同，并且当前播放的音乐不为空。那么播放当前选择的音乐。
		if ((MusicMessage::GetCurPlayingMusic() != iCurSelMusic) && (-1 != MusicMessage::GetCurPlayingMusic())
			&& (PLAYSTATEMENT_CLOSE != MusicMessage::GetMusicPlayState()))
		{
			//首先停止当前正在播放的音乐。
			CloseMusic();
			//然后打开选择的音乐，音乐的播放状态以及当前正在播放音乐的列表下标在其中已经设置。
			OpenMusic(iCurSelMusic);
		}
		else
		{
			OpenMusic(iCurSelMusic);
		}
		//开始播放新的音乐前的准备工作。------
		TCHAR szMusicLen[100] = { NULL };
		int iMusicLen = MusicMessage::GetPlayingMusicLen() / 1000;//在OpenMusic中已经设置过当前播放的曲目了。
		wsprintf(szMusicLen, TEXT("%02d:%02d"), (int)iMusicLen / 60, (int)iMusicLen % 60);
		SetDlgItemText(MusicMessage::musicWindowHwnd, IDC_MusicLen, szMusicLen);//设置音乐的总长度。
		//设置音乐进度条的范围。设置的范围是以秒为单位。
		SendMessage( GetDlgItem( MusicMessage::musicWindowHwnd, IDC_MusicSlider ),
			TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, iMusicLen));
		//将当前进度条的位置设置为0，初始态。
		SendMessage(GetDlgItem(MusicMessage::musicWindowHwnd,IDC_MusicSlider),
			TBM_SETPOS,(WPARAM)TRUE,(LPARAM)0);
		//开始解析音乐文件的歌词。并且设置该文件是否有歌词的标记。
		bool bHasLyric = MusicMessage::GetPlayingMusicLrc();
		if (bHasLyric)
		{
			SetTimer(MusicMessage::musicWindowHwnd, TIMER_DISPLAYMUSICLRC, 100, RealTimeLrc_TimerProc);
		}
		MusicMessage::SetHasLyric(bHasLyric);
		//设置歌词解析的实时下标。
		MusicMessage::SetCurLyricIndex(0);
		//开始播放音乐。
		mciSendString(TEXT("play music"), NULL, 0, NULL);
		//开始计时器。
		SetTimer(MusicMessage::musicWindowHwnd, TIMER_PLAYMUSIC, 100, MusicPlay_TimerProc);
	}
}

//暂停播放音乐。
void MusicMessage::PauseMusic()
{
	mciSendString(TEXT("pause music"), NULL, 0, NULL);
	MusicMessage::SetMusicPlayState(PLAYSTATEMENT_PAUSE);
	//并且停止音乐播放计时器。
	KillTimer(MusicMessage::musicWindowHwnd, TIMER_PLAYMUSIC);
}

//停止播放音乐。特别注意MusicMessage::SetCurPlayingMusic(-1);。
void MusicMessage::CloseMusic()
{
	mciSendString(TEXT("close music"), NULL, 0, NULL);
	//关闭音乐之后的收尾工作。
	MusicMessage::SetMusicPlayState(PLAYSTATEMENT_CLOSE);
	//停止之后
	MusicMessage::SetCurPlayingMusic(-1);
	//显示音乐播放进度的文本框归零。
	SetDlgItemText(MusicMessage::musicWindowHwnd, IDC_CurMusicPos, TEXT("00:00"));
	//音乐播放滑块归位。
	SendMessage(GetDlgItem(MusicMessage::musicWindowHwnd, IDC_MusicSlider), TBM_SETPOS, TRUE, 0);
	//使播放列表处于未选中状态。
	SendMessage(GetDlgItem(MusicMessage::musicWindowHwnd, IDC_LISTMUSIC), LB_SETCURSEL, (WPARAM)-1, (LPARAM)NULL);
	//设置当前歌词下标为0。
	MusicMessage::SetCurLyricIndex(0);
	//停止计时器。----
	KillTimer(MusicMessage::musicWindowHwnd, TIMER_PLAYMUSIC);
	if (MusicMessage::GetHasLyric())
	{
		KillTimer(MusicMessage::musicWindowHwnd, TIMER_DISPLAYMUSICLRC);
		//并且清空歌词列表。
		MusicMessage::ClearMusicLrcList();
		SendMessage(GetDlgItem(MusicMessage::musicWindowHwnd, IDC_MusicLrcPrevious), LB_INSERTSTRING, (WPARAM)-1, (LPARAM)TEXT(""));
		SendMessage(GetDlgItem(MusicMessage::musicWindowHwnd, IDC_CurMusicLrc), LB_INSERTSTRING, (WPARAM)-1, (LPARAM)TEXT(""));
		SendMessage(GetDlgItem(MusicMessage::musicWindowHwnd, IDC_MusicLrcNext), LB_INSERTSTRING, (WPARAM)-1, (LPARAM)TEXT(""));
	}
}

//获得当前正在播放音乐的长度,当前的单位是毫秒。
long MusicMessage::GetPlayingMusicLen()
{
	TCHAR szMusicLen[MAX_PATH] = { NULL };
	long lMusicLen = 0;
	mciSendString(TEXT("status music length"), szMusicLen, sizeof(szMusicLen), NULL);
	lMusicLen = _tcstol(szMusicLen, NULL, lMusicLen);//获得的音乐文件长度单位为毫秒。
	return lMusicLen;
}

//设置音乐从当前位置开始播放,当前的单位是毫秒。
void MusicMessage::PlayMusicFrom(int pos)
{
	//判断音乐播放的位置是否合法。
	if (pos < 0)
	{
		pos = 0;
	}
	if (pos > MusicMessage::GetPlayingMusicLen())
	{
		pos = MusicMessage::GetPlayingMusicLen();
	}
	//首先需要判断音乐是否是停止状态，若是停止状态，不能进行以下操作。
	if (PLAYSTATEMENT_CLOSE != MusicMessage::GetMusicPlayState())
	{
		TCHAR szCmdPlayMusic[MAX_PATH] = { NULL };
		wsprintf(szCmdPlayMusic, TEXT("play music from %d"), pos);
		mciSendString(szCmdPlayMusic, NULL, 0, NULL);
	}
}

//获得当前音乐的播放位置,当前的单位是毫秒。
long MusicMessage::GetCurMusicPos()
{
	long lPos = 0;
	TCHAR sPos[MAX_PATH] = { NULL };
	mciSendString(TEXT("status music position"), sPos, sizeof(sPos), NULL);
	lPos = _tcstol(sPos, NULL, 10);
	return lPos;
}

//获得当前音乐播放的音量，获得的参数是已经/10的。
int MusicMessage::GetCurMusicVol()
{
/*	TCHAR szMusicVol[MAX_PATH] = { NULL };
	long lMusicVol = 0;
	mciSendString(TEXT("status music volume"), szMusicVol, sizeof(szMusicVol), NULL);
	lMusicVol = _tcstol(szMusicVol, NULL, 10);
	return lMusicVol / 10;//音乐音量共分为1000份。而播放器中用的是百份制。*/
	return MusicMessage::curMusicPlayingVolume;
}

//设置当前音乐的播放音量，传入的参数是已经/10的。
void MusicMessage::SetCurMusicVol(int vol)
{
	if (vol < 0) vol = 0;
	if (vol >100) vol = 100;
	TCHAR szCmdMusicVol[MAX_PATH] = { NULL };
	wsprintf(szCmdMusicVol, TEXT("setaudio music volume to %d"), vol * 10);
	mciSendString(szCmdMusicVol, NULL, 0, NULL);
	MusicMessage::curMusicPlayingVolume = vol;
}

//设置音乐跳转到某一进度,当前的单位是毫秒。
void MusicMessage::SeekMusicTo(int iMusicPos)
{
	TCHAR szCmdSeekMusicTo[MAX_PATH] = { NULL };
	wsprintf(szCmdSeekMusicTo, TEXT("seek music to %d"), iMusicPos);
	mciSendString(szCmdSeekMusicTo, NULL, 0, NULL);
}

//获得当前音乐的播放模式。
int MusicMessage::GetMusicPlayMode()
{
	return MusicMessage::musicPlayMode;
}

//设置当前音乐的播放模式。
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

//播放上一曲或下一曲，或者随机播放函数。参数order是偏移量。
void MusicMessage::PlayNextOrLast(int order)
{
	int iMusicMode = MusicMessage::GetMusicPlayMode();
	//获得当前列表中音乐的总数目，这里要注意，vector一界面的list的音乐数据必须是一摸一样的。
	int iMusicItemCount = musicMessageList.size();
	int iTempSelMusic = MusicMessage::GetCurPlayingMusic();//先暂时存储下一首要播放歌曲的下标。
	switch (iMusicMode)
	{
		case PLAYMODE_ORDER:
		{
			if ((iTempSelMusic + order < iMusicItemCount) && (iTempSelMusic + order >= 0))
			{
				do
				{
					MusicMessage::CloseMusic();//先停止播放当前音乐。
					if ((iTempSelMusic + order > iMusicItemCount ) || (iTempSelMusic + order < 0))
					{
						break;
					}
					iTempSelMusic += order;//应该放在if之后，否则if中会出现判断异常。
					OpenMusic(iTempSelMusic);//再打开变化后的音乐。OpenMusic中会设置当前代开的音乐。
				} while ((MusicMessage::GetPlayingMusicLen() == 0) &&
					(PLAYSTATEMENT_CLOSE != MusicMessage::GetMusicPlayState()));//有些音乐文件破损，没有时间，应该检查。
				//符合条件，播放选中音乐。
				MusicMessage::CloseMusic();
				//while的判断条件需要加上(CLOSE!=MusicClass::GetMusicState())  因为music在close状态的时候所获得的的长度都是零。
				if ((iTempSelMusic < iMusicItemCount) && (iTempSelMusic >= 0))
				{
					MusicMessage::PlayMusic(iTempSelMusic);
				}
			}
			else
			{
				//全部音乐都损坏的情况。
				MusicMessage::CloseMusic();
			}
		}
			break;
		case PLAYMODE_REPEATONE:
		{
			int iFlag = order;//记录原来传进的order值。
			order = 0;//单曲循环模式。
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
				if (iFlag == 1) order++;//音乐文件破损时的操作。
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
			int iFlag = iTempSelMusic;//留下来便于作比较。
			do
			{
				srand((unsigned int)time(NULL));//初始化随机数发生器。
				//它需要提供一个种子，这个种子会对应一个随机数，如果使用相同的种子后面的rand()函数会出现一样的随机数。
				//用Time函数得到的系统时间作为种子,这样可以避免每次产生的随机数相同。
				if (0 == iMusicItemCount)
				{
					MessageBox(MusicMessage::musicWindowHwnd, TEXT("当前播放列表为空！请添加曲目！"), 
						TEXT("PlayNextOrLast RAND"), MB_OK);
					return;
				}
				//随机产生的音乐下标。
				int iNextMusic = -1;
				do//防止出现与当前播放的音乐下标相同的随机数。
				{
					iNextMusic = rand() % iMusicItemCount;//产生 0-iCount 以内的随机数。
					iTempSelMusic = iNextMusic;
				} while (iNextMusic == iFlag);
				//打开音乐，这样才能检查音乐的长度。
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

//自动播放下一首音乐。
void MusicMessage::AutoPlayNextMusic()
{
	//改成向Windows发送WM_MUSICEND消息来判断。
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


/*向音乐播放器列表中添加曲目。hwnd 和 播放列表的ID在MusicMessage中都已经给出
所以该函数仅要两个参数 insertIndex 插入的文件在列表中的哪一项
插入文件在列表中的名字 szName，并且不会改变musicMessageList中的值*/
bool MusicMessage::AddMusicToList(int insertIndex,TCHAR* szName)
{
	int iResult = SendMessage(GetDlgItem(MusicMessage::GetMusicWindowHwnd(), MusicMessage::GetMusicListControlID()),
		LB_INSERTSTRING, (WPARAM)-1, (LPARAM)szName);
	if (iResult == LB_ERR || iResult == LB_ERRSPACE)
	{
		MessageBox(MusicMessage::GetMusicWindowHwnd(), TEXT("向音乐列表中添加音乐文件失败！！"), TEXT("错误!"), MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}

/*得到音乐播放列表中第index项的名字。*/
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


//获得当前正在播放音乐的歌词。歌词所对应的时间也是毫秒级的。
bool MusicMessage::GetPlayingMusicLrc()
{
	//首先清空原来的歌词。
	musicLyric.erase(musicLyric.begin(), musicLyric.end());
	int iOffset = 0;//记录歌词的延后或推进信息。
	TCHAR szCurMusicPath[MAX_PATH] = { NULL };
	TCHAR szCurMusicLrcPath[MAX_PATH] = { NULL };
	memset(szCurMusicPath, 0, MAX_PATH*sizeof(TCHAR));
	memset(szCurMusicLrcPath, 0, MAX_PATH*sizeof(TCHAR));
	wsprintf(szCurMusicPath, TEXT("%s"), musicMessageList[MusicMessage::GetCurPlayingMusic()].GetMusicFileFullPath());
	lstrcpyn(szCurMusicLrcPath, szCurMusicPath, _tcschr(szCurMusicPath, '.') - szCurMusicPath + 1);
	lstrcat(szCurMusicLrcPath, _T(".lrc"));

	//接下来打开歌词文件。
	FileEncode feFile(szCurMusicLrcPath);
	if (!(feFile.GetFILEState()))
	{
		MusicMessage::ClearMusicLrcList();//clear当前项里面会插入空行。
		SendMessage(GetDlgItem(MusicMessage::musicWindowHwnd,
			IDC_MusicLrcPrevious), LB_INSERTSTRING, (WPARAM)-1, (LPARAM)TEXT(""));
		SendMessage(GetDlgItem(MusicMessage::musicWindowHwnd,
			IDC_MusicLrcNext), LB_INSERTSTRING, (WPARAM)-1, (LPARAM)TEXT(""));
		SendMessage(GetDlgItem(MusicMessage::musicWindowHwnd, IDC_CurMusicLrc),
			LB_INSERTSTRING, (WPARAM)0, (LPARAM)TEXT("未找到歌词文件"));
		return false;
	}
	//无论该歌词文件以什么方式编码，都转化为UNICODE方式读取，都转化为宽字符的方式。
	TCHAR* szLrcFileContent = NULL;//这个值最后要释放。calloc得到。
	if (feFile.IsUTF8())
	{
		szLrcFileContent = feFile.UTF8ToUnicode(feFile.ReadFileContent());
	}
	else
	{
		szLrcFileContent = feFile.AnsiToUnicode(feFile.ReadFileContent());
	}

	//得到歌词文件的全部内容之后开始解读歌词文件。
	TCHAR* pLrcCurLine = szLrcFileContent, *pLrcNextLine = NULL, *pCtemp = szLrcFileContent, *pLrcContent = NULL, *pCtemp1 = NULL;
	TCHAR szNum[10] = { NULL };
	int iWcharCount = wcslen(szLrcFileContent);
	Lyric stCurLrcLine = { NULL };
	//得到一行歌词。
	while (pCtemp - szLrcFileContent<iWcharCount)
	{
		ZeroMemory(&stCurLrcLine, sizeof(stCurLrcLine));//首先将歌词结构体清零，装载下一条歌词。
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
		//取得一行之后开始解析。
		pLrcContent = pLrcCurLine;
		while ((pLrcContent - szLrcFileContent<iWcharCount - 1) && (!((*pLrcContent == ']') && (*(pLrcContent + 1) != '['))))
		{
			pLrcContent++;
		}
		pLrcContent++;//获得字符串的起始地址；

		//解析改行歌词的时间。
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
					iMinute = _ttoi(szNum);//获得各次对应的分数。
					iNumIndex = 0;
					memset(szNum, 0, 10 * sizeof(TCHAR));
					//向后移。
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
						//将数据写入到MusicLrc中。
						stCurLrcLine.lTime = iMinute * 60000 + iSecond + iOffset;
						lstrcpy(stCurLrcLine.szMusicLrc, pLrcContent);
						musicLyric.push_back(stCurLrcLine);
					}
				}
				else//读入如[ti:天使][ar:五月天][al:241910][offset:0]  之类的歌词信息。
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
						iOffset = _ttoi(szLrcMeg);//iOffect就是以毫秒为单位的。
					}
				}
			}
			pCtemp1++;//?[]因为有这种情况。跳过无用的字节信息。[歌词][歌词]的情况。
		}
		pLrcCurLine = pLrcNextLine;//准备读入下一行。
	}
	free(szLrcFileContent);

	//对其排序。
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
	//在插入两条时间值为音乐长度的歌词,防止读取歌词的时候溢出。
	stCurLrcLine.lTime = MusicMessage::GetPlayingMusicLen();
	lstrcpy(stCurLrcLine.szMusicLrc, TEXT(""));
	musicLyric.push_back(stCurLrcLine);
	musicLyric.push_back(stCurLrcLine);
	return true;
}

//设置当前音乐播放至歌词的下标数。
bool MusicMessage::SetCurLyricIndex(int index)
{
	MusicMessage::iCurLyricIndex = index;
	return true;
}

//获得当前音乐播放至歌词的下标数。
int MusicMessage::GetCurLyricIndex()
{
	return MusicMessage::iCurLyricIndex;
}

//设置播放器当前是否正在显示桌面歌词。
bool MusicMessage::SetDeskTopLrcState(bool b)
{
	MusicMessage::bDeskTopLrcState = b;
	return true;
}

//获得播放器当前是否正在显示桌面歌词。
bool MusicMessage::GetDeskTopLrcState()
{
	return MusicMessage::bDeskTopLrcState;
}

//设置当前音乐播放器桌面歌词窗口的句柄。
bool MusicMessage::SetDeskTopLrcWnd(HWND hwnd)
{
	MusicMessage::hDeskTopLrcWnd = hwnd;
	return true;
}

//获得当前音乐播放器桌面歌词窗口的句柄。
HWND MusicMessage::GetDeskTopLrcWnd()
{
	return MusicMessage::hDeskTopLrcWnd;
}

//设置保存音量窗口的句柄。
bool MusicMessage::SetMusicVolumeWnd(HWND hwnd)
{
	MusicMessage::hMusicVolumeWnd = hwnd;
	return true;
}
//获得音量窗口的句柄。
HWND MusicMessage::GetMusicVolumeWnd()
{
	return MusicMessage::hMusicVolumeWnd;
}

//清空主界面的上下显示歌词窗口，HWND歌词窗口的句柄。
bool MusicMessage::ClearMusicLrcList()
{
	HWND hLrcPreviousWnd = GetDlgItem(MusicMessage::musicWindowHwnd, IDC_MusicLrcPrevious);
	HWND hLrcNextWnd = GetDlgItem(MusicMessage::musicWindowHwnd, IDC_MusicLrcNext);
	HWND hLrcCurWnd = GetDlgItem(MusicMessage::musicWindowHwnd, IDC_CurMusicLrc);
	//先清空上下方歌词显示列表的歌词。
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

//设置收缩音乐列表窗口的句柄。
bool MusicMessage::SetSearchMusicItemWnd(HWND hwnd)
{
	MusicMessage::hSearchMusicItemWnd = hwnd;
	return true;
}

//获得收缩音乐列表窗口的句柄。
HWND MusicMessage::GetSearchMusicItemWnd()
{
	return MusicMessage::hSearchMusicItemWnd;
}

//搜索音乐播放列表里面符合条件的条目，输入一个字符串。并选中返回找到的第一项。
int MusicMessage::FindMusicItemFromMusicList(TCHAR* szSearchItemMsg)
{
//	int iResult = SendMessage(GetDlgItem(MusicMessage::GetMusicWindowHwnd(), MusicMessage::GetMusicListControlID()),
//		LB_SELECTSTRING, (WPARAM)-1, (LPARAM)szSearchItemMsg);
	int iResult = -1;//用来表示查找到的选项。
	char* cszSearchItemMsg = FileEncode::UnicodeToAnsi((char*)szSearchItemMsg);
	//遍历整个musicMessageList列表。
	for (vector<MusicMessage>::iterator iterator = musicMessageList.begin();
		iterator != musicMessageList.end(); iterator++)
	{
		char* cszMusicFileName = FileEncode::UnicodeToAnsi((char*)(iterator->GetMusicFileName()));
		//开始取出每一项音乐文件的名字。
		if (strstr(cszMusicFileName,cszSearchItemMsg))
		{
			iResult = iterator - musicMessageList.begin();
			free(cszMusicFileName);
			break;
		}
		free(cszMusicFileName);
	}
	free(cszSearchItemMsg);
	if (iResult != -1)//如果查找到音乐文件，将查找到的音乐文件置顶，并选中。
	{
		SendMessage(GetDlgItem(MusicMessage::GetMusicWindowHwnd(), MusicMessage::GetMusicListControlID()),
			LB_SETCURSEL, (WPARAM)iResult, (LPARAM)0);
		//置顶
		SendMessage(GetDlgItem(MusicMessage::GetMusicWindowHwnd(), MusicMessage::GetMusicListControlID()),
			LB_SETTOPINDEX, (WPARAM)iResult, (LPARAM)0);
	}
	else//未查找到，显示消息提示框，告知用户未查询到相应信息。
	{
		TCHAR szSearchMusicMsg[MAX_PATH] = { NULL };
		//%s前面的转义字符是必须的
		wsprintf(szSearchMusicMsg, TEXT("抱歉！未在音乐列表中搜索到关于\r\n\t\"%s\"\r\n\t的曲目！！"), szSearchItemMsg);
		MessageBox(MusicMessage::GetMusicWindowHwnd(), szSearchMusicMsg, TEXT("Message"), MB_OK);
	}
	return iResult;
}