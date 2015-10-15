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

typedef size_t uint;

using namespace std;

template <typename T> string toString(const T &obj)
{
	stringstream ss;
	ss << obj;
	return ss.str();
}