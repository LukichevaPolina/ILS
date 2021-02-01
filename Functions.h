#ifndef FUNCTIONS
#define FUNCTIONS

double** AdjacencyMatrix(std::map <int, std::array<int, 2>> nodes);
double Distance(int x1, int y1, int x2, int y2);
std::pair <const int, double**> ParserFileToMatr(char* file);
int* GreedyAlg(double** matr, const int N);
double WayCost(int* way, double** matr, const int N);
int* LocalSearch(int* way, double** matr, const int N);
std::pair <int*, double> ILS(double** matr, const int N);
int* four_opt(int* way, double** matr, const int N);

#endif