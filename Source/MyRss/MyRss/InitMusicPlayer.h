/*
*文件描述：音乐播放器相关内容写入本地磁盘的实现
*/

#include"resource.h"
#include<Windows.h>
#include<windowsx.h>
#include<CommCtrl.h>
#include"MusicFunction.h"
#include"MusicMessage.h"
#include"MusicTabDlg.h"
#include<stdlib.h>
#include<io.h>
#include"FileEncode.h"


//刚开始打开音乐播放器的时候，向音乐播放列表内添加上一次音乐播放器已经添加的项目。
//初始化musicMessageList的信息，这个HWND是音乐播放器的句柄。
//读取上一次音乐播放器关闭后保存的列表数据，该函数必须在MusicMessage::InitMusicMessage(hwnd, IDC_LISTMUSIC);后调用。
bool InitMusicList(HWND hwnd);
//退出时保存音乐退出时的信息，HWND程序的实例句柄。
bool SaveMusicPlayerMessage(HWND hwnd);