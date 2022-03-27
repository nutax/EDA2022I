#include <iostream>
#include "rbtree.h"

/* Hice todo desde 0 y con un mejor manejo de allocamientos*/
/* Toda la implementación está en rbtree.h */
/* Se puede pasar a templates con facilidad */

int main() {
  RBtree<int> rb;

  /* Insertion */
  rb.insert(5);
  rb.insert(3);
  rb.insert(2);
  rb.insert(6);
  rb.insert(4);
  rb.insert(1);
  rb.insert(0);
  rb.insert(7);
  rb.insert(8);
  rb.insert(9);

  /* Lookup */
  if(rb.contains(5)) std::cout << "5 está presente\n";
  if(rb.contains(9)) std::cout << "9 está presente\n";
  if(!rb.contains(10)) std::cout << "10 no está presente\n";

  /* Iteration */
  auto print = [](int const& k){ std::cout << k << " ";};
  std::cout << "\nPreorder: "; rb.forEachPre(print);
  std::cout << "\nInorder: "; rb.forEachIn(print);
  std::cout << "\nPostorder: "; rb.forEachPos(print);
  std::cout << "\nFast (no order): "; rb.forEachFast(print);
  std::cout << "\n\n";

  return 0;
}