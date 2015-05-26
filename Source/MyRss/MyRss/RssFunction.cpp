/*
*�ļ�������Rss�Ķ���Rss��Ŀ��ӡ�ɾ��������߼���ʵ��
*/

#include"stdafx.h"
#include"RssFunction.h"

extern vector<string> RssFeedList;//����RssFeed��
extern vector<string> RssFeedLinks;//����RssFeed��������Ϣ��
extern vector< map<string, RssContent> > RssItemList;//����RssFeed��Ӧ��RssItem�����ݡ�


//��ý���Ŀؼ��ṩtip��ʾ��Ϣ��
bool InitRssTabDlgCtrolTip(HWND hwnd)
{
	int iControlID[3] = { IDC_AddRssFeedBtn, IDC_SearchRssItemBtn, IDC_RssSeeDescriptionBTN };
	TCHAR szControlTip[3][50] = { TEXT("���RssFeed����վ��"), TEXT("����RssItem��Ŀ"), TEXT("��ԭ��ҳ�鿴��������") };
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

//���RssList�е����ݡ�
//HWND hwnd�����ھ��, WPARAM rssListControlID�б�ID
bool ClearRssListBlank(HWND hwnd, WPARAM rssListControlID)
{
	HWND hRssListWnd = GetDlgItem(hwnd, rssListControlID);
	//����б��е�����Ŀ��������
	int iListItemCount = SendMessage(hRssListWnd, LB_GETCOUNT, (WPARAM)0, (LPARAM)0);
	if (iListItemCount == 1)//����ʣһ���ʱ���п����ǲ���Ŀո�
	{
		TCHAR szTestMsg[500] = { NULL };
		SendMessage(hRssListWnd, LB_GETTEXT, (WPARAM)0, (LPARAM)szTestMsg);//ѡ���������顣
		if (lstrlen(szTestMsg) == 0)//�б�����ֻʣ�ո��ʱ��
		{
			//ɾȥ�ո��Ա��µ�����롣
			SendMessage(hRssListWnd, LB_DELETESTRING, (WPARAM)0, (LPARAM)0);
		}
	}
	return true;
}

//��ȡRss�ļ�����Ϣ��
//������� �����ݴ��ļ��ľ���·����
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

//ˢ��RssFeed�б�����ȷ����ĳһ�
//HWND �����ڵľ����ˢ��RssFeed�б������ĳһ�
bool RefreshReeFeedInList(HWND hwnd)
{
	HWND hRssFeedListWnd = GetDlgItem(hwnd, IDC_RssFeedList);
	int iCurRssFeedIndex = SendMessage(hRssFeedListWnd, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	if (LB_ERR == iCurRssFeedIndex)
	{
		MessageBox(hwnd, TEXT("��δѡ��Ҫ���µ�RssFeed�"), TEXT("Message"), MB_OK);
		return false;
	}
	//Ȼ���RssFeedLink���б�����ȡ����Ӧ�����ӣ���ʼˢ�¡�
	TCHAR* szRssFeedLink = FileEncode::AnsiToUnicode((char*)(RssFeedLinks[iCurRssFeedIndex].c_str()));
	//�ȵõ��ݴ��ļ���ȫ·������ȫ·���Ƿ������⡣
	TCHAR szTempRssXmlPath[MAX_PATH] = { NULL };
	InitRssXMLTempSavePath(hwnd, szTempRssXmlPath);
	//��ʼ����xml�ı���Ϣ��
	HRESULT hResult = URLDownloadToFile(NULL, szRssFeedLink, szTempRssXmlPath, 0, NULL);
	if (hResult != S_OK)
	{
		MessageBox(hwnd, TEXT("ץȡRssFeedʧ�ܣ������������ã���"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return false;
	}
	//���ص������ݴ�·��֮��ʼ���롣
	string rssFeedTitle;
	map<string, RssContent> rssItemMap;//����RssFeed��RssItem��Ӧ����Ϣ��
	ReadXmlFile(szTempRssXmlPath, rssFeedTitle, rssItemMap);
	//��RssListItem��Ӧ��λ���滻��ԭ����map��
	RssItemList[iCurRssFeedIndex] = rssItemMap;
	return true;
}


//��������ץȡRss��XML���ݣ�����RssFeed�б��в������ݡ�
//���ҵõ�RssFeed�����ּ���RssFeedList�С�
//����RssFeed��Ӧ��RssItem ��map����RssItemList�С�
bool GetRssMsgFromLink(HWND hwnd, TCHAR* szRssFeedLink)
{
	//�ȵõ��ݴ��ļ���ȫ·������ȫ·���Ƿ������⡣
	TCHAR szTempRssXmlPath[MAX_PATH] = { NULL };
	InitRssXMLTempSavePath(hwnd, szTempRssXmlPath);
	//��ʼ����xml�ı���Ϣ��
	HRESULT hResult = URLDownloadToFile(NULL, szRssFeedLink, szTempRssXmlPath, 0, NULL);
	if (hResult != S_OK)
	{
		MessageBox(hwnd, TEXT("ץȡRssFeedʧ�ܣ������������ã���"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
		return false;
	}
	//����֮�󱣴��RssFeed�����ӡ�
	char* cszRssFeedLink = FileEncode::UnicodeToAnsi((char*)szRssFeedLink);
	RssFeedLinks.push_back(string(cszRssFeedLink));
	free(cszRssFeedLink);
	//���ص������ݴ�·��֮��ʼ���롣
	string rssFeedTitle;
	map<string, RssContent> rssItemMap;//����RssFeed��RssItem��Ӧ����Ϣ��
	ReadXmlFile(szTempRssXmlPath, rssFeedTitle, rssItemMap);
	//��RssFeedList�м���RssFeed�����֡�
	RssFeedList.push_back(rssFeedTitle);
	//����RssFeed���б��м����Ӧ���
	//�ڴ�֮ǰ�ȼ����еĴ��ڡ�
	ClearRssListBlank(hwnd, IDC_RssFeedList);
	TCHAR* szRssFeedTitle = FileEncode::UTF8ToUnicode((char*)rssFeedTitle.c_str());
	SendMessage(GetDlgItem(hwnd, IDC_RssFeedList), LB_INSERTSTRING, (WPARAM)-1, (LPARAM)szRssFeedTitle);
	free(szRssFeedTitle);
	//��RssItemList�м����RssItem��Ӧ����Ϣ��
	RssItemList.push_back(rssItemMap);
	return true;
}




//�ػ�RssFeedList��HWND�����ھ����WPARAM �ؼ���ID��
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
		SetTextColor(lpDrawItemStruct->hDC, selectItemRGB);//ѡ�е���ɫ��
	}
	else if (!(lpDrawItemStruct->itemState&ODS_SELECTED))
	{
		SetTextColor(lpDrawItemStruct->hDC, notSelectItemRGB);
	}
	if (IDC_RssItemList == iControlID)//�������ʾ������б�Ļ����ӿ��б���ĸ߶ȡ�
	{
		SendMessage(GetDlgItem(hwnd, iControlID), LB_SETITEMHEIGHT,
			(WPARAM)lpDrawItemStruct->itemID, (LPARAM)25);
	}
	TCHAR szRssMeg[MAX_RSSMSGLEN] = { NULL };
	SendMessage(GetDlgItem(hwnd, iControlID), LB_GETTEXT, (WPARAM)lpDrawItemStruct->itemID, (LPARAM)szRssMeg);
	DrawText(lpDrawItemStruct->hDC, szRssMeg, lstrlen(szRssMeg), &(lpDrawItemStruct->rcItem), DT_LEFT | DT_SINGLELINE);
	return true;
}


//����Rss���ݵ�������塣
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


//��ʼ��RssList��״̬��ʹ�䱳������������ı���һ����
//HWND Rss������ı�����
bool InitRssList(HWND hwnd)
{
	SendMessage(GetDlgItem(hwnd, IDC_RssFeedList), LB_INSERTSTRING, (WPARAM)-1, (LPARAM)TEXT(""));
	SendMessage(GetDlgItem(hwnd, IDC_RssItemList), LB_INSERTSTRING, (WPARAM)-1, (LPARAM)TEXT(""));
	return true;
}


//��ѡ�е�RssFeed����ʾ��RssItem���б��С�
//hwnd�����ڵľ����iRssFeedIndex��RssFeed�б��е�RssFeed���±ꡣ
bool DisplayRssFeedToRssItemList(HWND hwnd, int iRssFeedIndex)
{
	//�������RssItemList�����ȫ�����ݡ�
	HWND hRssItemListWnd = GetDlgItem(hwnd, IDC_RssItemList);
	for (int iCount = SendMessage(hRssItemListWnd, LB_GETCOUNT, (WPARAM)0, (LPARAM)0);
		iCount >= 0; --iCount)
	{
		SendMessage(hRssItemListWnd, LB_DELETESTRING, (WPARAM)iCount, (LPARAM)0);
	}//	IDC_RssFeedTitleEdit; IDC_RssItemList
	//��ʾ��ǰ��ʾ��RssFeed�����֡�
	TCHAR* szRssFeedTitle = FileEncode::UTF8ToUnicode((char*)(RssFeedList[iRssFeedIndex].c_str()));
	SetDlgItemText(hwnd, IDC_RssFeedTitleEdit, szRssFeedTitle);
	free(szRssFeedTitle);
	//�ٰѸ�RssFeed��Ӧ��RssItem���뵽RssItem���б��С�
	map<string, RssContent> rssItemMap = RssItemList[iRssFeedIndex];
	int index = 0;//ƴ���±ꡣ
	for (map<string, RssContent>::iterator iterator = rssItemMap.begin();
		iterator != rssItemMap.end(); iterator++)
	{
		//��ÿ��ַ�����
		TCHAR* szRssItemTitle = FileEncode::UTF8ToUnicode((char*)(iterator->first.c_str()));
		TCHAR szRssItemListTitle[500] = { NULL };
		wsprintf(szRssItemListTitle, TEXT("%d.  %s"), ++index, szRssItemTitle);
		SendMessage(hRssItemListWnd, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)szRssItemListTitle);
		free(szRssItemTitle);
	}
	return true;
}


//��ѡ�е�RssItem������ʾ������
//hwnd�����ڵľ����iRssItemIndex��RssItem�б��е�RssItem���±ꡣ
bool DisplayRssItemMsg(HWND hwnd, int iRssItemIndex)
{
	//�õ���ǰѡ�е�RssFeed���±ꡣ
	int iRssFeedIndex = RssMessage::GetCurRssFeedIndex();
	map<string, RssContent>::iterator iterator = RssItemList[iRssFeedIndex].begin();//�õ����ӦRssItem�����ݡ�
	//ʹ�������ƶ���ѡ�е��±괦��
	for (int index = 0; index < iRssItemIndex; index++)
	{
		iterator++;
	}
	//��ؼ���д�����ݡ�
	//IDC_RssItemDesLinkEdit  IDC_RssDescriptionEdit
	//д�����顣
	TCHAR* szRssItemDes = FileEncode::UTF8ToUnicode((char*)(iterator->second.Description.c_str()));
	TCHAR* szRssItemPubDate = FileEncode::UTF8ToUnicode((char*)(iterator->second.Date.c_str()));
	//��һЩƴ�ӡ�
	TCHAR szDescription[1000] = { NULL };
	wsprintf(szDescription, TEXT("�������ڣ�\r\n  %s\r\n  %s"), szRssItemPubDate, szRssItemDes);
	//��ʾ�������ݡ�
	SetDlgItemText(hwnd, IDC_RssDescriptionEdit, szDescription);

	//д��ȫ�ĵ����ӡ�
	TCHAR* szRssItemDesLink = FileEncode::UTF8ToUnicode((char*)(iterator->second.Web.c_str()));
	SetDlgItemText(hwnd, IDC_RssItemDesLinkEdit, szRssItemDesLink);
	free(szRssItemDesLink);
	free(szRssItemPubDate);
	free(szRssItemDes);
	return true;
}


//ɾ��RssFeed�б����һ��ѡ��Ҽ��˵���ɾ�����ǵ�ǰѡ������븸���ھ����
//ɾ��һ��RssFeed��һ������� RssFeed�ı��⣬���ӣ��Լ���ϸ��Ϣ��
//extern vector<string> RssFeedList;//����RssFeed��
//extern vector< map<string, RssContent> > RssItemList;//����RssFeed��Ӧ��RssItem�����ݡ�
bool DeleteRssFeedFromList(HWND hwnd)
{
	HWND hRssFeedListWnd = GetDlgItem(hwnd, IDC_RssFeedList);
	int iCurRssFeedIndex = SendMessage(hRssFeedListWnd, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	if (iCurRssFeedIndex == LB_ERR)
	{
		MessageBox(hwnd, TEXT("��ǰû��RssFeed�ѡ�У���"), TEXT("Message"), MB_OK);
		return false;
	}
	//ɾ��RssFeed�Ķ���
	RssFeedList.erase(RssFeedList.begin() + iCurRssFeedIndex);
	//������ɾ����Ӧ��RssFeedLink��Ϣ��
	RssFeedLinks.erase(RssFeedLinks.begin() + iCurRssFeedIndex);
	//������ɾ����Ӧ��Map��Ϣ��
	RssItemList.erase(RssItemList.begin() + iCurRssFeedIndex);
	//���RssFeed�б�����Ķ�Ӧ�
	for (int index = RssFeedList.size(); index >= iCurRssFeedIndex; index--)
	{
		SendMessage(hRssFeedListWnd, LB_DELETESTRING, (WPARAM)index, (LPARAM)0);
	}
	//���²���֮����
	for (int index = iCurRssFeedIndex; index < RssFeedList.size(); index++)
	{
		TCHAR* szRssFeedTitle = FileEncode::UTF8ToUnicode((char*)(RssFeedList[index].c_str()));
		SendMessage(hRssFeedListWnd, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)szRssFeedTitle);
		free(szRssFeedTitle);
	}
	return true;
}


//ɾ��RssItem�б����һ��ѡ��Ҽ��˵���ɾ�����ǵ�ǰѡ���
bool DeleteRssItemFromList(HWND hwnd)
{
	HWND hRssItemListWnd = GetDlgItem(hwnd, IDC_RssItemList);
	int iCurRssItemIndex = SendMessage(hRssItemListWnd, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	if (iCurRssItemIndex == LB_ERR)
	{
		MessageBox(hwnd, TEXT("��ǰû��RssItem�ѡ�У���"), TEXT("Message"), MB_OK);
		return false;
	}
	//���б���ѡ�����Լ�֮�����ȫ��ɾ����
	for (int index = SendMessage(hRssItemListWnd, LB_GETCOUNT, (WPARAM)0, (LPARAM)0);
		index >= iCurRssItemIndex; --index)
	{
		SendMessage(hRssItemListWnd, LB_DELETESTRING, (WPARAM)index, (LPARAM)0);
	}
	//��ʼɾ��RssItemList���Ӧmap��һ��pair��
	int iCurRssFeedIndex = RssMessage::GetCurRssFeedIndex();
	//���ݴ��ҵ���Ӧ��RssItem�
	map<string, RssContent> rssItemMap = RssItemList[iCurRssFeedIndex];
	int index1 = 0;//��¼�±ꡣ
	for (map<string, RssContent>::iterator iterator = rssItemMap.begin();
		iterator != rssItemMap.end(); iterator++)
	{
		if (index1 == iCurRssItemIndex)
		{
			rssItemMap.erase(iterator);//map��ɾ����
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