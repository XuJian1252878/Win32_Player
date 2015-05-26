/*
*文件描述：音乐播放器相关内容写入本地磁盘的实现
*/


#include"stdafx.h"
#include"InitMusicPlayer.h"

//包含音乐列表中每一首音乐的信息。
extern vector<MusicMessage> musicMessageList;
//存放音乐播放列表的全局变量。
extern vector<Lyric> musicLyric;
extern bool bIsAppFirseUse;


//刚开始打开音乐播放器的时候，向音乐播放列表内添加上一次音乐播放器已经添加的项目。
//初始化musicMessageList的信息，这个HWND是音乐播放器的句柄。
//读取上一次音乐播放器关闭后保存的列表数据，该函数必须在MusicMessage::InitMusicMessage(hwnd, IDC_LISTMUSIC);后调用。
bool InitMusicList(HWND hwnd)
{
	int iMusicCount = 0;//记录音乐文件的总数。
	TCHAR szMusicPlayerPath[MAX_PATH] = { NULL };
	TCHAR szAppRunPath[MAX_PATH] = { NULL };
	GetAppRunPath(hwnd, szAppRunPath);
	lstrcpy(szMusicPlayerPath, szAppRunPath);
	//构造播放器信息存储的文件夹。
	lstrcat(szMusicPlayerPath, TEXT("MusicPlayer"));
	//首先判断该文件夹是否存在。
	char* cszMusicPlayerPath = NULL;//这个值要手动释放。
	cszMusicPlayerPath = FileEncode::UnicodeToAnsi((char*)szMusicPlayerPath);
	//音乐播放器设置总目录。
	//ERROR_PATH_NOT_FOUND
	//One or more intermediate directories do not exist; this function will only create the final directory in the path.
	if (!bIsAppFirseUse)//只有用户不是第一次使用该软件时，才进行如下操作。
	{
		if ((_access(cszMusicPlayerPath, 0) == -1))//如果该文件夹不存在。
		{
			CreateDirectory(szMusicPlayerPath, NULL);
			if (GetLastError() == ERROR_PATH_NOT_FOUND)
			{
				MessageBox(hwnd, TEXT("MusicPlayer子文件夹丢失，程序将回到初始状态！！"), TEXT("Message"), MB_OK);
				free(cszMusicPlayerPath);
				return false;
			}
		}
		//音乐播放器的资源目录。
		TCHAR szMusicPlayerSrcPath[MAX_PATH] = { NULL };
		lstrcat(szMusicPlayerSrcPath, szMusicPlayerPath);
		lstrcat(szMusicPlayerSrcPath, TEXT("\\src"));
		char* cszMusicPlayerSrcPath = FileEncode::UnicodeToAnsi((char*)szMusicPlayerSrcPath);
		if (_access(cszMusicPlayerSrcPath, 0) == -1)
		{
			CreateDirectory(szMusicPlayerSrcPath, NULL);
			if (GetLastError() == ERROR_PATH_NOT_FOUND)
			{
				MessageBox(hwnd, TEXT("MusicPlayer\\src子文件夹丢失，程序将回到初始状态！！"), TEXT("Message"), MB_OK);
				free(cszMusicPlayerSrcPath);
				free(cszMusicPlayerPath);
				return false;
			}
		}
		//存储音乐播放列表的文件。
		TCHAR szMusicListMessagePath[MAX_PATH] = { NULL };//保存读出来一行的内容。
		lstrcat(szMusicListMessagePath, szMusicPlayerSrcPath);
		lstrcat(szMusicListMessagePath, TEXT("\\MusicListMessage.mls"));
		FILE* fpMusicList = NULL;
		_wfopen_s(&fpMusicList, szMusicListMessagePath, TEXT("rb"));//用二进制的方式读写保险一点。不会出现乱码。
		if (fpMusicList == NULL)
		{
			MessageBox(hwnd, TEXT("MusicPlayer\\src\\MusicListMessage.mls文件丢失，程序将回到初始状态！！"), TEXT("Message"), MB_OK);
			free(cszMusicPlayerSrcPath);
			free(cszMusicPlayerPath);
			return false;
		}
		//文件被成功打开，开始读取数据。
		TCHAR szMusicListMessage[MAX_PATH] = { NULL };//存储音乐列表文件的名称，短路径信息。
		MusicMessage tempMusicMessage;//暂时存储音乐文件的类信息。
		while (!feof(fpMusicList))
		{
			//先对暂存的数据存储地清零。
			memset(szMusicListMessage, 0, sizeof(szMusicListMessage));
			ZeroMemory(&tempMusicMessage, sizeof(tempMusicMessage));
			//结构体的名称不代表结构体的首位置吗？？？？？？
			_fgetts(szMusicListMessage, sizeof(szMusicListMessage), fpMusicList);//如此读取出来的字符串中含有\r\n。因此不能对其进行直接处理。
			if (lstrlen(szMusicListMessage) == 0)
			{
				fclose(fpMusicList);//在return之前千万别忘记关闭文件，否则下一次程序访问文件的时候，文件可能会打不开。
				return false;
			}
			//tempMusicMessage.SetMusicFileFullPath(musicListMessage);
			TCHAR musicFullPath[MAX_PATH];
			lstrcpyn(musicFullPath, szMusicListMessage, (_tcschr(szMusicListMessage, '\r') - szMusicListMessage + 1));//确保字符串的最后一位是'\0'。
			lstrcat(musicFullPath, TEXT("\0"));
			tempMusicMessage.SetMusicFileFullPath(musicFullPath);

			memset(szMusicListMessage, 0, sizeof(szMusicListMessage));
			_fgetts(szMusicListMessage, sizeof(szMusicListMessage), fpMusicList);
			if (lstrlen(szMusicListMessage) == 0)
			{
				fclose(fpMusicList);//在return之前千万别忘记关闭文件，否则下一次程序访问文件的时候，文件可能会打不开。
				return false;
			}
			TCHAR musicFileName[MAX_PATH] = { NULL };
			lstrcpyn(musicFileName, szMusicListMessage, (_tcschr(szMusicListMessage, '\r') - szMusicListMessage + 1));//确保字符串的最后一位是'\0'。
			lstrcat(musicFileName, TEXT("\0"));
			tempMusicMessage.SetMusicFileName(musicFileName);
			//向音乐文件播放列表中添加音乐项。
			TCHAR szMusicListItem[MAX_PATH] = { NULL };
			iMusicCount++;
			wsprintf(szMusicListItem, L"%d.  %s", iMusicCount, musicFileName);
			MusicMessage::AddMusicToList(-1, szMusicListItem);

			memset(szMusicListMessage, 0, sizeof(szMusicListMessage));
			_fgetts(szMusicListMessage, sizeof(szMusicListMessage), fpMusicList);
			if (lstrlen(szMusicListMessage) == 0)
			{
				fclose(fpMusicList);//在return之前千万别忘记关闭文件，否则下一次程序访问文件的时候，文件可能会打不开。
				return false;
			}
			TCHAR musicFileShortPath[MAX_PATH] = { NULL };
			lstrcpyn(musicFileShortPath, szMusicListMessage, (_tcschr(szMusicListMessage, '\r') - szMusicListMessage + 1));//确保字符串的最后一位是'\0'。
			lstrcat(musicFileShortPath, TEXT("\0"));
			tempMusicMessage.SetMusicFileShortPath(musicFileShortPath);

			//写入音乐文件的类型信息。
			memset(szMusicListMessage, 0, sizeof(szMusicListMessage));
			_fgetts(szMusicListMessage, sizeof(szMusicListMessage), fpMusicList);
			if (lstrlen(szMusicListMessage) == 0)
			{
				fclose(fpMusicList);//在return之前千万别忘记关闭文件，否则下一次程序访问文件的时候，文件可能会打不开。
				return false;
			}
			TCHAR szMusicFileType[20] = { NULL };
			lstrcpyn(szMusicFileType, szMusicListMessage, (_tcschr(szMusicListMessage, '\r') - szMusicListMessage + 1));
			lstrcat(szMusicFileType, TEXT("\0"));
			tempMusicMessage.SetMusicFileType(szMusicFileType);

			//将临时的MusicClass放入MusicList这个vector中。
			musicMessageList.push_back(tempMusicMessage);

			//设置显示的曲目总数。
			TCHAR sMusicCount[MAX_PATH];
			wsprintf(sMusicCount, TEXT("%d"), iMusicCount);
			SetDlgItemText(hwnd, IDC_MusicCount, sMusicCount);
		}
		//读入完成之后，关闭文件。
		fclose(fpMusicList);
		free(cszMusicPlayerSrcPath);
		free(cszMusicPlayerPath);
	}
	return true;
}

//退出时保存音乐退出时的信息，这个HWND是根窗口的HWND。
bool SaveMusicPlayerMessage(HWND hwnd)
{
	TCHAR szMusicPlayerPath[MAX_PATH] = { NULL };
	TCHAR szAppRunPath[MAX_PATH] = { NULL };
	GetAppRunPath(hwnd, szAppRunPath);
	lstrcpy(szMusicPlayerPath, szAppRunPath);
	//构造播放器信息存储的文件夹。
	lstrcat(szMusicPlayerPath, TEXT("MusicPlayer"));
	//首先判断该文件夹是否存在。
	/*	When used with files, the _access function determines whether the specified file exists 
	and can be accessed as specified by the value of mode. When used with directories, _access 
	determines only whether the specified directory exists; in Windows NT, all directories have read and write access.
	mode Value            Checks File For 
	00                              Existence only 
	02                              Write permission 
	04                              Read permission 
	06                              Read and write permission*/
	char* cszMusicPlayerPath = NULL;//这个值要手动释放。
	cszMusicPlayerPath = FileEncode::UnicodeToAnsi((char*)szMusicPlayerPath);
	//音乐播放器设置总目录。
	//ERROR_PATH_NOT_FOUND
	//One or more intermediate directories do not exist; this function will only create the final directory in the path. 
	if ((_access(cszMusicPlayerPath, 0) == -1))//如果该文件夹不存在。
	{
		CreateDirectory(szMusicPlayerPath, NULL);
		if (GetLastError() == ERROR_PATH_NOT_FOUND)
		{
			MessageBox(hwnd, TEXT("MusicPlayer子文件夹无法创建，中间文件夹丢失！！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			free(cszMusicPlayerPath);
			return false;
		}
	}
	//音乐播放器的资源目录。
	TCHAR szMusicPlayerSrcPath[MAX_PATH] = { NULL };
	lstrcat(szMusicPlayerSrcPath, szMusicPlayerPath);
	lstrcat(szMusicPlayerSrcPath, TEXT("\\src"));
	char* cszMusicPlayerSrcPath = FileEncode::UnicodeToAnsi((char*)szMusicPlayerSrcPath);
	if (_access(cszMusicPlayerSrcPath, 0) == -1)
	{
		CreateDirectory(szMusicPlayerSrcPath, NULL);
		if (GetLastError() == ERROR_PATH_NOT_FOUND)
		{
			MessageBox(hwnd, TEXT("MusicPlayer\\src子文件夹无法创建，中间文件夹丢失！！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			free(cszMusicPlayerSrcPath);
			free(cszMusicPlayerPath);
			return false;
		}
	}
	//存储音乐播放列表的文件。
	TCHAR szMusicListMessagePath[MAX_PATH] = { NULL };
	lstrcat(szMusicListMessagePath, szMusicPlayerSrcPath);
	lstrcat(szMusicListMessagePath, TEXT("\\MusicListMessage.mls"));
	FILE* fpMusicList = NULL;//保存音乐列表信息的文件指针。
	_wfopen_s(&fpMusicList, szMusicListMessagePath, TEXT("wb"));
	if (fpMusicList == NULL)//音乐列表信息文件打开失败。
	{
		MessageBox(hwnd, TEXT("打开音乐列表信息文件失败！当前音乐文件列表信息无法保存！！"),
			TEXT("ERROR"), MB_OK | MB_ICONERROR);
		free(cszMusicPlayerSrcPath);
		free(cszMusicPlayerPath);
		return false;
	}
	//文件打开成功之后。
	if (!musicMessageList.empty())
	{
		for (vector<MusicMessage>::iterator iterator = musicMessageList.begin();
			iterator != musicMessageList.end(); iterator++)
		{
			_fputts(iterator->GetMusicFileFullPath(), fpMusicList);
			_ftprintf(fpMusicList, TEXT("\r\n"));
			_fputts(iterator->GetMusicFileName(), fpMusicList);
			_ftprintf(fpMusicList, TEXT("\r\n"));
			_fputts(iterator->GetMusicFileShortPath(), fpMusicList);
			_ftprintf(fpMusicList, TEXT("\r\n"));
			_fputts(iterator->GetMusicFileType(), fpMusicList);
			if (iterator != musicMessageList.end() - 1)
			{
				_ftprintf(fpMusicList, TEXT("\r\n"));
			}
			else
			{
				//由于\r是回到一行的开头，而\n是跳到下一行相同的位置。
				//不这样做的话会导致在读入歌曲列表的时候将\n读进来，从而产生一个空白的歌曲项。
				_ftprintf(fpMusicList, TEXT("\r"));
			}
		}
	}
	fclose(fpMusicList);//关闭文件，解除占用。
	free(cszMusicPlayerSrcPath);
	free(cszMusicPlayerPath);
	return true;
}