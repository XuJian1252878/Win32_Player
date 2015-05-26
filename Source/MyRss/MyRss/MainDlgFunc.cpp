/*
*�ļ������������ڹ���ʵ�֣�Rss��Musicplayer�����л���ʵ�֣�
*/

#include"stdafx.h"
#include"MainDlgFunc.h"

extern HWND hHideTabDlgWnd;
extern HBRUSH hMainBkgndBrush;


//��ʼ�������������Ҫ�Ǽ��Ӧ�ó����Ƿ��һ�α�������
//HWND Ӧ�ó��������ڵľ����
//����true����ô�����ǵ�һ�α�������
bool InitMyRssPlayer(HWND hwnd)
{
	TCHAR szAppRunPath[MAX_PATH] = { NULL };
	GetAppRunPath(hwnd, szAppRunPath);
	TCHAR szAppSettingFilePath[MAX_PATH] = { NULL };//�����ļ��ı���·����
	lstrcpy(szAppSettingFilePath, szAppRunPath);
	lstrcat(szAppSettingFilePath, TEXT("MyRssPlayerSetting.txt"));//��������ļ���·����
	char* cszAppSettingFilePath = NULL;//���ֵҪ�ֶ��ͷš�
	cszAppSettingFilePath = FileEncode::UnicodeToAnsi((char*)szAppSettingFilePath);
	CreateFile(szAppSettingFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW,
		NULL, NULL);//����ļ��Ƿ���ڵĹ��ܡ�
	if (GetLastError() == ERROR_FILE_EXISTS)//��ô˵�������ļ��Ѵ��ڣ��û����ǵ�һ��ʹ�ø������
	{
		FILE* appSettingFile;
		_wfopen_s(&appSettingFile, szAppSettingFilePath, TEXT("wb"));//�����Ʒ�ʽ���ļ���
		_fputts(TEXT("��ӭʹ��MyRssPlayer!"), appSettingFile);
		fclose(appSettingFile);
		free(cszAppSettingFilePath);
		return false;
	}
	//˵�������ļ������ڣ��û��ǵ�һ��ʹ�ø������
	//����������������ڣ��ٵ�����ӭ���ڡ�
	ShowWindow(hwnd, SW_HIDE);
	DialogBoxParam((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_AppWelcomeDlg),
		NULL, AppWelcomeDlg_Proc,(LPARAM)hwnd);//���������أ��Ӵ���Ҳ���ء�
	return true;//�û��ǵ�һ��ʹ�ø������
}


//��������ʾ��ϵͳ���̣�HWND hwnd�������ڵľ����
void AppToTray(HWND hwnd, NOTIFYICONDATA& notifyicondata)
{
	//�������Ӵ��ڵ�״̬��
	if (MusicMessage::GetMusicVolumeWnd())
	{
		DestroyWindow(MusicMessage::GetMusicVolumeWnd());
		MusicMessage::SetMusicVolumeWnd(NULL);
	}
	if (MusicMessage::GetSearchMusicItemWnd())
	{
		DestroyWindow(MusicMessage::GetSearchMusicItemWnd());
		MusicMessage::SetSearchMusicItemWnd(NULL);
	}
	if (RssMessage::GetRssFeedSearchWnd())
	{
		DestroyWindow(RssMessage::GetRssFeedSearchWnd());
		RssMessage::SetRssFeedSearchWnd(NULL);
	}
	if (RssMessage::GetRssItemSearchWnd())
	{
		DestroyWindow(RssMessage::GetRssItemSearchWnd());
		RssMessage::SetRssItemSearchWnd(NULL);
	}
	if (hHideTabDlgWnd != NULL)
	{
		DestroyWindow(hHideTabDlgWnd);
	}

	ZeroMemory(&notifyicondata, sizeof(notifyicondata));
	//A handle to the icon to be added, modified, or deleted.
	notifyicondata.hIcon = LoadIcon((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDI_APPICON));
	notifyicondata.cbSize = (DWORD)sizeof(notifyicondata);
	// The Shell uses hWnd together with uID to identify which icon to operate on when Shell_NotifyIcon is invoked.
	notifyicondata.hWnd = hwnd;
	//The application-defined identifier of the taskbar icon.
	notifyicondata.uID = IDI_APPICON;
	//A null-terminated string that specifies the text for a standard ToolTip. 
	wsprintf(notifyicondata.szTip, TEXT("%s"), TEXT("MyApplication"));
	/*An application-defined message identifier. The system uses 
	this identifier to send notifications to the window identified in hWnd. 
	These notifications are sent when a mouse event occurs in the bounding 
	rectangle of the icon, or when the icon is selected or activated with the keyboard.*/
	notifyicondata.uCallbackMessage = WM_SHOWTRAY;
	//Specifies whether the Shell notify icon interface should use Windows 95 or Windows 2000 behavior.
//	notifyicondata.uVersion = NOTIFYICON_VERSION_4;//Use the Windows Vista behavior. Use this value for applications designed for Windows Vista and later.
	/*Flags that either indicate which of the other members contain valid data
	or provide additional information to the ToolTip as to how it should display.*/
	notifyicondata.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	
	//��ϵͳ������ʾ��
	Shell_NotifyIcon(NIM_ADD, &notifyicondata);
	//�������ش��ڡ�
	ShowWindow(hwnd, SW_HIDE);
}

//���ش����Tab�ؼ���
VOID LoadTabFrom(HWND hwnd, HWND* hMainDlgTab)
{
	/*TCITEM
	Specifies or receives the attributes of a tab item. It is used with the
	TCM_INSERTITEM, TCM_GETITEM, and TCM_SETITEM messages.
	This structure supersedes the TC_ITEM structure. */
	TCITEM tcitem;
	RECT rcTabControl;
	//ȡ��Tabѡ��Ŀͻ�����
	GetClientRect(GetDlgItem(hwnd, IDC_MainDlg_Tab), &rcTabControl);
	//�������ؽ�����Tab���棬ʹ�䲻�Ḳ��Tabѡ���
	rcTabControl.top += 0;
	rcTabControl.left += 76;
	rcTabControl.right -= 0;
	rcTabControl.bottom -= 0;
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
	//��������Tabѡ���
	/*The MoveWindow function changes the position and dimensions of the specified window.
	For a top-level window, the position and dimensions are relative to the upper-left corner of the screen.
	For a child window, they are relative to the upper-left corner of the parent window's client area.
	bRepaint:Specifies whether the window is to be repainted.*/
	hMainDlgTab[0] = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MainDlg_Tab_Rss), GetDlgItem(hwnd, IDC_MainDlg_Tab), MainDlg_RSS_Proc);
	//�������ڽ���ʹ����ӦTabѡ��Ĵ�С��
	MoveWindow(hMainDlgTab[0], rcTabControl.left, rcTabControl.top, rcTabControl.right - rcTabControl.left, rcTabControl.bottom - rcTabControl.top, TRUE);
	hMainDlgTab[1] = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MainDlg_Tab_MusicPlayer), GetDlgItem(hwnd, IDC_MainDlg_Tab), MainDlg_MusicPlayer_Proc);
	MoveWindow(hMainDlgTab[1], rcTabControl.left, rcTabControl.top, rcTabControl.right - rcTabControl.left, rcTabControl.bottom - rcTabControl.top, TRUE);
	//���������ʼ�����֮����ʾ����RSS�����Ķ����档
	/*The ShowWindow function sets the specified window's show state. */
	ShowWindow(hMainDlgTab[0], SW_SHOW);
	ShowWindow(hMainDlgTab[1], SW_HIDE);

	//����ÿ��Tabҳ��Сѡ��Ŀ�Ⱥ͸߶ȡ�
	SendMessage(GetDlgItem(hwnd, IDC_MainDlg_Tab), TCM_SETITEMSIZE, (WPARAM)0, (LPARAM)MAKELPARAM(75, 75));

	//����tabѡ���ImageList��
	HIMAGELIST hImageList = ImageList_Create(75, 75, ILC_COLOR, 0, 10);
	HBITMAP  hBitmapRss = GetSuitableHbitmap( hwnd, (HBITMAP)LoadImage((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
		MAKEINTRESOURCE(IDB_Rss_Tab), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_CREATEDIBSECTION) ,75 ,75);
	HBITMAP hBitmapMusicPlayer = GetSuitableHbitmap( hwnd, (HBITMAP)LoadImage((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
		MAKEINTRESOURCE(IDB_MusicPlayer_TAB), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_CREATEDIBSECTION), 75, 75);
	ImageList_Add(hImageList, hBitmapRss, NULL);
	ImageList_Add(hImageList, hBitmapMusicPlayer, NULL);
	SendDlgItemMessage(hwnd, IDC_MainDlg_Tab, TCM_SETIMAGELIST, (WPARAM)0, (LPARAM)(HIMAGELIST)hImageList);
	//�ͷ�λͼ��Դ��
	DeleteObject(hBitmapRss);
	DeleteObject(hBitmapMusicPlayer);

	//��Tab�ؼ��в���ѡ�ͼƬ��
	tcitem.mask =  TCIF_IMAGE;
	tcitem.iImage = 0;
	SendDlgItemMessage(hwnd, IDC_MainDlg_Tab, TCM_INSERTITEM, (WPARAM)0, (LPARAM)&tcitem);
	tcitem.iImage = 1;
	SendMessage(GetDlgItem(hwnd, IDC_MainDlg_Tab), TCM_INSERTITEM, (WPARAM)1, (LPARAM)&tcitem);
/*	//ͬʱΪѡ�����ToolTip��ʾ��
	TOOLINFO toolinfos[2];
	ZeroMemory(toolinfos, sizeof(toolinfos));
	TCHAR szToolMeg[2][20] = { TEXT("�ҵ�����"), TEXT("���ֹ�") };
	HWND hTTwnd = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_ALWAYSTIP | TTS_BALLOON,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hwnd, NULL,
		(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
	for (int index = 0; index < 2; index++)
	{
		toolinfos[index].cbSize = sizeof(toolinfos[index]);
		toolinfos[index].hinst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
		toolinfos[index].uFlags = TTF_IDISHWND | TTF_SUBCLASS;
		toolinfos[index].uId = (UINT_PTR)GetDlgItem(hwnd, IDC_MainDlg_Tab);
	}
	toolinfos[0].lpszText = szToolMeg[0];
	toolinfos[1].lpszText = szToolMeg[1];
	SendMessage(hTTwnd, TTM_ADDTOOL, (WPARAM)0, (LPARAM)&toolinfos[0]);
//	SendMessage(hTTwnd, TTM_ADDTOOL, (WPARAM)0, (LPARAM)&toolinfos[1]);
//	SendDlgItemMessage(hwnd, IDC_MainDlg_Tab, TCM_SETTOOLTIPS, (WPARAM)hTTwnd, (LPARAM)0);*/
}

/*���ָ����С��λͼ���
HWND ���ھ�����ɴ˻�û�ͼ��HDC
HBITMAP��ԭλͼ
iWidth:��Ҫת����λͼ�Ŀ�ȡ�
iHeight:��Ҫת����λͼ�ĸ߶ȣ�����Դλͼɾ������*/
HBITMAP GetSuitableHbitmap(HWND hwnd, HBITMAP oriHbitmap, int iWidth, int iHeight)
{
	//�мǣ�ֱ���þ����hdcװ��HBITMAP�ǲ��еġ�
	if (hwnd == NULL || oriHbitmap == NULL || iWidth <= 0 || iHeight <= 0)
	{
		MessageBox(NULL, TEXT("GetSuitableHbitmap failed!!"), TEXT("error"), MB_OK | MB_ICONERROR);
		return NULL;
	}
	HDC hdc = GetDC(hwnd);
	HDC hdcMem = CreateCompatibleDC(hdc);
	HDC hdcTemp = CreateCompatibleDC(hdc);
	//���Դλͼ���󣬵õ�Դλͼ�Ĵ�С��
	BITMAP bitmap;
	GetObject(oriHbitmap, sizeof(bitmap), &bitmap);
	//hdcѡ�����
	SelectObject(hdcTemp, oriHbitmap);
	//����һ����ʱλͼ�����λͼ�Ĵ�СΪ����Ĵ�С��
	HBITMAP hTempBitmap = CreateCompatibleBitmap(hdc, iWidth, iHeight);
	//hdcMemѡ��λͼ��λͼΪĿ���С��
	SelectObject(hdcMem, hTempBitmap);
	//����������������ͼ��
	SetStretchBltMode(hdcMem, HALFTONE);
	StretchBlt(hdcMem, 0, 0, iWidth, iHeight, hdcTemp, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
	//ɾ����Դ��
	DeleteObject(hdcTemp);
	DeleteObject(hdcMem);
	ReleaseDC(hwnd, hdc);
	//��λͼ��ԴҲҪ����ɾ����
//	DeleteObject(oriHbitmap);
	//���ض�Ӧ��λͼ��
	return hTempBitmap;
}

//�������Ի���ı������õ����Ի��򱳾���ˢ������Դλͼɾ����
HBRUSH GetBkgndImage(HWND hwnd, HBITMAP oriHbitmap, RECT rect)
{
	int iWidth = rect.right - rect.left;
	int iHeight = rect.bottom - rect.top;
	HBITMAP hTempBitmap = GetSuitableHbitmap(hwnd, oriHbitmap, iWidth, iHeight);
	if (hTempBitmap == NULL)
	{
		MessageBox(hwnd, TEXT("GetBkgndImage ���󣡣���"), TEXT("����"), MB_OK | MB_ICONERROR);
		return NULL;
	}
	//����������ˢ��
	HBRUSH hBkgndBrush = CreatePatternBrush(hTempBitmap);
	//�ͷ�λͼ��Դ��
	DeleteObject(hTempBitmap);
	return hBkgndBrush;
}


//��ÿؼ���Ӧ�Ļ�ˢ���������汳����һ�¡�
//hwnd������ iControlID����������ӿؼ�������Դλͼɾ����//��Ч�����ǽ�������λͼ��С��ؼ���һ����
HBRUSH GetControlBkgndImage(HWND hwnd, HBITMAP oriHbitmap, int iControlID)
{
	//���Ȼ���������汳����ͬ��HBITMAP��
	//���������Ŀͻ�����С��
	RECT mainWindowRect;
	GetWindowRect(hwnd, &mainWindowRect);
	HBITMAP hMainWindowBmp = GetSuitableHbitmap(hwnd, oriHbitmap,
		mainWindowRect.right - mainWindowRect.left, mainWindowRect.bottom-mainWindowRect.top);
	//��õ�ǰ�ؼ�����Ļ���ꡣ
	RECT controlRect;
	GetWindowRect(GetDlgItem(hwnd, iControlID), &controlRect);
	//Ȼ������ӿؼ�����ڸ��ؼ����Ͻǵ�λ�á�
	POINT controlRelativePoint;
	controlRelativePoint.x = controlRect.left - mainWindowRect.left;
	controlRelativePoint.y = controlRect.top - mainWindowRect.top;
	//�ڻ������������������ϵ�λͼ������
	HDC hdc = GetDC(hwnd);
	//�������ڴ�DC��
	HDC hMainWindowMemDC = CreateCompatibleDC(hdc);
	SelectObject(hMainWindowMemDC, hMainWindowBmp);
	//�ؼ��ڴ�DC��
	HDC hControlMemDC = CreateCompatibleDC(hdc);
	HBITMAP hControlBkgndBmp = CreateCompatibleBitmap(hdc, controlRect.right - controlRect.left,
		controlRect.bottom - controlRect.top);
	//��λͼѡ���ڴ�DC��������Ϊ������
	SelectObject(hControlMemDC, hControlBkgndBmp);
	//�������ӿؼ������λ�ý���BitBlt��ͼ��
	BitBlt(hControlMemDC, 0, 0, controlRect.right - controlRect.left+10, controlRect.bottom - controlRect.top+10,
		hMainWindowMemDC, controlRelativePoint.x, controlRelativePoint.y, SRCCOPY);//ע������Ķ��ǿͻ������ꡣ
	//����Ҫ��һ���������ؼ���Ӧ��λͼ��ˢ��
	//����������ˢ��
	HBRUSH hControlBkgndBrush = CreatePatternBrush(hControlBkgndBmp);
	//�ͷ���Դ��
	DeleteObject(hControlBkgndBmp);
	DeleteObject(hMainWindowMemDC);
	DeleteObject(hControlMemDC);
	ReleaseDC(hwnd, hdc);
	//�ͷ���Դ��
	DeleteObject(hMainWindowBmp);
	return hControlBkgndBrush;
}

//����״̬����
HWND CreateStatusBar(HWND hwnd)
{
	if (hwnd == NULL)
	{
		MessageBox(NULL, TEXT("����HWND CreateStatus(HWND hwnd)�Ĵ��ھ��Ϊ�գ�"), TEXT("��Ϣ"), MB_OK | MB_ICONERROR);
		return NULL;
	}
	HWND hwndStatus = CreateWindow(STATUSCLASSNAME, TEXT(""), SBARS_SIZEGRIP | WS_CHILD | WS_VISIBLE,
		0, 0, 0, 0, hwnd, NULL, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
	if (hwndStatus == NULL)
	{
		MessageBox(hwnd, TEXT("״̬���������ִ���"), TEXT("��Ϣ"), MB_OK | MB_ICONERROR);
		return NULL;
	}
	//�ƶ���ʼ��״̬����
	RECT rcClient;//�����ڿͻ����Ĵ�С��
	GetClientRect(hwnd, &rcClient);
	int iWidth[3];
	int iLength = (int) (rcClient.right / 3);//���ݸ����ڵĿ�����ָ�״̬���Ĵ�С��
	iWidth[0] = iLength;
	iWidth[1] = iLength * 2;
	iWidth[2] = iLength * 3;
	//����iWidth����������״̬����
	SendMessage(hwndStatus, SB_SETPARTS, (WPARAM)(sizeof(iWidth) / sizeof(int)), (LPARAM)iWidth);
	MoveWindow(hwndStatus, 0, 0, 0, 0, TRUE);
	//����״̬�������ֵ����֡�
	SendMessage(hwndStatus, SB_SETTEXT, (WPARAM)0, (LPARAM)TEXT("����"));
	//��״̬����ɫ��
	RECT statusRect;
	GetClientRect(hwndStatus, &statusRect);
	HDC hStatusDC = GetDC(hwndStatus);
	FillRect(hStatusDC, &statusRect, hMainBkgndBrush);
	ReleaseDC(hwndStatus, hStatusDC);
	//����״̬�������
	return hwndStatus;
}

/*���Ի�ؼ���λͼ�ı���ɫ��Ϊ͸����
HWND �ÿؼ��ľ����ע�ⲻ�Ǵ��ڵľ����
COLORREF ��ѡ��͸��ɫ����ɫ����λͼ����Ҫ��Ϊ͸������ɫ
�����޷���ֵ��*/  //��֪��Ϊ����Ч
void SetCtrolHbitmapTransparent(HWND hwnd, COLORREF crKey)
{
	//���÷ֲ����ԣ�͸��������ǰ�ᡣ
	DWORD dwExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
	dwExStyle |= WS_EX_LAYERED;
	SetWindowLong(hwnd, GWL_EXSTYLE, (LONG)dwExStyle);
	//��ʼ͸��������
	SetLayeredWindowAttributes(hwnd, crKey, 0, LWA_COLORKEY);
}


//��øó��������·����֪����·��\\��Ҫʹ�õĻ������lstrcpy������
bool GetAppRunPath(HWND hwnd,TCHAR* szAppRunPath)
{
	GetModuleFileName((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), szAppRunPath, MAX_PATH);
	if (szAppRunPath == NULL)
	{
		MessageBox(hwnd, TEXT("�޷���ó�������·������"), TEXT("ERROR!!"), MB_OK | MB_ICONERROR);
		return false;
	}
	//���Ȼ��˫б��·����
//	GetPrePath(szAppRunPath);
	//����ļ�������·����ȥ�������Ǹ�exe��
	for (int index = lstrlen(szAppRunPath); index >= 0; --index)
	{
		if (szAppRunPath[index] != '\\')
		{
			szAppRunPath[index] = '\0';
		}
		else
		{
			break;
		}
	}
	//��ó������и��ļ���·��֮�󷵻ء�
	return true;
}

//��ʼ������ؼ���tip��ʾ��Ϣ��hwnd��rss���ڵľ����//��Ч����֪Ϊ�Ρ�
bool InitMainDlgCtrolTip(HWND hwnd)
{
	int iControlID[3] = { IDC_AppSettingMenuBTN, IDC_MinMainDlgBTN, IDC_MinMainDlgBTN };
	TCHAR szControlTip[3][30] = { TEXT("����"), TEXT("��С����������"), TEXT("��С��������") };
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
	HWND hToolTipWnd = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
		0, 0, 0, 0, hwnd, NULL, hInstance, NULL);
	TOOLINFO toolinfo[3];
	ZeroMemory(toolinfo, sizeof(toolinfo));
	for (int index = 0; index < 3; index++)
	{
		toolinfo[index].cbSize = sizeof(TOOLINFO);
		toolinfo[index].uFlags = TTF_IDISHWND | TTF_SUBCLASS;
		toolinfo[index].uId = (UINT)GetDlgItem(hwnd, iControlID[index]);
		toolinfo[index].hinst = hInstance;
		toolinfo[index].lpszText = szControlTip[index];
		SendMessage(hToolTipWnd, TTM_ADDTOOL, (WPARAM)TRUE, (LPARAM)&toolinfo[index]);
	}
	return true;
}