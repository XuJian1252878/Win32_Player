/*
*�ļ����������ֲ����Ӵ��ڽ�����ش���
*/

#pragma once
#include"resource.h"
#include<Windows.h>
#include<windowsx.h>
#include<CommCtrl.h>//��Tab�ؼ�����ʾ�йء�
#include"MusicFunction.h"
#include"MusicMessage.h"
#include"MainDlgFunc.h"
#include"DeskTopSingleLrcDlg.h"
#include"MusicMessage.h"
#include"InitMusicPlayer.h"
#include"MusicVolumeDlg.h"
#include"SearchEditDlg.h"
#include<shellapi.h>
#pragma comment(lib,"shell32.lib")

//��������ֲ������֮���͸�MusicTabDlg����Ϣ��
#define WM_MUSICEND WM_USER+1

#define TIMER_PLAYMUSIC 1//�������ֵĶ�ʱ����ID��
#define TIMER_DISPLAYMUSICLRC 2//��ʾ��ʶ�ʱ����ID��
#define TIMER_DISPLAYDESKTOPSINGLELRC 3//��ʾ����ĵ��и�ʼ�ʱ����

//������Tabѡ������ֲ���������Ĵ��ڹ��̺�����
BOOL CALLBACK MainDlg_MusicPlayer_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
