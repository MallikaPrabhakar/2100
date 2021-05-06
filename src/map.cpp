#include "map.hpp"

vector<vector<int>> Map::map(MAP_SIZE, vector<int>(MAP_SIZE, 0));

void Map::setMap(int mapNumber)
{
	bool validMap = setBasicMap();
	//need to edit the if part out later
	if (!validMap)
	{
		//initialisation error
		return;
	}

	//actual map setup
	switch (mapNumber)
	{
	case -1:
		//blank map alread created by setBasicMap while initialising validMap
		break;
	case 0:
		/* code */

		map[3][6] = 1;
		map[3][7] = 1;
		map[3][8] = 1;
		map[3][9] = 1;
		map[3][10] = 1;
		map[3][11] = 1;
		map[3][12] = 1;
		map[5][6] = 1;
		map[4][6] = 1;
		map[6][6] = 1;

		map[10][15] = 1;
		map[10][16] = 1;
		map[10][17] = 1;
		map[10][18] = 1;
		map[10][14] = 1;
		map[10][13] = 1;
		map[10][12] = 1;
		map[10][11] = 1;
		map[11][15] = 1;
		map[12][15] = 1;
		map[7][15] = 1;
		map[10][15] = 1;
		map[9][15] = 1;
		map[8][15] = 1;

		map[18][4] = 1;
		map[18][5] = 1;
		map[18][6] = 1;
		map[18][7] = 1;
		map[18][8] = 1;
		map[18][9] = 1;
		map[17][4] = 1;
		map[16][4] = 1;
		map[15][4] = 1;
		map[14][4] = 1;

		map[18][14] = 1;
		map[18][15] = 1;
		map[18][16] = 1;
		map[18][17] = 1;
		map[18][18] = 1;
		map[18][19] = 1;
		map[19][19] = 1;
		map[20][19] = 1;
		map[21][19] = 1;
		break;

	default:
		throw(mapNumber);
		break;
	}
}

bool Map::setBasicMap()
{
	if (map.size() == MAP_SIZE && map[0].size() == MAP_SIZE)
	{
		//boundary wall creation
		for (int i = 0; i < MAP_SIZE; i++)
		{
			if (i == 0 || i == 24)
			{
				map[i] = vector<int>(MAP_SIZE, 1);
			}
			else
			{
				map[i][0] = 1;
				map[i][24] = 1;
			}
		}
		//home base of player 1
		map[1][1] = 2;
		map[1][2] = 2;
		map[1][3] = 2;
		map[2][1] = 2;
		map[2][2] = 2;
		map[2][3] = 2;

		//home base for player 2
		map[23][23] = 3;
		map[22][22] = 3;
		map[22][21] = 3;
		map[22][23] = 3;
		map[22][22] = 3;
		map[22][21] = 3;

		return true;
	}
	else
	{
		return false;
	}
}

int Map::sendMap()
{
	char mapChar[MAP_SIZE * MAP_SIZE];
	for (int i = 0; i < MAP_SIZE; ++i)
		for (int j = 0; j < MAP_SIZE; ++j)
			mapChar[i * MAP_SIZE + j] = map[i][j];
	return Network::sendRequest(mapChar, MAP_SIZE * MAP_SIZE);
}

int Map::recvMap()
{
	char mapChar[MAP_SIZE * MAP_SIZE];
	if (Network::recvRequest(mapChar, MAP_SIZE * MAP_SIZE) == 4)
		return 4;
	for (int i = 0; i < MAP_SIZE; ++i)
		for (int j = 0; j < MAP_SIZE; ++j)
			map[i][j] = mapChar[i * MAP_SIZE + j];
	return 0;
}
