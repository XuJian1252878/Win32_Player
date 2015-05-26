/*
*学号：1252878
*姓名：许舰
*联系电话：18817870106
*电邮地址：1252878xujian@tongji.edu.cn
*文件描述：主窗口功能实现（Rss和Musicplayer界面切换的实现）
*/

#include"stdafx.h"
#include"MainDlg.h"


bool bIsAppFirseUse = false;//检测用户是否第一次使用该软件。true是第一次使用。
HWND hMainDlgTab[2];//保存主界面Tab选项卡的数组变量。
HWND hwndStatus = NULL;
HBRUSH hMainBkgndBrush = NULL;//主界面的背景画刷。
char* SystemDefaultLocale = NULL;//保存系统原本的地域信息。
NOTIFYICONDATA notifyicondata;//系统托盘消息。
HWND hHideTabDlgWnd = NULL;//隐藏Tab窗口的句柄。


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MainDlg), NULL, MainDlg_Proc);
	return 0;
}

BOOL CALLBACK MainDlg_Proc(HWND hwnd, UINT uMeg, WPARAM wParam, LPARAM lParam)
{
	switch (uMeg)
	{
			case WM_INITDIALOG:
			{
				bIsAppFirseUse = InitMyRssPlayer(hwnd);//初始化MyRssPlayer软件，显示欢迎窗口等。
				LoadTabFrom(hwnd,hMainDlgTab);
				HBITMAP hBitmap = (HBITMAP)LoadImage((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
					MAKEINTRESOURCE(IDB_MainBkgnd), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_SHARED | LR_DEFAULTSIZE);
				RECT clientRect;
				GetClientRect(hwnd, &clientRect);
				hMainBkgndBrush = GetBkgndImage(hwnd, hBitmap, clientRect);
				//设置Tip提示消息。
//				InitMainDlgCtrolTip(hwnd);
				//显示状态栏。
				hwndStatus = CreateStatusBar(hwnd);
				//设置应用程序的图标。
				HICON hIcon = LoadIcon((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDI_APPICON));
				SendMessage(hwnd, WM_SETICON, (WPARAM)TRUE, (LPARAM)hIcon);
				//获得系统原来的地域信息。重要
				SystemDefaultLocale = _strdup(setlocale(LC_CTYPE, NULL));
				//用来配置地域的信息，设置当前程序使用的本地化信息。这样fputsw可以向text中写入中文。
				setlocale(LC_CTYPE, "chs");
				//创建用来隐藏Tab的窗口。
				//获得主窗口的屏幕坐标。
				RECT mainWindowRect;
				GetWindowRect(hwnd, &mainWindowRect);
				hHideTabDlgWnd = CreateDialogParam((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
					MAKEINTRESOURCE(IDD_HideMainTab), hwnd, HideMainTab_Proc, (LPARAM)&mainWindowRect);

			}
				break;
			case WM_LBUTTONDOWN:
			{
				//使左键按下的时候窗口能够移动。
				SendMessage(hwnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
			}
				break;
			case WM_SHOWTRAY:
			{
				switch (wParam)//the wParam parameter of the message contains the identifier of the taskbar icon in which the event occurred. 
				{
				case IDI_APPICON:
				{
					switch (lParam)//The lParam parameter holds the mouse or keyboard message associated with the event.
					{
					case WM_LBUTTONDBLCLK:
					{
						//从托盘重新显示程序。
						Shell_NotifyIcon(NIM_DELETE, &notifyicondata);
						ShowWindow(hwnd, SW_NORMAL);
						SetForegroundWindow(hwnd);//设置窗口在最前端显示。
						RECT mainWindowRect;
						GetWindowRect(hwnd, &mainWindowRect);
						hHideTabDlgWnd = CreateDialogParam((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
							MAKEINTRESOURCE(IDD_HideMainTab), hwnd, HideMainTab_Proc, (LPARAM)&mainWindowRect);
					}
						return TRUE;
					case WM_RBUTTONDOWN:
					{
						HMENU hTrayMenu = LoadMenu((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
							MAKEINTRESOURCE(IDR_TrayMenu));
						hTrayMenu = GetSubMenu(hTrayMenu, 0);
						POINT cursorScreenPoint;
						GetCursorPos(&cursorScreenPoint);
						TrackPopupMenuEx(hTrayMenu, TPM_LEFTALIGN, cursorScreenPoint.x,
							cursorScreenPoint.y, hwnd, NULL);//托盘右键菜单。
					}
						return TRUE;
					default:
						return FALSE;
					}
				}
					return TRUE;
				default:
					return FALSE;
				}
			}
				return TRUE;
			case WM_MOVE:
			{
				if (MusicMessage::GetMusicVolumeWnd())//当前音量窗口正在显示状态。
				{
					//找到音乐播放器音量按钮的位置。
					RECT volBtnScreenRect;
					GetWindowRect(GetDlgItem(MusicMessage::GetMusicWindowHwnd(), IDC_MusicVolume), &volBtnScreenRect);
					//获得音量按钮的位置之后，将位置信息发送给子窗口。
					SendMessage(MusicMessage::GetMusicVolumeWnd(), WM_MAINDLGMOVE_VolDlg,
					(WPARAM)volBtnScreenRect.right, (LPARAM)volBtnScreenRect.top + 7);
				}
				if (MusicMessage::GetSearchMusicItemWnd())//搜索窗口存在的话。
				{
					RECT searchItemBtnScreenRect;
					GetWindowRect(GetDlgItem(MusicMessage::GetMusicWindowHwnd(), IDC_SearchMusicItem),
						&searchItemBtnScreenRect);
					//将按钮右下角坐标传过去。
					SendMessage(MusicMessage::GetSearchMusicItemWnd(), WM_MAINDLGMOVE_SearchItemDlg,
						(WPARAM)searchItemBtnScreenRect.right, (LPARAM)searchItemBtnScreenRect.top);
				}
				if (RssMessage::GetRssFeedSearchWnd())//当前增加RssFeed的窗口在显示。
				{
					RECT searchRssFeedBtnRect;
					GetWindowRect(GetDlgItem(hMainDlgTab[0], IDC_AddRssFeedBtn),
						&searchRssFeedBtnRect);
					//将按钮的右上角屏幕坐标传递过去。
					SendMessage(RssMessage::GetRssFeedSearchWnd(), WM_MAINDLGMOVE_RssDlg, (WPARAM)searchRssFeedBtnRect.right,
						(LPARAM)searchRssFeedBtnRect.top);
				}
				if (RssMessage::GetRssItemSearchWnd())
				{
					RECT searchRssItemBtnRect;
					GetWindowRect(GetDlgItem(RssMessage::GetRssWindowHwnd(), IDC_SearchRssItemBtn),
						&searchRssItemBtnRect);
					//将按钮的左上角屏幕坐标传递过去。
					SendMessage(RssMessage::GetRssItemSearchWnd(), WM_MAINDLGMOVE_RssDlg, (WPARAM)searchRssItemBtnRect.left,
						(LPARAM)searchRssItemBtnRect.top);
				}
				if (hHideTabDlgWnd != NULL)
				{
					RECT mainWindowRect;
					GetWindowRect(hwnd, &mainWindowRect);
					SendMessage(hHideTabDlgWnd, WM_MAINDLGMOVE_HideTab,
						(WPARAM)mainWindowRect.left, (LPARAM)mainWindowRect.top);
				}
			}
				return TRUE;
			case WM_CTLCOLORDLG:
			{
				return (BOOL)hMainBkgndBrush;
			}
				break;
			case WM_CTLCOLORSTATIC:
			{
				return (BOOL)hMainBkgndBrush;
			}
				break;
			case WM_CTLCOLORBTN:
			{
				SetBkMode((HDC)wParam, TRANSPARENT);
				SetTextColor((HDC)wParam, RGB(255, 0, 0));
				return(BOOL)hMainBkgndBrush;
			}
				break;
			case WM_COMMAND:
			{
				switch (LOWORD(wParam))
				{
				case IDC_ToTrayBTN://程序最小化到系统托盘。
				{
					AppToTray(hwnd, notifyicondata);
				}
					break;
				case ID_QuitApp://托盘右键菜单退出项。
				{
					//从托盘删除显示程序。
					Shell_NotifyIcon(NIM_DELETE, &notifyicondata);
					SaveMusicPlayerMessage(hwnd);
					SaveRssMsg(hwnd);
					//还原区域设定,解决fputws向txt文件中写入中文的问题。
					setlocale(LC_CTYPE, SystemDefaultLocale);
					free(SystemDefaultLocale);
					EndDialog(hwnd, 0);
				}
					return TRUE;
				case IDC_MinMainDlgBTN://最小化窗口。
				{
					ShowWindow(hwnd, SW_MINIMIZE);
				}
					return TRUE;
				case IDC_AppSettingMenuBTN:
				{
					POINT point;//获得用户当前的鼠标坐标。
					GetCursorPos(&point);
					HMENU hMenu = LoadMenu((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDR_AppSetting_Menu));
					hMenu = GetSubMenu(hMenu, 0);
					TrackPopupMenuEx(hMenu, TPM_LEFTALIGN, point.x, point.y, hwnd, NULL);
				}
					return TRUE;
				default:
					break;
				}
			}
				break;
			case WM_DRAWITEM:
			{
				LPDRAWITEMSTRUCT lpDrawItemStruct = (LPDRAWITEMSTRUCT)lParam;
				switch (lpDrawItemStruct->CtlType)
				{
				case ODT_BUTTON:
				{
					DrawBkgndToButton(hwnd, wParam, lpDrawItemStruct);
				}
					return TRUE;
				default:
					return FALSE;
				}
			}
				return TRUE;
			case WM_NOTIFY://与Tab页面切换有关的消息。
			{
				//Sent by a common control to its parent window when an event has occurred or the control requires some information.
				TCHAR szToolMeg[2][20] = { TEXT("我的新闻"), TEXT("音乐馆") };
				LPNMTTDISPINFO lpNmttdispInfo = (LPNMTTDISPINFO)lParam;
				LPNMHDR lpNmhdr = &(lpNmttdispInfo->hdr);
				if ((lpNmhdr->hwndFrom) == GetDlgItem(hwnd, IDC_MainDlg_Tab))
				{
					/*Notifies a tab control's parent window that the currently 
					selected tab has changed. This message is sent in the form of a WM_NOTIFY message. */
					if (lpNmhdr->code == TCN_SELCHANGE)
					{
						//得到主页面Tab中当前选到的项。
						/*The SendDlgItemMessage function sends a message to the specified control in a dialog box.*/
						int curPageIndex = SendDlgItemMessage(hwnd, IDC_MainDlg_Tab, TCM_GETCURSEL, 0, 0);
						switch (curPageIndex)
						{
							case 0://当前的Tab选项是RSS新闻界面。
							{
								ShowWindow(hMainDlgTab[0], SW_SHOW);
								//如果跟音乐界面的子窗口处于开启状态，那么关闭他们。
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
								ShowWindow(hMainDlgTab[1], SW_HIDE);
							}
							break;
							case 1://当前的Tab选项是音乐播放器界面。
							{
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
								ShowWindow(hMainDlgTab[0], SW_HIDE);
								ShowWindow(hMainDlgTab[1], SW_SHOW);
							}
							break;
						default:
							break;
						}
					}
				}
			}
				break;
			case WM_CLOSE:
			{
				SaveRssMsg(hwnd);
				SaveMusicPlayerMessage(hwnd);
				DestroyWindow(hHideTabDlgWnd);
				//还原区域设定,解决fputws向txt文件中写入中文的问题。
				setlocale(LC_CTYPE, SystemDefaultLocale);
				free(SystemDefaultLocale);
				EndDialog(hwnd, 0);
			}
				break;
/*			case WM_DRAWITEM:
			{
								/*http://msdn.microsoft.com/en-us/library/windows/desktop/bb760550(v=vs.85).aspx#owner_drawn_tabs */
/*								LPDRAWITEMSTRUCT lpDrawItemStruct = (LPDRAWITEMSTRUCT)lParam;
								if (lpDrawItemStruct->CtlType == ODT_TAB && lpDrawItemStruct->CtlID == IDC_MainDlg_Tab)//是Tab的自绘消息。
								{
									HBITMAP  hBitmapClick = (HBITMAP)LoadImage((HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),
										MAKEINTRESOURCE(IDB_TabClick), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_CREATEDIBSECTION|LR_SHARED);
									HBITMAP hBitmapUnclick = (HBITMAP)LoadImage((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
										MAKEINTRESOURCE(IDB_TabUnclick), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_CREATEDIBSECTION|LR_SHARED);
									if (hBitmapClick == NULL || hBitmapUnclick == NULL)
									{
										MessageBox(hwnd, TEXT("load image failed!!"), TEXT("error"), MB_OK | MB_ICONERROR);
										exit(0);
									}
									//开始贴图。
									if (lpDrawItemStruct->itemState&ODS_SELECTED)
									{
//										BltImageToTabs(lpDrawItemStruct->hDC, MODE_MUSICPLAYER_TAB, hBitmapClick, lpDrawItemStruct->rcItem);
									}
									else{
//										BltImageToTabs(lpDrawItemStruct->hDC, MODE_RSS_TAB, hBitmapUnclick, lpDrawItemStruct->rcItem);
									}
								}
			}
				break; */
			default:
				break;
		}
		return FALSE;
}


int iHideTabWidth = 0;
int iHideTabHeight = 0;
//隐藏tab界面的窗口过程函数。
BOOL CALLBACK HideMainTab_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		//设置初始化位置。
		RECT mainWindowRect = *(RECT*)lParam;
		RECT hideTabWindowRect;//用来保存屏幕坐标。
		GetClientRect(hwnd, &hideTabWindowRect);
		iHideTabHeight = hideTabWindowRect.right;
		iHideTabWidth = hideTabWindowRect.bottom;
		//隐藏Tab的窗口实际应该在的位置，屏幕坐标。
		POINT hideTabWindowPoint;
		hideTabWindowPoint.x = mainWindowRect.left;
		hideTabWindowPoint.y = mainWindowRect.top + 195;
		SetWindowPos(hwnd, NULL, hideTabWindowPoint.x, hideTabWindowPoint.y, iHideTabWidth, iHideTabHeight,
			SWP_NOSIZE | SWP_NOZORDER);
	}
		return TRUE;
	case WM_CTLCOLORDLG:
	{
		HDC hHideTabDC = (HDC)wParam;
		HWND hHideTabWnd = (HWND)lParam;
		SetBkMode(hHideTabDC, TRANSPARENT);
	}
		return (BOOL)hMainBkgndBrush;
	case WM_MAINDLGMOVE_HideTab:
	{
		int iXPiexl = (int)wParam;
		int iYPiexl = (int)lParam;
		//接着设置窗口的位置。
		SetWindowPos(hwnd, NULL, iXPiexl, iYPiexl + 195, iHideTabWidth, iHideTabHeight, SWP_NOSIZE | SWP_NOZORDER);
	}
		return TRUE;
	default:
		return FALSE;
	}
	return FALSE;
}


