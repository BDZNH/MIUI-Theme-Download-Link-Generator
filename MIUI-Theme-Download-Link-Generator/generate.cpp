#include "generate.h"

long long ddlnow = 0;
BOOL isRuning;
char dLinktemp[300];


bool Generate(LPWSTR url)
{
	bool flag = false;
	//char ccurl[1000];
	char *ccurl = new char[1000];
	sprintf(ccurl, "%ws", url);
	const char urlTheme[] = "http://zhuti.xiaomi.com/detail/";
	//char urlLink[1000] = "http://thm.market.xiaomi.com/thm/download/v2/";
	char *urlLink = new char[1000];
	strcpy(urlLink, "http://thm.market.xiaomi.com/thm/download/v2/");
	//char directLink[1000];
	size_t len = strlen(urlLink);
	if (strlen(ccurl) < strlen(urlTheme))
	{
		flag = false;
		strcpy(urlLink, "啊哦 生成链接失败 看看是不是网络有问题或者链接错误");
		DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, urlLink, -1, NULL, 0);
		MultiByteToWideChar(CP_ACP, 0, urlLink, -1, url, dwNum);
		return flag;
	}
	char *p = &urlLink[len];
	strcpy(p, ccurl + (strlen(urlTheme)));


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
	delete[] urlTheme;
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


struct myprogress {
	curl_off_t lastruntime; /* type depends on version, see above */
	CURL *curl;
};

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}

/* this is how the CURLOPT_XFERINFOFUNCTION callback works */
static int xferinfo(void *p,
	curl_off_t dltotal, curl_off_t dlnow,
	curl_off_t ultotal, curl_off_t ulnow)
{
	

	if (dlnow - ddlnow > dltotal / 100.0)
	{
		ddlnow = (dlnow * 100 / dltotal);
	}

	if (isRuning==false)
		return 1;
	return 0;
}


DWORD WINAPI download(void)
{
	CURL *curl;
	CURLcode res = CURLE_OK;
	struct myprogress prog;
	FILE *fp;
	
	char path[255];


	curl = curl_easy_init();
	if (curl) {
		fp = fopen("softrain.mtz", "wb");
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

		if (res == CURLE_OK)
		{
			fclose(fp);
			MessageBox(NULL, L"Down finished", L"Download Thread", MB_OK);
			
		}
			

		/* always cleanup */
		isRuning=false;
		curl_easy_cleanup(curl);
	}
	
	return (DWORD)res;
}
