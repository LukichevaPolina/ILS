#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <map>

#include "Functions.h"

int main(int argc, char* argv[]) {
  std::pair <const int, double**> dataFromFile = ParserFileToMatr(argv[1]);
  int* resWay = GreedyAlg(dataFromFile.second, dataFromFile.first);
  for (int i = 0; i < dataFromFile.first; i++) {
    std::cout << resWay[i] << ' ';
  }
  std::cout << WayCost(resWay, dataFromFile.second, 1000);
  resWay = LocalSearch(resWay, dataFromFile.second, 1000);
  std::cout << WayCost(resWay, dataFromFile.second, 1000);
}


