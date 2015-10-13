#pragma once
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>

typedef unsigned int uint;
typedef unsigned char byte;

using namespace std;

#define fori(i, n) for(int i = 0; i < (int)(n); i++)
template <typename T, size_t N> size_t countof(T (&data)[N]) { return N; }
