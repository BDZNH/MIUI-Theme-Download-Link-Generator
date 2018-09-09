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

bool Genarate(std::string Geturl, char * tureUrl);

#endif