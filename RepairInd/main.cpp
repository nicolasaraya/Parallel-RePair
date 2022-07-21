#include "Repair.h"
#include "RepairParallelInd.h"
#include "metrictime2.hpp"
#include <time.h>
#include <cstdint>
#include <chrono>
using namespace std;

int main(int argc, char const *argv[]){
	srand(time(NULL));
	vector<int> datos; 
	
    if(argc != 3){
        cout<<" uso: "<<argv[0]<<" \"n°hebras\" \"n°datos \"" << endl;
        return 1;
    }
    int nThreads = atoi(argv[1]);
    long long int nDatos = atoi(argv[2]);
    if(nThreads > nDatos) nThreads = nDatos;

    cout << "Threads: "<< nThreads << ", Datos: "<< nDatos << endl; 


	DList* d = new DList();
	for(int i = 0; i<nDatos; i++) {
		datos.push_back(1);
		d->insertEnd(1);
	}
	//for(auto i : datos) cout << i << " ";
	cout << endl;
	

	double countS = 0;
	double countP = 0; 
	for(int i = 0; i < 1; i++){
		std::chrono::time_point<std::chrono::high_resolution_clock> a,b;
		a = std::chrono::high_resolution_clock::now();
		Repair* r = new Repair(&datos);
		r->cambiar();
		r->prints();

		b = std::chrono::high_resolution_clock::now();   
		std::chrono::duration<double> delta = b-a;        
        //std::cout << "# elapsed time ("<< "Sequential" <<"): " << delta.count()  << "s" << std::endl;
		countS += delta.count();

		delete(r);
	
		a = std::chrono::high_resolution_clock::now();
		RepairParallelInd(&datos, nThreads);
		b = std::chrono::high_resolution_clock::now();   
		delta = b-a;        
        //std::cout << "# elapsed time ("<< "Parallel" <<"): " << delta.count()  << "s" << std::endl;
		countP += delta.count();
	}

	cout << "Secuencial promedio: " << double(countS)/10 << endl; 
	cout << "Paralelo promedio: " << double(countP)/10 << endl; 
	
	


	return 0;
}



