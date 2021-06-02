#ifndef MAP_H
#define MAP_H

#include <vector>
#include <array>
#include <stack>
#include <random>
#include <assert.h>
#include "networking.hpp"

using namespace std;

#define MAP_SIZE 25
#define REMOVE_PROB 0.33

struct Map
{
	static vector<vector<bool>> map;

	static void setMap(int mapNumber = 0);
	static void generateRandomMaze();
	static vector<vector<array<bool, 4>>> generateRandomWalls();
	static void removeWalls(vector<vector<array<bool, 4>>> &walls);
	static void setBasicMap(); // sets the basic map boundaries and home bases
	static int sendMap();
	static int recvMap();
};

#endif
