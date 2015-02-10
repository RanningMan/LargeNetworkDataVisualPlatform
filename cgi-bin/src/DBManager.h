#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <vector>
#include <mysql.h>
#include "DBJob.h"

class DBManager {
	private:
		MYSQL *m_conn;       // holds info about the connection 
		MYSQL_STMT *m_stmt;  // pointer to handler of prepared stmt
		char *m_server;
		char *m_user;
		char *m_password;
		char *m_database;
		void init(const int &initial);
		void print_stmt_error (char *message);
	
	public:
		DBManager();  // default constructor
		DBManager(char *server, char *user, char *password, char *database, const int &initial);
		~DBManager();
		void update_processing(const int& job_id);
		void update_completed(const int& job_id, const std::string& new_vertices_str, const std::string& new_edges_str, const int& new_vertices_num, const int& new_edges_num, const double& new_mod);
		std::vector<int> check_jobs();
		DBJob get_job(const int& id);
		void close();
};

#endif
