/*
*ѧ�ţ�1252878
*��������
*��ϵ�绰��18817870106
*���ʵ�ַ��1252878xujian@tongji.edu.cn
*�ļ���������������ʾ����ͷ�ļ�
*/

#include"resource.h"
#include"MusicTabDlg.h"
#include<Windows.h>
#include<windowsx.h>
#include<CommCtrl.h>
#include"DeskTopSingleLrcDlgF.h"


BOOL CALLBACK DeskTop_SingleLrcDlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//��ʾ��ʵļ�ʱ����
VOID CALLBACK DesktopLrcSingle_TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);