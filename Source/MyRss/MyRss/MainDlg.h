/*
*�ļ������������ڹ���ʵ�֣�Rss��Musicplayer�����л���ʵ�֣�
*/


#pragma once

#include"resource.h"
#include<Windows.h>
#include<windowsx.h>
#include<CommCtrl.h>//��Tab�ؼ�����ʾ�йء�
#include"MainDlgFunc.h"
#include"InitMusicPlayer.h"
#include<locale.h>//��setlocale�йء�

//֪ͨ��������ʾ�����������ƶ��ˣ���ҲӦ�ø����ƶ��������淢�͸������������Ϣ��
//wparam ������ť����left����  lparam������ť����top���ꡣ
#define WM_MAINDLGMOVE_VolDlg WM_USER+2
#define WM_SHOWTRAY WM_USER+3//�����ϵͳ������ʾ���Լ����̵��Ҽ��˵���
//��WM_MAINDLGMOVE_VolDlg���ƣ��ṩ��������Ŀ���ڡ�
//wparam ��������right����  lparam��������top���ꡣ
#define WM_MAINDLGMOVE_SearchItemDlg WM_USER+4//��WM_MAINDLGMOVE_VolDlg���ƣ��ṩ��������Ŀ���ڡ�
#define WM_SEARCHITEMMSG WM_USER+5//�����������������������Ϣ��wParam���ַ�����ͷָ�롣lParam�Ǹ���ϢҪ���ظ��Ŀؼ���ID��
////wparam Rss��ť����right left����  lparamRss��ť����top���ꡣ
#define WM_MAINDLGMOVE_RssDlg WM_USER+6//��WM_MAINDLGMOVE_VolDlg���ƣ��ṩ��������Ŀ���ڡ�
//�ṩ���������ڵ����Ͻ����ꡣwParam lParam
#define WM_MAINDLGMOVE_HideTab WM_USER+7//��WM_MAINDLGMOVE_VolDlg����,�ṩ���������TAB�Ĵ����ƶ�

//������������ڹ��̺�����
BOOL CALLBACK MainDlg_Proc(HWND hwnd, UINT uMeg, WPARAM wParam, LPARAM lParam);
//����TAB���档
BOOL CALLBACK HideMainTab_Proc(HWND hwnd, UINT uMeg, WPARAM wParam, LPARAM lParam);





