#include "generate.h"

bool Generate(LPWSTR url)
{
	char ccurl[1000];
	sprintf(ccurl, "%ws", url);
	const char urlTheme[] = "http://zhuti.xiaomi.com/detail/";
	char urlLink[1000] = "http://thm.market.xiaomi.com/thm/download/v2/";
	//char directLink[1000];
	size_t len = strlen(urlLink);
	if (strlen(ccurl) < strlen(urlTheme))
	{
		strcpy(urlLink, "啊哦，链接有误");
		DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, urlLink, -1, NULL, 0); 
		MultiByteToWideChar(CP_ACP, 0, urlLink, -1, url, dwNum);
		return 0;
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
					strcpy(urlLink, a["downloadUrl"].GetString()); //success
				}
				else
				{
					strcpy(urlLink, "啊哦 生成链接失败 看看是不是网络有问题或者链接错误");
					DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, urlLink, -1, NULL, 0); 
					MultiByteToWideChar(CP_ACP, 0, urlLink, -1, url, dwNum);
					return false;
				}
			}
			else
			{
				strcpy(urlLink, "啊哦 生成链接失败 看看是不是网络有问题或者链接错误");
				DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, urlLink, -1, NULL, 0); 
				MultiByteToWideChar(CP_ACP, 0, urlLink, -1, url, dwNum);
				return false;
			}
		}
		else
		{
			strcpy(urlLink, "啊哦 生成链接失败 看看是不是网络有问题或者链接错误");
			DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, urlLink, -1, NULL, 0); 
			MultiByteToWideChar(CP_ACP, 0, urlLink, -1, url, dwNum);
			return false;
		}
	}
	else
	{
		strcpy(urlLink, "啊哦 生成链接失败 看看是不是网络有问题或者链接错误");
		DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, urlLink, -1, NULL, 0); 
		MultiByteToWideChar(CP_ACP, 0, urlLink, -1, url, dwNum);
		return false;
	}

	DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, urlLink, -1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, urlLink, -1, url, dwNum);
	return true;
}