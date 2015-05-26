/*
*文件描述：Rss阅读器界面的实现
*/

#include"stdafx.h"
#include"RssTabDlg.h"


vector<string> RssFeedList;//保存父结点。
vector<string> RssFeedLinks;//保存RssFeed的链接信息。
vector< map<string, RssContent> > RssItemList;//保存Rss下每一个子节点的内容。
HBRUSH hRssTabBkgndBrush = NULL;
HFONT hRssFeedFont = NULL;//RssFeed的字体。
HFONT hRssItemFont = NULL;//RssItem的字体。
HFONT hRssItemDescribeFont = NULL;//详细内容的字体。

BOOL CALLBACK MainDlg_RSS_Proc(HWND hwnd, UINT uMeg, WPARAM wParam, LPARAM lParam)
{
	switch (uMeg)
	{
	case WM_INITDIALOG:
	{
		//初始化RssMessage，重要。
		RssMessage::InitRssMessage(hwnd, IDC_RssFeedList, IDC_RssItemList);
		HBITMAP hRssTabBkgndBmp = (HBITMAP)LoadImage((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
			MAKEINTRESOURCE(IDB_RssTab_Bkgnd), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_CREATEDIBSECTION | LR_SHARED);
		RECT rssTabRect;
		GetClientRect(hwnd, &rssTabRect);
		hRssTabBkgndBrush = GetBkgndImage(hwnd, hRssTabBkgndBmp, rssTabRect);
		DeleteObject(hRssTabBkgndBmp);
		//创建RSSFeed的字体。
		hRssFeedFont = CreateRssMsgFont(hwnd, GB2312_CHARSET, CLIP_DEFAULT_PRECIS, TEXT("楷体"), 19, DEFAULT_QUALITY);
		hRssItemFont = CreateRssMsgFont(hwnd, GB2312_CHARSET, CLIP_DEFAULT_PRECIS, TEXT("楷体"), 17, DEFAULT_QUALITY);
		hRssItemDescribeFont = CreateRssMsgFont(hwnd, GB2312_CHARSET, CLIP_DEFAULT_PRECIS, TEXT("楷体"), 15, DEFAULT_QUALITY);
		//初始化Rss显示列表。
		InitRssList(hwnd);
		//初始化Rss阅读器的数据信息。
		InitRssMsg(hwnd);
		//初始化控件tip消息提示。
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
	case WM_SEARCHITEMMSG://收到搜索框输入的消息。
	{//返回在搜索框输入的文字信息。wParam是字符串的头指针。lParam是该消息要返回给的控件的ID。
		switch (lParam)//看一下该消息是要发给哪一个控件。
		{
		case IDC_AddRssFeedBtn:
		{
			RssMessage::SetRssFeedSearchWnd(NULL);//执行搜索操作的时候搜索窗口已关闭。
			//那么执行抓取RssFeed的操作。
			TCHAR szRssFeedLink[512] = { NULL };
			memset(szRssFeedLink, 0, sizeof(szRssFeedLink));
			lstrcpy(szRssFeedLink, (TCHAR*)wParam);
			GetRssMsgFromLink(hwnd, szRssFeedLink);
		}
			return TRUE;
		case IDC_SearchRssItemBtn:
		{
			TCHAR szRssItemTitle[512] = { NULL };//就算这样还是会有乱码。
			memset(szRssItemTitle, 0, sizeof(szRssItemTitle));//还是会有乱码……
			lstrcpy(szRssItemTitle, (TCHAR*)wParam);
			RssMessage::SetRssItemSearchWnd(NULL);
			//接下来进行查找操作。
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
		case IDC_AddRssFeedBtn://抓取RssFeed的按钮。
		case ID_RSSFEED_AddItem://RssFeed右键菜单上的的选项。
		{
			//改按钮被点击，弹出搜索对话框。
			if (RssMessage::GetRssFeedSearchWnd() == NULL)//搜索对话框没有弹出，弹出搜索对话框。
			{
				//首先获得按钮的屏幕坐标。
				RECT rssSearchBtnRect;
				GetWindowRect(GetDlgItem(hwnd, IDC_AddRssFeedBtn), &rssSearchBtnRect);
				HWND hRssSearchMsgWnd = CreateDialogParam((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
					MAKEINTRESOURCE(IDD_RssSearchMsgDlg), hwnd, RssSearchMsgDlg_Proc, (LPARAM)&rssSearchBtnRect);
				RssMessage::SetRssFeedSearchWnd(hRssSearchMsgWnd);
				//给窗口绑定父窗口的句柄数据。
				SetWindowLong(hRssSearchMsgWnd, GWL_USERDATA, (LONG)hwnd);
			}
			else//搜索按钮正在打开那么将其关闭
			{
				DestroyWindow(RssMessage::GetRssFeedSearchWnd());
				RssMessage::SetRssFeedSearchWnd(NULL);//设置全局变量。
			}
		}
			return TRUE;
		case ID_RSSFEED_DeleteItem://RssFeedList右键菜单选项。
		{
			DeleteRssFeedFromList(hwnd);
		}
			return TRUE;
		case ID_RSSITEM_DeleteItem://RssItemList右键菜单选项。
		{
			DeleteRssItemFromList(hwnd);
		}
			return TRUE;
		case IDC_SearchRssItemBtn://查询RssItem的按钮。
		{
			//改按钮被点击，弹出搜索对话框。
			if (RssMessage::GetRssItemSearchWnd() == NULL)//搜索对话框没有弹出，弹出搜索对话框。
			{
				//首先获得按钮的屏幕坐标。
				RECT rssSearchBtnRect;
				GetWindowRect(GetDlgItem(hwnd, IDC_SearchRssItemBtn), &rssSearchBtnRect);
				HWND hRssSearchMsgWnd = CreateDialogParam((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
					MAKEINTRESOURCE(IDD_SearchRssItemDlg), hwnd, SearchRssItemDlg_Proc, (LPARAM)&rssSearchBtnRect);
				RssMessage::SetRssItemSearchWnd(hRssSearchMsgWnd);
				//给窗口绑定父窗口的句柄数据。
				SetWindowLong(hRssSearchMsgWnd, GWL_USERDATA, (LONG)hwnd);
			}
			else//搜索按钮正在打开那么将其关闭
			{
				DestroyWindow(RssMessage::GetRssItemSearchWnd());
				RssMessage::SetRssItemSearchWnd(NULL);//设置全局变量。
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
			//用浏览器打开新闻的详情页。
			ShellExecute(hwnd, TEXT("open"), szRssItemDesLink, NULL, NULL, SW_SHOW);
		}
			return TRUE;
		case IDC_RssFeedList:
		{
			switch (HIWORD(wParam))//监控事件
			{
			case LBN_DBLCLK:
			{
				//将选中的RssFeed显示到RssItem的列表中。
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
				//将选中的RssItem相关详情显示到详情控件中。
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
		{//case在这里三个都无效。？？
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
		int iXcoordinate = (int)LOWORD(lParam);//这两个都是点击时的屏幕坐标。
		int iYcoordinate = (int)HIWORD(lParam);
		//看是来自哪个控件的右键点击操作。
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
				MessageBox(hwnd, TEXT("在获取音乐列表的每一项的高度时发生错误！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
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
				MessageBox(hwnd, TEXT("在获取音乐列表的每一项的高度时发生错误！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
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