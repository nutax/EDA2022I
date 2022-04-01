#include "BplusTree.h"



int main() {
    BplusTree b;
    b.insertar(1);
    b.insertar(4);
    b.insertar(7);
    b.insertar(10);
    b.insertar(17);
    b.insertar(21);
    b.insertar(31);
    b.insertar(25);
    b.insertar(19);
    b.insertar(20);
    b.insertar(28);
    b.insertar(42);
    b.insertar(2);
    b.insertar(3);
    b.insertar(0);
    b.bfs();
}

