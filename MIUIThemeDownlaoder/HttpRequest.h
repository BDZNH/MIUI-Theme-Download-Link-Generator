#include <string>
#include <curl/curl.h>
#include "Downloader.h"

bool httpRequestIme(const std::string& url,std::string& resp);
bool httpRequest(const std::string& url, FILE* file, DownloadCallBack* callback);