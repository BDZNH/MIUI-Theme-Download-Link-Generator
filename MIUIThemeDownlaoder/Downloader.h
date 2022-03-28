#pragma once

#include <string>
#include <atomic>

#include <stdio.h>


enum TRY_GEY_TYPE
{
	GET_DOWNLOAD_LINK_URL = 0,
	DOWNLOAD_THEME_AS_FILE = 1,
};

enum MESSAGE_TYPE
{
	GET_LINK_SUCCESS = 0,
	GET_LINK_FAILED = 1,
	DOWNLAOD_SUCCESS = 2,
	DOWNLOAD_FAILED = 3,
	INVALID_MIUI_THEME_URL = 4,
};

class DownloadCallBack
{
public:
	virtual void onMessage(const std::string& str, MESSAGE_TYPE type) = 0;
	virtual void onProgress(size_t total, size_t current) = 0;
	virtual void onStart() = 0;
	virtual void onFinished() = 0;
	virtual FILE* onSave(std::string filename) = 0;
	virtual void onURLChanged() = 0;
};

class Downloader
{
public:
	Downloader(DownloadCallBack* cb);
	~Downloader();
	bool isDownloading();
	bool setDownloadState();
	void tryget(const std::string& url, TRY_GEY_TYPE type,const std::string& version);
private:
	DownloadCallBack* mCallBack;
	std::atomic_bool mDownloading;
	std::string mUrl;
	std::string mDownloadLink;

	bool isValidMiuiThemeUrl(const std::string& url);
	bool checkResponseValid(const std::string& resp);
};
