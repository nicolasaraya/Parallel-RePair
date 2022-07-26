#include "Controller.h"

using namespace std;

Controller::Controller(vector<int>* datos){
    this->datos = datos;
    seqSize = datos->size();
    
    time_t rawtime; 
    struct tm* info; 
    time(&rawtime);
    info = localtime(&rawtime);
    string pathaux = "_" + to_string(info->tm_hour) + "_" + to_string(info->tm_min) + "_" + to_string(info->tm_sec) + "__"  + to_string(info->tm_year+1900) +  "-" + to_string(info->tm_mon) + "-" + to_string(info->tm_mday)  ;

    key_output_seq.open("./output/keys_output_seq" + pathaux + ".txt", std::ofstream::out | std::ofstream::trunc); //limpia el contenido del fichero
    key_output_par.open("./output/keys_output_par" + pathaux + ".txt", std::ofstream::out | std::ofstream::trunc); //limpia el contenido del fichero
    key_output_seq.clear();
    key_output_par.clear();
}


Controller::~Controller(){
    key_output_par.close();
    key_output_seq.close();
}

void Controller::Sequential(){
    if(seqSize > maxSeq){
        unsigned int parts = ( seqSize / maxSeq ) + 1;
        vector< vector<int> > subDatos(parts, vector<int>(0,0));

        cout << "Sequential steps: " << parts << endl;

        long long int k = 0;

        for(int i = 0; i < parts; i++){
            if(k == seqSize) break;
            for(int j = 0; j < maxSeq; j++){
                    if(k == seqSize) break;
                    subDatos.at(i).push_back(datos->at(k));
                    k++;
            }
        }

        Repair* r;
        vector<DList*> res(parts, new DList());

        for(int i = 0 ; i < parts; i++){
            r = new Repair(&subDatos.at(i), &key_output_seq);
            r->cambiar();
            res.at(i) = r->getSeq();
            delete(r);
        }

        for(int i = parts-1; i>0; i--){
            if(res.at(i)->size() == 0) continue;
            if(res.at(i)->size()==1){
                res.at(i-1)->insertEnd(res.at(i)->first()->getNum());
                continue;
            } 
            mergeDList(res.at(i-1), res.at(i));
            res.pop_back();
        }
        resSeq = res.at(0);

        r = new Repair(res.at(0), &key_output_par);
        while(r->Compressible()){
            r->cambiar();
            resSeq = r->getSeq();
            if(r!=NULL) delete(r);
            r = new Repair(resSeq, &key_output_par);
        }
        if(r!=NULL) delete(r);
    }
    else{
        Repair* r = new Repair(datos, &key_output_seq);
        r->cambiar();
        resSeq = r->getSeq();
        return; 
    }
}


void Controller::Parallel(int nThreads){
	unsigned long long int nDatos = seqSize;
    unsigned int nParts = nThreads;
	unsigned int partSize = nDatos/nParts; 
    while(partSize > maxSeq){
        nParts++;
        partSize = nDatos/nParts;
    }
    vector< vector<int> > subDatos(nParts, vector<int>(0,0));

    cout << "Parallel parts: " << nParts  << ", size: " << partSize << endl;

    unsigned long long int k = 0;
    for(int i = 0; i < nParts; i++){
        for(int j = 0; j < partSize; j++){
            subDatos.at(i).push_back(datos->at(k));
            k++;
        }
    }

    while(k!=datos->size()){ //relleno el ultimo con los datos que queden. 
        subDatos.at(nParts-1).push_back(datos->at(k));
        k++;
    }
    
    vector<DList*> res(nParts, new DList());
    
    omp_set_num_threads(nThreads);

    for(int i = 0; i < nParts; i+=nThreads){
        #pragma omp parallel for
        for(int j = i; j < nThreads; j++){
            Repair* r = new Repair(&subDatos.at(j), &key_output_par);
            r->cambiar();
            res.at(j) = r->getSeq();
            delete(r);
        }
    }

    for(int i = nThreads-1; i>0; i--){
        mergeDList(res.at(i-1), res.at(i));
        res.pop_back();
    }

    Repair* r = NULL;
    r = new Repair(res.at(0), &key_output_par);
    while(r->Compressible()){
        r->cambiar();
        resSeq = r->getSeq();
        if(r!=NULL) delete(r);
        r = new Repair(resSeq, &key_output_par);
    }
    if(r!=NULL) delete(r);
    
	return;

}

void Controller::mergeDList(DList* a, DList* b){
    Nodo* tailA = a->back();
    Nodo* firstB = b->first();
    a->merge(a,b);
    tailA->setNext(firstB);
    firstB->setPrev(tailA);
}

DList* Controller::getSeq(){
    return resSeq;
}