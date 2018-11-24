#include "generate.h"

bool Genarate(std::string Geturl, char * tureUrl)
{

	std::string url("http://thm.market.xiaomi.com/thm/download/v2/");
	const std::string headOfUrl2("http://zhuti.xiaomi.com/detail/");

	if (Geturl.size() <= url.size())
	{
		strcpy(tureUrl, u8"错误的链接");
		return false;
	}

	url += Geturl.substr(headOfUrl2.size(), Geturl.size() - headOfUrl2.size());


	CURL* curl = curl_easy_init();

	// Set remote URL.
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

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
		//assert(document.HasMember("apiData"));
		if (document.HasMember("apiData"))
		{
			const rapidjson::Value& a = document["apiData"];
			if (a.IsObject())
			{
				if (a.HasMember("downloadUrl"))
				{
					//std::cout << "\n\ndownloadUrl: " << a["downloadUrl"].GetString() << std::endl;
					strcpy(tureUrl, a["downloadUrl"].GetString());
				}
			}
		}

		
		//assert(a.IsObject());
		//assert(a.HasMember("downloadUrl"));
		//std::cout << "\n\ndownloadUrl: " << a["downloadUrl"].GetString() << std::endl;

		////Json::Value jsonData;
		////Json::Reader jsonReader;

		////if (jsonReader.parse(*httpData, jsonData))
		////{

		//	std::string Json(jsonData.toStyledString());

		//	//use regex to find the direct_download_link.
		//	//std::string pattern("http(s)?://([\\w-]+\\.)+[\\w-]+(/[\\w- ./?%&=]*)?");
		//	std::string pattern("(https?|ftp|file)://[-A-Za-z0-9+&@#/%?=~_|!:,.;]+[-A-Za-z0-9+&@#/%=~_|]");
		//	pattern = "[[:alpha:]]*" + pattern + "[[:alpha:]]*";
		//	std::regex r(pattern);
		//	std::sregex_iterator it(Json.begin(), Json.end(), r);

		//	std::string temp(it->str());
		//	strcpy(tureUrl, temp.c_str());

		//}
		//else
		//{
		//	strcpy(tureUrl, u8"生成链接失败");
		//	return false;
		//}
	}
	else
	{
		strcpy(tureUrl, u8"啊哦 生成链接失败 看看是不是网络有问题或者链接错误");
		return false;
	}

	return true;
}