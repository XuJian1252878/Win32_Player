/*
*ѧ�ţ�1252878
*��������
*��ϵ�绰��18817870106
*���ʵ�ַ��1252878xujian@tongji.edu.cn
*�ļ����������ֲ����߼������ʵ��
*/

#pragma once

#include"resource.h"
#include"FileEncode.h"
#include"MusicMessage.h"
#include"MusicTabDlg.h"
#include"resource.h"
#include<Commdlg.h>//OPENFILENAME �ṹ�����ڵ�λ�á�
#include<shlobj.h>//BROWSEINFO�ṹ��Ķ��塣

//��ʾ����������ʱ�Ĵ�������Ϣ��
void ShowError();
//�ж���ӵ������ļ��Ƿ��ظ���
bool IsRepeat(TCHAR* musicFileName);//˵���ڱ����ʱ���ǽ�.h�ļ��������������.cpp��Ȼ���ٱ��룬�����������ݣ��ٱ��롣
//��÷ָ�Ϊ//��·����
TCHAR* GetPrePath(TCHAR *sMusicName);
//�ӵ�������������ļ��ĺ�����
void AddMusicFile(HWND hwnd);
//�������ֵļ�ʱ����
VOID CALLBACK MusicPlay_TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
//��ʾʵʱ���ָ�ʵļ�ʱ����
VOID CALLBACK RealTimeLrc_TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);


//��ʼ�����ֲ���ģʽComboBox������������ֲ��Ŵ��ھ��HWND��
bool InitPlayModeMessage(HWND hwnd);
//���û��϶��������Ž�������ʱ����Ӧ�ĺ�����
void PlayingSliderChange(HWND hwnd, HWND hwndCtrol, WPARAM wParam);
//���Ի�ؼ���Button�ؼ����뱳����
/*hwnd �����ڵľ�� wParam lParam WM_DRAWITEM��Ϣ�Ĳ���*/
void DrawBkgndToButton(HWND hwnd, WPARAM odBtnID, LPDRAWITEMSTRUCT lpDrawItemStruct);
/*������������ļ��Ĳ˵���
������������ھ��HWND �ɴ˻��Ӧ�ó���ʵ�����*/
bool LoadOperateFileMenu(HWND hwnd);


/*�Բ���ģʽ��ComboBox�����ػ�
����������hwnd �����ھ��
(LPDRAWITEMSTRUCT)lParam �ػ���ص���Ϣ
*/
bool RedrawPlayModeComboBox(HWND hwnd, LPDRAWITEMSTRUCT lpDrawItemStruct);
/*�����ֲ����б�����ػ�
����������hwnd �����ھ��
(LPDRAWITEMSTRUCT)lParam �ػ���ص���Ϣ*/
bool RedrawMusicList(HWND hwnd, LPDRAWITEMSTRUCT lpDrawItemStruct);

/*�Ե�ǰ���ָ���н����ػ�,�����ʾ�б�
����������hwnd �����ھ��
(LPDRAWITEMSTRUCT)lParam �ػ���ص���Ϣ*/
bool RedrawCurMusicLrc(HWND hwnd, LPDRAWITEMSTRUCT lpDrawItemStruct);
/*���ѳ������ָ���н����ػ�,�����ʾ�б�
����������hwnd �����ھ��
(LPDRAWITEMSTRUCT)lParam �ػ���ص���Ϣ*/
bool RedrawPreviousMusicLrc(HWND hwnd, LPDRAWITEMSTRUCT lpDrawItemStruct);
/*��δ�������ָ���н����ػ�,�����ʾ�б�
����������hwnd �����ھ��
(LPDRAWITEMSTRUCT)lParam �ػ���ص���Ϣ*/
bool RedrawNextMusicLrc(HWND hwnd, LPDRAWITEMSTRUCT lpDrawItemStruct);

//������ǰ���ֲ������������е�ǰ��ʾ��ʵ����塣
HFONT CreateMusicLrcFont(HWND hwnd, BYTE lfCharSet, BYTE lfClipPrecision, TCHAR* lfFaceName, LONG lfHeight, BYTE lfQuality);

//�����ֲ����б���ɾ��һ����Ŀ������ɾ�������ļ�
//HWND �����ھ��,iCurSelIndex �б��е�ǰѡ�е��±ꡣ
//musicMessageList�ж�Ӧ����ĿҲ��ɾ����
bool DeleteMusicItemFromList(int iCurSelIndex);

//�����ֲ����б���ɾ��һ����Ŀ��Ҳɾ�������ļ�
//HWND �����ھ��,iCurSelIndex �б��е�ǰѡ�е��±ꡣ
//musicMessageList�ж�Ӧ����ĿҲ��ɾ����
bool DeleteMusicItemFromDisk(int iCurSelIndex);


//������ֲ����б��е�������Ŀ��HWND �����ھ����
//musicMessageList�ж�Ӧ����ĿҲ��ɾ����
bool ClearAllMusicItemFromList();

//����ļ���Ŀ¼�ĺ�����HWND �����ھ����TCHAR* szSelFolderNameѡ���ļ��е����֡�
bool BrowseForFolder(HWND hwnd, TCHAR* szSelFolderPath);
//��ӱ��������ļ����е������ļ�,HWND �����ھ����
//bIsDragFolder �ļ����Ƿ��Ǳ���ק�����ģ����Ǳ���ק�����ģ���ôszDragFolderName���Ǳ���ק�����ļ��е����ơ�
bool AddLocalMusicFolder(HWND hwnd, bool bIsDragFolder,TCHAR* szDragFolderName = NULL);

//��һ�������ļ�·����vector��������������ļ���
bool AddMusicMessageFromVector(vector<wstring>& szMusicFileFullPathVector);

//��ö��������ļ���·����Ϣ��szMusicFileFullPath�Ѿ��Ǻ���\\��ȫ·��
//�ú�����·����Ϣ��������musicMessageList�б�
bool AddMusicMessageFromPath(TCHAR* szMusicFileFullPath);


//��ʼ�����ֽ���Ŀؼ�tip��ʾ��Ϣ��
bool InitMusicTabDlgCtrolTip(HWND hwnd);

