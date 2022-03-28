#include <iostream>
#include "rbtree.h"

/* Hice todo desde 0 y con un mejor manejo de allocamientos*/
/* Toda la implementación está en rbtree.h */
/* Se puede pasar a templates con facilidad */


/* 
Métodos públicos (Interfaz)
  1. insert
  2. insertar (llama insert)
  3. erase
  4. eliminarNodo (llama erase)
  5. contains
  6. forEachIn
  7. inorden (llama forEachIn -> vector)
  8. forEachPre
  9. preorden (llama forEachPre -> vector)
  10. forEachPos
  11. posorden (llama forEachPos -> vector)
  12. forEachFast
*/


int main() {
  RBtree rb;

  /* Ejemplo con métodos de nombres estandar */

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

  /* Erasure */
  rb.erase(4);
  rb.erase(8);

  /* Iteration */
  auto print = [](int const& k){ std::cout << k << " ";};
  std::cout << "\nPreorder: "; rb.forEachPre(print);
  std::cout << "\nInorder: "; rb.forEachIn(print);
  std::cout << "\nPostorder: "; rb.forEachPos(print);
  std::cout << "\nFast (no order): "; rb.forEachFast(print);
  std::cout << "\n\n";




  /* Ejemplo con métodos con nombres solicitados */

  /* Insertion */
  rb.insertar(5);
  rb.insertar(3);
  rb.insertar(2);
  rb.insertar(6);
  rb.insertar(4);
  rb.insertar(1);
  rb.insertar(0);
  rb.insertar(7);
  rb.insertar(8);
  rb.insertar(9);

  /* Erasure */
  rb.eliminarNodo(4);
  rb.eliminarNodo(8);

  /* Iteration */
  const auto inorder = rb.inorden();
  const auto preorder = rb.preorden();
  const auto posorder = rb.posorden();
  std::cout << "\nPreorder: ";for(auto const& k : inorder) std::cout << k << " ";
  std::cout << "\nInorder: "; for(auto const& k : preorder) std::cout << k << " ";
  std::cout << "\nPostorder: "; for(auto const& k : posorder) std::cout << k << " ";
  std::cout << "\n\n";
  return 0;
}