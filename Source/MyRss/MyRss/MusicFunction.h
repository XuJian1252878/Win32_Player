/*
*学号：1252878
*姓名：许舰
*联系电话：18817870106
*电邮地址：1252878xujian@tongji.edu.cn
*文件描述：音乐播放逻辑的相关实现
*/

#pragma once

#include"resource.h"
#include"FileEncode.h"
#include"MusicMessage.h"
#include"MusicTabDlg.h"
#include"resource.h"
#include<Commdlg.h>//OPENFILENAME 结构体所在的位置。
#include<shlobj.h>//BROWSEINFO结构体的定义。

//显示程序发生错误时的错误码信息。
void ShowError();
//判断添加的音乐文件是否重复。
bool IsRepeat(TCHAR* musicFileName);//说明在编译的时候，是将.h文件里面的内容拉到.cpp中然后再编译，即先引入内容，再编译。
//获得分割为//的路径。
TCHAR* GetPrePath(TCHAR *sMusicName);
//从电脑中添加音乐文件的函数。
void AddMusicFile(HWND hwnd);
//播放音乐的计时器。
VOID CALLBACK MusicPlay_TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
//显示实时音乐歌词的计时器。
VOID CALLBACK RealTimeLrc_TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);


//初始化音乐播放模式ComboBox。传入参数音乐播放窗口句柄HWND。
bool InitPlayModeMessage(HWND hwnd);
//当用户拖动歌曲播放进度条的时候响应的函数。
void PlayingSliderChange(HWND hwnd, HWND hwndCtrol, WPARAM wParam);
//对自绘控件的Button控件载入背景。
/*hwnd 父窗口的句柄 wParam lParam WM_DRAWITEM消息的参数*/
void DrawBkgndToButton(HWND hwnd, WPARAM odBtnID, LPDRAWITEMSTRUCT lpDrawItemStruct);
/*载入操作音乐文件的菜单。
传入参数，窗口句柄HWND 由此获得应用程序实例句柄*/
bool LoadOperateFileMenu(HWND hwnd);


/*对播放模式的ComboBox进行重绘
两个参数：hwnd 父窗口句柄
(LPDRAWITEMSTRUCT)lParam 重绘相关的信息
*/
bool RedrawPlayModeComboBox(HWND hwnd, LPDRAWITEMSTRUCT lpDrawItemStruct);
/*对音乐播放列表进行重绘
两个参数：hwnd 父窗口句柄
(LPDRAWITEMSTRUCT)lParam 重绘相关的信息*/
bool RedrawMusicList(HWND hwnd, LPDRAWITEMSTRUCT lpDrawItemStruct);

/*对当前音乐歌词行进行重绘,歌词显示列表
两个参数：hwnd 父窗口句柄
(LPDRAWITEMSTRUCT)lParam 重绘相关的信息*/
bool RedrawCurMusicLrc(HWND hwnd, LPDRAWITEMSTRUCT lpDrawItemStruct);
/*对已唱过音乐歌词行进行重绘,歌词显示列表
两个参数：hwnd 父窗口句柄
(LPDRAWITEMSTRUCT)lParam 重绘相关的信息*/
bool RedrawPreviousMusicLrc(HWND hwnd, LPDRAWITEMSTRUCT lpDrawItemStruct);
/*对未唱过音乐歌词行进行重绘,歌词显示列表
两个参数：hwnd 父窗口句柄
(LPDRAWITEMSTRUCT)lParam 重绘相关的信息*/
bool RedrawNextMusicLrc(HWND hwnd, LPDRAWITEMSTRUCT lpDrawItemStruct);

//创建当前音乐播放器主界面中当前显示歌词的字体。
HFONT CreateMusicLrcFont(HWND hwnd, BYTE lfCharSet, BYTE lfClipPrecision, TCHAR* lfFaceName, LONG lfHeight, BYTE lfQuality);

//从音乐播放列表中删除一个曲目，但不删除本地文件
//HWND 父窗口句柄,iCurSelIndex 列表中当前选中的下标。
//musicMessageList中对应的项目也会删除。
bool DeleteMusicItemFromList(int iCurSelIndex);

//从音乐播放列表中删除一个曲目，也删除本地文件
//HWND 父窗口句柄,iCurSelIndex 列表中当前选中的下标。
//musicMessageList中对应的项目也会删除。
bool DeleteMusicItemFromDisk(int iCurSelIndex);


//清空音乐播放列表中的所有条目，HWND 父窗口句柄。
//musicMessageList中对应的项目也会删除。
bool ClearAllMusicItemFromList();

//浏览文件夹目录的函数，HWND 父窗口句柄，TCHAR* szSelFolderName选择文件夹的名字。
bool BrowseForFolder(HWND hwnd, TCHAR* szSelFolderPath);
//添加本地音乐文件夹中的音乐文件,HWND 父窗口句柄。
//bIsDragFolder 文件夹是否是被拖拽进来的，若是被拖拽进来的，那么szDragFolderName就是被拖拽进来文件夹的名称。
bool AddLocalMusicFolder(HWND hwnd, bool bIsDragFolder,TCHAR* szDragFolderName = NULL);

//从一个包含文件路径的vector容器中添加音乐文件。
bool AddMusicMessageFromVector(vector<wstring>& szMusicFileFullPathVector);

//获得读入音乐文件的路径信息。szMusicFileFullPath已经是含有\\的全路径
//该函数中路径信息将被加入musicMessageList列表。
bool AddMusicMessageFromPath(TCHAR* szMusicFileFullPath);


//初始化音乐界面的控件tip提示消息。
bool InitMusicTabDlgCtrolTip(HWND hwnd);

