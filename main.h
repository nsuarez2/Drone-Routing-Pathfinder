#ifndef p4_main_h
#define p4_main_h

#include <iostream>
#include <string>
#include <sstream>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <queue>
#include <deque>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <iomanip>
#include <iterator>
#include <algorithm>

using namespace std;

#define DOUBLEMAX 1.7976931348623157E+308
#define INTMAX 32767

struct option longOpts[] = {
    {"clientType", required_argument, NULL, 'c'},
    {"help", no_argument, NULL, 'h'},
};

struct Vertex {
    int x;
    int y;
};

struct Edge {
    int begin;
    int end;
    double distance;
};
ostream& operator<<(ostream& os, const Edge& obj) {
    if (obj.begin < obj.end)
        return os << obj.begin << " " << obj.end << "\n";
    return os << obj.end << " " << obj.end << "\n";
}

struct Subset {
    int parent;
    int size = 1;
};

struct shortDisComp {
    bool operator() (const Edge &a, const Edge &b) const {
        return (a.distance > b.distance);
    }
};

struct Path {
    deque<int> data;
    double cost = 0;
    
    void push(int val) {
        data.push_back(val);
    }
    void pop() {
        data.pop_back();
    }
    int& back() {
        return data.back();
    }
    int& secondBack() {
        return data[data.size() - 2];
    }
    size_t length() {
        return data.size();
    }
    int& operator[](size_t idx) {
        return data[idx];
    }
};

// 1D vector class representing half the adjacency matrix (since its symmetric)
// Cool for memory, bad for speed :(

//class Distances {
//private:
//    vector<double> data;
//public:
//    Distances(int num) {
//        data.resize(num * (num + 1) / 2, 0);
//    }
//    void set(int i, int j, double val){
//        data[i * (i + 1) / 2 + j] = val;
//    }
//    double at(int a, int b) {
//        pair<int,int> p = minmax(a,b);
//        return data[p.second * (p.second + 1) / 2 + p.first];
//    }
//};


// Meh

//class Distances {
//private:
//    vector<double> data;
//    int size;
//public:
//    Distances(int num) {
//        size = num;
//        data.resize(num * num);
//    }
//    double& at(int i, int j) {
//        return data[(i * size) + j];
//    }
//};

// Returns the distace (as a double) between 2 verticies
double Distance(Vertex&, Vertex&);

// Returns representative of subset
int findRoot(deque<Subset>&, int);

// Creates union of two subsets
void unite(deque<Subset>&, int, int);

// B&B function for solving TSP
void permute(Path&, deque<int>&, Path&, vector<double>&);

// Calculates the lowerbound
double lowerbound(deque<int>::iterator, deque<int>::iterator,
                  vector<double>&, int);

// Sets the 'best' path using a nearest neighbor algorithm
void upperbound(deque<int>::iterator, deque<int>::iterator,
                vector<double>&, Path&);

int numFacil = 0;



#endif