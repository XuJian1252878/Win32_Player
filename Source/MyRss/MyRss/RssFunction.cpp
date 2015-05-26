/*
*文件描述：Rss阅读器Rss条目添加、删除等相关逻辑的实现
*/

#include"stdafx.h"
#include"RssFunction.h"

extern vector<string> RssFeedList;//保存RssFeed。
extern vector<string> RssFeedLinks;//保存RssFeed的链接信息。
extern vector< map<string, RssContent> > RssItemList;//保存RssFeed对应的RssItem的内容。


//向该界面的控件提供tip提示消息。
bool InitRssTabDlgCtrolTip(HWND hwnd)
{
	int iControlID[3] = { IDC_AddRssFeedBtn, IDC_SearchRssItemBtn, IDC_RssSeeDescriptionBTN };
	TCHAR szControlTip[3][50] = { TEXT("添加RssFeed访问站点"), TEXT("搜索RssItem条目"), TEXT("至原网页查看新闻详情") };
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
	HWND hToolTipWnd = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
		0, 0, 0, 0, hwnd, NULL, hInstance, NULL);
	TOOLINFO toolinfo[3];
	ZeroMemory(toolinfo, sizeof(toolinfo));
	for (int index = 0; index < 3; index++)
	{
		toolinfo[index].cbSize = sizeof(TOOLINFO);
		toolinfo[index].hinst = hInstance;
		toolinfo[index].lpszText = szControlTip[index];
		toolinfo[index].uId = (UINT)GetDlgItem(hwnd, iControlID[index]);
		toolinfo[index].uFlags = TTF_IDISHWND | TTF_SUBCLASS;
		SendMessage(hwnd, TTM_ADDTOOL, (WPARAM)0, (LPARAM)&toolinfo[index]);
	}
	return true;
}

//清空RssList中的内容。
//HWND hwnd父窗口句柄, WPARAM rssListControlID列表ID
bool ClearRssListBlank(HWND hwnd, WPARAM rssListControlID)
{
	HWND hRssListWnd = GetDlgItem(hwnd, rssListControlID);
	//检查列表中的子条目的总数。
	int iListItemCount = SendMessage(hRssListWnd, LB_GETCOUNT, (WPARAM)0, (LPARAM)0);
	if (iListItemCount == 1)//当仅剩一项的时候，有可能是插入的空格。
	{
		TCHAR szTestMsg[500] = { NULL };
		SendMessage(hRssListWnd, LB_GETTEXT, (WPARAM)0, (LPARAM)szTestMsg);//选出第零项检查。
		if (lstrlen(szTestMsg) == 0)//列表项中只剩空格的时候。
		{
			//删去空格以便新的项插入。
			SendMessage(hRssListWnd, LB_DELETESTRING, (WPARAM)0, (LPARAM)0);
		}
	}
	return true;
}

//读取Rss文件的信息。
//传入参数 本地暂存文件的绝对路径。
bool ReadXmlFile(TCHAR* lpszTempRssXmlPath, string& rssFeedTitle, map<string, RssContent>& mXML)
{
	string Title;
	RssContent Con;
	memset(&Con, 0, sizeof(Con));
	char* cszTempRssXmlPath = FileEncode::UnicodeToAnsi((char*)lpszTempRssXmlPath);
	TiXmlDocument *myDocument = new TiXmlDocument(cszTempRssXmlPath);
	free(cszTempRssXmlPath);
	myDocument->LoadFile();
	TiXmlElement *RootElement = myDocument->RootElement();
	TiXmlElement *FirstElement = RootElement->FirstChildElement();
	TiXmlElement *MyElement = FirstElement->FirstChildElement();
	rssFeedTitle = MyElement->GetText();
	while (MyElement != NULL)
	{
		while ((MyElement->ValueTStr()) != "item")
		{
			MyElement = MyElement->NextSiblingElement();
		}
		TiXmlElement *MyElement_1 = MyElement->FirstChildElement();
		while (MyElement_1->ValueTStr() != "title")
		{
			MyElement_1 = MyElement_1->NextSiblingElement();
		}
		Title = MyElement_1->GetText();
		while (MyElement_1->ValueTStr() != "link")
		{
			MyElement_1 = MyElement_1->NextSiblingElement();
		}
		Con.Web = MyElement_1->GetText();
		while (MyElement_1->ValueTStr() != "pubDate")
		{
			MyElement_1 = MyElement_1->NextSiblingElement();
		}
		Con.Date = MyElement_1->GetText();
		while (MyElement_1->ValueTStr() != "description")
		{
			MyElement_1 = MyElement_1->NextSiblingElement();
		}
		Con.Description = MyElement_1->GetText();
		mXML.insert(pair<string, RssContent>(Title, Con));
		MyElement = MyElement->NextSiblingElement();
	}
	return true;
}

//刷新RssFeed列表里面确定的某一项。
//HWND 父窗口的句柄，刷新RssFeed列表里面的某一项。
bool RefreshReeFeedInList(HWND hwnd)
{
	HWND hRssFeedListWnd = GetDlgItem(hwnd, IDC_RssFeedList);
	int iCurRssFeedIndex = SendMessage(hRssFeedListWnd, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	if (LB_ERR == iCurRssFeedIndex)
	{
		MessageBox(hwnd, TEXT("您未选中要更新的RssFeed项！"), TEXT("Message"), MB_OK);
		return false;
	}
	//然后从RssFeedLink的列表里面取出对应的链接，开始刷新。
	TCHAR* szRssFeedLink = FileEncode::AnsiToUnicode((char*)(RssFeedLinks[iCurRssFeedIndex].c_str()));
	//先得到暂存文件的全路径，看全路径是否有问题。
	TCHAR szTempRssXmlPath[MAX_PATH] = { NULL };
	InitRssXMLTempSavePath(hwnd, szTempRssXmlPath);
	//开始下载xml文本信息。
	HRESULT hResult = URLDownloadToFile(NULL, szRssFeedLink, szTempRssXmlPath, 0, NULL);
	if (hResult != S_OK)
	{
		MessageBox(hwnd, TEXT("抓取RssFeed失败！请检查网络设置！！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return false;
	}
	//下载到本地暂存路径之后开始读入。
	string rssFeedTitle;
	map<string, RssContent> rssItemMap;//保存RssFeed中RssItem对应的信息。
	ReadXmlFile(szTempRssXmlPath, rssFeedTitle, rssItemMap);
	//在RssListItem对应的位置替换掉原来的map。
	RssItemList[iCurRssFeedIndex] = rssItemMap;
	return true;
}


//从链接中抓取Rss的XML数据，并向RssFeed列表中插入数据。
//并且得到RssFeed的名字加入RssFeedList中。
//将该RssFeed对应的RssItem 的map加入RssItemList中。
bool GetRssMsgFromLink(HWND hwnd, TCHAR* szRssFeedLink)
{
	//先得到暂存文件的全路径，看全路径是否有问题。
	TCHAR szTempRssXmlPath[MAX_PATH] = { NULL };
	InitRssXMLTempSavePath(hwnd, szTempRssXmlPath);
	//开始下载xml文本信息。
	HRESULT hResult = URLDownloadToFile(NULL, szRssFeedLink, szTempRssXmlPath, 0, NULL);
	if (hResult != S_OK)
	{
		MessageBox(hwnd, TEXT("抓取RssFeed失败！请检查网络设置！！"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return false;
	}
	//下载之后保存该RssFeed的链接。
	char* cszRssFeedLink = FileEncode::UnicodeToAnsi((char*)szRssFeedLink);
	RssFeedLinks.push_back(string(cszRssFeedLink));
	free(cszRssFeedLink);
	//下载到本地暂存路径之后开始读入。
	string rssFeedTitle;
	map<string, RssContent> rssItemMap;//保存RssFeed中RssItem对应的信息。
	ReadXmlFile(szTempRssXmlPath, rssFeedTitle, rssItemMap);
	//向RssFeedList中加入RssFeed的名字。
	RssFeedList.push_back(rssFeedTitle);
	//在向RssFeed的列表中加入对应的项。
	//在此之前先检查空行的存在。
	ClearRssListBlank(hwnd, IDC_RssFeedList);
	TCHAR* szRssFeedTitle = FileEncode::UTF8ToUnicode((char*)rssFeedTitle.c_str());
	SendMessage(GetDlgItem(hwnd, IDC_RssFeedList), LB_INSERTSTRING, (WPARAM)-1, (LPARAM)szRssFeedTitle);
	free(szRssFeedTitle);
	//向RssItemList中加入该RssItem对应的信息。
	RssItemList.push_back(rssItemMap);
	return true;
}




//重画RssFeedList，HWND父窗口句柄，WPARAM 控件的ID。
bool RedrawRssList(HWND hwnd, WPARAM iControlID, LPDRAWITEMSTRUCT lpDrawItemStruct,
	COLORREF notSelectItemRGB,COLORREF selectItemRGB)
{
	if (lpDrawItemStruct->itemID == -1)
	{
		return false;
	}
	SetBkMode(lpDrawItemStruct->hDC, TRANSPARENT);
/*	if ((lpDrawItemStruct->itemState&ODS_SELECTED) && (lpDrawItemStruct->itemAction&(ODA_DRAWENTIRE | ODA_SELECT)))
	{
//		DrawFocusRect(lpDrawItemStruct->hDC, &(lpDrawItemStruct->rcItem));
	}
	if (!(lpDrawItemStruct->itemState&ODS_SELECTED) && (lpDrawItemStruct->itemAction&(ODA_DRAWENTIRE | ODA_SELECT)))
	{
//		DrawFocusRect(lpDrawItemStruct->hDC, &(lpDrawItemStruct->rcItem));
	}*/
	if (lpDrawItemStruct->itemState&ODS_SELECTED)
	{
		SetTextColor(lpDrawItemStruct->hDC, selectItemRGB);//选中的颜色。
	}
	else if (!(lpDrawItemStruct->itemState&ODS_SELECTED))
	{
		SetTextColor(lpDrawItemStruct->hDC, notSelectItemRGB);
	}
	if (IDC_RssItemList == iControlID)//如果是显示详情的列表的话，加宽列表项的高度。
	{
		SendMessage(GetDlgItem(hwnd, iControlID), LB_SETITEMHEIGHT,
			(WPARAM)lpDrawItemStruct->itemID, (LPARAM)25);
	}
	TCHAR szRssMeg[MAX_RSSMSGLEN] = { NULL };
	SendMessage(GetDlgItem(hwnd, iControlID), LB_GETTEXT, (WPARAM)lpDrawItemStruct->itemID, (LPARAM)szRssMeg);
	DrawText(lpDrawItemStruct->hDC, szRssMeg, lstrlen(szRssMeg), &(lpDrawItemStruct->rcItem), DT_LEFT | DT_SINGLELINE);
	return true;
}


//创建Rss内容的相关字体。
HFONT CreateRssMsgFont(HWND hwnd, BYTE lfCharSet, BYTE lfClipPrecision, TCHAR* lfFaceName, LONG lfHeight, BYTE lfQuality)
{
	LOGFONT logfont;
	ZeroMemory(&logfont, sizeof(logfont));
	logfont.lfCharSet = lfCharSet;
	logfont.lfClipPrecision = lfClipPrecision;
	lstrcpy(logfont.lfFaceName, lfFaceName);
	logfont.lfHeight = lfHeight;
	logfont.lfQuality = lfQuality;
	HFONT hFont = CreateFontIndirect(&logfont);
	return hFont;
}


//初始化RssList的状态，使其背景能与主界面的背景一样。
//HWND Rss主界面的背景。
bool InitRssList(HWND hwnd)
{
	SendMessage(GetDlgItem(hwnd, IDC_RssFeedList), LB_INSERTSTRING, (WPARAM)-1, (LPARAM)TEXT(""));
	SendMessage(GetDlgItem(hwnd, IDC_RssItemList), LB_INSERTSTRING, (WPARAM)-1, (LPARAM)TEXT(""));
	return true;
}


//将选中的RssFeed向显示到RssItem的列表中。
//hwnd父窗口的句柄。iRssFeedIndex在RssFeed列表中的RssFeed的下标。
bool DisplayRssFeedToRssItemList(HWND hwnd, int iRssFeedIndex)
{
	//首先清空RssItemList里面的全部数据。
	HWND hRssItemListWnd = GetDlgItem(hwnd, IDC_RssItemList);
	for (int iCount = SendMessage(hRssItemListWnd, LB_GETCOUNT, (WPARAM)0, (LPARAM)0);
		iCount >= 0; --iCount)
	{
		SendMessage(hRssItemListWnd, LB_DELETESTRING, (WPARAM)iCount, (LPARAM)0);
	}//	IDC_RssFeedTitleEdit; IDC_RssItemList
	//显示当前显示的RssFeed的名字。
	TCHAR* szRssFeedTitle = FileEncode::UTF8ToUnicode((char*)(RssFeedList[iRssFeedIndex].c_str()));
	SetDlgItemText(hwnd, IDC_RssFeedTitleEdit, szRssFeedTitle);
	free(szRssFeedTitle);
	//再把该RssFeed对应的RssItem加入到RssItem的列表中。
	map<string, RssContent> rssItemMap = RssItemList[iRssFeedIndex];
	int index = 0;//拼凑下标。
	for (map<string, RssContent>::iterator iterator = rssItemMap.begin();
		iterator != rssItemMap.end(); iterator++)
	{
		//获得宽字符串。
		TCHAR* szRssItemTitle = FileEncode::UTF8ToUnicode((char*)(iterator->first.c_str()));
		TCHAR szRssItemListTitle[500] = { NULL };
		wsprintf(szRssItemListTitle, TEXT("%d.  %s"), ++index, szRssItemTitle);
		SendMessage(hRssItemListWnd, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)szRssItemListTitle);
		free(szRssItemTitle);
	}
	return true;
}


//将选中的RssItem详情显示出来。
//hwnd父窗口的句柄。iRssItemIndex在RssItem列表中的RssItem的下标。
bool DisplayRssItemMsg(HWND hwnd, int iRssItemIndex)
{
	//得到当前选中的RssFeed的下标。
	int iRssFeedIndex = RssMessage::GetCurRssFeedIndex();
	map<string, RssContent>::iterator iterator = RssItemList[iRssFeedIndex].begin();//得到相对应RssItem的数据。
	//使迭代器移动到选中的下标处。
	for (int index = 0; index < iRssItemIndex; index++)
	{
		iterator++;
	}
	//向控件中写入数据。
	//IDC_RssItemDesLinkEdit  IDC_RssDescriptionEdit
	//写入详情。
	TCHAR* szRssItemDes = FileEncode::UTF8ToUnicode((char*)(iterator->second.Description.c_str()));
	TCHAR* szRssItemPubDate = FileEncode::UTF8ToUnicode((char*)(iterator->second.Date.c_str()));
	//做一些拼接。
	TCHAR szDescription[1000] = { NULL };
	wsprintf(szDescription, TEXT("发布日期：\r\n  %s\r\n  %s"), szRssItemPubDate, szRssItemDes);
	//显示详情内容。
	SetDlgItemText(hwnd, IDC_RssDescriptionEdit, szDescription);

	//写入全文的链接。
	TCHAR* szRssItemDesLink = FileEncode::UTF8ToUnicode((char*)(iterator->second.Web.c_str()));
	SetDlgItemText(hwnd, IDC_RssItemDesLinkEdit, szRssItemDesLink);
	free(szRssItemDesLink);
	free(szRssItemPubDate);
	free(szRssItemDes);
	return true;
}


//删除RssFeed列表里的一个选项。右键菜单，删除的是当前选中项。传入父窗口句柄。
//删除一个RssFeed的一切相关项 RssFeed的标题，链接，以及详细信息。
//extern vector<string> RssFeedList;//保存RssFeed。
//extern vector< map<string, RssContent> > RssItemList;//保存RssFeed对应的RssItem的内容。
bool DeleteRssFeedFromList(HWND hwnd)
{
	HWND hRssFeedListWnd = GetDlgItem(hwnd, IDC_RssFeedList);
	int iCurRssFeedIndex = SendMessage(hRssFeedListWnd, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	if (iCurRssFeedIndex == LB_ERR)
	{
		MessageBox(hwnd, TEXT("当前没有RssFeed项被选中！！"), TEXT("Message"), MB_OK);
		return false;
	}
	//删除RssFeed的对象。
	RssFeedList.erase(RssFeedList.begin() + iCurRssFeedIndex);
	//接下来删除对应的RssFeedLink信息。
	RssFeedLinks.erase(RssFeedLinks.begin() + iCurRssFeedIndex);
	//接下来删除对应的Map信息。
	RssItemList.erase(RssItemList.begin() + iCurRssFeedIndex);
	//清空RssFeed列表里面的对应项。
	for (int index = RssFeedList.size(); index >= iCurRssFeedIndex; index--)
	{
		SendMessage(hRssFeedListWnd, LB_DELETESTRING, (WPARAM)index, (LPARAM)0);
	}
	//重新插入之后的项。
	for (int index = iCurRssFeedIndex; index < RssFeedList.size(); index++)
	{
		TCHAR* szRssFeedTitle = FileEncode::UTF8ToUnicode((char*)(RssFeedList[index].c_str()));
		SendMessage(hRssFeedListWnd, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)szRssFeedTitle);
		free(szRssFeedTitle);
	}
	return true;
}


//删除RssItem列表里的一个选项，右键菜单，删除的是当前选中项。
bool DeleteRssItemFromList(HWND hwnd)
{
	HWND hRssItemListWnd = GetDlgItem(hwnd, IDC_RssItemList);
	int iCurRssItemIndex = SendMessage(hRssItemListWnd, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	if (iCurRssItemIndex == LB_ERR)
	{
		MessageBox(hwnd, TEXT("当前没有RssItem项被选中！！"), TEXT("Message"), MB_OK);
		return false;
	}
	//把列表中选中项以及之后的项全部删掉。
	for (int index = SendMessage(hRssItemListWnd, LB_GETCOUNT, (WPARAM)0, (LPARAM)0);
		index >= iCurRssItemIndex; --index)
	{
		SendMessage(hRssItemListWnd, LB_DELETESTRING, (WPARAM)index, (LPARAM)0);
	}
	//开始删除RssItemList相对应map的一对pair。
	int iCurRssFeedIndex = RssMessage::GetCurRssFeedIndex();
	//根据此找到对应的RssItem项。
	map<string, RssContent> rssItemMap = RssItemList[iCurRssFeedIndex];
	int index1 = 0;//记录下标。
	for (map<string, RssContent>::iterator iterator = rssItemMap.begin();
		iterator != rssItemMap.end(); iterator++)
	{
		if (index1 == iCurRssItemIndex)
		{
			rssItemMap.erase(iterator);//map的删除。
			break;
		}
		index1++;
	}
	
	int index2 = 0;
	for (map<string, RssContent>::iterator iterator = rssItemMap.begin();
		iterator != rssItemMap.end(); iterator++)
	{
		if (index2 >= iCurRssItemIndex)
		{
			TCHAR szRssItemMsg[500] = { NULL };
			TCHAR* szRssItemTitle = FileEncode::UTF8ToUnicode((char*)(iterator->first.c_str()));
			wsprintf(szRssItemMsg, TEXT("%d.  %s"), index2 + 1, szRssItemTitle);
			SendMessage(hRssItemListWnd, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)szRssItemMsg);
			free(szRssItemTitle);
		}
		index2 ++ ;
	}
	return true;
}