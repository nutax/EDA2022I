#include <vector>
#include <stdexcept>

/* By José Ignacio Huby Ochoa */

#define PARENT(id) mem[id].p
#define RIGHT(id) mem[id].r
#define LEFT(id) mem[id].l
#define KEY(id) mem[id].k
#define VAL(id) mem[id].v

/* 5 parts: Types, Constants, Memory, Internal procedures, Interface */

class RBtree{

/* Types */
using K = int;
using V = int;
using ID = int;
struct Node{K k; V v; ID p,l,r;};

/* Constants */
constexpr static ID initial_capacity = 512;
constexpr static ID null = ((1 << (sizeof(ID)*8 - 2) - 1) * 2) + 1;


/* Memory */
ID root = null;
std::vector<Node> mem;


/* Internal procedures */
int NEW(K const& k, V const& v, ID const& p){
  const int i = mem.size();
  mem.push_back({k, v, p, null, null});
  return i;
}

void updateParent(ID parent, ID old_child, ID new_child){
  if (parent == null) root = new_child;
  else if (LEFT(parent) == old_child) LEFT(parent) = new_child;
  else (RIGHT(parent) == old_child) RIGHT(parent) = new_child;
  
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

void searchNode(K const& k){
  ID current = root;
  while (current != null){
    if()
  }
}

void fixInsertion(ID current){
  
} 



/* Interface */
public:
RBtree(){
  mem.reserve(initial_capacity);
}

~RBtree(){}

void insert(K const& k, V const& v){
  
}

V& at(K const& k){
  
}

void erase(K const& k){
  
}

bool isPresent(K const& k){
  
}

void forEachIn(std::function<void(K const&, V&)> f){
  
}

void forEachPre(std::function<void(K const&, V&)> f){
  
}

void forEachPos(std::function<void(K const&, V&)> f){
  
}

};