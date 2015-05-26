/*
*ѧ�ţ�1252878
*��������
*��ϵ�绰��18817870106
*���ʵ�ַ��1252878xujian@tongji.edu.cn
*�ļ������������ڹ���ʵ�֣�Rss��Musicplayer�����л���ʵ�֣�
*/

#include"stdafx.h"
#include"MainDlg.h"


bool bIsAppFirseUse = false;//����û��Ƿ��һ��ʹ�ø������true�ǵ�һ��ʹ�á�
HWND hMainDlgTab[2];//����������Tabѡ������������
HWND hwndStatus = NULL;
HBRUSH hMainBkgndBrush = NULL;//������ı�����ˢ��
char* SystemDefaultLocale = NULL;//����ϵͳԭ���ĵ�����Ϣ��
NOTIFYICONDATA notifyicondata;//ϵͳ������Ϣ��
HWND hHideTabDlgWnd = NULL;//����Tab���ڵľ����


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
				bIsAppFirseUse = InitMyRssPlayer(hwnd);//��ʼ��MyRssPlayer�������ʾ��ӭ���ڵȡ�
				LoadTabFrom(hwnd,hMainDlgTab);
				HBITMAP hBitmap = (HBITMAP)LoadImage((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
					MAKEINTRESOURCE(IDB_MainBkgnd), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_SHARED | LR_DEFAULTSIZE);
				RECT clientRect;
				GetClientRect(hwnd, &clientRect);
				hMainBkgndBrush = GetBkgndImage(hwnd, hBitmap, clientRect);
				//����Tip��ʾ��Ϣ��
//				InitMainDlgCtrolTip(hwnd);
				//��ʾ״̬����
				hwndStatus = CreateStatusBar(hwnd);
				//����Ӧ�ó����ͼ�ꡣ
				HICON hIcon = LoadIcon((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDI_APPICON));
				SendMessage(hwnd, WM_SETICON, (WPARAM)TRUE, (LPARAM)hIcon);
				//���ϵͳԭ���ĵ�����Ϣ����Ҫ
				SystemDefaultLocale = _strdup(setlocale(LC_CTYPE, NULL));
				//�������õ������Ϣ�����õ�ǰ����ʹ�õı��ػ���Ϣ������fputsw������text��д�����ġ�
				setlocale(LC_CTYPE, "chs");
				//������������Tab�Ĵ��ڡ�
				//��������ڵ���Ļ���ꡣ
				RECT mainWindowRect;
				GetWindowRect(hwnd, &mainWindowRect);
				hHideTabDlgWnd = CreateDialogParam((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
					MAKEINTRESOURCE(IDD_HideMainTab), hwnd, HideMainTab_Proc, (LPARAM)&mainWindowRect);

			}
				break;
			case WM_LBUTTONDOWN:
			{
				//ʹ������µ�ʱ�򴰿��ܹ��ƶ���
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
						//������������ʾ����
						Shell_NotifyIcon(NIM_DELETE, &notifyicondata);
						ShowWindow(hwnd, SW_NORMAL);
						SetForegroundWindow(hwnd);//���ô�������ǰ����ʾ��
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
							cursorScreenPoint.y, hwnd, NULL);//�����Ҽ��˵���
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
				if (MusicMessage::GetMusicVolumeWnd())//��ǰ��������������ʾ״̬��
				{
					//�ҵ����ֲ�����������ť��λ�á�
					RECT volBtnScreenRect;
					GetWindowRect(GetDlgItem(MusicMessage::GetMusicWindowHwnd(), IDC_MusicVolume), &volBtnScreenRect);
					//���������ť��λ��֮�󣬽�λ����Ϣ���͸��Ӵ��ڡ�
					SendMessage(MusicMessage::GetMusicVolumeWnd(), WM_MAINDLGMOVE_VolDlg,
					(WPARAM)volBtnScreenRect.right, (LPARAM)volBtnScreenRect.top + 7);
				}
				if (MusicMessage::GetSearchMusicItemWnd())//�������ڴ��ڵĻ���
				{
					RECT searchItemBtnScreenRect;
					GetWindowRect(GetDlgItem(MusicMessage::GetMusicWindowHwnd(), IDC_SearchMusicItem),
						&searchItemBtnScreenRect);
					//����ť���½����괫��ȥ��
					SendMessage(MusicMessage::GetSearchMusicItemWnd(), WM_MAINDLGMOVE_SearchItemDlg,
						(WPARAM)searchItemBtnScreenRect.right, (LPARAM)searchItemBtnScreenRect.top);
				}
				if (RssMessage::GetRssFeedSearchWnd())//��ǰ����RssFeed�Ĵ�������ʾ��
				{
					RECT searchRssFeedBtnRect;
					GetWindowRect(GetDlgItem(hMainDlgTab[0], IDC_AddRssFeedBtn),
						&searchRssFeedBtnRect);
					//����ť�����Ͻ���Ļ���괫�ݹ�ȥ��
					SendMessage(RssMessage::GetRssFeedSearchWnd(), WM_MAINDLGMOVE_RssDlg, (WPARAM)searchRssFeedBtnRect.right,
						(LPARAM)searchRssFeedBtnRect.top);
				}
				if (RssMessage::GetRssItemSearchWnd())
				{
					RECT searchRssItemBtnRect;
					GetWindowRect(GetDlgItem(RssMessage::GetRssWindowHwnd(), IDC_SearchRssItemBtn),
						&searchRssItemBtnRect);
					//����ť�����Ͻ���Ļ���괫�ݹ�ȥ��
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
				case IDC_ToTrayBTN://������С����ϵͳ���̡�
				{
					AppToTray(hwnd, notifyicondata);
				}
					break;
				case ID_QuitApp://�����Ҽ��˵��˳��
				{
					//������ɾ����ʾ����
					Shell_NotifyIcon(NIM_DELETE, &notifyicondata);
					SaveMusicPlayerMessage(hwnd);
					SaveRssMsg(hwnd);
					//��ԭ�����趨,���fputws��txt�ļ���д�����ĵ����⡣
					setlocale(LC_CTYPE, SystemDefaultLocale);
					free(SystemDefaultLocale);
					EndDialog(hwnd, 0);
				}
					return TRUE;
				case IDC_MinMainDlgBTN://��С�����ڡ�
				{
					ShowWindow(hwnd, SW_MINIMIZE);
				}
					return TRUE;
				case IDC_AppSettingMenuBTN:
				{
					POINT point;//����û���ǰ��������ꡣ
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
			case WM_NOTIFY://��Tabҳ���л��йص���Ϣ��
			{
				//Sent by a common control to its parent window when an event has occurred or the control requires some information.
				TCHAR szToolMeg[2][20] = { TEXT("�ҵ�����"), TEXT("���ֹ�") };
				LPNMTTDISPINFO lpNmttdispInfo = (LPNMTTDISPINFO)lParam;
				LPNMHDR lpNmhdr = &(lpNmttdispInfo->hdr);
				if ((lpNmhdr->hwndFrom) == GetDlgItem(hwnd, IDC_MainDlg_Tab))
				{
					/*Notifies a tab control's parent window that the currently 
					selected tab has changed. This message is sent in the form of a WM_NOTIFY message. */
					if (lpNmhdr->code == TCN_SELCHANGE)
					{
						//�õ���ҳ��Tab�е�ǰѡ�����
						/*The SendDlgItemMessage function sends a message to the specified control in a dialog box.*/
						int curPageIndex = SendDlgItemMessage(hwnd, IDC_MainDlg_Tab, TCM_GETCURSEL, 0, 0);
						switch (curPageIndex)
						{
							case 0://��ǰ��Tabѡ����RSS���Ž��档
							{
								ShowWindow(hMainDlgTab[0], SW_SHOW);
								//��������ֽ�����Ӵ��ڴ��ڿ���״̬����ô�ر����ǡ�
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
							case 1://��ǰ��Tabѡ�������ֲ��������档
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
				//��ԭ�����趨,���fputws��txt�ļ���д�����ĵ����⡣
				setlocale(LC_CTYPE, SystemDefaultLocale);
				free(SystemDefaultLocale);
				EndDialog(hwnd, 0);
			}
				break;
/*			case WM_DRAWITEM:
			{
								/*http://msdn.microsoft.com/en-us/library/windows/desktop/bb760550(v=vs.85).aspx#owner_drawn_tabs */
/*								LPDRAWITEMSTRUCT lpDrawItemStruct = (LPDRAWITEMSTRUCT)lParam;
								if (lpDrawItemStruct->CtlType == ODT_TAB && lpDrawItemStruct->CtlID == IDC_MainDlg_Tab)//��Tab���Ի���Ϣ��
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
									//��ʼ��ͼ��
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
//����tab����Ĵ��ڹ��̺�����
BOOL CALLBACK HideMainTab_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		//���ó�ʼ��λ�á�
		RECT mainWindowRect = *(RECT*)lParam;
		RECT hideTabWindowRect;//����������Ļ���ꡣ
		GetClientRect(hwnd, &hideTabWindowRect);
		iHideTabHeight = hideTabWindowRect.right;
		iHideTabWidth = hideTabWindowRect.bottom;
		//����Tab�Ĵ���ʵ��Ӧ���ڵ�λ�ã���Ļ���ꡣ
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
		//�������ô��ڵ�λ�á�
		SetWindowPos(hwnd, NULL, iXPiexl, iYPiexl + 195, iHideTabWidth, iHideTabHeight, SWP_NOSIZE | SWP_NOZORDER);
	}
		return TRUE;
	default:
		return FALSE;
	}
	return FALSE;
}


