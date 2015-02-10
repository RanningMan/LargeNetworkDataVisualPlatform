#include "DBJob.h"

DBJob::DBJob() {
}

DBJob::DBJob(const int& type, const std::string& query_v, const std::string& current_v_list, const int& inputlow, const int& inputhigh) : m_type(type), m_query_v(query_v), m_current_v_list(current_v_list), m_inputhigh(inputhigh), m_inputlow(inputlow)  {

}

DBJob::~DBJob() {
}

int DBJob::getType() {
	return m_type;
}

std::string DBJob::getQueryV() {
	return m_query_v;
}

std::string DBJob::getCurrentVertices() {
	return m_current_v_list;
}

int DBJob::getInputLow() {
	return m_inputlow;
}

int DBJob::getInputHigh() {
	return m_inputhigh;
}
