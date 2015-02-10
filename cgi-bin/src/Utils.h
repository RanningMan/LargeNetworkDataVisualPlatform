#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <utility>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <queue>
#include <algorithm>
#include <map>
#include <hash_map.h>
#include <stack>
#include <cmath>
#include <mysql.h>
#include <boost/thread.hpp>
#include "Log.h"
#include "DBJob.h"
#include "DBManager.h"

class Utils {
	public:
		static void split(const std::string &s, const char& delim, std::vector<std::string>& tokens);
		static void split(const std::string &s, const char& delim, std::set<std::string>& tokens);
		static std::string numToStr(const int &num);
		static std::string numToStr(const double &num);
		static int strToInt(const std::string& s);
};
