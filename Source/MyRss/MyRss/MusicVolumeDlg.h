/*
*�ļ����������ֲ������������Ӵ��ڽ�����ش���
*/

#include"resource.h"
#include<Windows.h>
#include<windowsx.h>
#include"MusicMessage.h"
#include"MusicTabDlg.h"

// �������ڵĴ��ڹ��̺����������������Ƿ�ģ̬�Ի���ӵ���Լ�����Ϣѭ����
BOOL CALLBACK MusicVolumeDlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//�����������ı��ʱ��
void VolumeSliderChange(HWND hwnd, HWND hwndCtrol, WPARAM wParam);
//��������ť�������ʱ���������ĳ�ʼ״̬,HWND ���������ڵľ����
bool InitVolumeSlider(HWND hwnd);