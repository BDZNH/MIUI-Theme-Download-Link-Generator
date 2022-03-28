#include "Downloader.h"
#include <string.h>
#include <regex>
#include <functional>
#include "HttpRequest.h"
#include <json/json.h>
#include "urlcoding.h"

Downloader::Downloader(DownloadCallBack* cb) :mCallBack(cb) 
{
	mDownloading.store(false);
}

Downloader::~Downloader()
{
}

bool Downloader::isDownloading()
{
	return false;
}

bool Downloader::setDownloadState()
{
	return false;
}

void Downloader::tryget(const std::string& url, TRY_GEY_TYPE type, const std::string& version)
{
	
	switch (type)
	{
	case GET_DOWNLOAD_LINK_URL:
		{
		    mCallBack->onStart();
			if (isValidMiuiThemeUrl(url)) {
				if (url != mUrl) {
					mCallBack->onURLChanged();
					mUrl = url;
				}
				std::string themeId = url.substr(strlen("http://zhuti.xiaomi.com/detail/"));
				std::string themeLinkApi = "https://thm.market.xiaomi.com/thm/download/v2/" + themeId + "?capability=w%2Cb%2Cs%2Cm%2Ch5%2Cv%3A8%2Cvw&miuiUIVersion=" + version;
				std::string resp;

				if (httpRequestIme(themeLinkApi, resp) && checkResponseValid(resp))
				{
					mCallBack->onMessage(mDownloadLink, GET_LINK_SUCCESS);
				}
				else
				{
					mCallBack->onMessage(resp, GET_LINK_FAILED);
				}
			}
			else
			{
				mCallBack->onMessage(url, INVALID_MIUI_THEME_URL);
			}
			mCallBack->onFinished();
		}
		break;
	case DOWNLOAD_THEME_AS_FILE:
	{
		mCallBack->onStart();
		strCoding urlcoding;
		std::string filename = urlcoding.getName(url.c_str());
		FILE* file = mCallBack->onSave(filename);
		if (file)
		{
			httpRequest(url, file, mCallBack);
		}
		mCallBack->onFinished();
	}
		break;
	default:
		break;
	}
}

bool Downloader::isValidMiuiThemeUrl(const std::string& url)
{
	if (strncmp(url.c_str(), "http://zhuti.xiaomi.com/detail/", strlen("http://zhuti.xiaomi.com/detail/")) == 0)
	{
		static std::regex pattern("^http://zhuti.xiaomi.com/detail/[0-9a-z]{8}-[0-9a-z]{4}-[0-9a-z]{4}-[0-9a-z]{4}-[0-9a-z]{12}$");
		return std::regex_match(url, pattern);
	}
	return false;
}

bool Downloader::checkResponseValid(const std::string& resp)
{
	Json::Value rootValue;
	Json::Reader reader;
	if (reader.parse(resp, rootValue))
	{
		if (rootValue.isMember("apiCode") && rootValue.isMember("apiData") && rootValue["apiData"].isObject())
		{
			Json::Value apidata = rootValue["apiData"];
			if (apidata.isMember("downloadUrl") && apidata.isMember("fileSize"))
			{
				mDownloadLink = apidata["downloadUrl"].asString();
				return true;
			}
		}
	}
	return false;
}
