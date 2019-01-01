#include "stdafx.h"
#include "download.h"
#include "ProcessBar.h"

long ddlnow = 0;
extern BOOL state;
struct myprogress {
	TIMETYPE lastruntime; /* type depends on version, see above */
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
	struct myprogress *myp = (struct myprogress *)p;
	CURL *curl = myp->curl;
	TIMETYPE curtime = 0;

	curl_easy_getinfo(curl, TIMEOPT, &curtime);

	/* under certain circumstances it may be desirable for certain functionality
	   to only run every N seconds, in order to do this the transaction time can
	   be used */
//	if ((curtime - myp->lastruntime) >= MINIMAL_PROGRESS_FUNCTIONALITY_INTERVAL) {
//		myp->lastruntime = curtime;
//#ifdef TIME_IN_US
//		fprintf(stderr, "TOTAL TIME: %" CURL_FORMAT_CURL_OFF_T ".%06ld\r\n",
//			(curtime / 1000000), (long)(curtime % 1000000));
//#else
//		fprintf(stderr, "TOTAL TIME: %f \r\n", curtime);
//#endif
//	}
//
//	fprintf(stderr, "UP: %" CURL_FORMAT_CURL_OFF_T " of %" CURL_FORMAT_CURL_OFF_T
//		"  DOWN: %" CURL_FORMAT_CURL_OFF_T " of %" CURL_FORMAT_CURL_OFF_T
//		"\r\n",
//		ulnow, ultotal, dlnow, dltotal);

	if (dlnow - ddlnow > dltotal / 100.0)
	{
		ddlnow = (dlnow * 100 / dltotal);
	}

	if (state==false)
		return 1;
	return 0;
}

#if LIBCURL_VERSION_NUM < 0x072000
/* for libcurl older than 7.32.0 (CURLOPT_PROGRESSFUNCTION) */
static int older_progress(void *p,
	double dltotal, double dlnow,
	double ultotal, double ulnow)
{
	return xferinfo(p,
		(curl_off_t)dltotal,
		(curl_off_t)dlnow,
		(curl_off_t)ultotal,
		(curl_off_t)ulnow);
}
#endif

DWORD WINAPI download(void)
{
	CURL *curl;
	CURLcode res = CURLE_OK;
	struct myprogress prog;
	FILE *fp;
	const char *url = "http://f7.market.xiaomi.com/download/ThemeMarket/0d3d050bd40b3e2114fbaadf6947a32d66841a9a4/Pure+%E8%BD%BB%E9%9B%A8-1.0.0.27.mtz";
	
	curl = curl_easy_init();
	if (curl) {
		fp = fopen("bbb.mtz", "wb");
		prog.lastruntime = 0;
		prog.curl = curl;

		curl_easy_setopt(curl, CURLOPT_URL, "http://f7.market.xiaomi.com/download/ThemeMarket/0d3d050bd40b3e2114fbaadf6947a32d66841a9a4/Pure+%E8%BD%BB%E9%9B%A8-1.0.0.27.mtz");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
#if LIBCURL_VERSION_NUM >= 0x072000
		/* xferinfo was introduced in 7.32.0, no earlier libcurl versions will
		   compile as they won't have the symbols around.

		   If built with a newer libcurl, but running with an older libcurl:
		   curl_easy_setopt() will fail in run-time trying to set the new
		   callback, making the older callback get used.

		   New libcurls will prefer the new callback and instead use that one even
		   if both callbacks are set. */

		curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, xferinfo);
		/* pass the struct pointer into the xferinfo function, note that this is
		   an alias to CURLOPT_PROGRESSDATA */
		curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &prog);
#else
		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, older_progress);
		/* pass the struct pointer into the progress function */
		curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &prog);
#endif

		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
		res = curl_easy_perform(curl);

		if (res == CURLE_OK)
			MessageBox(NULL, L"Down finished", L"Download Thread", MB_OK);

		/* always cleanup */
		curl_easy_cleanup(curl);
	}
	
	return (int)res;
}


//size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
//	size_t written = fwrite(ptr, size, nmemb, stream);
//	return written;
//}

//std::string getname(const char *str)
//{
//	strCoding ss;
//	std::string temp;
//	int len = strlen(str);
//	for (int i = len - 1; i >= 0; i--)
//	{
//		if (str[i] == '/')
//			break;
//		else
//			temp.push_back(str[i]);
//	}
//	std::reverse(temp.begin(), temp.end());
//	temp = ss.UrlUTF8Decode(temp);
//	return temp;
//}

//int progress_callback(void *clientp,
//	double dltotal,
//	double dlnow,
//	double ultotal,
//	double ulnow)
//{
//	ddlnow = (dlnow / dltotal) * 100;
//	if (stat == false)
//		return 1;
//	return 0;
//}
//
//DWORD WINAPI download(void) {
//	CURL *curl;
//	FILE *fp;
//	CURLcode res;
//	const char *url = "http://f7.market.xiaomi.com/download/ThemeMarket/0d3d050bd40b3e2114fbaadf6947a32d66841a9a4/Pure+%E8%BD%BB%E9%9B%A8-1.0.0.27.mtz";
//	char outfilename[FILENAME_MAX] = "«·”Í.mtz";
//	std::string name = "«·”Í.mtz";
//	char progress_data[10] = { 0 };
//
//	// getname()
//	curl = curl_easy_init();
//	if (curl) {
//		fp = fopen(name.c_str(), "wb");
//		curl_easy_setopt(curl, CURLOPT_URL, url);
//		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
//		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
//		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);
//		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_callback);
//		//curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, progress_data);
//		res = curl_easy_perform(curl);
//		if (res == CURLE_OK)
//		{
//			MessageBox(NULL, L"Download finished", L"download fini", MB_OK);
//		}
//		/* always cleanup */
//		curl_easy_cleanup(curl);
//		fclose(fp);
//	}
//	return 0;
//}
