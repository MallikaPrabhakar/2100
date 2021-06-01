#ifndef MAP_H
#define MAP_H

#include <vector>
#include <stack>
#include <random>
#include "networking.hpp"
#include <iostream>

using namespace std;

#define MAP_SIZE 25

struct Map
{
	static vector<vector<bool>> map;

	static void setMap(int mapNumber = 0);
	static void generateRandomMaze();
	static void setBasicMap(); // sets the basic map boundaries and home bases
	static int sendMap();
	static int recvMap();
};

#endif
