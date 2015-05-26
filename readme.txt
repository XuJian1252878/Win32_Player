学号：1252878
姓名：许舰
联系电话：18817870106
电邮地址：1252878xujian@tongji.edu.cn
注明事项：
	1.程序使用的开发环境为Windows8.1 和 Visual Studio 2013
	2.关于8.3文件格式的问题（windows8及windows8.1操作系统） ***********
		程序的音乐播放逻辑实现主要是靠MCIERROR mciSendString ( 
		LPCTSTR lpszCommand, // 指令字符串
		LPTSTR lpszReturnString, // 返回信息字符串
		UINT cchReturn, // 返回信息字符串长度
		HANDLE hwndCallback // 回叫窗口的句柄
		); 这个函数，在第一个参数（指令字符串）中指定的音乐文件路径名必须是短路径格式（8.3格式文件名），
		在Windows8.1操作系统中8.3格式文件名会被禁用，这会导致GetShortPathName函数调用失败，
		得不到文件的短路径，从而程序的音乐播放功能彻底失效，
		（参考链接：http://msdn.microsoft.com/en-us/library/aa364989(VS.85).aspx） ，
		可以将8.3格式文件名功能打开，再重新向磁盘中考入音乐文件，程序功能即恢复正常。
		（8.3格式文件功能打开方式可参考：http://jingyan.baidu.com/article/295430f10e6bc80c7e00503c.html）。
