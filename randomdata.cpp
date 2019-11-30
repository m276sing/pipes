#include <memory>
//#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"
#include <vector>
#include <iostream>
#include <pthread.h>
#include<sstream>
#include <algorithm>
#include <time.h>
#include<random>
#include <signal.h>
#include <iomanip>

pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond3 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock3 = PTHREAD_MUTEX_INITIALIZER;


void printThreadTime(pthread_t thread_id,std::string msg) {
	clockid_t cid;
	struct timespec ts;
	if (!pthread_getcpuclockid(thread_id, &cid)) {
		struct timespec ts; 
		if (clock_gettime(cid, &ts) == -1)
		{
			//handle error	
		}
		//printf("%4ld.%03ld\n", ts.tv_sec, ts.tv_nsec / 1000000);
		//std::cout << ts.tv_sec << " " << ts.tv_nsec << std::endl;
		std::cout << msg<< (ts.tv_sec * 1000000) + (ts.tv_nsec / 1000)<<std::endl;
		
	}
		
}

int randon_number_generator(int lower, int upper) {
	std::random_device device;
	std::mt19937 rnge(device());
	std::uniform_int_distribution<int> distribution(lower, upper);
	return distribution(rnge);
}
struct arguments {
	int V;
	std::vector<std::vector<int>> E_arr;
};
std::vector<std::vector<int>> parse_input(std::string input, int vertices_no) {

	std::istringstream ss(input);
	std::string token1, token2;
	ss >> token1 >> token2;
	token2.erase(0, 1);
	token2.erase(token2.length() - 1);
	std::string delim = ">,";
	token2 += ">,";
	std::vector<std::vector<int>> input_vector;
	size_t pos = 0;
	std::string substring;
	while ((pos = token2.find(delim)) != std::string::npos) {

		substring = token2.substr(0, pos);
		std::string delim2 = ",";
		std::string substr1 = "";
		std::string substr2 = "";
		size_t index = 0;
		index = substring.find(delim2);
		substr1 = substring.substr(0, index);
		substr2 = substring.substr(++index);
		std::istringstream s1(substr1.substr(1));//string stream is used to 
		std::istringstream s2(substr2);
		int n1 = 0;
		int n2 = 0;
		s1 >> n1;
		s2 >> n2;
		//if (n1 >= vertices_no || n2 >= vertices_no) //if a vertex doesn't lie in vertices then throw an exception
		//	throw VertextNumberEception();

		std::vector<int> temp;
		temp.push_back(n1);
		temp.push_back(n2);
		input_vector.push_back(temp);
		token2.erase(0, pos + delim.length());
	}
	return input_vector;
}

void* approx_vc_2(void* _args) {
	
	pthread_mutex_lock(&lock3);
	pthread_cond_wait(&cond3, &lock3);
	struct arguments* args = (struct arguments*)_args;
	int V = args->V;
	std::vector<std::vector<int>> E_arr = args->E_arr;
	std::vector<int> vertex_cover;

	//int i = 0;
	while (E_arr.size() != 0) {
		int i = randon_number_generator(0, E_arr.size()-1);
		int a = E_arr[i][0];
		int b = E_arr[i][1];
		vertex_cover.push_back(a);
		vertex_cover.push_back(b);
		E_arr.erase(E_arr.begin() + i);
		for (int j = 0; j < E_arr.size(); j++) {
			if (a == E_arr[j][0] || a == E_arr[j][1] || b == E_arr[j][1] || b == E_arr[j][0]) {
				E_arr.erase(E_arr.begin() + j);
				j--;
			}
		}
	}
	std::sort(vertex_cover.begin(), vertex_cover.end());
	printThreadTime(pthread_self(), "VC-2 CPU Time:");
	std::cout << "APPROX-VC-2: ";
	for (size_t i = 0; i < vertex_cover.size(); i++) {
		std::cout << vertex_cover[i] << " ";
	}
	std::cout << std::endl;
	

	pthread_mutex_unlock(&lock3);
}
void* approx_vc_1(void *_args) {

	pthread_mutex_lock(&lock2);
	pthread_cond_wait(&cond2, &lock2);

	struct arguments *args = (struct arguments*)_args;
	int V = args->V;
	std::vector<std::vector<int>> E_arr = args->E_arr;

	std::vector<int> vertex_cover;
	for (int i = 0; i < V; i++) {

		if (E_arr.size() == 0)
			break;
		int degree = 0;
		int vertex;
		for (int k = 0; k < V; k++) {

			int count = 0;
			for (int j = 0; j < E_arr.size(); j++) {

				if (E_arr[j][0] == k || E_arr[j][1] == k) {
					count++;
				}
			}
			if (count > degree) {
				degree = count;
				vertex = k;
			}
		}
		vertex_cover.push_back(vertex);
		for (int k = 0; k < E_arr.size(); k++) {
			if (E_arr[k][0] == vertex || E_arr[k][1] == vertex) {
				E_arr.erase(E_arr.begin() + k);
				k--;
			}
		}


	}

	std::sort(vertex_cover.begin(), vertex_cover.end());
	printThreadTime(pthread_self(), "VC-1 CPU Time:");
	std::cout << "APPROX-VC-1: ";
	for (size_t i = 0; i < vertex_cover.size(); i++) {
		std::cout << vertex_cover[i] << " ";
	}
	std::cout <<std::endl;
	
	//pthread_cond_wait(&cond2, &lock2);
	
	pthread_cond_signal(&cond3);
	pthread_mutex_unlock(&lock2);
	return NULL;
}

sig_atomic_t thread_status;
#define RUNNING         0
#define FINISHED        1

void CleanThread(void* args) {
	thread_status = FINISHED;
}

//minisat sort remaining
void* min_vertex_cover(void *_args) {
	
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	pthread_cleanup_push(CleanThread, NULL);
	pthread_mutex_lock(&lock1);
	struct arguments *args = (struct arguments *)_args;
	int V = args->V;
	std::vector<std::vector<int>> E_arr = args->E_arr;

	std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());

	for (int k = 1; k < V; k++) {
		std::vector<std::vector<Minisat::Lit>> x_vector;
		
		Minisat::vec<Minisat::Lit> clause_vector;
		for (int i = 0; i < V; i++) {
			std::vector<Minisat::Lit> x_vec;
			for (int j = 0; j < k; j++) {
				x_vec.push_back(Minisat::mkLit(solver->newVar()));
			}
			x_vector.push_back(x_vec);
		}

		
		for (int i = 0; i < k; i++) {
			for (int j = 0; j < V; j++) {
				
				clause_vector.push(x_vector[j][i]);
			}
			solver->addClause(clause_vector);
			clause_vector.clear();
		}
		
		for (int m = 0; m < V; m++) {
			for (int p = 0; p < k; p++) {
				for (int q = p + 1; q < k; q++) {
					clause_vector.push(~x_vector[m][p]);
					clause_vector.push(~x_vector[m][q]);
					solver->addClause(clause_vector);
					clause_vector.clear();
				}

			}
		}
	
		for (int m = 0; m < k; m++) {
			for (int p = 0; p < V; p++) {
				for (int q = p + 1; q < V; q++) {
					clause_vector.push(~x_vector[p][m]);
					clause_vector.push(~x_vector[q][m]);
					solver->addClause(clause_vector);
					clause_vector.clear();
				}
			}
		}
	

		for (size_t count = 0; count < E_arr.size(); count++) {
			int i = E_arr[count][0];
			int j = E_arr[count][1];
			for (int m = 0; m < k; m++) {
				clause_vector.push(x_vector[i][m]);
				clause_vector.push(x_vector[j][m]);
			}
			solver->addClause(clause_vector);
			clause_vector.clear();
		}
		bool res = solver->solve();
		//std::vector<
		std::vector<int> vertex_cover;
		if (res) {

			
			for (int i = 0; i < V; i++) {
				for (int j = 0; j < k; j++) {
					if (solver->modelValue(x_vector[i][j]) == Minisat::l_True)
						vertex_cover.push_back(i);
						//std::cout << i << " ";
				}

			}
			printThreadTime(pthread_self(), "CNF CPU Time:");
			std::cout << "CNF-SAT-VC: ";
			for (size_t i = 0; i < vertex_cover.size(); i++) {
				std::cout << vertex_cover[i] << " ";
			}

			std::cout << std::endl;
			break;
		}
		solver.reset(new Minisat::Solver());
	}
	//pthread_cond_wait(&cond1, &lock1);
	
	thread_status = FINISHED;
	pthread_cond_signal(&cond2);/* Now allow 3rd thread to process */
	pthread_mutex_unlock(&lock1);
	pthread_cleanup_pop(1);
	return NULL;
}



void ControlTime(pthread_t thread_id, int time_limit) {

	clockid_t cid;
	struct timespec ts;
	if (pthread_getcpuclockid(thread_id, &cid)) {
		/* Error */
		std::cerr << "Error: can't get cpu clock of the thread" << std::endl;
	}

	while (thread_status == RUNNING) {
		clock_gettime(cid, &ts);
		//std::cout << cid;
		//std::cout << "time while running:" << std::setprecision(6) << ts.tv_nsec/1000 << std::endl;
		if (ts.tv_sec > time_limit) {
			
			pthread_cancel(thread_id);
			std::cout << "CNF-SAT-VC: timeout in the output"<<std::endl;
			pthread_cond_signal(&cond2);
			return;
		}
	}
	clock_gettime(cid, &ts);
//	std::cout << "time while Finish:" << std::setprecision(6)<< ts.tv_nsec/1000 << std::endl;
}
int main(void) {

	int V;
	while (!std::cin.eof()) {

		std::string input;
		std::getline(std::cin, input);
		if (input[0] == 'V') {
			std::istringstream ss(input.substr(2));
			ss >> V;
		}
		else if (input[0] == 'E') {
			std::vector<std::vector<int>> E_arr;
			E_arr = parse_input(input, V);
			struct arguments *args = new arguments;
			args->V = V;
			args->E_arr = E_arr; 

			pthread_t thread1, thread2, thread3;
			thread_status = RUNNING;
			int time_limit = 120;
			pthread_create(&thread1, NULL, min_vertex_cover, args);
			pthread_create(&thread2, NULL, approx_vc_1, args);
			pthread_create(&thread3, NULL, approx_vc_2, args);
			
			ControlTime(thread1, time_limit);
			/*printThreadTime(thread1, "CNF CPU Time:");
			printThreadTime(thread2, "VC-1 CPU Time:");
			printThreadTime(thread3, "VC-12 CPU Time:");*/
			pthread_join(thread1, NULL);
			pthread_join(thread2, NULL);
			pthread_join(thread3, NULL);
			printThreadTime(thread1, "CNF CPU Time:");
			printThreadTime(thread2, "VC-1 CPU Time:");
			printThreadTime(thread3, "VC-12 CPU Time:");
		}

	}


	return 0;
}
