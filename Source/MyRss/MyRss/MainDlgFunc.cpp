/*
*文件描述：主窗口功能实现（Rss和Musicplayer界面切换的实现）
*/

#include"stdafx.h"
#include"MainDlgFunc.h"

extern HWND hHideTabDlgWnd;
extern HBRUSH hMainBkgndBrush;


//初始化软件，现在主要是检测应用程序是否第一次被启动。
//HWND 应用程序主窗口的句柄。
//返回true，那么程序是第一次被启动。
bool InitMyRssPlayer(HWND hwnd)
{
	TCHAR szAppRunPath[MAX_PATH] = { NULL };
	GetAppRunPath(hwnd, szAppRunPath);
	TCHAR szAppSettingFilePath[MAX_PATH] = { NULL };//设置文件的保存路径。
	lstrcpy(szAppSettingFilePath, szAppRunPath);
	lstrcat(szAppSettingFilePath, TEXT("MyRssPlayerSetting.txt"));//获得设置文件的路径。
	char* cszAppSettingFilePath = NULL;//这个值要手动释放。
	cszAppSettingFilePath = FileEncode::UnicodeToAnsi((char*)szAppSettingFilePath);
	CreateFile(szAppSettingFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW,
		NULL, NULL);//检测文件是否存在的功能。
	if (GetLastError() == ERROR_FILE_EXISTS)//那么说明设置文件已存在，用户不是第一次使用该软件。
	{
		FILE* appSettingFile;
		_wfopen_s(&appSettingFile, szAppSettingFilePath, TEXT("wb"));//二进制方式打开文件。
		_fputts(TEXT("欢迎使用MyRssPlayer!"), appSettingFile);
		fclose(appSettingFile);
		free(cszAppSettingFilePath);
		return false;
	}
	//说明设置文件不存在，用户是第一次使用该软件。
	//先隐藏软件的主窗口，再弹出欢迎窗口。
	ShowWindow(hwnd, SW_HIDE);
	DialogBoxParam((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_AppWelcomeDlg),
		NULL, AppWelcomeDlg_Proc,(LPARAM)hwnd);//父窗口隐藏，子窗口也隐藏。
	return true;//用户是第一次使用该软件。
}


//将程序显示到系统托盘，HWND hwnd父主窗口的句柄。
void AppToTray(HWND hwnd, NOTIFYICONDATA& notifyicondata)
{
	//检查各种子窗口的状态。
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
	
	//向系统托盘显示。
	Shell_NotifyIcon(NIM_ADD, &notifyicondata);
	//并且隐藏窗口。
	ShowWindow(hwnd, SW_HIDE);
}

//加载窗体的Tab控件。
VOID LoadTabFrom(HWND hwnd, HWND* hMainDlgTab)
{
	/*TCITEM
	Specifies or receives the attributes of a tab item. It is used with the
	TCM_INSERTITEM, TCM_GETITEM, and TCM_SETITEM messages.
	This structure supersedes the TC_ITEM structure. */
	TCITEM tcitem;
	RECT rcTabControl;
	//取得Tab选项卡的客户区。
	GetClientRect(GetDlgItem(hwnd, IDC_MainDlg_Tab), &rcTabControl);
	//调整加载进来的Tab界面，使其不会覆盖Tab选项卡。
	rcTabControl.top += 0;
	rcTabControl.left += 76;
	rcTabControl.right -= 0;
	rcTabControl.bottom -= 0;
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
	//创建两个Tab选项卡。
	/*The MoveWindow function changes the position and dimensions of the specified window.
	For a top-level window, the position and dimensions are relative to the upper-left corner of the screen.
	For a child window, they are relative to the upper-left corner of the parent window's client area.
	bRepaint:Specifies whether the window is to be repainted.*/
	hMainDlgTab[0] = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MainDlg_Tab_Rss), GetDlgItem(hwnd, IDC_MainDlg_Tab), MainDlg_RSS_Proc);
	//调整窗口界面使其适应Tab选项卡的大小。
	MoveWindow(hMainDlgTab[0], rcTabControl.left, rcTabControl.top, rcTabControl.right - rcTabControl.left, rcTabControl.bottom - rcTabControl.top, TRUE);
	hMainDlgTab[1] = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MainDlg_Tab_MusicPlayer), GetDlgItem(hwnd, IDC_MainDlg_Tab), MainDlg_MusicPlayer_Proc);
	MoveWindow(hMainDlgTab[1], rcTabControl.left, rcTabControl.top, rcTabControl.right - rcTabControl.left, rcTabControl.bottom - rcTabControl.top, TRUE);
	//当主界面初始化完成之后显示的是RSS新闻阅读界面。
	/*The ShowWindow function sets the specified window's show state. */
	ShowWindow(hMainDlgTab[0], SW_SHOW);
	ShowWindow(hMainDlgTab[1], SW_HIDE);

	//设置每个Tab页面小选项的宽度和高度。
	SendMessage(GetDlgItem(hwnd, IDC_MainDlg_Tab), TCM_SETITEMSIZE, (WPARAM)0, (LPARAM)MAKELPARAM(75, 75));

	//设置tab选项的ImageList。
	HIMAGELIST hImageList = ImageList_Create(75, 75, ILC_COLOR, 0, 10);
	HBITMAP  hBitmapRss = GetSuitableHbitmap( hwnd, (HBITMAP)LoadImage((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
		MAKEINTRESOURCE(IDB_Rss_Tab), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_CREATEDIBSECTION) ,75 ,75);
	HBITMAP hBitmapMusicPlayer = GetSuitableHbitmap( hwnd, (HBITMAP)LoadImage((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
		MAKEINTRESOURCE(IDB_MusicPlayer_TAB), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_CREATEDIBSECTION), 75, 75);
	ImageList_Add(hImageList, hBitmapRss, NULL);
	ImageList_Add(hImageList, hBitmapMusicPlayer, NULL);
	SendDlgItemMessage(hwnd, IDC_MainDlg_Tab, TCM_SETIMAGELIST, (WPARAM)0, (LPARAM)(HIMAGELIST)hImageList);
	//释放位图资源。
	DeleteObject(hBitmapRss);
	DeleteObject(hBitmapMusicPlayer);

	//向Tab控件中插入选项卡图片。
	tcitem.mask =  TCIF_IMAGE;
	tcitem.iImage = 0;
	SendDlgItemMessage(hwnd, IDC_MainDlg_Tab, TCM_INSERTITEM, (WPARAM)0, (LPARAM)&tcitem);
	tcitem.iImage = 1;
	SendMessage(GetDlgItem(hwnd, IDC_MainDlg_Tab), TCM_INSERTITEM, (WPARAM)1, (LPARAM)&tcitem);
/*	//同时为选项卡增加ToolTip提示。
	TOOLINFO toolinfos[2];
	ZeroMemory(toolinfos, sizeof(toolinfos));
	TCHAR szToolMeg[2][20] = { TEXT("我的新闻"), TEXT("音乐馆") };
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

/*获得指定大小的位图句柄
HWND 窗口句柄，由此获得绘图的HDC
HBITMAP：原位图
iWidth:需要转化的位图的宽度。
iHeight:需要转化的位图的高度，不把源位图删除。。*/
HBITMAP GetSuitableHbitmap(HWND hwnd, HBITMAP oriHbitmap, int iWidth, int iHeight)
{
	//切记，直接拿句柄的hdc装入HBITMAP是不行的。
	if (hwnd == NULL || oriHbitmap == NULL || iWidth <= 0 || iHeight <= 0)
	{
		MessageBox(NULL, TEXT("GetSuitableHbitmap failed!!"), TEXT("error"), MB_OK | MB_ICONERROR);
		return NULL;
	}
	HDC hdc = GetDC(hwnd);
	HDC hdcMem = CreateCompatibleDC(hdc);
	HDC hdcTemp = CreateCompatibleDC(hdc);
	//获得源位图对象，得到源位图的大小。
	BITMAP bitmap;
	GetObject(oriHbitmap, sizeof(bitmap), &bitmap);
	//hdc选入对象。
	SelectObject(hdcTemp, oriHbitmap);
	//创建一个暂时位图，这个位图的大小为需求的大小。
	HBITMAP hTempBitmap = CreateCompatibleBitmap(hdc, iWidth, iHeight);
	//hdcMem选入位图。位图为目标大小。
	SelectObject(hdcMem, hTempBitmap);
	//接下来进行伸缩贴图。
	SetStretchBltMode(hdcMem, HALFTONE);
	StretchBlt(hdcMem, 0, 0, iWidth, iHeight, hdcTemp, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
	//删除资源。
	DeleteObject(hdcTemp);
	DeleteObject(hdcMem);
	ReleaseDC(hwnd, hdc);
	//对位图资源也要进行删除。
//	DeleteObject(oriHbitmap);
	//返回对应的位图。
	return hTempBitmap;
}

//设置主对话框的背景。得到主对话框背景画刷。不把源位图删除。
HBRUSH GetBkgndImage(HWND hwnd, HBITMAP oriHbitmap, RECT rect)
{
	int iWidth = rect.right - rect.left;
	int iHeight = rect.bottom - rect.top;
	HBITMAP hTempBitmap = GetSuitableHbitmap(hwnd, oriHbitmap, iWidth, iHeight);
	if (hTempBitmap == NULL)
	{
		MessageBox(hwnd, TEXT("GetBkgndImage 错误！！！"), TEXT("错误"), MB_OK | MB_ICONERROR);
		return NULL;
	}
	//创建背景画刷。
	HBRUSH hBkgndBrush = CreatePatternBrush(hTempBitmap);
	//释放位图资源。
	DeleteObject(hTempBitmap);
	return hBkgndBrush;
}


//获得控件对应的画刷，与主界面背景相一致。
//hwnd父窗口 iControlID父窗口里的子控件，不把源位图删除。//有效，但是截下来的位图大小与控件不一样。
HBRUSH GetControlBkgndImage(HWND hwnd, HBITMAP oriHbitmap, int iControlID)
{
	//首先获得与主界面背景相同的HBITMAP。
	//获得主界面的客户区大小。
	RECT mainWindowRect;
	GetWindowRect(hwnd, &mainWindowRect);
	HBITMAP hMainWindowBmp = GetSuitableHbitmap(hwnd, oriHbitmap,
		mainWindowRect.right - mainWindowRect.left, mainWindowRect.bottom-mainWindowRect.top);
	//获得当前控件的屏幕坐标。
	RECT controlRect;
	GetWindowRect(GetDlgItem(hwnd, iControlID), &controlRect);
	//然后算出子控件相对于父控件左上角的位置。
	POINT controlRelativePoint;
	controlRelativePoint.x = controlRect.left - mainWindowRect.left;
	controlRelativePoint.y = controlRect.top - mainWindowRect.top;
	//在获得主界面在这个区域上的位图背景。
	HDC hdc = GetDC(hwnd);
	//主界面内存DC。
	HDC hMainWindowMemDC = CreateCompatibleDC(hdc);
	SelectObject(hMainWindowMemDC, hMainWindowBmp);
	//控件内存DC。
	HDC hControlMemDC = CreateCompatibleDC(hdc);
	HBITMAP hControlBkgndBmp = CreateCompatibleBitmap(hdc, controlRect.right - controlRect.left,
		controlRect.bottom - controlRect.top);
	//将位图选入内存DC，将来作为背景。
	SelectObject(hControlMemDC, hControlBkgndBmp);
	//接下来从控件的相对位置进行BitBlt贴图。
	BitBlt(hControlMemDC, 0, 0, controlRect.right - controlRect.left+10, controlRect.bottom - controlRect.top+10,
		hMainWindowMemDC, controlRelativePoint.x, controlRelativePoint.y, SRCCOPY);//注意操作的都是客户区坐标。
	//最重要的一步，创建控件对应的位图画刷。
	//创建背景画刷。
	HBRUSH hControlBkgndBrush = CreatePatternBrush(hControlBkgndBmp);
	//释放资源。
	DeleteObject(hControlBkgndBmp);
	DeleteObject(hMainWindowMemDC);
	DeleteObject(hControlMemDC);
	ReleaseDC(hwnd, hdc);
	//释放资源。
	DeleteObject(hMainWindowBmp);
	return hControlBkgndBrush;
}

//创建状态栏。
HWND CreateStatusBar(HWND hwnd)
{
	if (hwnd == NULL)
	{
		MessageBox(NULL, TEXT("传入HWND CreateStatus(HWND hwnd)的窗口句柄为空！"), TEXT("消息"), MB_OK | MB_ICONERROR);
		return NULL;
	}
	HWND hwndStatus = CreateWindow(STATUSCLASSNAME, TEXT(""), SBARS_SIZEGRIP | WS_CHILD | WS_VISIBLE,
		0, 0, 0, 0, hwnd, NULL, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
	if (hwndStatus == NULL)
	{
		MessageBox(hwnd, TEXT("状态栏创建出现错误！"), TEXT("消息"), MB_OK | MB_ICONERROR);
		return NULL;
	}
	//移动初始化状态栏。
	RECT rcClient;//主窗口客户区的大小。
	GetClientRect(hwnd, &rcClient);
	int iWidth[3];
	int iLength = (int) (rcClient.right / 3);//根据父窗口的宽度来分割状态栏的大小。
	iWidth[0] = iLength;
	iWidth[1] = iLength * 2;
	iWidth[2] = iLength * 3;
	//根据iWidth数组来设置状态栏。
	SendMessage(hwndStatus, SB_SETPARTS, (WPARAM)(sizeof(iWidth) / sizeof(int)), (LPARAM)iWidth);
	MoveWindow(hwndStatus, 0, 0, 0, 0, TRUE);
	//设置状态栏各部分的文字。
	SendMessage(hwndStatus, SB_SETTEXT, (WPARAM)0, (LPARAM)TEXT("就绪"));
	//给状态栏上色。
	RECT statusRect;
	GetClientRect(hwndStatus, &statusRect);
	HDC hStatusDC = GetDC(hwndStatus);
	FillRect(hStatusDC, &statusRect, hMainBkgndBrush);
	ReleaseDC(hwndStatus, hStatusDC);
	//返回状态栏句柄。
	return hwndStatus;
}

/*将自绘控件上位图的背景色设为透明。
HWND 该控件的句柄，注意不是窗口的句柄。
COLORREF 被选作透明色的颜色。即位图上需要变为透明的颜色
函数无返回值。*/  //不知道为何无效
void SetCtrolHbitmapTransparent(HWND hwnd, COLORREF crKey)
{
	//设置分层属性，透明操作的前提。
	DWORD dwExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
	dwExStyle |= WS_EX_LAYERED;
	SetWindowLong(hwnd, GWL_EXSTYLE, (LONG)dwExStyle);
	//开始透明操作。
	SetLayeredWindowAttributes(hwnd, crKey, 0, LWA_COLORKEY);
}


//获得该程序的运行路径。知道根路径\\。要使用的话最好用lstrcpy拷贝。
bool GetAppRunPath(HWND hwnd,TCHAR* szAppRunPath)
{
	GetModuleFileName((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), szAppRunPath, MAX_PATH);
	if (szAppRunPath == NULL)
	{
		MessageBox(hwnd, TEXT("无法获得程序运行路径！！"), TEXT("ERROR!!"), MB_OK | MB_ICONERROR);
		return false;
	}
	//首先获得双斜杠路径。
//	GetPrePath(szAppRunPath);
	//获得文件的运行路径，去掉最后的那个exe。
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
	//获得程序运行跟文件夹路径之后返回。
	return true;
}

//初始化界面控件的tip提示信息。hwnd是rss窗口的句柄。//无效，不知为何。
bool InitMainDlgCtrolTip(HWND hwnd)
{
	int iControlID[3] = { IDC_AppSettingMenuBTN, IDC_MinMainDlgBTN, IDC_MinMainDlgBTN };
	TCHAR szControlTip[3][30] = { TEXT("设置"), TEXT("最小化到任务栏"), TEXT("最小化到托盘") };
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