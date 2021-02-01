#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <map>
#include <ctime>
#include <cstdlib>
#include <random>
#include <algorithm>
#include <chrono>

#include "Functions.h"

double** AdjacencyMatrix(std::map <int, std::array<int, 2>> nodes) {
  double** adjMatrix = new double* [nodes.size()];
  for (int i = 0; i < nodes.size(); ++i)
    adjMatrix[i] = new double[nodes.size()];

  std::map <int, std::array<int, 2>>::iterator it1 = nodes.begin();

  for (int i = 0; it1 != nodes.end(); ++i, ++it1) {
    std::map <int, std::array<int, 2>>::iterator it2 = it1;
    for (int j = i; it2 != nodes.end(); ++j, ++it2) {
      double dist = Distance(it1->second[0], it1->second[1], it2->second[0], it2->second[1]);
      adjMatrix[i][j] = dist;
      adjMatrix[j][i] = dist;
    }
  }
  return adjMatrix;
}

double Distance(int x1, int y1, int x2, int y2) {
  return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

std::pair <const int, double**> ParserFileToMatr(char* file) {
  std::string line;
  std::ifstream in(file);

  if (in.is_open()) {
    getline(in, line);
    const int N = stoi(line);
    std::map <int, std::array<int, 2>> nodes;

    while (getline(in, line)) {
      std::string* data = new std::string[3];
      int i = 0;
      while (line.find(' ') != std::string::npos) {
        data[i] = line.substr(0, line.find(' '));
        line.erase(line.find(data[i]), data[i].size() + 1);
        ++i;
      }
      nodes[int(stoi(data[0]))] = { stoi(data[1]), stoi(line) };
    }

    std::map <int, std::array<int, 2>>::iterator it = nodes.begin();
    double** adjMatrix = new double* [N];
    for (int i = 0; i < N; ++i)
      adjMatrix[i] = new double[N];
    adjMatrix = AdjacencyMatrix(nodes);
    return { N, adjMatrix };
  }
}


void arrCpy(int dest[], const int source[], int n) {
  for (n--; n >= 0; n--) {
    dest[n] = source[n];
  }
}

int* GreedyAlg(double** matr, const int N) {
  float coolcost = 10000000, cost = 0;
  int count = 0;
  int* coolpath = new int[N];
  for (int i = 0; i < N; ++i) {
    cost = 0, count = 0;
    int check[1000] = { 0 };
    int path[1000] = { 0 };
    check[i] = 2;
    int k = i;
    int flagflag = 1;
    while (flagflag != 0 || count == 0)
    {
      int min = 100000000, minnum = 0, flag = 0;
      for (int j = 0; j < N; j++)
        if (matr[k][j] != 0 && matr[k][j] < min && (check[j] == 0 || check[j] == 2 && count == N - 1))
        {
          min = matr[k][j];
          minnum = j;
          flag = 1;
        }
      if (flag == 0)
        flagflag = 0;
      else {
        check[minnum] = 1;
        path[count] = minnum + 1;
        cost += min;
        count++;
        k = minnum;
      }
    }
    if (count == N && cost < coolcost) {
      for (int i = 0; i <= N; i++)
        coolpath[i] = path[i];
      coolcost = cost;
    }
  }
  return coolpath;
}

int* LocalSearch(int* way, double** matr, const int N)
{
  int* best_way = new int[1000];
  int* new_way = new int[1000];
  std::copy(way, way + N, best_way);
  bool improved = 1;
  int l = 0;
  while (improved) {
    improved = 0;
    for (int i = 1; i < N - 2; ++i)
      for (int j = i + 2; j < N; ++j) {
        std::copy(best_way, best_way + N, new_way);
        for (int k = i; k <= j; ++k) {
          new_way[k] = best_way[j + i - k];
        }
        if (WayCost(new_way, matr, N) < WayCost(best_way, matr, N)) {
          std::copy(new_way, new_way + N, best_way);
          improved = 1;
        }
      }
    std::copy(best_way, best_way + N, way);
  }
  std::cout << std::endl;
  return best_way;
}

std::pair <int*, double> ILS(double** matr, const int N) {
  int* Way = new int[N];
  int* curWay = new int[N];
  Way = GreedyAlg(matr, N);
  Way = LocalSearch(Way, matr, N);
  double min_dist = WayCost(Way, matr, N);
  for (int i = 0; i < 100; i++) {
    curWay = four_opt(Way, matr, N);
    curWay = LocalSearch(curWay, matr, N);
    double cost = WayCost(curWay, matr, N);
    if (WayCost(Way, matr, N) > cost) {
      std::copy(curWay, curWay + N, Way);
      min_dist = cost;
    }
  }
  return { Way, min_dist };
}

double WayCost(int* way, double** matr, const int N)
{
  way[N] = way[0];
  double cost = 0;
  for (int i = 0; i < N; ++i) {
    cost += matr[way[i] - 1][way[i + 1] - 1];
  }
  return cost;
}

int* four_opt(int* way, double** matr, const int N)
{
  double cost = WayCost(way, matr, N);
  bool improved = true;
  int pos1, pos1_p, pos2, pos2_p, pos3, pos3_p, first = 0, last = N - 1;
  int* new_way = new int[N];
  int k = 0;
  improved = false;
  srand(time(0));
  pos1 = 1 + rand() % (N / 2);
  pos1_p = pos1 - 1;
  pos2 = 1 + pos1 + rand() % (N - pos1);
  pos2_p = pos2 - 1;
  pos3 = pos2 + rand() % ((N - pos2) + 1);
  pos3_p = pos3 - 1;
  std::copy(way, way + N, new_way);

  int pos = pos1 - first + 1;
  for (int i = pos3; i <= last; ++i) {
    new_way[pos++] = way[i];
  }
  for (int i = pos2; i <= pos3_p; ++i) {
    new_way[pos++] = way[i];
  }
  for (int i = pos1 + 1; i <= pos2_p; ++i) {
    new_way[pos++] = way[i];
  }
  return new_way;
}

