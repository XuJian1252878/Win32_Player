/*
*ѧ�ţ�1252878
*��������
*��ϵ�绰��18817870106
*���ʵ�ַ��1252878xujian@tongji.edu.cn
*�ļ�������Rss�Ķ����������д�뱾�ش��̵�ʵ��
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


//��ʼ��rssXML�ļ�����ʱ����·����ȷ������ʱ�ļ����ڲ����Խ�����HWND �����ڵľ����
//�ú������ݴ��RssXml���ļ�ȫ·�����أ�ͨ���������ķ�ʽ����\\RssReader\\src\\TempRssXml.txt
bool InitRssXMLTempSavePath(HWND hwnd, TCHAR* szTempRssXmlPath);

//����Rss�Ķ����ѱ������Ϣ��
//extern vector<string> RssFeedList;//���游��㡣
//extern vector< map<string, RssContent> > RssItemList;//����Rss��ÿһ���ӽڵ�����ݡ�
bool SaveRssMsg(HWND hwnd);

//��ȡRss�Ķ����ѱ������Ϣ����ʼ��Rss�Ķ�����
//extern vector<string> RssFeedList;//���游��㡣
//extern vector< map<string, RssContent> > RssItemList;//����Rss��ÿһ���ӽڵ�����ݡ�
bool InitRssMsg(HWND hwnd);