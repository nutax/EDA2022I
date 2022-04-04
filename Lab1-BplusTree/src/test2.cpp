#include "BplusTree.h"



int main() {
    BplusTree b;
    for(int i = 1; i<21; ++i) b.insertar(i);
    
    for(int i = 12; i>=6; --i) b.borrar(i);

    for(int i = 13; i<19; ++i) b.borrar(i);

    b.borrar(5);
    b.borrar(4);
    b.borrar(19);
    
    b.print();
    
}

