#include "Utils.h"

void Utils::split(const std::string &s, const char& delim, std::vector<std::string>& tokens) {
    std::stringstream ss(s);
    std::string tok;
    while (getline(ss, tok, delim)) {
        tokens.push_back(tok);
    }
}

void Utils::split(const std::string &s, const char& delim, std::set<std::string>& tokens) {
    std::stringstream ss(s);
    std::string tok;
    while (getline(ss, tok, delim)) {
        tokens.insert(tok);
    }
}

std::string Utils::numToStr(const int &num) {
	std::stringstream sstm;
	sstm << num;
	return sstm.str();
}

std::string Utils::numToStr(const double &num) {
	std::stringstream sstm;
	sstm << num;
	return sstm.str();
}

int Utils::strToInt(const std::string& s) {
	std::stringstream sstm(s);
	int n;
	sstm >> n;
	return n;
}
