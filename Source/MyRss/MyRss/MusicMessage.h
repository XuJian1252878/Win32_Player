/*
*学号：1252878
*姓名：许舰
*联系电话：18817870106
*电邮地址：1252878xujian@tongji.edu.cn
*文件描述：音乐文件相关信息以及音乐播放逻辑的实现
*/

#ifndef MUSICMESSAGE_H//预防重复编译。
#define MUSICMESSAGE_H

#include<vector>
#include<mmsystem.h>
#include <stdlib.h> /*srand函数式随机数发生器的初始化函数。*/
#include <time.h> /*用到了time函数，所以要有这个头文件，time函数用来获得系统时间。*/
#include <tchar.h>
#include<windows.h>
#include<windowsx.h>
#include"resource.h"
#include"FileEncode.h"
#include<CommCtrl.h>
using namespace std;//vector在命名空间里。


//音乐的播放状态。
#define PLAYSTATEMENT_PLAY 1001
#define PLAYSTATEMENT_PAUSE 1002
#define PLAYSTATEMENT_CLOSE 1003

#define PLAYMODE_ORDER 0  //顺序播放
#define PLAYMODE_RAND 1        //随机播放
#define PLAYMODE_REPEATONE 2 //单曲循环
#define PLAYMODE_REPEATALL 3  //列表循环

#define MAX_MUSIC_TYPE 10//音乐文件后缀数组最大字符数。
#define MAX_MUSIC_PATH 256//音乐文件名最大字符数。
#define MAX_MUSIC_LYRIC 200//音乐一句歌词的最大字符数。

#define MM_LIST_CONTROL_ID_NOT_FOUND 0x123//音乐播放器控件并未设置。

struct Lyric
{
	long lTime;  //毫秒
	TCHAR szMusicLrc[MAX_PATH];  //歌词
};//用于存放时间及对应的歌词的结构体

class MusicMessage
{
public:
	//音乐文件的个体信息。
	TCHAR* GetMusicFileName();
	TCHAR* GetMusicFileFullPath();
	TCHAR* GetMusicFileShortPath();
	TCHAR* GetMusicFileType();
	void SetMusicFileName(TCHAR* musicFileName);
	void SetMusicFileFullPath(TCHAR* musicFileFullPath);
	void SetMusicFileShortPath(TCHAR* musicFileShortPath);
	void SetMusicFileType(TCHAR* musicFileType);

	//音乐的控制函数。
	//初始化MusicMessage，设置父窗口句柄，以及控件ID，实际上就是调用了SetMusicWindowHwnd，SetMusicListControlID。
	static void InitMusicMessage(HWND hwnd, int musicListControlID);
	//设置当前音乐播放列表控件所在父窗口的句柄。
	static void SetMusicWindowHwnd(HWND hwnd);
	//获得当前音乐播放列表控件所在父窗口的句柄。
	static HWND GetMusicWindowHwnd();
	//设置当前音乐播放列表控件的ID。
	static void SetMusicListControlID(int SetMusicListControlID);//主对话框初始化的时候一定不能忘记的步骤。
	//获得当前音乐播放列表控件的ID。
	static int GetMusicListControlID();
	//获得当前音乐的播放状态。
	static int GetMusicPlayState();
	//设置当前音乐的播放状态。
	static void SetMusicPlayState(int palyState);
	//获得当前正在播放音乐的下标。
	static int GetCurPlayingMusic();
	//从音乐播放列表控件中，获得当前选中音乐文件的下标。
	static int GetCurSelMusicFromMusicListControl();
	//设置当前正在播放音乐的下标。
	static void SetCurPlayingMusic(int iCurPlayingMusic);
	//播放音乐。
	static void PlayMusic(int iSelMusicIndex = -1);
	//暂停音乐。
	static void PauseMusic();
	//关闭音乐。
	static void CloseMusic();
	//获取当前播放音乐的长度,当前的单位是毫秒。
	static long GetPlayingMusicLen();
	//设置音乐从当前位置播放,当前的单位是毫秒。
	static void PlayMusicFrom(int pos);
	//获取音乐当前播放的位置,当前的单位是毫秒。
	static long GetCurMusicPos();
	//获得当前音乐的播放音量，获得的音量是已经/10的。
	static int GetCurMusicVol();
	//设置当前音乐播放的音量，获得的参数是已经/10的。
	static void SetCurMusicVol(int vol);
	//播放上一曲或下一曲，或者随机播放函数。
	static void PlayNextOrLast(int order = 0);
	//获得当前音乐的播放模式。
	static int GetMusicPlayMode();
	//设置当前音乐的播放模式。
	static bool SetMusicPlayMode(int iMusicPlayMode = PLAYMODE_ORDER);
	//在四中模式下自动播放下一首音乐。
	static void AutoPlayNextMusic();
	//设置音乐跳转至设定位置,当前的单位是毫秒。
	static void SeekMusicTo(int iMusicPos);
	/*向音乐播放器列表中添加曲目。hwnd 和 播放列表的ID在MusicMessage中都已经给出
	所以该函数仅要两个参数 insertIndex 插入的文件在列表中的哪一项,-1表示从队末插入文件
	插入文件在列表中的名字 szName*/
	static bool AddMusicToList(int insertIndex,TCHAR* szName);
	/*得到音乐播放列表中第index项的名字。以及存放名字的数组szMusicName*/
	static bool GetMusicNameInList(int index,TCHAR* szMusicName);
	//设置当前音乐播放至歌词的下标数。
	static bool SetCurLyricIndex(int index);
	//获得当前音乐播放至歌词的下标数。
	static int GetCurLyricIndex();
	//载入当前正在播放音乐的歌词。
	static bool GetPlayingMusicLrc();
	//设置当前播放的音乐是否有歌词文件。
	static bool SetHasLyric(bool b);
	//设置当前播放的音乐是否有歌词文件。
	static bool GetHasLyric();
	//设置播放器当前是否正在显示桌面歌词。
	static bool SetDeskTopLrcState(bool b);
	//获得播放器当前是否正在显示桌面歌词。
	static bool GetDeskTopLrcState();
	//设置当前音乐播放器桌面歌词窗口的句柄。
	static bool SetDeskTopLrcWnd(HWND hwnd);
	//获得当前音乐播放器桌面歌词窗口的句柄。
	static HWND GetDeskTopLrcWnd();
	//清空主界面的上下显示歌词窗口，HWND歌词窗口的句柄。
	static bool ClearMusicLrcList();
	//设置保存音量窗口的句柄。
	static bool SetMusicVolumeWnd(HWND hwnd);
	//获得音量窗口的句柄。
	static HWND GetMusicVolumeWnd();
	//设置收缩音乐列表窗口的句柄。
	static bool SetSearchMusicItemWnd(HWND hwnd);
	//获得收缩音乐列表窗口的句柄。
	static HWND GetSearchMusicItemWnd();
	//搜索音乐播放列表里面符合条件的条目，输入一个字符串。并选中
	//If the search is successful, the return value is the index of the selected item. If
	//the search is unsuccessful, the return value is LB_ERR and the current selection is not changed. 
	static int FindMusicItemFromMusicList(TCHAR* szSearchItemMsg);



private:
	TCHAR musicFileName[MAX_MUSIC_PATH];//音乐文件名，不包括后缀名，路径。
	TCHAR musicFileFullPath[MAX_MUSIC_PATH];//音乐文件的全名，包括后缀，路径。
	TCHAR musicFileShortPath[MAX_MUSIC_PATH];//音乐文件的短路进名，用此路径名播放音乐。
	TCHAR musicFileType[MAX_MUSIC_TYPE];//保存音乐的类型。直接就是类型名称。
	static int musicListControlID;//音乐列表控件的资源ID。方便获得下标的操作。
	static int musicPlayState;
	static int curPlayingMusic;//当前正在播放音乐的下标数值。musicMessageList在此列表中。
	static int musicPlayMode;
	static int curMusicPlayingVolume;//当前正在播放的音乐的音量，百份制的，原来的是千份制的。
	//记录当前歌词滚到第几条的变量。
	static int iCurLyricIndex;
	static bool bHasLyric;//当前播放歌曲是否有歌词文件。
	static bool bDeskTopLrcState;//播放器当前是否正在显示桌面歌词。
	static HWND musicWindowHwnd;
	static HWND hDeskTopLrcWnd;//显示桌面歌词窗口的句柄。
	static HWND hMusicVolumeWnd;//音量窗口的句柄。
	static HWND hSearchMusicItemWnd;//搜索音乐条目的窗口句柄。
	//在PlayMusic中使用，进行打开音乐的操作，以便后续操作的进行。
	static void OpenMusic(int iCurSelMusic);
};


#endif