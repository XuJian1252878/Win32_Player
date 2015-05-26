/*
*ѧ�ţ�1252878
*��������
*��ϵ�绰��18817870106
*���ʵ�ַ��1252878xujian@tongji.edu.cn
*�ļ�������Rssվ����Ϣ����Ŀ��Ϣ������ز���
*/


#pragma once
#include"resource.h"
#include"RssTabDlg.h"
#include"FileEncode.h"
#include"InitMusicPlayer.h"
#include"MainDlg.h"
#include"MainDlgFunc.h"
#include"RssFunction.h"

class RssMessage
{
public:
	//�õ������������±꣬���ǲ��������еġ�
	static bool SetCurRssFeedIndex(int index);
	static int GetCurRssFeedIndex();
	static bool SetCurRssItemIndex(int index);
	static int GetCurRssItemIndex();
	//���ó�ʼ��RssMessage�������Ϣ��
	static bool InitRssMessage(HWND hwnd,int rssFeedListId,int rssItemListID);
	//����RssTab���ڵľ����
	static bool SetRssWindowHwnd(HWND hwnd);
	//���RssTab���ڵľ����
	static HWND GetRssWindowHwnd();
	//����RssItem�б�ľ����
	static bool SetRssItemListWnd(HWND hwnd);
	//ȡ��RssItem�б�ľ����
	static HWND GetRssItemListWnd();
	//��������RssFeed���ڵľ����
	static bool SetRssFeedSearchWnd(HWND hwnd);
	//�������RssFeed���ڵľ����
	static HWND GetRssFeedSearchWnd();
	//���ò���RssItem���ڵľ����
	static bool SetRssItemSearchWnd(HWND hwnd);
	//��ò���RssItem���ڵľ����
	static HWND GetRssItemSearchWnd();
	//��RssItem���б��в����ض���RssItem��
	//����ֵ�ǲ��ҵ����±꣬û�в��ҵ�����ô����-1��
	//�������Ҫ��������Ϣ��
	static int FindRssItemFromList(TCHAR* szSearchItemMsg);
private:
	static int iCurselRssfeed;
	static int iCurselRssItem;
	static HWND hRssWindowWnd;
	static HWND hRssFeedSearchWnd;
	static HWND hRssItemSearchWnd;
	static HWND hRssFeedListWnd;
	static HWND hRssItemListWnd;
};