#include "generate.h"

long long ddlnow = 0;				//下载进度
bool isRuning;						//下载线程状态
char dLinktemp[300];				//用一个全局变量来保存生成的链接
char Version[7][63] = {
	{"?capability=w%2Cb%2Cs%2Cm%2Ch5%2Cv%3A8%2Cvw&miuiUIVersion=V4\0"},
	{"?capability=w%2Cb%2Cs%2Cm%2Ch5%2Cv%3A8%2Cvw&miuiUIVersion=V5\0"},
	{"?capability=w%2Cb%2Cs%2Cm%2Ch5%2Cv%3A8%2Cvw&miuiUIVersion=V6\0"},
	{"\0"},
	{"?capability=w%2Cb%2Cs%2Cm%2Ch5%2Cv%3A8%2Cvw&miuiUIVersion=V8\0"},
	{"\0"},
	{"?capability=w%2Cb%2Cs%2Cm%2Ch5%2Cv%3A8%2Cvw&miuiUIVersion=V10\0"}
};

bool Generate(LPWSTR url,int version)
{
	bool flag = false;
	//char ccurl[1000];
	char *ccurl = new char[1000];
	sprintf(ccurl, "%ws", url);
	const char urlTheme[] = "http://zhuti.xiaomi.com/detail/";
	//char urlLink[1000] = "http://thm.market.xiaomi.com/thm/download/v2/";
	char *urlLink = new char[1000];
	strcpy(urlLink, "https://thm.market.xiaomi.com/thm/download/v2/");
	//char directLink[1000];
	
	if (strlen(ccurl) < strlen(urlTheme))
	{
		flag = false;
		strcpy(urlLink, "啊哦 生成链接失败 看看是不是网络有问题或者链接错误");
		DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, urlLink, -1, NULL, 0);
		MultiByteToWideChar(CP_ACP, 0, urlLink, -1, url, dwNum);
		return flag;
	}
	size_t len = strlen(urlLink);
	char *p = &urlLink[len];
	strcpy(p, ccurl + (strlen(urlTheme)));
	len = strlen(urlLink);
	p = &urlLink[len];
	strcpy(p, Version[version - 1]);

	CURL* curl = curl_easy_init();

	// Set remote URL.
	curl_easy_setopt(curl, CURLOPT_URL, urlLink);

	// Don't bother trying IPv6, which would increase DNS resolution time.
	curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);

	// Don't wait forever, time out after 10 seconds.
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);

	// Follow HTTP redirects if necessary.
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

	// Response information.
	int httpCode(0);
	std::string RequestData;

	// Hook up data handling function.
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);

	// Hook up data container (will be passed as the last parameter to the
	// callback handling function).  Can be any pointer type, since it will
	// internally be passed as a void pointer.
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &RequestData);

	// Run our HTTP GET command, capture the HTTP response code, and clean up.
	curl_easy_perform(curl);
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
	curl_easy_cleanup(curl);

	if (httpCode == 200)
	{
		rapidjson::Document document;
		document.Parse(RequestData.c_str());
		if (document.HasMember("apiData")) // check is there apidata
		{
			const rapidjson::Value& a = document["apiData"];  //get the apidata
			if (a.IsObject())
			{
				if (a.HasMember("downloadUrl"))
				{
					flag = true;
					strcpy(urlLink, a["downloadUrl"].GetString()); //success
					strcpy(dLinktemp, urlLink);
				}
				else
				{
					flag = false;
				}
			}
			else
			{
				flag = false;
			}
		}
		else
		{
			flag = false;
		}
	}
	else
	{
		flag = false;
	}
	if (!flag)
	{
		strcpy(urlLink, "啊哦 生成链接失败 看看是不是网络有问题或者链接错误");
	}

	//char to wchar
	DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, urlLink, -1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, urlLink, -1, url, dwNum);


	delete[] urlLink;
	delete[] ccurl;
	return flag;
}

BOOL CopyToClipboard(HWND hWnd, const WCHAR* pszData, const int nDataLen)
{
	if (OpenClipboard(hWnd))
	{
		EmptyClipboard();
		HGLOBAL clipbuffer;
		char *buffer;
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, nDataLen + 1);
		buffer = (char *)::GlobalLock(clipbuffer);
		//strcpy(buffer, pszData);
		sprintf(buffer, "%ws", pszData);
		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_TEXT, clipbuffer);
		CloseClipboard();
		return TRUE;
	}
	return FALSE;
}




/* this is how the CURLOPT_XFERINFOFUNCTION callback works */
static int xferinfo(void *p,
	curl_off_t dltotal, curl_off_t dlnow,
	curl_off_t ultotal, curl_off_t ulnow)
{
	

	if (dlnow - ddlnow > dltotal / 100.0)
	{
		ddlnow = (dlnow * 100 / dltotal);  //更新下载进度
	}

	if (isRuning==false)
		return CURLE_UNSUPPORTED_PROTOCOL;
	return 0;
}


DWORD WINAPI download(LPVOID lpParameter)
{
	CURL *curl;
	CURLcode res = CURLE_OK;
	struct myprogress prog;
	FILE *fp;
	ddlnow = 0;

	HWND hWnd = (HWND)lpParameter;
	
	strCoding urlDecode;
	std::string fileName = urlDecode.getName(dLinktemp);
	curl = curl_easy_init();
	if (curl) {
		fp = fopen(fileName.c_str(), "wb");
		prog.lastruntime = 0;
		prog.curl = curl;

		curl_easy_setopt(curl, CURLOPT_URL, dLinktemp);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

		/* xferinfo was introduced in 7.32.0, no earlier libcurl versions will
		   compile as they won't have the symbols around.*/


		curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, xferinfo);
		/* pass the struct pointer into the xferinfo function, note that this is
		   an alias to CURLOPT_PROGRESSDATA */
		curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &prog);


		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
		res = curl_easy_perform(curl);
		fclose(fp);
		if (res == CURLE_OK)
		{
			PlaySound((LPCTSTR)IDR_WAVE_SUCCESS, nullptr, SND_RESOURCE | SND_ASYNC | SND_NOSTOP);
			MessageBox(hWnd, L"下载完成", L"Hello", MB_OK);
		}

		/* always cleanup */
		isRuning=false;
		curl_easy_cleanup(curl);
	}
	return (DWORD)res;
}


