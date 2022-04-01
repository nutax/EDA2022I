#include <iostream>
#include "BplusTree.h"
#include <fstream>
#include <chrono>

int main() {


    // Read file
    std::ifstream texto;
    texto.open("./output.txt");

    int datos[ 1000000 ];
    int element;
    if (texto.is_open()) {
        int i = 0;
        while (texto >> element) {
            datos[i++] = element;
        }
    } 

    int64_t tiempoInseccion = 0;
    for (int t=0; t<1; ++t){
        auto tree = BplusTree{};
        
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        for (uint32_t i=1; i<11; ++i){
            tree.insertar( i );
        }
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

        // Calcular tiempo
        auto tiempo = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        tiempoInseccion += tiempo;
        tree.bfs();
    }
    std::cout << tiempoInseccion << std::endl;
}
