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

		Json::Value jsonData;
		Json::Reader jsonReader;

		if (jsonReader.parse(*httpData, jsonData))
		{

			std::string Json(jsonData.toStyledString());
	
			//用正则匹配链接
			/*std::string pattern("http(s)?://([\\w-]+\\.)+[\\w-]+(/[\\w- ./?%&=]*)?");*/
			std::string pattern("(https?|ftp|file)://[-A-Za-z0-9+&@#/%?=~_|!:,.;]+[-A-Za-z0-9+&@#/%=~_|]");
			pattern = "[[:alpha:]]*" + pattern + "[[:alpha:]]*";
			std::regex r(pattern);
			std::sregex_iterator it(Json.begin(), Json.end(), r);

			std::string temp(it->str());
			strcpy(tureUrl, temp.c_str());

		}
		else
		{
			strcpy(tureUrl, u8"生成链接失败");
			return false;
		}
	}
	else
	{
		//std::cout << "Couldn't GET from " << url << " - exiting" << std::endl;
		strcpy(tureUrl, u8"生成链接失败");
		return false;
	}

	return true;
}

#endif