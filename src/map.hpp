#ifndef MAP_H
#define MAP_H

#include <vector>

using namespace std;

struct Map
{
	static vector<vector<int>> map;

	static void setMap(int mapNumber = 0); // 0 is for player setting the map, else preset maps (will need to define them as global variables in game.cpp)
};

#endif
