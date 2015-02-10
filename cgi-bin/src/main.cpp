#include "Generator.h"

Log logger;
Generator gen;


void worker_thread(const int job_id) {

	logger.write("worker thread started for job: " + Utils::numToStr(job_id));
	
	// init database connection
	DBManager dbm = DBManager("localhost", "appAdmin", "GWCtMcS^@M", "vis_demo", 0);
	DBJob job = dbm.get_job(job_id);
	dbm.close();
	
	string new_vertices_str = "", new_edges_str = "";
	bool flag = true;
	int vertices_num = 0, egdes_num = 0;
	double mod = 0;
	
	// as a side effect new_vertices_str and new_edges_str are modified to contain the result
	gen.run(job, new_vertices_str, new_edges_str, vertices_num, egdes_num, mod, flag);

	dbm = DBManager("localhost", "appAdmin", "GWCtMcS^@M", "vis_demo", 0);
	dbm.update_completed(job_id, new_vertices_str, new_edges_str, vertices_num, egdes_num, mod);
	dbm.close();
	
	logger.write("worker thread completed for job: " + Utils::numToStr(job_id));
}

int main(int argc, char* argv[])
{
	logger.init("log.out");
	
	gen.init();
	
	//****************....finished preloading....**************************
	logger.write("Preloading finished, waiting for new jobs.");
	
	// init database connection
	DBManager dbm = DBManager("localhost", "appAdmin", "GWCtMcS^@M", "vis_demo", 1);
	while (true) {
		vector<int> job_ids = dbm.check_jobs();
		
		// if new jobs exists
		if (job_ids.size() > 0) {
			
			// update jobs as processing
			for (vector<int>::size_type i = 0; i != job_ids.size(); i++) {
				dbm.update_processing(job_ids[i]);
				logger.write("update processing " + Utils::numToStr(job_ids[i]));
			}
			
			// spawn enough threads to handle jobs
			for (vector<int>::size_type i = 0; i != job_ids.size(); i++) {
				logger.write("processing job " + Utils::numToStr(job_ids[i]));
				boost::thread t(&worker_thread, job_ids[i]);
			}
		}
		
		sleep(2);
		job_ids.clear();
	}
	
	// clean up and exit
	dbm.close();
	
	return 0;
}
