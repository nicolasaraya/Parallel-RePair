#include <time.h>
#include "Controller.h"
#include "metrictime2.hpp"

using namespace std;

const string file = "cat.fna";  //para leer otro file editar esto. 
//A = 0
//G = 6
//C = 2
//T = 19

void test(int nThreads, int option, long long int nDatos);
void readFile(int nThreads, int option);
void makeFile(DList* d, int id, string pathaux); 
string info();

int main(int argc, char const *argv[]){
	srand(time(NULL));
    if(argc == 4) test(atoi(argv[1]), atoi(argv[2]), atoll(argv[3]));
	if(argc == 3) readFile(atoi(argv[1]),atoi(argv[2]));
	return 0;
}


void readFile(int nThreads, int option){
	ifstream input; 
	input.open("./input/" + file);
    string pathaux = info(); 
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
	
	if(option == 1 or option == 2){
		Controller* contr = new Controller(&datos, pathaux, 1);
		TIMERSTART(PAR);
		contr->Parallel(nThreads);
		d2 = contr->getSeq();
		TIMERSTOP(PAR);
		makeFile(d2, 1, pathaux);
        delete(contr);
	}
	
	if(option == 0 or option == 2){
		Controller* contr = new Controller(&datos, pathaux, 0);
		TIMERSTART(SEQ);
		contr->Sequential();
		d1 = contr->getSeq();
		TIMERSTOP(SEQ);
		makeFile(d1, 0, pathaux);
        delete(contr);
	}

	if(d1 != NULL) delete(d1);
	if(d2 != NULL) delete(d2);
}

void test(int nThreads, int option, long long int nDatos){	
    if(nThreads > nDatos) nThreads = nDatos;
    cout << "Threads: "<< nThreads << ", Datos: "<< nDatos << endl; 

	vector<int> datos; 
	for(long long int i = 0; i<nDatos; i++)  datos.push_back(1);
	cout << endl;
	
	DList* d1 = NULL;
	DList* d2 = NULL;

	if(option == 0 or option == 2){
		Controller* contr = new Controller(&datos, info(), 0);
		TIMERSTART(SEQ);
		contr->Sequential();
		d1 = contr->getSeq();
		TIMERSTOP(SEQ);
		if(d1 != NULL) d1->prints();
		cout << endl;
        delete(contr);
	}

	if(option == 1 or option == 2){
		Controller* contr = new Controller(&datos, info(), 1);
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


void makeFile(DList* d, int id, string pathaux){
	ofstream out;
    string aux = file;
    aux.pop_back();
    aux.pop_back();
    aux.pop_back();
    aux.pop_back();
	if (id == 0) out.open("./output/compressed/" + aux + "_sequential" + pathaux + ".fna", std::ofstream::out | std::ofstream::trunc);
	if (id == 1) out.open("./output/compressed/"  + aux + "_parallel" + pathaux + ".fna", std::ofstream::out | std::ofstream::trunc);

	Iterator it = d->begin();
	while(it.hasNext()){
		out << it.next()->getNum() << " ";
	}

	out.close();

}


std::string info(){
    time_t rawtime; 
    struct tm* info; 
    time(&rawtime);
    info = localtime(&rawtime);
    std::string pathaux = "_";
    pathaux += to_string(info->tm_year+1900);
    if(info->tm_mon < 10) pathaux += "0";
    pathaux += to_string(info->tm_mon);
    if(info->tm_mday < 10) pathaux += "0";
    pathaux += to_string(info->tm_mday);
    pathaux += "_"; 
    pathaux += to_string(info->tm_hour);
    pathaux += to_string(info->tm_min); 
    pathaux += to_string(info->tm_sec);
    return pathaux; 
}
