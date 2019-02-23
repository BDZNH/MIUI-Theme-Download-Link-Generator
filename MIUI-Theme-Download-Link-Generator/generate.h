//preprocessor definitions start
#ifdef _WIN32

#ifndef _CRT_NONSTDC_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#endif // !

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef CURL_STATICLIB
#define CURL_STATICLIB
#endif // CURL_STATICLIB

#endif // _WIN32

//preprocessor definitions end


#ifndef __GENERATE_H
#define __GENERATE_H

#include <Windows.h>

#include <string>
#include <cstring>

#include <curl/curl.h>
#include <rapidjson/document.h>

#include "urlcoding.h"
#include "resource.h"
#include "header.h"


// additional dependencies
#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wldap32.lib")
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "Normaliz.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "WINMM.LIB")

#ifdef _DEBUG
#pragma comment(lib, "libcurl_a_debug.lib")
#else
#pragma comment(lib, "libcurl_a.lib")
#endif // _DEBUG
#endif // win32

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

	struct myprogress {
		curl_off_t lastruntime; /* type depends on version, see above */
		CURL *curl;
	};

	size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
		size_t written = fwrite(ptr, size, nmemb, stream);
		return written;
	}
}

bool Generate(LPWSTR url,int version);
BOOL CopyToClipboard(HWND hWnd, const WCHAR* pszData, const int nDataLen);
DWORD WINAPI download(LPVOID lpParameter);

#endif