#pragma once
#include <string>
#include <random>

namespace utils
{
	inline std::string RandomString(int length)
	{
		int len = length;
		const char* charset = "abcdefghijklmnopqestuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
		std::string result = "";

		std::random_device rdevice;
		std::mt19937 rng(rdevice());
		std::uniform_int_distribution<std::mt19937::result_type> dist(0, strlen(charset));

		while (len > 0)
		{
			result.push_back(charset[dist(rng)]);
			--len;
		}

		return result;
	}
}