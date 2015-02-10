#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <ctime>

class Log {
	private:
		std::ofstream m_stream;
	
	public:
		Log();
		Log(const std::string &filename);
		~Log();
		void init(const std::string &filename);
		void write(const std::string &line);
};

#endif
