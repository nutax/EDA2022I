#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <vector>
#include <cstdint>
#include <algorithm>
#include <queue>
#include <iostream>

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

    static constexpr Index null = UINT32_MAX;
    static constexpr Size degree = 4;
    static constexpr Size capacity = degree-1;
    static constexpr Size mid = degree/2;
    
    struct Node{
        Size size;
        Key key[capacity+1];
        Index child[degree+1];
        Index parent;
    };


    Index root;
    std::vector<Node> mem;



    public:
    BplusTree(){
        root = null;
        mem.reserve(1024*128);
    }

    ~BplusTree(){}

    void insertar(Key const& key){
        if(root == null){
            root = NEW_NODE;
            SIZE(root) = 1;
            KEY(root, 0) = key;
            CHILD(root, 0) = null;
            PARENT(root) = null;
            return;
        }
        
        
        Index current = root;
        Index parent;
        while(IS_NOT_LEAF(current)){
            parent = current;

            for(int i = 0; i< SIZE(current); ++i){
                if(key < KEY(current, i)){
                    current = CHILD(current, i);
                    break;
                }

                if(i == (SIZE(current)-1)){
                    current = CHILD(current, i + 1);
                    break;
                }

            }
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
        std::copy(&(KEY(current,mid)), &(KEY(current,capacity)), &(KEY(other_half,0)));
        CHILD(other_half, 0) = null;
        PARENT(other_half) = PARENT(current);
        int dummy;
        do{
            if(current == root){
                root = NEW_NODE;
                PARENT(current) = root;
                PARENT(other_half) = root;
                SIZE(root) = 1;
                KEY(root, 0) = KEY(current, SIZE(current));
                CHILD(root, 0) = current;
                CHILD(root, 1) = other_half;
                PARENT(root) = null;
                return;
            }
        
            Index child = other_half;
            Key k = KEY(current, SIZE(current));
            current = PARENT(current);
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
            SIZE(other_half) = degree - mid;
            std::copy(&(KEY(current,mid+1)), &(KEY(current,capacity)), &(KEY(other_half,0)));
            std::copy(&(CHILD(current,mid+1)), &(CHILD(current,degree)), &(CHILD(other_half,0)));
            PARENT(other_half) = PARENT(current);
        }while(true);
    }


    void borrar(Key const& key){

    }
    void bfs(){
        if(root == null) return;
        std::queue<Index> q;
        q.push(root);
        while(!q.empty()){
            const Index current = q.front();
            if(CHILD(current, 0) != null)
            for(int i = 0; i<(SIZE(current)+1); ++i){
                if(CHILD(current, i) != null) q.push(CHILD(current, i));
            }
            for(int i = 0; i<SIZE(current); ++i){
                std::cout << KEY(root, i) << ' ';
            }
            std::cout<<std::endl;
        }
    }
};

#endif