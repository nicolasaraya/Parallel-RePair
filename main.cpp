#include <time.h>
#include "Controller.h"
#include "metrictime2.hpp"

using namespace std;

const string pathIn = "./input/file.fna"; 

//A = 0
//G = 6
//C = 2
//T = 19


void test(int nThreads, long long int nDatos, int option);
void readFile(int nThreads, int option);
void makeFile(DList* d, int id); 

int main(int argc, char const *argv[]){
	srand(time(NULL));
	
    if(argc == 4) test(atoi(argv[1]), atoll(argv[2]), atoi(argv[3]));
	if(argc == 3) readFile(atoi(argv[1]),atoi(argv[2]));
    
	return 0;
}


void readFile(int nThreads, int option){
	ifstream input; 
	input.open(pathIn);
	string aux;

	std::getline(input, aux); //trash
	aux.clear();
	vector<int> datos; 

	while(std::getline(input,aux)){
		for(auto i : aux){
			datos.push_back(i - 65);
		}
		aux.clear();
	}

	input.close(); 

	DList* d1 = NULL;
	DList* d2 = NULL;

	if(option == 0 or option == 2){
		Controller* contr = new Controller(&datos);
		TIMERSTART(SEQ);
		contr->Sequential();
		d1 = contr->getSeq();
		TIMERSTOP(SEQ);

		makeFile(d1, 0);
	}

	if(option == 1 or option == 2){
		Controller* contr = new Controller(&datos);
		TIMERSTART(PAR);
		contr->Parallel(nThreads);
		d2 = contr->getSeq();
		TIMERSTOP(PAR);

		makeFile(d2, 1);
	}
	
	if(d1 != NULL) delete(d1);
	if(d2 != NULL) delete(d2);


}

void test(int nThreads, long long int nDatos, int option){	
    if(nThreads > nDatos) nThreads = nDatos;
    cout << "Threads: "<< nThreads << ", Datos: "<< nDatos << endl; 

	vector<int> datos; 
	for(long long int i = 0; i<nDatos; i++)  datos.push_back(1);
	cout << endl;
	
	DList* d1 = NULL;
	DList* d2 = NULL;

	if(option == 0 or option == 2){
		Controller* contr = new Controller(&datos);
		TIMERSTART(SEQ);
		contr->Sequential();
		d1 = contr->getSeq();
		TIMERSTOP(SEQ);
		if(d1 != NULL) d1->prints();
		cout << endl;
	}

	if(option == 1 or option == 2){
		Controller* contr = new Controller(&datos);
		TIMERSTART(PAR);
		contr->Parallel(nThreads);
		d2 = contr->getSeq();
		TIMERSTOP(PAR);
		if(d2 != NULL) d2->prints();
		delete(contr);
	}
	
	if(d1 != NULL) delete(d1);
	if(d2 != NULL) delete(d2);
}


void makeFile(DList* d, int id){
	ofstream out;

	time_t rawtime; 
    struct tm* info; 
    time(&rawtime);
    info = localtime(&rawtime);
    string pathaux = "_" + to_string(info->tm_hour) + "_" + to_string(info->tm_min) + "_" + to_string(info->tm_sec) + "__"  + to_string(info->tm_year+1900) +  "-" + to_string(info->tm_mon) + "-" + to_string(info->tm_mday)  ;

	if (id == 0) out.open("./output/compressed/SeqCompressedFile" + pathaux + ".fna", std::ofstream::out | std::ofstream::trunc);
	if (id == 1) out.open("./output/compressed/ParallelCompressedFile" + pathaux + ".fna", std::ofstream::out | std::ofstream::trunc);

	Iterator it = d->begin();
	while(it.hasNext()){
		out << it.next()->getNum();
	}

	out.close();

}