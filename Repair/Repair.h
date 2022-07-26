#ifndef REPAIR_H
#define REPAIR_H
#include <map>
#include <fstream>
#include "Heap.h"
#include "DList.h"


using namespace std; 


class Repair{
    private:
        DList* seq;
        int mayor = 30;
        Heap* hp; 
        map<pair<int, int>,Data*> mp;
        ofstream* key_output;
        void crearHeap();
        void buscaOcurrencias(Nodo*);
        void actualizaHeap(Nodo*, Nodo*);
        void insertarHeap(Nodo*, Nodo*);
        void actualizarLista(Nodo*, Nodo*);
        int countReemplace;
        
    public:
        Repair(vector<int>* datos, ofstream* );
        Repair(DList*, ofstream*);
        ~Repair(); 
        void cambiar();
        void prints();
        void crearSeq(vector<int>* datos); 
        DList* getSeq();
        bool Compressible();
};



#endif