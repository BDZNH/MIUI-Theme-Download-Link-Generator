#define _CRT_NONSTDC_NO_WARNINGS

#ifndef __GENERATE_H
#define __GENERATE_H

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <regex>
#include <fstream>
#include <cstdlib>
#include <cstring>

#include <curl/curl.h>
#include <json/json.h>


#pragma warning(disable : 4996)


namespace
{
	std::size_t callback(
		const char* in,
		std::size_t size,
		std::size_t num,
		std::string* out)
	{
		const std::size_t totalBytes(size * num);
		out->append(in, totalBytes);
		return totalBytes;
	}
}

inline bool Genarate(std::string Geturl, char * tureUrl)
{
	//const std::string url("http://thm.market.xiaomi.com/thm/download/v2/b3e91bfb-e4a1-4695-a24f-24fddb3a595c/");
	//std::string Geturl;
	//std::cout << "粘贴或者输入主题链接(如 http://zhuti.xiaomi.com/detail/b3e91bfb-e4a1-4695-a24f-24fddb3a595c), 然后按下Enter:\n";
	std::cin >> Geturl;
	std::string url("http://thm.market.xiaomi.com/thm/download/v2/");
	const std::string headOfUrl2("http://zhuti.xiaomi.com/detail/");

	if (Geturl.size() <= url.size())
	{
		//std::cout << "错误的链接" << std::endl;
		//system("pause");
		strcpy(tureUrl, u8"错误的链接");
		return 0;
	}

	url += Geturl.substr(headOfUrl2.size(), Geturl.size() - headOfUrl2.size());

	//std::cout << url << std::endl;
	/*std::string::iterator it = url.begin();
	for (int i = 0; i <= headOfUrl2.size(); i++)
		it++;
	while (it!=url.end())
	{
		headOfUrl1 += (std::string)(*it);
	}*/



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
	std::unique_ptr<std::string> httpData(new std::string());

	// Hook up data handling function.
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);

	// Hook up data container (will be passed as the last parameter to the
	// callback handling function).  Can be any pointer type, since it will
	// internally be passed as a void pointer.
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());

	// Run our HTTP GET command, capture the HTTP response code, and clean up.
	curl_easy_perform(curl);
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
	curl_easy_cleanup(curl);

	if (httpCode == 200)
	{
		//std::cout << "\nGot successful response from " << url << std::endl;

		// Response looks good - done using Curl now.  Try to parse the results
		// and print them out.
		Json::Value jsonData;
		Json::Reader jsonReader;

		if (jsonReader.parse(*httpData, jsonData))
		{
			//std::cout << "Successfully parsed JSON data" << std::endl;
			//std::cout << "\nJSON data received:" << std::endl;
			//std::cout << jsonData.toStyledString() << std::endl;

			std::string Json(jsonData.toStyledString());
			//std::cout << Json << std::endl;


			//用正则匹配链接
			/*std::string pattern("http(s)?://([\\w-]+\\.)+[\\w-]+(/[\\w- ./?%&=]*)?");*/
			std::string pattern("(https?|ftp|file)://[-A-Za-z0-9+&@#/%?=~_|!:,.;]+[-A-Za-z0-9+&@#/%=~_|]");
			pattern = "[[:alpha:]]*" + pattern + "[[:alpha:]]*";
			std::regex r(pattern);
			std::sregex_iterator it(Json.begin(), Json.end(), r);
			//std::cout << "URL: " << it->str() << std::endl;

			std::string temp(it->str());
			strcpy(tureUrl, temp.c_str());

			/*std::ofstream DownloadLink("./Link.txt");

			if (!DownloadLink.is_open())
			{
				std::cout << "文件\"Link.txt\"不存在,检查权限." << std::endl;
			}*/

			//DownloadLink << "复制以下链接到浏览器地址栏进行下载\n\n" << it->str() << std::endl;


			const std::string dateString(jsonData["date"].asString());
			const std::size_t unixTimeMs(
				jsonData["milliseconds_since_epoch"].asUInt64());
			const std::string timeString(jsonData["time"].asString());

			//std::cout << "Natively parsed:" << std::endl;
			//std::cout << "\tDate string: " << dateString << std::endl;
			//std::cout << "\tUnix timeMs: " << unixTimeMs << std::endl;
			//std::cout << "\tTime string: " << timeString << std::endl;
			//std::cout << std::endl;
		}
		else
		{
			//std::cout << "Could not parse HTTP data as JSON" << std::endl;
			//std::cout << "HTTP data was:\n" << *httpData.get() << std::endl;
			return 1;
		}
	}
	else
	{
		std::cout << "Couldn't GET from " << url << " - exiting" << std::endl;
		return 1;
	}

	//system("notepad.exe Link.txt");

	return 0;
}

#endif