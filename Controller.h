#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <iostream> 
#include <fstream>
#include <omp.h>
#include "DList.h"
#include "Repair.h"



class Controller{
    public:
        Controller(vector<int>*); 
        ~Controller();
        void Sequential();
        void Parallel(int);
        DList* getSeq();
    private: 
        //var
        int maxSeq = 200000; 
        vector<int>* datos;
        long long int seqSize; 
        DList* resSeq = NULL;
        ofstream key_output_seq;
        ofstream key_output_par;
        //methods
        void mergeDList(DList*, DList*);
        void RepairParallel();
        void RepairSequential();
};

#endif