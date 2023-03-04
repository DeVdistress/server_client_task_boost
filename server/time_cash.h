#pragma once
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include "file_info.h"

using TimeType = std::chrono::seconds;
using DirListType = std::pair<std::string, std::vector<FileInfo>>;
using Clock = std::chrono::system_clock;
using CashListType = std::map<std::string, std::pair<Clock::time_point, std::vector<FileInfo>>>;

class TimeCash
{
	TimeType time_limit;
	CashListType list;

	inline bool outOfTime(Clock::time_point& start, Clock::time_point& stop) {
		return std::chrono::duration_cast<TimeType>(stop - start) > time_limit;
	}

public:
	TimeCash(TimeType time_limit_ = std::chrono::seconds(30)): time_limit(time_limit_){}

	bool isValid(std::string& key, Clock::time_point time_stamp = Clock::now());
	bool getList(DirListType& lst, Clock::time_point time_stamp = Clock::now());
	void setList(DirListType& lst, Clock::time_point time_stamp = Clock::now());
};

