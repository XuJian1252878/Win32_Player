/*
*�ļ������������ڹ���ʵ�֣�Rss��Musicplayer�����л���ʵ�ֵȣ�
*/

#pragma once

#include"MainDlg.h"
#include"MusicTabDlg.h"
#include"RssTabDlg.h"
#include"MusicFunction.h"
#include<shellapi.h>//ϵͳ����ͼ�ꡣ
#include"AppWelcomeDlg.h"

const int MODE_RSS_TAB = 0x123;
const int MODE_MUSICPLAYER_TAB = 0x124;

//���ش����Tab�ؼ���
VOID LoadTabFrom(HWND hwnd, HWND* hMainDlgTab);
//��Сͼ����ӵ�Tabs�ϡ����ذ�����ͼ���ڴ�DC��
//BOOL BltImageToTabs(HDC hdc,int TAB_MODE,HBITMAP hBitmap,RECT rect);

//�������Ի���ı�����
/*hdc �����ڵ�HDC
hBitmap �����ڱ���ͼƬ
rect  �����ڵľ�������*/
HBRUSH GetBkgndImage(HWND hwnd, HBITMAP hBitmap, RECT rect);
//����������ײ���״̬��������״̬���ľ�������븸���ڵľ����
HWND CreateStatusBar(HWND hwnd);
/*���ָ����С��λͼ���
HWND ���ھ�����ɴ˻�û�ͼ��HDC
HBITMAP��ԭλͼ
iWidth:��Ҫת����λͼ�Ŀ�ȡ�
iHeight:��Ҫת����λͼ�ĸ߶�*/
HBITMAP GetSuitableHbitmap(HWND hwnd, HBITMAP oriHbitmap, int iWidth, int iHeight);
/*���Ի�ؼ���λͼ�ı���ɫ��Ϊ͸����
HWND �ÿؼ��ľ����ע�ⲻ�Ǵ��ڵľ����
COLORREF ��ѡ��͸��ɫ����ɫ����λͼ����Ҫ��Ϊ͸������ɫ
�����޷���ֵ��*/
void SetCtrolHbitmapTransparent(HWND hwnd, COLORREF crKey);

//��øó��������·����
bool GetAppRunPath(HWND hwnd,TCHAR* szAppRunPath);

//��������ʾ��ϵͳ���̣�HWND hwnd�������ڵľ����
void AppToTray(HWND hwnd, NOTIFYICONDATA& notifyicondata);

//��ÿؼ���Ӧ�Ļ�ˢ���������汳����һ�¡�
//hwnd������ iControlID����������ӿؼ�������Դλͼɾ����
HBRUSH GetControlBkgndImage(HWND hwnd, HBITMAP oriHbitmap, int iControlID);

//��ʼ������ؼ���tip��ʾ��Ϣ��hwnd��rss���ڵľ����
bool InitMainDlgCtrolTip(HWND hwnd);

//��ʼ�������������Ҫ�Ǽ��Ӧ�ó����Ƿ��һ�α�������Ȼ������Ӧ�Ķ�����
//HWND Ӧ�ó��������ڵľ����
bool InitMyRssPlayer(HWND hwnd);