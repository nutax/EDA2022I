#include <iostream>
#include "rbtree.h"

/* Hice todo desde 0 y con un mejor manejo de allocamientos*/
/* Toda la implementación está en rbtree.h */
/* Se puede pasar a templates con facilidad */

int main() {
  RBtree r;
  r.insert(5);
  r.insert(3);
  r.insert(2);
  r.insert(6);
  r.insert(4);
  if(r.isPresent(5)) std::cout << "Hello World!\n";
  return 0;
}