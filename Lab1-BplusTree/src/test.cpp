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
    auto v = b.bfs();
    for(auto& e : v) std::cout<<e<<' ';
    
    std::cout<<std::endl;

    b.insertar(2);
    b.insertar(3);
    b.insertar(0);
    v = b.bfs();
    for(auto& e : v) std::cout<<e<<' ';
}

