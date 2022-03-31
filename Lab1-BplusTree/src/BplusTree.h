#ifndef BPLUSTREE_H
#define BPLUSTREE_H

struct Hoja{
    
    // Constructor
    Hoja(int);
};


struct Nodo{
    // Constructor
    Nodo(int);
};

class BplusTree
{
public:
    BplusTree();
    void insertar(int);
    void borrar(int);

    ~BplusTree();
};






#endif