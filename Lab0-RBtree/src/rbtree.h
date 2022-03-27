#include <vector>
#include <queue>
#include <functional>

/* By José Ignacio Huby Ochoa */

#define PARENT(id) mem[id].p
#define RIGHT(id) mem[id].r
#define LEFT(id) mem[id].l
#define KEY(id) mem[id].k
#define COLOR(id) mem[id].c
#define UNCLE (LEFT(grandparent) == parent) ? RIGHT(grandparent) : LEFT(grandparent)

#define RED 0
#define BLACK 1

/* 5 parts: Types, Constants, Memory, Internal procedures, Interface */

template<typename K>
class RBtree{

/* Types */
//using K = int;
using ID = unsigned;
using COL = bool;
struct Node{K k; ID p,l,r; COL c;};

/* Constants */
constexpr static ID initial_capacity = 512;
constexpr static ID null = ((1 << (sizeof(ID)*8 - 2) - 1) * 2) + 1;


/* Memory */
ID root = null;
std::vector<Node> mem;


/* Internal procedures */
int newNode(K const& k){
  const int i = mem.size();
  mem.push_back({k, null, null, null, RED});
  return i;
}

void updateParent(ID parent, ID old_child, ID new_child){
  if (parent == null) root = new_child;
  else if (LEFT(parent) == old_child) LEFT(parent) = new_child;
  else RIGHT(parent) = new_child;
  
  if(new_child != null) PARENT(new_child) = parent;
}

void rotateRight(ID current){
  ID parent = PARENT(current);
  ID left = LEFT(current);

  LEFT(current) = RIGHT(left);
  if(RIGHT(left) != null){
    PARENT(RIGHT(left)) = current;
  }

  RIGHT(left) = current;
  PARENT(current) = left;

  updateParent(parent, current, left);
}

void rotateLeft(ID current){
  ID parent = PARENT(current);
  ID right = RIGHT(current);

  RIGHT(current) = LEFT(right);
  if(LEFT(right) != null){
    PARENT(LEFT(right)) = current;
  }

  LEFT(right) = current;
  PARENT(current) = right;

  updateParent(parent, current, right);
}

ID searchNode(K const& k){
  ID current = root;
  while (current != null){
    if(k == KEY(current)) return current;
    else if(k < KEY(current)) current = LEFT(current);
    else current = RIGHT(current);
  }
  return null;
}

void fixInsertion(ID current){
  ID parent = PARENT(current);

  if(parent == null) return; //Es root

  if(COLOR(parent) == BLACK) return; //Padre es negro

  ID grandparent = PARENT(parent);
  if(grandparent == null) {COLOR(parent) = BLACK; return;} //Padre es root y rojo

  ID uncle = UNCLE;

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



/* Interface */
public:
RBtree(){
  mem.reserve(initial_capacity);
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
}

void erase(K const& k){
  
}

bool isPresent(K const& k){
  ID current = root;

  while (current != null){
    int a;
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
  for(auto const& mem_node : mem) f(mem_node.k);
}

};