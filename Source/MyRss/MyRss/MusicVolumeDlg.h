/*
*文件描述：音乐播放音量控制子窗口界面相关处理
*/

#include"resource.h"
#include<Windows.h>
#include<windowsx.h>
#include"MusicMessage.h"
#include"MusicTabDlg.h"

// 音量窗口的窗口过程函数，该音量窗口是非模态对话框，拥有自己的消息循环。
BOOL CALLBACK MusicVolumeDlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//音量滑动条改变的时候。
void VolumeSliderChange(HWND hwnd, HWND hwndCtrol, WPARAM wParam);
//当音量按钮被点击的时候，音量条的初始状态,HWND 音量条窗口的句柄。
bool InitVolumeSlider(HWND hwnd);