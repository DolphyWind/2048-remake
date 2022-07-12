#pragma once
#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <array>
using namespace std;

template<typename T>
string toString(T arg)
{
	stringstream ss;
	ss << arg;
	return ss.str();
}

