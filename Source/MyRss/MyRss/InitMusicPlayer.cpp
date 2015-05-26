/*
*�ļ����������ֲ������������д�뱾�ش��̵�ʵ��
*/


#include"stdafx.h"
#include"InitMusicPlayer.h"

//���������б���ÿһ�����ֵ���Ϣ��
extern vector<MusicMessage> musicMessageList;
//������ֲ����б��ȫ�ֱ�����
extern vector<Lyric> musicLyric;
extern bool bIsAppFirseUse;


//�տ�ʼ�����ֲ�������ʱ�������ֲ����б��������һ�����ֲ������Ѿ���ӵ���Ŀ��
//��ʼ��musicMessageList����Ϣ�����HWND�����ֲ������ľ����
//��ȡ��һ�����ֲ������رպ󱣴���б����ݣ��ú���������MusicMessage::InitMusicMessage(hwnd, IDC_LISTMUSIC);����á�
bool InitMusicList(HWND hwnd)
{
	int iMusicCount = 0;//��¼�����ļ���������
	TCHAR szMusicPlayerPath[MAX_PATH] = { NULL };
	TCHAR szAppRunPath[MAX_PATH] = { NULL };
	GetAppRunPath(hwnd, szAppRunPath);
	lstrcpy(szMusicPlayerPath, szAppRunPath);
	//���첥������Ϣ�洢���ļ��С�
	lstrcat(szMusicPlayerPath, TEXT("MusicPlayer"));
	//�����жϸ��ļ����Ƿ���ڡ�
	char* cszMusicPlayerPath = NULL;//���ֵҪ�ֶ��ͷš�
	cszMusicPlayerPath = FileEncode::UnicodeToAnsi((char*)szMusicPlayerPath);
	//���ֲ�����������Ŀ¼��
	//ERROR_PATH_NOT_FOUND
	//One or more intermediate directories do not exist; this function will only create the final directory in the path.
	if (!bIsAppFirseUse)//ֻ���û����ǵ�һ��ʹ�ø����ʱ���Ž������²�����
	{
		if ((_access(cszMusicPlayerPath, 0) == -1))//������ļ��в����ڡ�
		{
			CreateDirectory(szMusicPlayerPath, NULL);
			if (GetLastError() == ERROR_PATH_NOT_FOUND)
			{
				MessageBox(hwnd, TEXT("MusicPlayer���ļ��ж�ʧ�����򽫻ص���ʼ״̬����"), TEXT("Message"), MB_OK);
				free(cszMusicPlayerPath);
				return false;
			}
		}
		//���ֲ���������ԴĿ¼��
		TCHAR szMusicPlayerSrcPath[MAX_PATH] = { NULL };
		lstrcat(szMusicPlayerSrcPath, szMusicPlayerPath);
		lstrcat(szMusicPlayerSrcPath, TEXT("\\src"));
		char* cszMusicPlayerSrcPath = FileEncode::UnicodeToAnsi((char*)szMusicPlayerSrcPath);
		if (_access(cszMusicPlayerSrcPath, 0) == -1)
		{
			CreateDirectory(szMusicPlayerSrcPath, NULL);
			if (GetLastError() == ERROR_PATH_NOT_FOUND)
			{
				MessageBox(hwnd, TEXT("MusicPlayer\\src���ļ��ж�ʧ�����򽫻ص���ʼ״̬����"), TEXT("Message"), MB_OK);
				free(cszMusicPlayerSrcPath);
				free(cszMusicPlayerPath);
				return false;
			}
		}
		//�洢���ֲ����б���ļ���
		TCHAR szMusicListMessagePath[MAX_PATH] = { NULL };//���������һ�е����ݡ�
		lstrcat(szMusicListMessagePath, szMusicPlayerSrcPath);
		lstrcat(szMusicListMessagePath, TEXT("\\MusicListMessage.mls"));
		FILE* fpMusicList = NULL;
		_wfopen_s(&fpMusicList, szMusicListMessagePath, TEXT("rb"));//�ö����Ƶķ�ʽ��д����һ�㡣����������롣
		if (fpMusicList == NULL)
		{
			MessageBox(hwnd, TEXT("MusicPlayer\\src\\MusicListMessage.mls�ļ���ʧ�����򽫻ص���ʼ״̬����"), TEXT("Message"), MB_OK);
			free(cszMusicPlayerSrcPath);
			free(cszMusicPlayerPath);
			return false;
		}
		//�ļ����ɹ��򿪣���ʼ��ȡ���ݡ�
		TCHAR szMusicListMessage[MAX_PATH] = { NULL };//�洢�����б��ļ������ƣ���·����Ϣ��
		MusicMessage tempMusicMessage;//��ʱ�洢�����ļ�������Ϣ��
		while (!feof(fpMusicList))
		{
			//�ȶ��ݴ�����ݴ洢�����㡣
			memset(szMusicListMessage, 0, sizeof(szMusicListMessage));
			ZeroMemory(&tempMusicMessage, sizeof(tempMusicMessage));
			//�ṹ������Ʋ�����ṹ�����λ���𣿣���������
			_fgetts(szMusicListMessage, sizeof(szMusicListMessage), fpMusicList);//��˶�ȡ�������ַ����к���\r\n����˲��ܶ������ֱ�Ӵ���
			if (lstrlen(szMusicListMessage) == 0)
			{
				fclose(fpMusicList);//��return֮ǰǧ������ǹر��ļ���������һ�γ�������ļ���ʱ���ļ����ܻ�򲻿���
				return false;
			}
			//tempMusicMessage.SetMusicFileFullPath(musicListMessage);
			TCHAR musicFullPath[MAX_PATH];
			lstrcpyn(musicFullPath, szMusicListMessage, (_tcschr(szMusicListMessage, '\r') - szMusicListMessage + 1));//ȷ���ַ��������һλ��'\0'��
			lstrcat(musicFullPath, TEXT("\0"));
			tempMusicMessage.SetMusicFileFullPath(musicFullPath);

			memset(szMusicListMessage, 0, sizeof(szMusicListMessage));
			_fgetts(szMusicListMessage, sizeof(szMusicListMessage), fpMusicList);
			if (lstrlen(szMusicListMessage) == 0)
			{
				fclose(fpMusicList);//��return֮ǰǧ������ǹر��ļ���������һ�γ�������ļ���ʱ���ļ����ܻ�򲻿���
				return false;
			}
			TCHAR musicFileName[MAX_PATH] = { NULL };
			lstrcpyn(musicFileName, szMusicListMessage, (_tcschr(szMusicListMessage, '\r') - szMusicListMessage + 1));//ȷ���ַ��������һλ��'\0'��
			lstrcat(musicFileName, TEXT("\0"));
			tempMusicMessage.SetMusicFileName(musicFileName);
			//�������ļ������б�����������
			TCHAR szMusicListItem[MAX_PATH] = { NULL };
			iMusicCount++;
			wsprintf(szMusicListItem, L"%d.  %s", iMusicCount, musicFileName);
			MusicMessage::AddMusicToList(-1, szMusicListItem);

			memset(szMusicListMessage, 0, sizeof(szMusicListMessage));
			_fgetts(szMusicListMessage, sizeof(szMusicListMessage), fpMusicList);
			if (lstrlen(szMusicListMessage) == 0)
			{
				fclose(fpMusicList);//��return֮ǰǧ������ǹر��ļ���������һ�γ�������ļ���ʱ���ļ����ܻ�򲻿���
				return false;
			}
			TCHAR musicFileShortPath[MAX_PATH] = { NULL };
			lstrcpyn(musicFileShortPath, szMusicListMessage, (_tcschr(szMusicListMessage, '\r') - szMusicListMessage + 1));//ȷ���ַ��������һλ��'\0'��
			lstrcat(musicFileShortPath, TEXT("\0"));
			tempMusicMessage.SetMusicFileShortPath(musicFileShortPath);

			//д�������ļ���������Ϣ��
			memset(szMusicListMessage, 0, sizeof(szMusicListMessage));
			_fgetts(szMusicListMessage, sizeof(szMusicListMessage), fpMusicList);
			if (lstrlen(szMusicListMessage) == 0)
			{
				fclose(fpMusicList);//��return֮ǰǧ������ǹر��ļ���������һ�γ�������ļ���ʱ���ļ����ܻ�򲻿���
				return false;
			}
			TCHAR szMusicFileType[20] = { NULL };
			lstrcpyn(szMusicFileType, szMusicListMessage, (_tcschr(szMusicListMessage, '\r') - szMusicListMessage + 1));
			lstrcat(szMusicFileType, TEXT("\0"));
			tempMusicMessage.SetMusicFileType(szMusicFileType);

			//����ʱ��MusicClass����MusicList���vector�С�
			musicMessageList.push_back(tempMusicMessage);

			//������ʾ����Ŀ������
			TCHAR sMusicCount[MAX_PATH];
			wsprintf(sMusicCount, TEXT("%d"), iMusicCount);
			SetDlgItemText(hwnd, IDC_MusicCount, sMusicCount);
		}
		//�������֮�󣬹ر��ļ���
		fclose(fpMusicList);
		free(cszMusicPlayerSrcPath);
		free(cszMusicPlayerPath);
	}
	return true;
}

//�˳�ʱ���������˳�ʱ����Ϣ�����HWND�Ǹ����ڵ�HWND��
bool SaveMusicPlayerMessage(HWND hwnd)
{
	TCHAR szMusicPlayerPath[MAX_PATH] = { NULL };
	TCHAR szAppRunPath[MAX_PATH] = { NULL };
	GetAppRunPath(hwnd, szAppRunPath);
	lstrcpy(szMusicPlayerPath, szAppRunPath);
	//���첥������Ϣ�洢���ļ��С�
	lstrcat(szMusicPlayerPath, TEXT("MusicPlayer"));
	//�����жϸ��ļ����Ƿ���ڡ�
	/*	When used with files, the _access function determines whether the specified file exists 
	and can be accessed as specified by the value of mode. When used with directories, _access 
	determines only whether the specified directory exists; in Windows NT, all directories have read and write access.
	mode Value            Checks File For 
	00                              Existence only 
	02                              Write permission 
	04                              Read permission 
	06                              Read and write permission*/
	char* cszMusicPlayerPath = NULL;//���ֵҪ�ֶ��ͷš�
	cszMusicPlayerPath = FileEncode::UnicodeToAnsi((char*)szMusicPlayerPath);
	//���ֲ�����������Ŀ¼��
	//ERROR_PATH_NOT_FOUND
	//One or more intermediate directories do not exist; this function will only create the final directory in the path. 
	if ((_access(cszMusicPlayerPath, 0) == -1))//������ļ��в����ڡ�
	{
		CreateDirectory(szMusicPlayerPath, NULL);
		if (GetLastError() == ERROR_PATH_NOT_FOUND)
		{
			MessageBox(hwnd, TEXT("MusicPlayer���ļ����޷��������м��ļ��ж�ʧ����"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			free(cszMusicPlayerPath);
			return false;
		}
	}
	//���ֲ���������ԴĿ¼��
	TCHAR szMusicPlayerSrcPath[MAX_PATH] = { NULL };
	lstrcat(szMusicPlayerSrcPath, szMusicPlayerPath);
	lstrcat(szMusicPlayerSrcPath, TEXT("\\src"));
	char* cszMusicPlayerSrcPath = FileEncode::UnicodeToAnsi((char*)szMusicPlayerSrcPath);
	if (_access(cszMusicPlayerSrcPath, 0) == -1)
	{
		CreateDirectory(szMusicPlayerSrcPath, NULL);
		if (GetLastError() == ERROR_PATH_NOT_FOUND)
		{
			MessageBox(hwnd, TEXT("MusicPlayer\\src���ļ����޷��������м��ļ��ж�ʧ����"), TEXT("ERROR"), MB_OK | MB_ICONERROR);
			free(cszMusicPlayerSrcPath);
			free(cszMusicPlayerPath);
			return false;
		}
	}
	//�洢���ֲ����б���ļ���
	TCHAR szMusicListMessagePath[MAX_PATH] = { NULL };
	lstrcat(szMusicListMessagePath, szMusicPlayerSrcPath);
	lstrcat(szMusicListMessagePath, TEXT("\\MusicListMessage.mls"));
	FILE* fpMusicList = NULL;//���������б���Ϣ���ļ�ָ�롣
	_wfopen_s(&fpMusicList, szMusicListMessagePath, TEXT("wb"));
	if (fpMusicList == NULL)//�����б���Ϣ�ļ���ʧ�ܡ�
	{
		MessageBox(hwnd, TEXT("�������б���Ϣ�ļ�ʧ�ܣ���ǰ�����ļ��б���Ϣ�޷����棡��"),
			TEXT("ERROR"), MB_OK | MB_ICONERROR);
		free(cszMusicPlayerSrcPath);
		free(cszMusicPlayerPath);
		return false;
	}
	//�ļ��򿪳ɹ�֮��
	if (!musicMessageList.empty())
	{
		for (vector<MusicMessage>::iterator iterator = musicMessageList.begin();
			iterator != musicMessageList.end(); iterator++)
		{
			_fputts(iterator->GetMusicFileFullPath(), fpMusicList);
			_ftprintf(fpMusicList, TEXT("\r\n"));
			_fputts(iterator->GetMusicFileName(), fpMusicList);
			_ftprintf(fpMusicList, TEXT("\r\n"));
			_fputts(iterator->GetMusicFileShortPath(), fpMusicList);
			_ftprintf(fpMusicList, TEXT("\r\n"));
			_fputts(iterator->GetMusicFileType(), fpMusicList);
			if (iterator != musicMessageList.end() - 1)
			{
				_ftprintf(fpMusicList, TEXT("\r\n"));
			}
			else
			{
				//����\r�ǻص�һ�еĿ�ͷ����\n��������һ����ͬ��λ�á�
				//���������Ļ��ᵼ���ڶ�������б��ʱ��\n���������Ӷ�����һ���հ׵ĸ����
				_ftprintf(fpMusicList, TEXT("\r"));
			}
		}
	}
	fclose(fpMusicList);//�ر��ļ������ռ�á�
	free(cszMusicPlayerSrcPath);
	free(cszMusicPlayerPath);
	return true;
}