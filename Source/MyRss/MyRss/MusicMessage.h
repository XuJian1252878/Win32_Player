/*
*ѧ�ţ�1252878
*��������
*��ϵ�绰��18817870106
*���ʵ�ַ��1252878xujian@tongji.edu.cn
*�ļ������������ļ������Ϣ�Լ����ֲ����߼���ʵ��
*/

#ifndef MUSICMESSAGE_H//Ԥ���ظ����롣
#define MUSICMESSAGE_H

#include<vector>
#include<mmsystem.h>
#include <stdlib.h> /*srand����ʽ������������ĳ�ʼ��������*/
#include <time.h> /*�õ���time����������Ҫ�����ͷ�ļ���time�����������ϵͳʱ�䡣*/
#include <tchar.h>
#include<windows.h>
#include<windowsx.h>
#include"resource.h"
#include"FileEncode.h"
#include<CommCtrl.h>
using namespace std;//vector�������ռ��


//���ֵĲ���״̬��
#define PLAYSTATEMENT_PLAY 1001
#define PLAYSTATEMENT_PAUSE 1002
#define PLAYSTATEMENT_CLOSE 1003

#define PLAYMODE_ORDER 0  //˳�򲥷�
#define PLAYMODE_RAND 1        //�������
#define PLAYMODE_REPEATONE 2 //����ѭ��
#define PLAYMODE_REPEATALL 3  //�б�ѭ��

#define MAX_MUSIC_TYPE 10//�����ļ���׺��������ַ�����
#define MAX_MUSIC_PATH 256//�����ļ�������ַ�����
#define MAX_MUSIC_LYRIC 200//����һ���ʵ�����ַ�����

#define MM_LIST_CONTROL_ID_NOT_FOUND 0x123//���ֲ������ؼ���δ���á�

struct Lyric
{
	long lTime;  //����
	TCHAR szMusicLrc[MAX_PATH];  //���
};//���ڴ��ʱ�估��Ӧ�ĸ�ʵĽṹ��

class MusicMessage
{
public:
	//�����ļ��ĸ�����Ϣ��
	TCHAR* GetMusicFileName();
	TCHAR* GetMusicFileFullPath();
	TCHAR* GetMusicFileShortPath();
	TCHAR* GetMusicFileType();
	void SetMusicFileName(TCHAR* musicFileName);
	void SetMusicFileFullPath(TCHAR* musicFileFullPath);
	void SetMusicFileShortPath(TCHAR* musicFileShortPath);
	void SetMusicFileType(TCHAR* musicFileType);

	//���ֵĿ��ƺ�����
	//��ʼ��MusicMessage�����ø����ھ�����Լ��ؼ�ID��ʵ���Ͼ��ǵ�����SetMusicWindowHwnd��SetMusicListControlID��
	static void InitMusicMessage(HWND hwnd, int musicListControlID);
	//���õ�ǰ���ֲ����б�ؼ����ڸ����ڵľ����
	static void SetMusicWindowHwnd(HWND hwnd);
	//��õ�ǰ���ֲ����б�ؼ����ڸ����ڵľ����
	static HWND GetMusicWindowHwnd();
	//���õ�ǰ���ֲ����б�ؼ���ID��
	static void SetMusicListControlID(int SetMusicListControlID);//���Ի����ʼ����ʱ��һ���������ǵĲ��衣
	//��õ�ǰ���ֲ����б�ؼ���ID��
	static int GetMusicListControlID();
	//��õ�ǰ���ֵĲ���״̬��
	static int GetMusicPlayState();
	//���õ�ǰ���ֵĲ���״̬��
	static void SetMusicPlayState(int palyState);
	//��õ�ǰ���ڲ������ֵ��±ꡣ
	static int GetCurPlayingMusic();
	//�����ֲ����б�ؼ��У���õ�ǰѡ�������ļ����±ꡣ
	static int GetCurSelMusicFromMusicListControl();
	//���õ�ǰ���ڲ������ֵ��±ꡣ
	static void SetCurPlayingMusic(int iCurPlayingMusic);
	//�������֡�
	static void PlayMusic(int iSelMusicIndex = -1);
	//��ͣ���֡�
	static void PauseMusic();
	//�ر����֡�
	static void CloseMusic();
	//��ȡ��ǰ�������ֵĳ���,��ǰ�ĵ�λ�Ǻ��롣
	static long GetPlayingMusicLen();
	//�������ִӵ�ǰλ�ò���,��ǰ�ĵ�λ�Ǻ��롣
	static void PlayMusicFrom(int pos);
	//��ȡ���ֵ�ǰ���ŵ�λ��,��ǰ�ĵ�λ�Ǻ��롣
	static long GetCurMusicPos();
	//��õ�ǰ���ֵĲ�����������õ��������Ѿ�/10�ġ�
	static int GetCurMusicVol();
	//���õ�ǰ���ֲ��ŵ���������õĲ������Ѿ�/10�ġ�
	static void SetCurMusicVol(int vol);
	//������һ������һ��������������ź�����
	static void PlayNextOrLast(int order = 0);
	//��õ�ǰ���ֵĲ���ģʽ��
	static int GetMusicPlayMode();
	//���õ�ǰ���ֵĲ���ģʽ��
	static bool SetMusicPlayMode(int iMusicPlayMode = PLAYMODE_ORDER);
	//������ģʽ���Զ�������һ�����֡�
	static void AutoPlayNextMusic();
	//����������ת���趨λ��,��ǰ�ĵ�λ�Ǻ��롣
	static void SeekMusicTo(int iMusicPos);
	/*�����ֲ������б��������Ŀ��hwnd �� �����б��ID��MusicMessage�ж��Ѿ�����
	���Ըú�����Ҫ�������� insertIndex ������ļ����б��е���һ��,-1��ʾ�Ӷ�ĩ�����ļ�
	�����ļ����б��е����� szName*/
	static bool AddMusicToList(int insertIndex,TCHAR* szName);
	/*�õ����ֲ����б��е�index������֡��Լ�������ֵ�����szMusicName*/
	static bool GetMusicNameInList(int index,TCHAR* szMusicName);
	//���õ�ǰ���ֲ�������ʵ��±�����
	static bool SetCurLyricIndex(int index);
	//��õ�ǰ���ֲ�������ʵ��±�����
	static int GetCurLyricIndex();
	//���뵱ǰ���ڲ������ֵĸ�ʡ�
	static bool GetPlayingMusicLrc();
	//���õ�ǰ���ŵ������Ƿ��и���ļ���
	static bool SetHasLyric(bool b);
	//���õ�ǰ���ŵ������Ƿ��и���ļ���
	static bool GetHasLyric();
	//���ò�������ǰ�Ƿ�������ʾ�����ʡ�
	static bool SetDeskTopLrcState(bool b);
	//��ò�������ǰ�Ƿ�������ʾ�����ʡ�
	static bool GetDeskTopLrcState();
	//���õ�ǰ���ֲ����������ʴ��ڵľ����
	static bool SetDeskTopLrcWnd(HWND hwnd);
	//��õ�ǰ���ֲ����������ʴ��ڵľ����
	static HWND GetDeskTopLrcWnd();
	//����������������ʾ��ʴ��ڣ�HWND��ʴ��ڵľ����
	static bool ClearMusicLrcList();
	//���ñ����������ڵľ����
	static bool SetMusicVolumeWnd(HWND hwnd);
	//����������ڵľ����
	static HWND GetMusicVolumeWnd();
	//�������������б��ڵľ����
	static bool SetSearchMusicItemWnd(HWND hwnd);
	//������������б��ڵľ����
	static HWND GetSearchMusicItemWnd();
	//�������ֲ����б����������������Ŀ������һ���ַ�������ѡ��
	//If the search is successful, the return value is the index of the selected item. If
	//the search is unsuccessful, the return value is LB_ERR and the current selection is not changed. 
	static int FindMusicItemFromMusicList(TCHAR* szSearchItemMsg);



private:
	TCHAR musicFileName[MAX_MUSIC_PATH];//�����ļ�������������׺����·����
	TCHAR musicFileFullPath[MAX_MUSIC_PATH];//�����ļ���ȫ����������׺��·����
	TCHAR musicFileShortPath[MAX_MUSIC_PATH];//�����ļ��Ķ�·�������ô�·�����������֡�
	TCHAR musicFileType[MAX_MUSIC_TYPE];//�������ֵ����͡�ֱ�Ӿ����������ơ�
	static int musicListControlID;//�����б�ؼ�����ԴID���������±�Ĳ�����
	static int musicPlayState;
	static int curPlayingMusic;//��ǰ���ڲ������ֵ��±���ֵ��musicMessageList�ڴ��б��С�
	static int musicPlayMode;
	static int curMusicPlayingVolume;//��ǰ���ڲ��ŵ����ֵ��������ٷ��Ƶģ�ԭ������ǧ���Ƶġ�
	//��¼��ǰ��ʹ����ڼ����ı�����
	static int iCurLyricIndex;
	static bool bHasLyric;//��ǰ���Ÿ����Ƿ��и���ļ���
	static bool bDeskTopLrcState;//��������ǰ�Ƿ�������ʾ�����ʡ�
	static HWND musicWindowHwnd;
	static HWND hDeskTopLrcWnd;//��ʾ�����ʴ��ڵľ����
	static HWND hMusicVolumeWnd;//�������ڵľ����
	static HWND hSearchMusicItemWnd;//����������Ŀ�Ĵ��ھ����
	//��PlayMusic��ʹ�ã����д����ֵĲ������Ա���������Ľ��С�
	static void OpenMusic(int iCurSelMusic);
};


#endif