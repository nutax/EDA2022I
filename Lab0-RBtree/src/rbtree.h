#include <vector>
#include <queue>
#include <functional>

/* By José Ignacio Huby Ochoa */
/* La teoria y el algoritmo se baso de https://www.happycoders.eu/algorithms/red-black-tree-java */
/* Pero NO se copio el código, solo se uso de referencia. */


//-----------------------------------------------------------------------------------------------------------------//
/* IMPORTANTE: Las reglas se estiran y se incumplehn en algunos casos, pero la funcionalidad termina siendo igual */
//-----------------------------------------------------------------------------------------------------------------//


/* 
Métodos públicos (Interfaz)
  1. insert
  2. insertar (llama insert)
  3. erase
  4. eliminarNodo (llama erase)
  5. contains
  6. forEachIn
  7. inorder (llama forEachIn -> vector)
  8. forEachPre
  9. preorder (llama forEachPre -> vector)
  10. forEachPos
  11. posorder (llama forEachPos -> vector)
  12. forEachFast
*/

//La explicacion de estos macros esta en la sección de memoria, más abajo
#define PARENT(id) mem[id].p
#define RIGHT(id) mem[id].r
#define LEFT(id) mem[id].l
#define KEY(id) mem[id].k
#define COLOR(id) mem[id].c

#define RED 0
#define BLACK 1

#define IS_BLACK(id) (id == null || COLOR(id) == BLACK)

/* 5 parts: Types, Constants, Memory, Internal procedures, Interface */

//template<typename K> // Descomentar y comentar using K, para usar Templates
class RBtree{

using K = int; //Para reemplazar con facilidad o sustituirlo por un Template
using ID = unsigned; // Para sustituir el uso de punteros y por posiciones relativas en un vector
using COL = bool; // Para señalar el color de un nodo
struct Node{K k; ID p,l,r; COL c;}; //Estructura del nodo

/* Constants */
constexpr static ID initial_capacity = 512; // Se alloca esta cantidad de nodos iniciales para evitar reallocaciones
constexpr static ID null = ((1 << (sizeof(ID)*8 - 2) - 1) * 2) + 1; //Sustituto de nullptr para usar propio manejo de memoria
constexpr static ID nil = 0; //Referncia a un nodo especial (ubicado en la posicion 0 del vector) para poder realizar la eliminación correctamente


/* Memory */ 
// En esta implementacion busco reducir la cantidad de peticiones al sistema operativo para allocar memoria
// Para ello, utilizo un vector como una forma rapida y simple de tener mi propia gestion de memoria
// Los índices del vector son mis punteros. En lugar de hacer ptr->k, se utiliza como mem[id].k
// Para facilitar la escritura se crearon macros.
ID root = null; 
std::vector<Node> mem;


/* Internal procedures */

// La dirección relativa de un nuevo nodo es la posición final del vector
int newNode(K const& k){
  const ID new_id = mem.size();
  mem.push_back({k, null, null, null, RED});
  return new_id;
}

// Se busca que los nodos esten compactos en memoria para facilitar su gestion
// Si se elimina un nodo se sustituye el último elemento por este y se elimina el último.
void deleteNode(ID current){
  const ID last = mem.size() - 1;
  if(last != current){
    const ID parent = PARENT(last), left = LEFT(last), right = RIGHT(last);
    if(parent != null)if(last == LEFT(parent)) LEFT(parent) = current; else RIGHT(parent) = current;
    if(left != null) PARENT(left) = current;
    if(right != null) PARENT(right) = current;
    mem[current] = mem[last];
  }
  mem.pop_back(); 
}

// Para cambiar un hijo por otro
void changeChild(ID parent, ID old_child, ID new_child){
  if (parent == null) root = new_child;
  else if (LEFT(parent) == old_child) LEFT(parent) = new_child;
  else RIGHT(parent) = new_child;
  
  if(new_child != null) PARENT(new_child) = parent;
}

// Para rotar a la derecha como en un BST
void rotateRight(ID current){
  ID parent = PARENT(current);
  ID left = LEFT(current);

  LEFT(current) = RIGHT(left);
  if(RIGHT(left) != null){
    PARENT(RIGHT(left)) = current;
  }

  RIGHT(left) = current;
  PARENT(current) = left;

  changeChild(parent, current, left);
}

// Para rotar a la izquierda como en un BST
void rotateLeft(ID current){
  ID parent = PARENT(current);
  ID right = RIGHT(current);

  RIGHT(current) = LEFT(right);
  if(LEFT(right) != null){
    PARENT(LEFT(right)) = current;
  }

  LEFT(right) = current;
  PARENT(current) = right;

  changeChild(parent, current, right);
}

// Para buscar como en BST
ID searchNode(K const& k){
  ID current = root;
  while (current != null){
    if(k < KEY(current)) current = LEFT(current);
    else if (k > KEY(current)) current = RIGHT(current);
    else return current;
  }
  return null;
}

//Para corregir la inserción, depende del tio
void fixInsertion(ID current){
  ID parent = PARENT(current);

  if(parent == null) return; //Es root

  if(COLOR(parent) == BLACK) return; //Padre es negro

  ID grandparent = PARENT(parent);
  if(grandparent == null) {COLOR(parent) = BLACK; return;} //Padre es root y rojo

  ID uncle = (LEFT(grandparent) == parent) ? RIGHT(grandparent) : LEFT(grandparent);

  if(uncle != null && COLOR(uncle) == RED){
    COLOR(parent) = BLACK; COLOR(grandparent) = RED; COLOR(uncle) = BLACK;
    fixInsertion(grandparent);
    return;
  } // Tio y padre son rojos

  if(parent == LEFT(grandparent)){
    if(current == RIGHT(parent)){
      rotateLeft(parent);
      parent = current;
    } // Caso triangulo con punta hacia izquierda
    rotateRight(grandparent); //Caso linea recta creciente
  } 

  else{
    if(current == LEFT(parent)){
      rotateRight(parent);
      parent = current;
    } // Caso triangulo con punta hacia derecha
    rotateLeft(grandparent); //Caso linea recta decreciente
  } 

  COLOR(parent) = BLACK; COLOR(grandparent) = RED;
}


void forEachIn_r(std::function<void(K const&)> f, ID current){
  if(current == null) return;
  forEachIn_r(f, LEFT(current)); f(KEY(current)); forEachIn_r(f, RIGHT(current));
}

void forEachPre_r(std::function<void(K const&)> f, ID current){
  if(current == null) return;
  f(KEY(current)); forEachPre_r(f, LEFT(current)); forEachPre_r(f, RIGHT(current));
}

void forEachPos_r(std::function<void(K const&)> f, ID current){
  if(current == null) return;
  forEachPos_r(f, LEFT(current)); forEachPos_r(f, RIGHT(current)); f(KEY(current));
}

//Se elimina el nodo específico como en un BST cuando tiene a lo sumo un hijo
// Se reemplaza por alguno de sus hijos
ID eraseWithSingleChild(ID current){
  ID promoted;

  if(LEFT(current) != null){
    changeChild(PARENT(current), current, LEFT(current));
    promoted = LEFT(current);
  } // Tiene hijo izquierdo

  else if (RIGHT(current) != null){
    changeChild(PARENT(current), current, RIGHT(current));
    promoted = RIGHT(current);
  } // Tiene hijo derecho

  else{
    ID new_child = (COLOR(current) == BLACK) ? nil : null;
    changeChild(PARENT(current), current, new_child);
    promoted = new_child;
  } // No tiene hijos

  deleteNode(current);
  return promoted;
}

//Sucesor inorder
ID smallest(ID current){
  while(LEFT(current) != null){
    current = LEFT(current);
  }
  return current;
}

//Para corregir la eliminación, depende del hermano
void fixErasure(ID current){
  if(current == root) return; // Es root


  ID sibling = (current == LEFT(PARENT(current))) ? RIGHT(PARENT(current)) : LEFT(PARENT(current));


  if(COLOR(sibling) == RED){
    COLOR(sibling) = BLACK;
    COLOR(PARENT(current)) = RED;

    if(current == LEFT(PARENT(current))) rotateLeft(PARENT(current));
    else rotateRight(current);

    sibling = (current == LEFT(PARENT(current))) ? RIGHT(PARENT(current)) : LEFT(PARENT(current));
  } // Hermano es rojo


  if(IS_BLACK(LEFT(sibling)) && IS_BLACK(RIGHT(sibling))){
    COLOR(sibling) = RED;

    if(COLOR(PARENT(current)) == RED) COLOR(PARENT(current)) = BLACK; // Hermano negro con hijos negros y padre rojo
    else fixErasure(PARENT(current)); // Hermano negro con hijos negros y padre negro

  } //Hermano negro con hijos negros

  else {
    bool is_left = current == LEFT(PARENT(current));
    
    if(is_left){
      if(IS_BLACK(RIGHT(sibling))){
        COLOR(LEFT(sibling)) = BLACK;
        COLOR(sibling) = RED;
        rotateRight(sibling);
        sibling = RIGHT(PARENT(current));
      }
    }else {
      if(IS_BLACK(LEFT(sibling))){
        COLOR(RIGHT(sibling)) = BLACK;
        COLOR(sibling) = RED;
        rotateLeft(sibling);
        sibling = LEFT(PARENT(current));
      }
    } // Hermano negro con un hijo rojo y otro negro hacia afuera

    COLOR(sibling) = COLOR(PARENT(current));
    COLOR(PARENT(current)) = BLACK;
    if(is_left){
      COLOR(RIGHT(sibling)) = BLACK;
      rotateLeft(PARENT(current));
    }else{
      COLOR(LEFT(sibling)) = BLACK;
      rotateRight(PARENT(current));
    }

  } // Hermano negro con al menos un hijo rojo
}



/* Interface */
public:
RBtree(){
  mem.reserve(initial_capacity);
  mem.push_back({K{}, null, null, null, BLACK});
}

~RBtree(){}

void insert(K const& k){
  ID current = root;
  ID parent = null;

  while (current != null){
    parent = current;
    if (k < KEY(current)) current = LEFT(current);
    else if (k > KEY(current)) current = RIGHT(current);
    else return; 
  }

  current = newNode(k);
  if (parent == null) root = current;
  else if (k < KEY(parent)) LEFT(parent) = current;
  else RIGHT(parent) = current;
  
  PARENT(current) = parent;

  fixInsertion(current);
  COLOR(root) = BLACK;
}

void erase(K const& k){
  ID current = searchNode(k);

  if(current == null) return; //No existe

  ID promoted;
  COL color;
  
  if(LEFT(current) == null || RIGHT(current) == null){
    color = COLOR(current);
    promoted = eraseWithSingleChild(current);
  } //Tiene a lo sumo un hijo

  else{
    ID successor = smallest(RIGHT(current));
    color = COLOR(successor);
    KEY(current) = KEY(successor);
    promoted = eraseWithSingleChild(successor);
  } //Tiene dos hijos

  if(color == BLACK){
    fixErasure(promoted);
    if(promoted == nil){
      changeChild(PARENT(promoted), promoted, null);
    }
  }// Nodo eliminado es negro

  COLOR(root) = BLACK;
}

bool contains(K const& k){
  ID current = root;

  while (current != null){
    if (k < KEY(current)) current = LEFT(current);
    else if (k > KEY(current)) current = RIGHT(current);
    else return true; 
  }

  return false;
}

void forEachIn(std::function<void(K const&)> f){
  forEachIn_r(f, root);
}

void forEachPre(std::function<void(K const&)> f){
  forEachPre_r(f, root);
}

void forEachPos(std::function<void(K const&)> f){
  forEachPos_r(f, root);
}

void forEachFast(std::function<void(K const&)> f){
  const ID size = mem.size();
  for(ID id = 1; id<size; ++id) f(KEY(id));
}

inline void insertar(K const& k){
  insert(k);
}

inline void eliminarNodo(K const& k){
  erase(k);
}

inline std::vector<K> inorden(){
  std::vector<K> vec;
  vec.reserve(mem.size());
  forEachIn_r([&vec](K const& k) {vec.push_back(k);}, root);
  return vec;
}

inline std::vector<K> preorden(){
  std::vector<K> vec;
  vec.reserve(mem.size());
  forEachPre_r([&vec](K const& k) {vec.push_back(k);}, root);
  return vec;
}

inline std::vector<K> posorden(){
  std::vector<K> vec;
  vec.reserve(mem.size());
  forEachPos_r([&vec](K const& k) {vec.push_back(k);}, root);
  return vec;
}

};