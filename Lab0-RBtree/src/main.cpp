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
  r.insert(1);
  r.insert(0);
  r.insert(7);
  r.insert(8);
  r.insert(9);
  if(r.isPresent(5)) std::cout << "5 está presente\n";
  if(!r.isPresent(10)) std::cout << "10 no está presente\n";

  auto print = [](int const& k){ std::cout << k << " ";};
  std::cout << "\nPreorder: "; r.forEachPre(print);
  std::cout << "\nInorder: "; r.forEachIn(print);
  std::cout << "\nPostorder: "; r.forEachPos(print);
  std::cout << "\nFast (no order): "; r.forEachFast(print);
  std::cout << "\n\n";

  return 0;
}