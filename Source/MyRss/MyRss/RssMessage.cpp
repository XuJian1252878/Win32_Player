/*
*ѧ�ţ�1252878
*��������
*��ϵ�绰��18817870106
*���ʵ�ַ��1252878xujian@tongji.edu.cn
*�ļ�������Rssվ����Ϣ����Ŀ��Ϣ������ز���
*/

#include"stdafx.h"
#include"RssMessage.h"

extern vector<string> RssFeedList;//���游��㡣
extern vector< map<string, RssContent> > RssItemList;//����Rss��ÿһ���ӽڵ�����ݡ�

int RssMessage::iCurselRssfeed = -1;
int RssMessage::iCurselRssItem = -1;
HWND RssMessage::hRssFeedSearchWnd = NULL;
HWND RssMessage::hRssItemSearchWnd = NULL;
HWND RssMessage::hRssWindowWnd = NULL;
HWND RssMessage::hRssFeedListWnd;
HWND RssMessage::hRssItemListWnd;

//�õ������������±꣬
bool RssMessage::SetCurRssFeedIndex(int index)
{
	RssMessage::iCurselRssfeed = index;
	return true;
}
//�õ������������±꣬
int RssMessage::GetCurRssFeedIndex()
{
	return RssMessage::iCurselRssfeed;
}
//�õ������������±꣬
bool RssMessage::SetCurRssItemIndex(int index)
{
	RssMessage::iCurselRssItem = index;
	return true;
}
//�õ������������±꣬
int RssMessage::GetCurRssItemIndex()
{
	return RssMessage::iCurselRssItem;
}

//��������RssFeed���ڵľ����
bool RssMessage::SetRssFeedSearchWnd(HWND hwnd)
{
	RssMessage::hRssFeedSearchWnd = hwnd;
	return true;
}
//�������RssFeed���ڵľ����
HWND RssMessage::GetRssFeedSearchWnd()
{
	return RssMessage::hRssFeedSearchWnd;
}
//���ò���RssItem���ڵľ����
bool RssMessage::SetRssItemSearchWnd(HWND hwnd)
{
	RssMessage::hRssItemSearchWnd = hwnd;
	return true;
}
//��ò���RssItem���ڵľ����
HWND RssMessage::GetRssItemSearchWnd()
{
	return RssMessage::hRssItemSearchWnd;
}


//���ó�ʼ��RssMessage�������Ϣ��
bool RssMessage::InitRssMessage(HWND hwnd, int rssFeedListId, int rssItemListID)
{
	RssMessage::hRssWindowWnd = hwnd;
	RssMessage::hRssFeedListWnd = GetDlgItem(hwnd, rssFeedListId);
	RssMessage::hRssItemListWnd = GetDlgItem(hwnd, rssItemListID);
	return true;
}

//����RssTab���ڵľ����
bool RssMessage::SetRssWindowHwnd(HWND hwnd)
{
	RssMessage::hRssWindowWnd = hwnd;
	return true;
}
//���RssTab���ڵľ����
HWND RssMessage::GetRssWindowHwnd()
{
	return RssMessage::hRssWindowWnd;
}

//����RssItem�б�ľ����
bool RssMessage::SetRssItemListWnd(HWND hwnd)
{
//	RssMessage::hRssItemSearchWnd = hwnd;
	RssMessage::hRssItemListWnd = hwnd;
	return true;
}
//ȡ��RssItem�б�ľ����
HWND RssMessage::GetRssItemListWnd()
{
	return RssMessage::hRssItemListWnd;
//	return RssMessage::hRssItemSearchWnd;
}

//��RssItem���б��в����ض���RssItem��
//����ֵ�ǲ��ҵ����±꣬û�в��ҵ�����ô����-1��
//�������Ҫ��������Ϣ��
int RssMessage::FindRssItemFromList(TCHAR* szSearchItemMsg)
{
	int iResult = -1;
	char* cszSearchItemMsg = FileEncode::UnicodeToAnsi((char*)szSearchItemMsg);
	//��RssItemList��map����Ѱ���ַ�����
	if (RssMessage::GetCurRssFeedIndex() == -1)
	{
		return -1;
	}
	map<string,RssContent> rssItemMap = RssItemList[RssMessage::GetCurRssFeedIndex()];
	//��rssItemMap ��������RssItem��Title��
	int index = 0;//��¼���ҵ�����һ���±ꡣ
	bool bFlag = false;
	for (map<string, RssContent>::iterator iterator = rssItemMap.begin();
		iterator != rssItemMap.end(); iterator++,++index)//ǰ�Ӻͺ�ӵ�����
	{
		char* cszRssItemTitle = FileEncode::UTF8ToAnsi((char*)(iterator->first.c_str()));
		//��ʼ�Ƚ�ÿһ��ȡ����RssItem��Title��
		if (strstr(cszRssItemTitle, cszSearchItemMsg))
		{
			bFlag = true;
			free(cszRssItemTitle);
			break;
		}
		free(cszRssItemTitle);
	}
	if (bFlag)//�ҵ��ض���Title����ôָ���±ꡣ
	{
		iResult = index;
	}

	free(cszSearchItemMsg);
	SendMessage(GetDlgItem(RssMessage::GetRssWindowHwnd(), GetDlgCtrlID(RssMessage::GetRssItemListWnd())),
		LB_SETCURSEL, (WPARAM)iResult, (LPARAM)0);
	return iResult;
}