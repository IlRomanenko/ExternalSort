#pragma once
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <functional>
#include <algorithm>

using namespace std;
typedef size_t uint;

#ifdef _DEBUG
#define dbg(x) cerr << #x << " " << x << " " << __LINE__ << endl;
#define debugCode(x); x
#else
#define dbg(x)
#define debugCode(x); 
#endif

#define fori(i, n) for (int i = 0; i < (int)(n); i++)

template <typename T> string toString(const T &obj)
{
    stringstream ss;
    ss << obj;
    return ss.str();
}

template <typename T> unique_ptr<T> make_unique(T *obj)
{
    return unique_ptr<T>(obj);
}


