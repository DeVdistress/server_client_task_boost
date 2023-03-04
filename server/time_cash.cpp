// #include <algorithm>
// #include <iterator>
#include "time_cash.h"

bool TimeCash::isValid(std::string& key, Clock::time_point time_stamp)
{
	bool res = false;

	if (!list.empty()) {
		auto it = list.find(key);
		if (it != list.end() && outOfTime(it->second.first, time_stamp))
			res = true;
	}

	return res;
}

bool TimeCash::getList(DirListType& lst, Clock::time_point time_stamp)
{
	bool res = isValid(lst.first, time_stamp);

	if (res) {
		auto it = list.find(lst.first);
		lst.second = it->second.second;
		/*
			lst.second.resize(it->second.second.size());
			std::copy(it->second.second.begin(), it->second.second.end(),
				std::back_inserter(lst.second));
		*/
	}

	return res;
}

void TimeCash::setList(DirListType& lst, Clock::time_point time_stamp)
{
	list[lst.first] = std::make_pair(time_stamp, lst.second);
}