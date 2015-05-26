/*
*�ļ�������Rss�Ķ��������ʵ��
*/

#include"stdafx.h"
#include"RssTabDlg.h"


vector<string> RssFeedList;//���游��㡣
vector<string> RssFeedLinks;//����RssFeed��������Ϣ��
vector< map<string, RssContent> > RssItemList;//����Rss��ÿһ���ӽڵ�����ݡ�
HBRUSH hRssTabBkgndBrush = NULL;
HFONT hRssFeedFont = NULL;//RssFeed�����塣
HFONT hRssItemFont = NULL;//RssItem�����塣
HFONT hRssItemDescribeFont = NULL;//��ϸ���ݵ����塣

BOOL CALLBACK MainDlg_RSS_Proc(HWND hwnd, UINT uMeg, WPARAM wParam, LPARAM lParam)
{
	switch (uMeg)
	{
	case WM_INITDIALOG:
	{
		//��ʼ��RssMessage����Ҫ��
		RssMessage::InitRssMessage(hwnd, IDC_RssFeedList, IDC_RssItemList);
		HBITMAP hRssTabBkgndBmp = (HBITMAP)LoadImage((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			MAKEINTRESOURCE(IDB_RssTab_Bkgnd), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_CREATEDIBSECTION | LR_SHARED);
		RECT rssTabRect;
		GetClientRect(hwnd, &rssTabRect);
		hRssTabBkgndBrush = GetBkgndImage(hwnd, hRssTabBkgndBmp, rssTabRect);
		DeleteObject(hRssTabBkgndBmp);
		//����RSSFeed�����塣
		hRssFeedFont = CreateRssMsgFont(hwnd, GB2312_CHARSET, CLIP_DEFAULT_PRECIS, TEXT("����"), 19, DEFAULT_QUALITY);
		hRssItemFont = CreateRssMsgFont(hwnd, GB2312_CHARSET, CLIP_DEFAULT_PRECIS, TEXT("����"), 17, DEFAULT_QUALITY);
		hRssItemDescribeFont = CreateRssMsgFont(hwnd, GB2312_CHARSET, CLIP_DEFAULT_PRECIS, TEXT("����"), 15, DEFAULT_QUALITY);
		//��ʼ��Rss��ʾ�б�
		InitRssList(hwnd);
		//��ʼ��Rss�Ķ�����������Ϣ��
		InitRssMsg(hwnd);
		//��ʼ���ؼ�tip��Ϣ��ʾ��
		InitRssTabDlgCtrolTip(hwnd);
	}
		return TRUE;
	case WM_CTLCOLORDLG:
	{
		HDC hRssDlgDC = (HDC)wParam;
		HWND hRssDlgWnd = (HWND)lParam;
		SetBkMode(hRssDlgDC, TRANSPARENT);
	}
		return (BOOL)hRssTabBkgndBrush;
	case WM_LBUTTONDOWN:
	{
		if (RssMessage::GetRssFeedSearchWnd() != NULL)
		{
			DestroyWindow(RssMessage::GetRssFeedSearchWnd());
			RssMessage::SetRssFeedSearchWnd(NULL);
		}
		if (RssMessage::GetRssItemSearchWnd() != NULL)
		{
			DestroyWindow(RssMessage::GetRssItemSearchWnd());
			RssMessage::SetRssItemSearchWnd(NULL);
		}
	}
		return TRUE;
	case WM_SEARCHITEMMSG://�յ��������������Ϣ��
	{//�����������������������Ϣ��wParam���ַ�����ͷָ�롣lParam�Ǹ���ϢҪ���ظ��Ŀؼ���ID��
		switch (lParam)//��һ�¸���Ϣ��Ҫ������һ���ؼ���
		{
		case IDC_AddRssFeedBtn:
		{
			RssMessage::SetRssFeedSearchWnd(NULL);//ִ������������ʱ�����������ѹرա�
			//��ôִ��ץȡRssFeed�Ĳ�����
			TCHAR szRssFeedLink[512] = { NULL };
			memset(szRssFeedLink, 0, sizeof(szRssFeedLink));
			lstrcpy(szRssFeedLink, (TCHAR*)wParam);
			GetRssMsgFromLink(hwnd, szRssFeedLink);
		}
			return TRUE;
		case IDC_SearchRssItemBtn:
		{
			TCHAR szRssItemTitle[512] = { NULL };//�����������ǻ������롣
			memset(szRssItemTitle, 0, sizeof(szRssItemTitle));//���ǻ������롭��
			lstrcpy(szRssItemTitle, (TCHAR*)wParam);
			RssMessage::SetRssItemSearchWnd(NULL);
			//���������в��Ҳ�����
			RssMessage::FindRssItemFromList(szRssItemTitle);
		}
			return TRUE;
		default:
			return FALSE;
		}
	}
		return TRUE;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_AddRssFeedBtn://ץȡRssFeed�İ�ť��
		case ID_RSSFEED_AddItem://RssFeed�Ҽ��˵��ϵĵ�ѡ�
		{
			//�İ�ť����������������Ի���
			if (RssMessage::GetRssFeedSearchWnd() == NULL)//�����Ի���û�е��������������Ի���
			{
				//���Ȼ�ð�ť����Ļ���ꡣ
				RECT rssSearchBtnRect;
				GetWindowRect(GetDlgItem(hwnd, IDC_AddRssFeedBtn), &rssSearchBtnRect);
				HWND hRssSearchMsgWnd = CreateDialogParam((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
					MAKEINTRESOURCE(IDD_RssSearchMsgDlg), hwnd, RssSearchMsgDlg_Proc, (LPARAM)&rssSearchBtnRect);
				RssMessage::SetRssFeedSearchWnd(hRssSearchMsgWnd);
				//�����ڰ󶨸����ڵľ�����ݡ�
				SetWindowLong(hRssSearchMsgWnd, GWL_USERDATA, (LONG)hwnd);
			}
			else//������ť���ڴ���ô����ر�
			{
				DestroyWindow(RssMessage::GetRssFeedSearchWnd());
				RssMessage::SetRssFeedSearchWnd(NULL);//����ȫ�ֱ�����
			}
		}
			return TRUE;
		case ID_RSSFEED_DeleteItem://RssFeedList�Ҽ��˵�ѡ�
		{
			DeleteRssFeedFromList(hwnd);
		}
			return TRUE;
		case ID_RSSITEM_DeleteItem://RssItemList�Ҽ��˵�ѡ�
		{
			DeleteRssItemFromList(hwnd);
		}
			return TRUE;
		case IDC_SearchRssItemBtn://��ѯRssItem�İ�ť��
		{
			//�İ�ť����������������Ի���
			if (RssMessage::GetRssItemSearchWnd() == NULL)//�����Ի���û�е��������������Ի���
			{
				//���Ȼ�ð�ť����Ļ���ꡣ
				RECT rssSearchBtnRect;
				GetWindowRect(GetDlgItem(hwnd, IDC_SearchRssItemBtn), &rssSearchBtnRect);
				HWND hRssSearchMsgWnd = CreateDialogParam((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
					MAKEINTRESOURCE(IDD_SearchRssItemDlg), hwnd, SearchRssItemDlg_Proc, (LPARAM)&rssSearchBtnRect);
				RssMessage::SetRssItemSearchWnd(hRssSearchMsgWnd);
				//�����ڰ󶨸����ڵľ�����ݡ�
				SetWindowLong(hRssSearchMsgWnd, GWL_USERDATA, (LONG)hwnd);
			}
			else//������ť���ڴ���ô����ر�
			{
				DestroyWindow(RssMessage::GetRssItemSearchWnd());
				RssMessage::SetRssItemSearchWnd(NULL);//����ȫ�ֱ�����
			}
		}
			return TRUE;
		case ID_RSSFEED_Refresh:
		{
			RefreshReeFeedInList(hwnd);
		}
			return TRUE;
		case IDC_RssSeeDescriptionBTN:
		{
			TCHAR szRssItemDesLink[500] = { NULL };
			GetDlgItemText(hwnd, IDC_RssItemDesLinkEdit, szRssItemDesLink, sizeof(szRssItemDesLink));
			if (lstrlen(szRssItemDesLink) == 0)
			{
				return TRUE;
			}
			//������������ŵ�����ҳ��
			ShellExecute(hwnd, TEXT("open"), szRssItemDesLink, NULL, NULL, SW_SHOW);
		}
			return TRUE;
		case IDC_RssFeedList:
		{
			switch (HIWORD(wParam))//����¼�
			{
			case LBN_DBLCLK:
			{
				//��ѡ�е�RssFeed��ʾ��RssItem���б��С�
				int iRssFeedIndex = SendMessage((HWND)lParam, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
				if (iRssFeedIndex == LB_ERR)
				{
					break;
				}
				RssMessage::SetCurRssFeedIndex(iRssFeedIndex);
				DisplayRssFeedToRssItemList(hwnd, iRssFeedIndex);
			}
				return TRUE;
			default:
				return FALSE;
			}
		}
			return TRUE;
		case IDC_RssItemList:
		{
			switch (HIWORD(wParam))
			{
			case LBN_DBLCLK:
			{
				//��ѡ�е�RssItem���������ʾ������ؼ��С�
				int iRssItemIndex = SendMessage((HWND)lParam, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
				if (iRssItemIndex == LB_ERR)
				{
					break;
				}
				DisplayRssItemMsg(hwnd, iRssItemIndex);
				RssMessage::SetCurRssItemIndex(iRssItemIndex);
			}
			default:
				return FALSE;
			}
		}
			return TRUE;
		default:
			return FALSE;
		}
		return TRUE;
	}
		return FALSE;
	case WM_CTLCOLORLISTBOX:
	{
		HDC hRssListDC = (HDC)wParam;
		HWND hRssListWnd = (HWND)lParam;
		SetBkMode(hRssListDC, TRANSPARENT);
		switch (GetDlgCtrlID(hRssListWnd))
		{
		case IDC_RssFeedList:
		{
			SetTextColor(hRssListDC, RGB(255, 0, 0));
			SendMessage(hRssListWnd, WM_SETFONT, (WPARAM)hRssFeedFont, (LPARAM)0);
		}
			return (BOOL)hRssTabBkgndBrush;
		case IDC_RssItemList:
		{
			SetTextColor(hRssListDC, RGB(0, 255, 0));
			SendMessage(hRssListWnd, WM_SETFONT, (WPARAM)hRssItemFont, (LPARAM)0);
		}
			return (BOOL)hRssTabBkgndBrush;
		default:
			return FALSE;
		}
	}
		return FALSE;
/*An edit control that is not read-only or disabled
sends the WM_CTLCOLOREDIT message to its parent window when the control is about to be drawn.*/
/*A static control, or an edit control that is read-only or disabled,
sends the WM_CTLCOLORSTATIC message to its parent window when the control is about to be drawn.*/
	case WM_CTLCOLORSTATIC:
	{
		HDC hStaticDC = (HDC)wParam;
		HWND hStaticWnd = (HWND)lParam;
		SetBkMode(hStaticDC, TRANSPARENT);
		switch (GetDlgCtrlID(hStaticWnd))
		{//case��������������Ч������
		case IDC_RssDescriptionEdit:
		{
			SetTextColor(hStaticDC, RGB(0, 0, 0));
		}
			return (BOOL)hRssTabBkgndBrush;
		default:
			return (BOOL)hRssTabBkgndBrush;
		}
	}
		return FALSE;
	case WM_CONTEXTMENU:
	{
		HWND hClickWnd = (HWND)wParam;
		int iXcoordinate = (int)LOWORD(lParam);//���������ǵ��ʱ����Ļ���ꡣ
		int iYcoordinate = (int)HIWORD(lParam);
		//���������ĸ��ؼ����Ҽ����������
		switch (GetDlgCtrlID(hClickWnd))
		{
		case IDC_RssFeedList:
		{
			HWND hRssFeedListWnd = GetDlgItem(hwnd, IDC_RssFeedList);
			HMENU hMenu = LoadMenu((HINSTANCE)GetWindowLong(hwnd, GWLP_HINSTANCE), MAKEINTRESOURCE(IDR_RssFeed_Menu));
			hMenu = GetSubMenu(hMenu, 0);
			RECT rssFeedListScreenRect;
			GetWindowRect(hRssFeedListWnd, &rssFeedListScreenRect);
			int iTopIndex = SendMessage(hRssFeedListWnd, LB_GETTOPINDEX, (WPARAM)0, (LPARAM)0);
			int iRssFeedListItemHeight = SendMessage(hRssFeedListWnd, LB_GETITEMHEIGHT, (WPARAM)iTopIndex, (LPARAM)0);
			if (iRssFeedListItemHeight == LB_ERR)
			{
				MessageBox(hwnd, TEXT("�ڻ�ȡ�����б��ÿһ��ĸ߶�ʱ��������"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			}
			int iCurIndex = iTopIndex + (int)((iYcoordinate - rssFeedListScreenRect.top) / iRssFeedListItemHeight);
			SendMessage(hRssFeedListWnd, LB_SETCURSEL, (WPARAM)iCurIndex, (LPARAM)0);
			TrackPopupMenuEx(hMenu, TPM_LEFTALIGN, iXcoordinate, iYcoordinate, hwnd, NULL);
		}
			return TRUE;
/*		case IDC_RssItemList:
		{
			HWND hRssItemListWnd = GetDlgItem(hwnd, IDC_RssItemList);
			HMENU hMenu = LoadMenu((HINSTANCE)GetWindowLong(hwnd, GWL_USERDATA), MAKEINTRESOURCE(IDR_RssItem_Menu));
			hMenu = GetSubMenu(hMenu, 0);
			RECT rssItemListScreenRect;
			GetWindowRect(hRssItemListWnd, &rssItemListScreenRect);
			int iTopIndex = SendMessage(hRssItemListWnd, LB_GETTOPINDEX, (WPARAM)0, (LPARAM)0);
			int iRssItemListItemHeight = SendMessage(hRssItemListWnd, LB_GETITEMHEIGHT, (WPARAM)iTopIndex, (LPARAM)0);
			if (iRssItemListItemHeight == LB_ERR)
			{
				MessageBox(hwnd, TEXT("�ڻ�ȡ�����б��ÿһ��ĸ߶�ʱ��������"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			}
			int iCurIndex = iTopIndex + (int)((iYcoordinate - rssItemListScreenRect.top) / iRssItemListItemHeight);
			SendMessage(hRssItemListWnd, LB_SETCURSEL, (WPARAM)iCurIndex, (LPARAM)0);
			TrackPopupMenuEx(hMenu, TPM_LEFTALIGN, iXcoordinate, iYcoordinate, hwnd, NULL);
		}
			return TRUE;*/
		default:
			return FALSE;
		}
	}
		return TRUE;
	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT lpDrawItemStruct = (LPDRAWITEMSTRUCT)lParam;
		switch (lpDrawItemStruct->CtlType)
		{
		case ODT_LISTBOX:
		{
			switch (lpDrawItemStruct->CtlID)
			{
			case IDC_RssFeedList:
			{
				RedrawRssList(hwnd, wParam, lpDrawItemStruct, RGB(0, 0, 0), RGB(255, 0, 0));
			}
				return TRUE;
			case IDC_RssItemList:
			{
				RedrawRssList(hwnd, wParam, lpDrawItemStruct, RGB(255, 255, 255), RGB(255, 0, 0));
			}
				return TRUE;
			default:
				return FALSE;
			}
		}
			return FALSE;
		case ODT_BUTTON:
		{
			DrawBkgndToButton(hwnd, wParam, lpDrawItemStruct);
		}
			return TRUE;
		default:
			return FALSE;
		}
	}
		return TRUE;//
	case WM_CLOSE:
	{
		DeleteObject(hRssTabBkgndBrush);
		DeleteObject(hRssFeedFont);
		DeleteObject(hRssItemFont);
		DeleteObject(hRssItemDescribeFont);
		DestroyWindow(hwnd);
	}
		return FALSE;
	}
	return FALSE;
}