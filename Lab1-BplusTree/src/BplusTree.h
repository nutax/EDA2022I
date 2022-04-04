#ifndef BPLUSTREE_H
#define BPLUSTREE_H

//Por José Ignacio Huby Ochoa

#include <vector>
#include <cstdint>
#include <algorithm>
#include <queue>
#include <iostream>
#include <cstring>

//Macros para facilitar el uso de un vector como gestor de memoria
#define SIZE(i) mem[i].size
#define KEY(i,j) mem[i].key[j]
#define CHILD(i,j) mem[i].child[j]
#define PARENT(i) mem[i].parent
#define IS_LEAF(i) (mem[i].child[0] == null)
#define IS_NOT_LEAF(i) (mem[i].child[0] != null)
#define NEXT(i) mem[i].child[degree]
#define NEW_NODE mem.size(); mem.emplace_back()
#define CAN_GIVE(i) (SIZE(i) > min)

class BplusTree{
    using Key = uint32_t;
    using Value = uint32_t; // No se usa, pero en la practica deberia haber un value
    using Index = uint32_t;  // Sustituto de ptr, es el indice del vector
    using Size = uint32_t;

    static constexpr Index null = 0; //La posición 0 en el vector es dummy para permitir null = 0
    static constexpr Size degree = 21; //Más rapido con grado 6
    static constexpr Size capacity = degree-1;
    static constexpr Size mid = degree/2;
    static constexpr Size min = (degree/2) + (degree%2) - 1;
    
    struct Node{
        Size size;
        Key key[capacity+1];
        Index child[degree+1] = {0};
        uint32_t dummy[1]; //Mejora la performance con estos extras bytes
    };


    Index root; 
    std::vector<Node> mem; //Toda la memoria dinámica que se usara

    void eraseInternal(Index internal, int i){
        Index current = CHILD(internal, i + 1);
        while(IS_NOT_LEAF(current)){
            current = CHILD(current, 0);
        }
        KEY(internal, i) = KEY(current, 0);
    }

    public:
    BplusTree(){
        root = null;
        mem.reserve(1024*128); //Evitar allocaciones
        mem.emplace_back();
    }

    ~BplusTree(){}

    void insertar(Key const& key){
        if(root == null){ 
            root = NEW_NODE;
            SIZE(root) = 1;
            KEY(root, 0) = key;
            return;
        } //Si no hay root
        
        int level = 0; //Head del stack
        Index parent[12]; //Stack de parents
        Index current = root;
        while(IS_NOT_LEAF(current)){
            parent[level++] = current;
            int i = 0;
            for(; i< SIZE(current); ++i){
                if(key < KEY(current, i)) break;
            }
            current = CHILD(current, i);
        } //Encontrar nodo hoja y el camino de parents

        int i = SIZE(current) - 1;
        SIZE(current) += 1;
        while(i>= 0 && key < KEY(current, i)){
            KEY(current, i + 1) = KEY(current, i);
            i -= 1;
        }
        KEY(current, i + 1) = key; //Insertar el key
        
        
        if(SIZE(current) <= capacity) return; //Si no hay overflow, termina

        SIZE(current) = mid;
        Index other_half = NEW_NODE;
        SIZE(other_half) = degree - mid;
        memcpy(&(KEY(other_half,0)), &(KEY(current,mid)), sizeof(Key)*(degree - mid));
        NEXT(other_half) = NEXT(current);
        NEXT(current) = other_half;
                    //Se hace split y el centro se queda en el nodo hoja y tambien sube
                    //Se actualiza la double linked list

        do{
            if(current == root){
                root = NEW_NODE;
                
                SIZE(root) = 1;
                KEY(root, 0) = KEY(current, SIZE(current));
                CHILD(root, 0) = current;
                CHILD(root, 1) = other_half;
                
                return;
            } //Si es el root, se crear otro root y termina
        
            Index child = other_half;
            Key k = KEY(current, SIZE(current)); //Se obtiene la key que sube
            current = parent[--level]; //Se atiende al padre
            int i = SIZE(current) - 1;
            SIZE(current) += 1;
            while(i>= 0 && key < KEY(current, i)){
                KEY(current, i + 1) = KEY(current, i);
                CHILD(current, i + 2) = CHILD(current, i+1);
                i -= 1;
            }
            KEY(current, i + 1) = k;
            CHILD(current, i + 2) = other_half; //Se inserta la key y el hijo

            if(SIZE(current) <= capacity) return; //Si no hay overflow, termina

            SIZE(current) = mid;
            other_half = NEW_NODE;
            SIZE(other_half) = degree - mid - 1;
            memcpy(&(KEY(other_half,0)), &(KEY(current,mid+1)), sizeof(Key)*(degree - mid - 1));
            memcpy(&(CHILD(other_half,0)), &(CHILD(current,mid+1)), sizeof(Index)*(degree - mid));
                //Se hace split y el centro sube, pero no se queda en el nodo hijo 
        }while(true);
    }

    std::vector<int> bfs(){
        std::vector<int> result;
        if(root == null) return result;
        std::queue<Index> q;
        q.push(root);
        while(!q.empty()){
            const Index current = q.front();
            if(CHILD(current, 0) != null)
            for(int i = 0; i<(SIZE(current)+1); ++i){
                if(CHILD(current, i) != null) q.push(CHILD(current, i));
            }
            for(int i = 0; i<SIZE(current); ++i){
                result.push_back( KEY(current, i) );
            }
            q.pop();
        }
        return result;
    }

    int nodes(){return mem.size();}


    void borrar(Key const& key){
        if(root == null) return; //Si no hay root, termina

        
        int i;
        int level = 0; //Head del stack
        int child[10]; //Stack de childs
        Index parent[10]; //Stack de parents
        Index current = root;
        Index internal = null;
        int iinternal;
        while(IS_NOT_LEAF(current)){
            for(i = 0; i< SIZE(current); ++i){
                if(key < KEY(current, i)) break;
                else if(key == KEY(current, i)) {
                    internal = current;
                    iinternal = i;
                }
            }
            child[level] = i;
            parent[level++] = current;
            current = CHILD(current, i);
        } //Encontrar nodo hoja y el camino de parents

        
        int ileft = i - 1;
        int iright = i + 1;


        for(i = 0; i< SIZE(current); ++i){
            if(key == KEY(current, i)) break;
        } //Encontrar key en nodo hoja
        

        if(i == SIZE(current)) return; //Si no hay key, termina

        SIZE(current) -= 1;
        for(; i< SIZE(current); ++i){
           KEY(current, i) = KEY(current, i+1);
        } //Eliminar el key en nodo hoja
        
        

        if(current == root){
            if(SIZE(root) == 0) root = null; //Si el root está vacio, libera memoria
            return;
        } //Si solo esta el root, termina


        if(SIZE(current) >= min) {
            if(internal != null){
                eraseInternal(internal, i);
            }
            return;
        } // Si no hay copia interna y cumple el tamaño, termina 


        Index sibling;
        if(ileft >= 0){
            sibling = CHILD(parent[level], ileft);
            if(CAN_GIVE(sibling)){
                for(int j = SIZE(current); j > 0; --j){
                    KEY(current, j) = KEY(current, j - 1);
                }
                KEY(current, 0) = KEY(sibling, SIZE(sibling));
                SIZE(current) += 1;
                SIZE(sibling) -= 1;
                KEY(parent[level], ileft) = KEY(current, 0);
                return;
            }
        }
        if(iright <= SIZE(parent[level])){
            sibling = CHILD(parent[level], iright);
            if(CAN_GIVE(sibling)){
                KEY(current, SIZE(current)) = KEY(sibling, 0);
                SIZE(current) += 1;
                SIZE(sibling) -= 1;
                for(int j = 0; j < SIZE(sibling); ++j){
                    KEY(sibling, j) = KEY(sibling, j + 1);
                }
                KEY(parent[level], iright - 1) = KEY(sibling, 0);
                return;
            }
        } //Si algun hermano puede prestar, presta, arregla al padre y termina

        Index deleted;
        if(ileft >= 0){
            sibling = CHILD(parent[level], ileft);
            for(int j = 0; j < SIZE(current); ++j){
                KEY(sibling, SIZE(sibling) + j) = KEY(current, j);
            }
            SIZE(sibling) += SIZE(current);
            NEXT(sibling) = NEXT(current);
            deleted = current;
            i = ileft;
        }
        else if(iright <= SIZE(parent[level])){
            sibling = CHILD(parent[level], ileft);
            for(int j = 0; j < SIZE(sibling); ++j){
                KEY(current, SIZE(current) + j) = KEY(sibling, j);
            }
            SIZE(current) += SIZE(sibling);
            NEXT(current) = NEXT(sibling);
            deleted = sibling;
            i = iright - 1;

        } //Si existe algun hermano y ninguno puede prestar, se juntan

        do{
            current = parent[level];

            if(current == root && SIZE(current) == 1){
                root = (deleted == CHILD(current, 0)) ? CHILD(current, 1) : CHILD(current, 0);
                break;
            } // Si es root y tiene size 1, se cambia de root al hijo no eliminado y termina
            
            for(int j = i; j< SIZE(current); ++j){
                KEY(current, j) = KEY(current, j+1);
            } // Se elimina la llave
            
            int remove_child = 0;
            for(; remove_child <= SIZE(current); ++remove_child){
                if(remove_child == deleted) break;
            }
            for(; remove_child < SIZE(current); ++remove_child){
                CHILD(current, remove_child) =  CHILD(current, remove_child + 1);
            }
            CHILD(current, SIZE(current)) = null;
            SIZE(current) -= 1; //Se remueve el hijo eliminado

            if(current == root) break; //Si es root, termina

            if(SIZE(current) >= min) break; //Si no hace underflow, termina

            
            ileft = child[level] - 1;
            iright = child[level] + 1;
            level -= 1;
            if(ileft >= 0){
                sibling = CHILD(parent[level], ileft);
                if(CAN_GIVE(sibling)){
                    for(int j = SIZE(current); j > 0; --j){
                        KEY(current, j) = KEY(current, j - 1);
                    }
                    for(int j = SIZE(current) + 1; j > 0; --j){
                        CHILD(current, j) = CHILD(current, j - 1);
                    }
                    KEY(current, 0) = KEY(parent[level], ileft);
                    KEY(parent[level], ileft) = KEY(sibling, SIZE(sibling) - 1);
                    CHILD(current, 0) = CHILD(sibling, SIZE(sibling));
                    CHILD(sibling, SIZE(sibling)) = null;
                    SIZE(current) += 1;
                    SIZE(sibling) -= 1;
                    break;
                }
            }
            if(iright <= SIZE(parent[level])){
                sibling = CHILD(parent[level], iright);
                if(CAN_GIVE(sibling)){
                    KEY(current, SIZE(current)) = KEY(parent[level], iright - 1);
                    KEY(parent[level], iright - 1) = KEY(sibling, 0);
                    CHILD(current, SIZE(current) + 1) = CHILD(sibling, 0);
                    CHILD(sibling, SIZE(sibling)) = null;
                    SIZE(current) += 1;
                    SIZE(sibling) -= 1;
                    for(int j = 0; j < SIZE(sibling); ++j){
                        KEY(sibling, j) = KEY(sibling, j + 1);
                    }
                    for(int j = 0; j <= SIZE(sibling); ++j){
                        CHILD(sibling, j) = CHILD(sibling, j + 1);
                    }
                    break;
                }
            } //Si algun hermano puede prestar, el padre presta al actual, aquel al padre, y termina

            if(ileft >= 0){
                sibling = CHILD(parent[level], ileft);
                KEY(sibling, SIZE(sibling)) = KEY(parent[level], ileft);
                for(int j = 0; j < SIZE(current); ++j){
                    KEY(sibling, SIZE(sibling) + j + 1) = KEY(current, j);
                }
                for(int j = 0; j < (SIZE(current) + 1); ++j){
                    CHILD(sibling, SIZE(sibling) + j + 1) = CHILD(current, j);
                }
                SIZE(sibling) += SIZE(current) + 1;
                deleted = current;
                i = ileft;
            }
            else if(iright <= SIZE(parent[level])){
                sibling = CHILD(parent[level], ileft);
                KEY(current, SIZE(current)) = KEY(parent[level], iright - 1);
                for(int j = 0; j < SIZE(sibling); ++j){
                    KEY(current, SIZE(current) + j + 1) = KEY(sibling, j);
                }
                for(int j = 0; j < (SIZE(current) + 1); ++j){
                    CHILD(current, SIZE(current) + j + 1) = CHILD(sibling, j);
                }
                SIZE(current) += SIZE(sibling);
                deleted = sibling;
                i = iright - 1;

            } //Si existe algun hermano y ninguno puede prestar, se juntan

        }while(true);

        if(internal != null){
            internal = root;
            bool found = false;
            while(true){
                for(i = 0; i< SIZE(internal); ++i){
                    if(key < KEY(internal, i)) break;
                    else if(key == KEY(internal, i)) found = true;
                }
                if(found) break;
                internal = CHILD(internal, i);
            } //Encontrar nodo hoja y el camino de parents
            eraseInternal(internal, i);
        }
    }
};

#endif