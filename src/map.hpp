#ifndef MAP_H
#define MAP_H

#include <vector>
#include <random>
#include "networking.hpp"

using namespace std;

#define MAP_SIZE 25

struct Map
{
	static vector<vector<bool>> map;

	static void setMap(int mapNumber = 5);

	static void setBasicMap();				//sets the basic map boundaries and home bases
	static int sendMap();
	static int recvMap();
};

#endif
