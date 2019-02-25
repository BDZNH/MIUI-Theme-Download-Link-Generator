#pragma once
#include <string>

class Theme
{
public:
	Theme();
	~Theme();
	void activate();
	bool setLink(std::string link);
	std::string getDownloadLink();
	std::string getThemeName();
private:
	std::string s_Link;
	std::string s_DownloadLink;
	std::string s_id;
	std::string s_Name;
};

