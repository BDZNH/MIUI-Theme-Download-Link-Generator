#define _CRT_NONSTDC_NO_WARNINGS
//preprocessor definitions
#ifdef _WIN32

#ifndef CURL_STATICLIB
#define CURL_STATICLIB
#endif // CURL_STATICLIB

#endif // _WIN32

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
//#include <json/json.h>
#include <rapidjson/document.h>


// additional dependencies
#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wldap32.lib")
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "Normaliz.lib")
#pragma comment(lib, "winmm.lib")
#ifdef _DEBUG
#pragma comment(lib, "libcurl_a_debug.lib")
#else
#pragma comment(lib, "libcurl_a.lib")
#endif // _DEBUG
#endif // win32



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

bool Genarate(std::string Geturl, char * tureUrl);

#endif