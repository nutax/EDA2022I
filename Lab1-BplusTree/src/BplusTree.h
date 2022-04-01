#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <vector>
#include <cstdint>
#include <algorithm>
#include <queue>
#include <iostream>
#include <cstring>

#define SIZE(i) mem[i].size
#define KEY(i,j) mem[i].key[j]
#define CHILD(i,j) mem[i].child[j]
#define PARENT(i) mem[i].parent
#define IS_LEAF(i) (mem[i].child[0] == null)
#define IS_NOT_LEAF(i) (mem[i].child[0] != null)
#define NEW_NODE mem.size(); mem.emplace_back()


class BplusTree{
    using Key = uint32_t;
    using Value = uint32_t;
    using Index = uint32_t;
    using Size = uint32_t;

    static constexpr Index null = 0;
    static constexpr Size degree = 21; //Muchisimo mas rapido con grado 6
    static constexpr Size capacity = degree-1;
    static constexpr Size mid = degree/2;
    
    struct Node{
        Size size;
        Key key[capacity+1];
        Index child[degree+1] = {0};
        uint32_t dummy[1];
    };


    Index root;
    std::vector<Node> mem;


    public:
    BplusTree(){
        root = null;
        mem.reserve(1024*128);
        mem.emplace_back();
    }

    ~BplusTree(){}

    void insertar(Key const& key){
        if(root == null){
            root = NEW_NODE;
            SIZE(root) = 1;
            KEY(root, 0) = key;
            return;
        }
        
        int level = 0;
        Index parent[8];
        Index current = root;
        while(IS_NOT_LEAF(current)){
            parent[level++] = current;
            int i = 0;
            for(; i< SIZE(current); ++i){
                if(key < KEY(current, i)) break;
            }
            current = CHILD(current, i);
        }

        int i = SIZE(current) - 1;
        SIZE(current) += 1;
        while(i>= 0 && key < KEY(current, i)){
            KEY(current, i + 1) = KEY(current, i);
            i -= 1;
        }
        KEY(current, i + 1) = key;
        
        
        if(SIZE(current) <= capacity) return;

        SIZE(current) = mid;
        Index other_half = NEW_NODE;
        SIZE(other_half) = degree - mid;
        memcpy(&(KEY(other_half,0)), &(KEY(current,mid)), sizeof(Key)*(degree - mid));
        
        
        do{
            if(current == root){
                root = NEW_NODE;
                
                SIZE(root) = 1;
                KEY(root, 0) = KEY(current, SIZE(current));
                CHILD(root, 0) = current;
                CHILD(root, 1) = other_half;
                
                return;
            }
        
            Index child = other_half;
            Key k = KEY(current, SIZE(current));
            current = parent[--level];
            int i = SIZE(current) - 1;
            SIZE(current) += 1;
            while(i>= 0 && key < KEY(current, i)){
                KEY(current, i + 1) = KEY(current, i);
                CHILD(current, i + 2) = CHILD(current, i+1);
                i -= 1;
            }
            KEY(current, i + 1) = k;
            CHILD(current, i + 2) = other_half;

            if(SIZE(current) <= capacity) return;

            SIZE(current) = mid;
            other_half = NEW_NODE;
            SIZE(other_half) = degree - mid - 1;
            memcpy(&(KEY(other_half,0)), &(KEY(current,mid+1)), sizeof(Key)*(degree - mid - 1));
            memcpy(&(CHILD(other_half,0)), &(CHILD(current,mid+1)), sizeof(Index)*(degree - mid));
            
        }while(true);
    }


    void borrar(Key const& key){

    }
    std::vector<Key> bfs(){
        std::vector<Key> result;
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
};

#endif