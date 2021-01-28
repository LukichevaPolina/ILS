#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <map>

#include "Functions.h"

int main(int argc, char* argv[]) {
    std::pair <const int, double**> dataFromFile = ParserFileToMatr(argv[1]);
    int *resWay = GreedyAlg(dataFromFile.second, dataFromFile.first);
    if (CheckGCycle(resWay, dataFromFile.first)) {
        for (int i = 0; i < dataFromFile.first; i++) {
            std::cout << resWay[i];
        }
    }
    else {
        std::cout << "Error" << std::endl;
    }
    std::cout << WayCost(resWay, dataFromFile.second, 1000);
    resWay = LocalSearch(resWay, dataFromFile.second, 1000);
}


