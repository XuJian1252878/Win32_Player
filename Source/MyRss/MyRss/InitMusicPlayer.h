/*
*�ļ����������ֲ������������д�뱾�ش��̵�ʵ��
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


//�տ�ʼ�����ֲ�������ʱ�������ֲ����б��������һ�����ֲ������Ѿ���ӵ���Ŀ��
//��ʼ��musicMessageList����Ϣ�����HWND�����ֲ������ľ����
//��ȡ��һ�����ֲ������رպ󱣴���б����ݣ��ú���������MusicMessage::InitMusicMessage(hwnd, IDC_LISTMUSIC);����á�
bool InitMusicList(HWND hwnd);
//�˳�ʱ���������˳�ʱ����Ϣ��HWND�����ʵ�������
bool SaveMusicPlayerMessage(HWND hwnd);