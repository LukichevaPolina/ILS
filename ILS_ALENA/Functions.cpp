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
        /*for (; it != nodes.end(); ++it) {
            std::cout << it->first << " " << it->second[0] << ' ' << it->second[1] << std::endl;
        }*/
        double** adjMatrix = new double* [N];
        for (int i = 0; i < N; ++i)
            adjMatrix[i] = new double[N];
        adjMatrix = AdjacencyMatrix(nodes);
        return { N, adjMatrix };
    }
}


int* GreedyAlg(double** matr, const int N) {
    double* dist = new double[N]; // мин рассто€ние
    int* way = new int[N] { 0 };  // мин путь
    int* visited = new int[N]; // посещенные вершины
    double temp, min;
    int min_ind = 0, k = 0;
    for (int i = 0; i < N; i++)
    {
        dist[i] = DBL_MAX;
        visited[i] = 1;
    }
    dist[0] = 0;
    
    while (min_ind != INT_MAX) {
        min_ind = INT_MAX;
        min = DBL_MAX;
        for (int i = 0; i < N; i++)
        { // ≈сли вершину ещЄ не обошли и метка меньше min
            if ((visited[i] == 1) && (dist[i] < min))
            {
                min = dist[i];
                min_ind = i;
            }
        }
        
        if (min_ind != INT_MAX)
        {
            for (int i = 0; i < N; i++)
            {
                if (matr[min_ind][i] > 0 && visited[i] == 1)
                {
                    temp = min + matr[min_ind][i];
                    if (temp < dist[i])
                    {
                        dist[i] = temp;
                    }
                }
            }
            visited[min_ind] = 0;
            way[k] = min_ind + 1;
            k += 1;
        }
    }

  return way;

}

int* LocalSearch(int* way, double** matr, const int N)
{
  for (int k = 0; k < N; ++k) {
    double global_diff = -0.1, diff = -0.1;
    double currentCost = WayCost(way, matr, N), newCost = currentCost + 1;
    int* new_way = new int[N];
    int imin, jmin;
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
      if (newCost < currentCost)
        for (int k = 0; k < N; ++k) {
          new_way[k] = way[k];
        }
    }
  }
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

bool CheckGCycle(int* way, const int N) {
    int counter = 0;
    for (int i = 1; i <= N; i++) {
        counter = 0;
        for (int j = 0; j < N; j++) {
            if (i == way[j]) {
                counter += 1;
            }
        }
        if (counter != 1) {
            return false;
        }
    }
    return true;
}


int* Pereturbation(int* initWay, double** matr, const int N) {
    /*srand(time(0));
    int k = rand() % (N - 2 + 1) + 2;
    int* indexes = new int[k];
    int* shuf_indexes = new int[k];
    int* resWay = new int[N];
    int elemN = 0, flag = 0, i = 0;
    int* arr = new int[N];
    for (i = 0; i < N; i++) {
        arr[i] = i;
    }
    i = 0;
    while (i < k) {
        elemN = rand() % N;
        flag = 0;
        for (int j = 0; j < i; j++) {
            if (indexes[j] == arr[elemN])
            {
                flag = 1;
                break;
            }
        }
        if (flag == 0) {
            indexes[i] = arr[elemN];
            i += 1;
        }
    }
    std::copy(indexes, indexes + k, shuf_indexes);
    auto rng = std::default_random_engine{};
    std::shuffle(shuf_indexes, shuf_indexes+k, rng);
    for (i = 0; i < N; i++) {
        int flag = 0;
        for (int j = 0; j < k; j++) {
            if (i == indexes[j]) {
                flag = 1;
                resWay[i] = initWay[shuf_indexes[j]];
                break;
            }
        }
        if (flag == 0) {
            resWay[i] = initWay[i];
        }
    }*/
    int* resWay = new int[N];
    resWay = LocalSearch(resWay, matr, N);
    resWay = LocalSearch(resWay, matr, N);
    return resWay;
}


std::pair <int*, double> ILS(double** matr, const int N) {
    int* Way = new int[N];
    Way = GreedyAlg(matr, N);
    for (int i = 0; i < N; i++) {
        std::cout << Way[i] << ' ';
    }
    std::cout << std::endl;
    std::cout << "GA cost: " << WayCost(Way, matr, N) << std::endl;
    std::cout << std::endl;
    //std::cout << WayCost(resWay, matr, N) << std::endl;
    Way = LocalSearch(Way, matr, N);
    double min_dist = WayCost(Way, matr, N);
    for (int i = 0; i < 100; i++) {
        int* curWay = new int[N];
        double cost = WayCost(Way, matr, N);
        curWay = Pereturbation(Way, matr, N);
        curWay = LocalSearch(curWay, matr, N);
        double curCost = WayCost(curWay, matr, N);
        if (curCost < cost) {
            std::copy(curWay, curWay + N, Way);
            min_dist = cost;
        }
    }
    return { Way, min_dist };
}
