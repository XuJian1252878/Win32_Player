/*
*ѧ�ţ�1252878
*��������
*��ϵ�绰��18817870106
*���ʵ�ַ��1252878xujian@tongji.edu.cn
*�ļ�������Rss�Ķ��������ʵ��
*/

#pragma once

#include"resource.h"
#include<Windows.h>
#include<windowsx.h>
#include<CommCtrl.h>//��Tab�ؼ�����ʾ�йء�
#include"InitRssReader.h"
#include"RssFunction.h"
#include"RssSearchMsgDlg.h"
#include"RssMessage.h"
#include"MainDlg.h"
#include"SearchRssItemDlg.h"


//������Tabѡ���RSS�����Ķ��Ľ���Ĵ��ڹ��̺�����
BOOL CALLBACK MainDlg_RSS_Proc(HWND hwnd, UINT uMeg, WPARAM wParam, LPARAM lParam);