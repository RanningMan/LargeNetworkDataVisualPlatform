#include "DBManager.h"
#include "Utils.h"
#include <iostream>

DBManager::DBManager() {
	// nothing
}

DBManager::DBManager(char *server, char *user, char *password, char *database, const int &initial) : m_server(server), m_user(user), m_password(password), m_database(database) {
	init(initial);
 }
 
 DBManager::~DBManager() {
 }
 
 void DBManager::print_stmt_error (char *message) {
	fprintf (stderr, "%s\n", message);
	if (m_stmt != NULL) {
		fprintf (stderr, "Error %u (%s): %s\n",
		mysql_stmt_errno (m_stmt),
		mysql_stmt_sqlstate (m_stmt),
		mysql_stmt_error (m_stmt));
	}
}
 
 void DBManager::close() {
	// deallocate statement handler
	mysql_stmt_close(m_stmt);
	// close connection
	mysql_close(m_conn);
 }
 
 void DBManager::init(const int &initial) {
	if (initial == 1) {
			// call to initialize MySQL library
		if (mysql_library_init(0, NULL, NULL)) {
			fprintf(stderr, "could not initialize MySQL library\n");
			exit(1);
		}
	}

	// Connect to database
	// NOTE: mysql_init() has the side effect of calling mysql_thread_init()
	m_conn = mysql_init(NULL);
	if (!mysql_real_connect(m_conn, m_server, m_user, m_password, m_database, 0, NULL, 0)) {
		fprintf(stderr, "%s\n", mysql_error(m_conn));
		exit(1);
	}

	m_stmt = mysql_stmt_init(m_conn);  // allocate statement handler
	if (m_stmt == NULL) {
		print_stmt_error("could not prepare statement");
		exit(1);
	}
 }

void DBManager::update_processing(const int& job_id) {
	char *stmt_str = "UPDATE vis_jobs SET status='processing' WHERE id = ?";
	
	MYSQL_BIND param[1];
	int my_job_id;
	
	if (mysql_stmt_prepare(m_stmt, stmt_str, strlen(stmt_str)) != 0) {
		print_stmt_error("could not prepare update statement");
		return;
	}
	
	// zero the parameter structures, and then perform all parameter initialization that is constant and does not change for each row
	memset((void *)param, 0, sizeof(param));
	
	// set up my_id parameter
	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = (void *) &my_job_id;
	param[0].is_unsigned = 0;
	param[0].is_null = 0;
	// buffer length, length need not be set
	
	if (mysql_stmt_bind_param(m_stmt, param) != 0) {
		print_stmt_error("could not bind parameters for update");
		return;
	}
	
	my_job_id = job_id;
	
	if (mysql_stmt_execute(m_stmt) != 0) {
		print_stmt_error("could not execute statement");
		return;
	}
}

void DBManager::update_completed(const int& job_id, const std::string& new_vertices_str, const std::string& new_edges_str, const int& new_vertices_num, const int& new_edges_num, const double& new_mod) {
	char *stmt_str = "UPDATE vis_jobs SET status = 'complete', new_v_list = ?, new_e_list = ?, end_time = ?, nodes_num = ?, edge_num = ?, modularity = ? WHERE id = ?";
	
	MYSQL_BIND param[7];
	int my_job_id;
	int my_vertices_num; 
	int my_edges_num;
	double my_mod;
	char my_new_v_list[new_vertices_str.length()];
	char my_new_e_list[new_edges_str.length()];
	unsigned long my_new_v_list_length, my_new_e_list_length;
	MYSQL_TIME my_end_time;
	time_t clock;
	struct tm *cur_time;
	
	if (mysql_stmt_prepare(m_stmt, stmt_str, strlen(stmt_str)) != 0) {
		print_stmt_error("could not prepare update statement");
		return;
	}
	
	// zero the parameter structures, and then perform all parameter initialization that is constant and does not change for each row
	memset((void *)param, 0, sizeof(param));
	
	// set up my_new_v_list parameter
	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = (void *) &my_new_v_list;
	param[0].buffer_length = sizeof(my_new_v_list);
	param[0].length = &my_new_v_list_length;
	param[0].is_null = 0;
	
	// set up my_new_e_list parameter
	param[1].buffer_type = MYSQL_TYPE_STRING;
	param[1].buffer = (void *) &my_new_e_list;
	param[1].buffer_length = sizeof(my_new_e_list);
	param[1].length = &my_new_e_list_length;
	param[1].is_null = 0;
	
	// set up my_end_time
	param[2].buffer_type = MYSQL_TYPE_DATETIME;
	param[2].buffer = (void *) &my_end_time;
	param[2].is_null = 0;
	
	//set up vertices number
	param[3].buffer_type = MYSQL_TYPE_LONG;
	param[3].buffer = (void *) &my_vertices_num;
	param[3].is_unsigned = 0;
	param[3].is_null = 0;
	
	//set up edges number
	param[4].buffer_type = MYSQL_TYPE_LONG;
	param[4].buffer = (void *) &my_edges_num;
	param[4].is_unsigned = 0;
	param[4].is_null = 0;
	
	//set up modularity 
	param[5].buffer_type = MYSQL_TYPE_DOUBLE;
	param[5].buffer = (void *) &my_mod;
	param[5].is_null = 0;
	
	// set up my_job_id parameter
	param[6].buffer_type = MYSQL_TYPE_LONG;
	param[6].buffer = (void *) &my_job_id;
	param[6].is_unsigned = 0;
	param[6].is_null = 0;
	
	if (mysql_stmt_bind_param(m_stmt, param) != 0) {
		print_stmt_error("could not bind parameters for update");
		return;
	}
	
	// get current time
	(void)time (&clock);
	
	// set variables
	(void) strcpy(my_new_v_list, new_vertices_str.c_str());
	(void) strcpy(my_new_e_list, new_edges_str.c_str());
	my_new_v_list_length = strlen(my_new_v_list);
	my_new_e_list_length = strlen(my_new_e_list);
	my_job_id = job_id;
	my_vertices_num = new_vertices_num;
	my_edges_num = new_edges_num;
	my_mod = new_mod;

	cur_time = localtime(&clock);
	my_end_time.year = cur_time->tm_year + 1900;
	my_end_time.month = cur_time->tm_mon + 1;
	my_end_time.day = cur_time->tm_mday;
	my_end_time.hour = cur_time->tm_hour;
	my_end_time.minute = cur_time->tm_min;
	my_end_time.second = cur_time->tm_sec;
	my_end_time.second_part = 0;
	my_end_time.neg = 0;
	
	if (mysql_stmt_execute(m_stmt) != 0) {
		print_stmt_error("could not execute statement");
		return;
	}
}

std::vector<int> DBManager::check_jobs() {
	std::vector<int> job_ids;

	char *stmt_str = "SELECT id FROM vis_jobs WHERE status = 'pending' ORDER BY start_time ASC";
	MYSQL_BIND result[1];
	int my_job_id;

	if (mysql_stmt_prepare(m_stmt, stmt_str, strlen(stmt_str)) != 0) {
		print_stmt_error("could not prepare select statement");
		return job_ids;
	}
	
	if (mysql_stmt_field_count(m_stmt) != 1) {
		print_stmt_error("unexpected column count from select");
		return job_ids;
	}
	
	// initialize result column structures
	memset((void *)result, 0, sizeof(result)); // zero the structures
	
	// set up my_id parameter
	result[0].buffer_type = MYSQL_TYPE_LONG;
	result[0].buffer = (void *) &my_job_id;
	result[0].is_unsigned = 0;
	result[0].is_null = 0;
	
	if (mysql_stmt_bind_result(m_stmt, result) != 0) {
		print_stmt_error("could not bind parameters for select");
		return job_ids;
	}
	
	if (mysql_stmt_execute(m_stmt) != 0) {
		print_stmt_error("could not execute select");
		return job_ids;
	}
	
	// fetch result set into client memory; this is optional, but it allows mysql_stmt_num_rows() to be called to determine the number of rows in the result set
	if (mysql_stmt_store_result(m_stmt) != 0) {
		print_stmt_error("could not buffer result set");
		return job_ids;
	}
	
	while (mysql_stmt_fetch(m_stmt) == 0) {  // fetch each row
		// store into vector
		job_ids.push_back(my_job_id);
	}
	
	// deallocate result set
	mysql_stmt_free_result(m_stmt);
	
	return job_ids;
}

DBJob DBManager::get_job(const int& id) {
	DBJob data;

	char *stmt_str = "SELECT type, query_v, current_v_list, nodes_num, edge_num, modularity, user_input_lower, user_input_higher FROM vis_jobs WHERE id = ?";
	MYSQL_BIND param[1];
	MYSQL_BIND result[8];
	int my_job_id;
	int my_type;
	int my_vertices_num; 
	int my_edges_num;
	double my_mod;
	int my_input_low;
	int my_input_high;
	char my_query_v[100], my_current_v_list[2000];
	unsigned long my_query_v_length, my_current_v_list_length;

	if (mysql_stmt_prepare(m_stmt, stmt_str, strlen(stmt_str)) != 0) {
		print_stmt_error("could not prepare select statement");
		return data;
	}
	
	if (mysql_stmt_field_count(m_stmt) != 8) {
		print_stmt_error("unexpected column count from select");
		return data;
	}
	
	// initialize result column structures
	memset((void *)param, 0, sizeof(param));
	memset((void *)result, 0, sizeof(result)); // zero the structures
	
	// set up my_id parameter
	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = (void *) &my_job_id;
	param[0].is_unsigned = 0;
	param[0].is_null = 0;
	
	// set up my_type parameter
	result[0].buffer_type = MYSQL_TYPE_LONG;
	result[0].buffer = (void *) &my_type;
	result[0].is_unsigned = 0;
	result[0].is_null = 0;
	
	// set up my_query_v parameter
	result[1].buffer_type = MYSQL_TYPE_STRING;
	result[1].buffer = (void *) &my_query_v;
	result[1].buffer_length = sizeof(my_query_v);
	result[1].length = &my_query_v_length;
	result[1].is_null = 0;
	
	// set up my_curent_v_list parameter
	result[2].buffer_type = MYSQL_TYPE_STRING;
	result[2].buffer = (void *) &my_current_v_list;
	result[2].buffer_length = sizeof(my_current_v_list);
	result[2].length = &my_current_v_list_length;
	result[2].is_null = 0;
	
	//set up nodes_num
	result[3].buffer_type = MYSQL_TYPE_LONG;
	result[3].buffer = (void *) &my_vertices_num;
	result[3].is_unsigned = 0;
	result[3].is_null = 0;
	
	//set up edges number
	result[4].buffer_type = MYSQL_TYPE_LONG;
	result[4].buffer = (void *) &my_edges_num;
	result[4].is_unsigned = 0;
	result[4].is_null = 0;
	
	//set up modularity 
	result[5].buffer_type = MYSQL_TYPE_DOUBLE;
	result[5].buffer = (void *) &my_mod;
	result[5].is_null = 0;
	
	//set up input low range number
	result[6].buffer_type = MYSQL_TYPE_LONG;
	result[6].buffer = (void *) &my_input_low;
	result[6].is_unsigned = 0;
	result[6].is_null = 0;
	
	//set up input high range number
	result[7].buffer_type = MYSQL_TYPE_LONG;
	result[7].buffer = (void *) &my_input_high;
	result[7].is_unsigned = 0;
	result[7].is_null = 0;
	
	
	if (mysql_stmt_bind_param(m_stmt, param) != 0) {
		print_stmt_error("could not bind parameters for select");
		return data;
	}
	
	if (mysql_stmt_bind_result(m_stmt, result) != 0) {
		print_stmt_error("could not bind results for select");
		return data;
	}
	
	// set variable
	my_job_id = id;
	
	if (mysql_stmt_execute(m_stmt) != 0) {
		print_stmt_error("could not execute select");
		return data;
	}
	
	// fetch result set into client memory; this is optional, but it allows mysql_stmt_num_rows() to be called to determine the number of rows in the result set
	if (mysql_stmt_store_result(m_stmt) != 0) {
		print_stmt_error("could not buffer result set");
		return data;
	}

	mysql_stmt_fetch(m_stmt);  // fetch the single result
	data = DBJob(my_type, my_query_v, my_current_v_list, my_input_low, my_input_high);
	
	// deallocate result set
	mysql_stmt_free_result(m_stmt);
	
	return data;
}

