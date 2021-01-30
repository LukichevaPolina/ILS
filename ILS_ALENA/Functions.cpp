#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <map>
#include <ctime>
#include <cstdlib>

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
  // std::map <std::string, int[2]> nodes = new ;
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
    for (; it != nodes.end(); ++it) {
      std::cout << it->first << " " << it->second[0] << ' ' << it->second[1] << std::endl;
    }
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

  //double* dist = new double[N]; // мин рассто€ние
  //int* way = new int[N] { 0 };  // мин путь
  //int* visited = new int[N]; // посещенные вершины
  //double temp, min;
  //int min_ind = 0, k = 0;
  //for (int i = 0; i < N; i++)
  //{
  //  dist[i] = DBL_MAX;
  //  visited[i] = 1;
  //}
  //dist[0] = 0;

  //while (min_ind != INT_MAX) {
  //  min_ind = INT_MAX;
  //  min = DBL_MAX;
  //  for (int i = 0; i < N; i++)
  //  { // ≈сли вершину ещЄ не обошли и метка меньше min
  //    if ((visited[i] == 1) && (dist[i] < min))
  //    {
  //      min = dist[i];
  //      min_ind = i;
  //    }
  //  }

  //  if (min_ind != INT_MAX)
  //  {
  //    for (int i = 0; i < N; i++)
  //    {
  //      if (matr[min_ind][i] > 0 && visited[i] == 1)
  //      {
  //        temp = min + matr[min_ind][i];
  //        if (temp < dist[i])
  //        {
  //          dist[i] = temp;
  //        }
  //      }
  //    }
  //    visited[min_ind] = 0;
  //    way[k] = min_ind + 1;
  //    k += 1;
  //  }
  //}
  //return way;
  //std::cout << minCost;
  //return minPath;
}

int* LocalSearch(int* way, double** matr, const int N)
{
  double global_diff = -0.1, diff = -0.1;
  double currentCost = WayCost(way, matr, N), newCost = currentCost + 1;
  int* new_way = new int[N];
  int imin, jmin;
  for (int l = 0; l < 1; ++l) {
    while (currentCost <= newCost) {
      srand(time(0));
      int firstV = rand() % (N - 1), secondV = rand() % (N - 1);
      while (firstV == secondV) {
        firstV = rand() % (N - 1), secondV = rand() % (N - 1);
      }
      if (firstV > secondV) {
        int a = firstV;
        firstV = secondV;
        secondV = a;
      }
      for (int k = 0; k < N; ++k) {
        new_way[k] = way[k];
      }
      for (int i = firstV; i <= (secondV + firstV) / 2; ++i) {
        int a = new_way[i];
        new_way[i] = new_way[firstV + (secondV - i)];
        new_way[firstV + (secondV - i)] = a;
      }
      newCost = WayCost(new_way, matr, N);
      if (newCost < currentCost) {
        for (int k = 0; k < N; ++k) {
          new_way[k] = way[k];
        }
        currentCost = newCost;
        break;
      }
    }
  }
  std::cout << currentCost;
  return way;
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
