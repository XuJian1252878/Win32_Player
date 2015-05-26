/*
*ѧ�ţ�1252878
*��������
*��ϵ�绰��18817870106
*���ʵ�ַ��1252878xujian@tongji.edu.cn
*�ļ�������Rss�Ķ���Rss��Ŀ��ӡ�ɾ��������߼���ʵ��
*/

#pragma once
#include"resource.h"
#include"MainDlg.h"
#include"MainDlgFunc.h"
#include"RssTabDlg.h"
#include<iostream>
#include<map>
#include<vector>
#include<string>
#include<fstream>
#include <algorithm>
#include"InitRssReader.h"
#include<Urlmon.h>//URLDownloadToFile
#include "tinyxml.h"
#include "tinystr.h"
#include <windows.h>
#include <atlstr.h>
#include"RssMessage.h"
using namespace std;

#define MAX_RSSMSGLEN 1024//����rss��Ϣ�ַ�����󳤶ȡ�
//�ṹ�嶨��
struct RssContent
{
	string Web;
	string Date;
	string Description;
};

//��ý���Ŀؼ��ṩtip��ʾ��Ϣ��
bool InitRssTabDlgCtrolTip(HWND hwnd);

//��ȡRss�ļ�����Ϣ��
//������� �����ݴ��ļ��ľ���·����
bool ReadXmlFile(TCHAR* lpszTempRssXmlPath, string& rssFeedTitle, map<string, RssContent>& mXML);

//��������ץȡRss��XML���ݡ�
bool GetRssMsgFromLink(HWND hwnd, TCHAR* szRssFeedLink);
//��RssTreeView��ImageList��,hwnd RssTreeView�����ڵľ����
//bool InitRssTreeViewImageLists(HWND hwnd);
/*//�����б��в��븸��㣬�ø����������һ��Rss���ӣ�LPTSTR lpszItem�Ǹ�����ͷ�����֡�
HTREEITEM AddRssFatherNode(HWND hwnd, LPTSTR lpszItem);
//��ָ�������������ӽڵ�������С����س��븸���ľ����
bool AddRssChildNode(HWND hwnd, HTREEITEM hFatherNodeTreeItem, map<string, RssContent>& RssItemMsgMap);
//�����м���һ��RSS Feed��ô�������ӽڵ�һ����롣
//LPTSTR lpszItem ����㣨��Rss Feed������֣���RssContent>& RssItemMsgMap �ø�����µ����ݡ�
//�ڲ�������AddRssFatherNode AddRssChildNode
bool AddRssFeedToTree(HWND hwnd, LPTSTR lpszItem, map<string, RssContent>& RssItemMsgMap);*/


//�ػ�RssList��HWND�����ھ����WPARAM �ؼ���ID��COLORREF notSelectItemRGBδѡ�е���ɫ��COLORREF selectItemRGBѡ�е���ɫ��
bool RedrawRssList(HWND hwnd, WPARAM iControlID, LPDRAWITEMSTRUCT lpDrawItemStruct,
	COLORREF notSelectItemRGB, COLORREF selectItemRGB);

//����Rss���ݵ�������塣
HFONT CreateRssMsgFont(HWND hwnd, BYTE lfCharSet, BYTE lfClipPrecision, TCHAR* lfFaceName, LONG lfHeight, BYTE lfQuality);

//��ʼ��RssList��״̬��ʹ�䱳������������ı���һ����
//HWND Rss������ı�����
bool InitRssList(HWND hwnd);

//��ѡ�е�RssFeed����ʾ��RssItem���б��С�
//hwnd�����ڵľ����iRssFeedIndex��RssFeed�б��е�RssFeed���±ꡣ
bool DisplayRssFeedToRssItemList(HWND hwnd, int iRssFeedIndex);

//��ѡ�е�RssItem������ʾ������
//hwnd�����ڵľ����iRssItemIndex��RssItem�б��е�RssItem���±ꡣ
bool DisplayRssItemMsg(HWND hwnd, int iRssItemIndex);

//���RssList�е����ݡ�
bool ClearRssListBlank(HWND hwnd, WPARAM listControlID);

//ɾ��RssFeed�б����һ��ѡ��Ҽ��˵���ɾ�����ǵ�ǰѡ���
bool DeleteRssFeedFromList(HWND hwnd);

//ɾ��RssItem�б����һ��ѡ��Ҽ��˵���ɾ�����ǵ�ǰѡ���
bool DeleteRssItemFromList(HWND hwnd);

//ˢ��RssFeed�б�����ȷ����ĳһ�
//HWND �����ڵľ����ˢ��RssFeed�б������ĳһ�
bool RefreshReeFeedInList(HWND hwnd);
