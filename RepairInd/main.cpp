#include <time.h>
#include "Controller.h"
#include "metrictime2.hpp"

using namespace std;

int main(int argc, char const *argv[]){
	srand(time(NULL));
	
    if(argc != 3){
        cout<<" uso: "<<argv[0]<<" \"n°hebras\" \"n°datos \"" << endl;
        return 1;
    }

    int nThreads = atoi(argv[1]);
    long long int nDatos = atoi(argv[2]);
	
    if(nThreads > nDatos) nThreads = nDatos;
    cout << "Threads: "<< nThreads << ", Datos: "<< nDatos << endl; 

	vector<int> datos; 
	for(long long int i = 0; i<nDatos; i++)  datos.push_back(1);
	cout << endl;
	

	Controller* contr = new Controller(&datos);
	TIMERSTART(SEQ);
	contr->Sequential();
	DList* d1 = contr->getSeq();
	TIMERSTOP(SEQ);


	TIMERSTART(PAR);
	contr->Parallel(nThreads);
	DList* d2 = contr->getSeq();
	TIMERSTOP(PAR);

	if(d1 != NULL) d1->prints();
	if(d2 != NULL) d2->prints();


	delete(contr);
	delete(d1);
	delete(d2);

	
	


	return 0;
}



