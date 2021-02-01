//#include <iostream>
//#include <string>
//#include <fstream>
//#include <array>
//#include <map>
//
//#include "Functions.h"
//
//int main(int argc, char* argv[]) {
//  std::pair <const int, double**> dataFromFile = ParserFileToMatr(argv[1]);
//  int* resWay = GreedyAlg(dataFromFile.second, dataFromFile.first);
//  /*for (int i = 0; i < dataFromFile.first; i++) {
//    std::cout << resWay[i] << ' ';
//  }*/
//  std::cout << WayCost(resWay, dataFromFile.second, dataFromFile.first);
//  resWay = LocalSearch(resWay, dataFromFile.second, dataFromFile.first);
//  std::cout << WayCost(resWay, dataFromFile.second, dataFromFile.first) << std::endl;
//  for (int i = 0; i < dataFromFile.first; i++) {
//    std::cout << resWay[i] << ' ';
//  }
//}

#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <map>

#include "Functions.h"

int main(int argc, char* argv[]) {
  std::pair <const int, double**> dataFromFile = ParserFileToMatr(argv[1]);
  std::pair <int*, double> Res = ILS(dataFromFile.second, dataFromFile.first);
  for (int i = 0; i < dataFromFile.first; ++i) {
    std::cout << Res.first[i] << ' ';
  }
  std::cout << Res.second << std::endl;
}
