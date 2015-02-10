#ifndef DBJOB_H
#define DBJOB_H

#include <string>
#include <stdlib.h>
#include <iostream>

using namespace std;

class DBJob {
	private:
		std::string m_query_v, m_current_v_list;
		int m_type;
		int m_inputlow;
		int m_inputhigh;
	
	public:
		DBJob();
		DBJob(const int& type, const std::string& query_v, const std::string& current_v_list, const int& inputlow, const int& inputhigh);
		~DBJob();
		int getType();
		std::string getQueryV();
		std::string getCurrentVertices();
		int getInputLow();
		int getInputHigh();		
};

#endif
