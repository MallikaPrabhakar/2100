#include "map.hpp"

vector<vector<bool>> Map::map(MAP_SIZE, vector<bool>(MAP_SIZE, 0));

void Map::setMap(int mapNumber)
{
	setBasicMap();

	//actual map setup
	switch (mapNumber)
	{
	case 0:
		setMap(rand() % 5 + 1);
		break;
	case 1:
		//windmill

		//cross at centre:
		for (int i = 5; i < 20; i++)
		{
			map[12][i] = 1;
			map[i][12] = 1;
		}

		//top-left
		map[6][4] = 1;
		map[6][5] = 1;
		map[6][6] = 1;
		map[6][7] = 1;
		map[3][7] = 1;
		map[4][7] = 1;
		map[5][7] = 1;

		//bottom-left
		map[6][17] = 1;
		map[6][18] = 1;
		map[6][19] = 1;
		map[6][20] = 1;
		map[5][17] = 1;
		map[4][17] = 1;
		map[3][17] = 1;

		//top-right
		map[18][4] = 1;
		map[18][5] = 1;
		map[18][6] = 1;
		map[18][7] = 1;
		map[19][7] = 1;
		map[20][7] = 1;
		map[21][7] = 1;

		//bottom-right
		map[18][17] = 1;
		map[18][18] = 1;
		map[18][19] = 1;
		map[18][20] = 1;
		map[19][17] = 1;
		map[20][17] = 1;
		map[21][17] = 1;

		break;

	case 2:
		//tunnels

		//tunnel base
		for (int i = 7; i < 18; i++)
		{
			map[i][4] = 1;
			map[i][8] = 1;
			map[i][12] = 1;
			map[i][16] = 1;
			map[i][20] = 1;
		}
		//longer tunnels extensions
		for (int i = 0; i < 4; i++)
		{
			map[3 + i][8] = 1;
			map[18 + i][8] = 1;
			map[3 + i][16] = 1;
			map[18 + i][16] = 1;
		}

		break;

	case 3:
		//bamboo

		//bamboo base
		for (int i = 4; i < 21; i++)
		{
			map[4][i] = 1;
			map[9][i] = 1;
			map[15][i] = 1;
			map[20][i] = 1;
		}

		//middle bamboo extension
		map[9][3] = 1;
		map[9][21] = 1;
		map[15][3] = 1;
		map[15][21] = 1;

		//leaves
		for (int i = 0; i < 4; i++)
		{
			if (i % 2 == 0)
			{
				map[5][6 + 4 * i] = 1;
				map[6][6 + 4 * i] = 1;
				map[10][6 + 4 * i] = 1;
				map[11][6 + 4 * i] = 1;
				map[16][6 + 4 * i] = 1;
				map[17][6 + 4 * i] = 1;
				map[21][6 + 4 * i] = 1;
				map[22][6 + 4 * i] = 1;
			}
			else
			{
				map[3][6 + 4 * i] = 1;
				map[2][6 + 4 * i] = 1;
				map[8][6 + 4 * i] = 1;
				map[7][6 + 4 * i] = 1;
				map[14][6 + 4 * i] = 1;
				map[13][6 + 4 * i] = 1;
				map[18][6 + 4 * i] = 1;
				map[19][6 + 4 * i] = 1;
			}
		}

		break;

	case 4:
		//ruins

		//islands of walls:
		map[2][8] = 1;
		map[2][9] = 1;
		map[2][10] = 1;

		map[3][8] = 1;
		map[3][15] = 1;
		map[3][16] = 1;

		map[4][15] = 1;
		map[4][16] = 1;
		map[4][19] = 1;

		map[5][4] = 1;
		map[5][5] = 1;
		map[5][19] = 1;

		map[6][3] = 1;
		map[6][4] = 1;
		map[6][5] = 1;
		map[6][6] = 1;
		map[6][9] = 1;
		map[6][18] = 1;
		map[6][19] = 1;
		map[6][20] = 1;
		map[6][21] = 1;

		map[7][9] = 1;
		map[7][13] = 1;
		map[7][14] = 1;
		map[7][20] = 1;
		map[7][21] = 1;

		map[8][13] = 1;
		map[8][14] = 1;
		map[8][15] = 1;
		map[8][16] = 1;

		map[9][15] = 1;
		map[9][16] = 1;

		map[10][4] = 1;
		map[10][5] = 1;
		map[10][22] = 1;

		map[11][4] = 1;
		map[11][5] = 1;
		map[11][9] = 1;
		map[11][10] = 1;
		map[11][11] = 1;
		map[11][22] = 1;

		map[12][4] = 1;
		map[12][5] = 1;
		map[12][6] = 1;
		map[12][11] = 1;
		map[12][12] = 1;
		map[12][17] = 1;
		map[12][18] = 1;
		map[12][19] = 1;
		map[12][22] = 1;

		map[13][6] = 1;
		map[13][18] = 1;
		map[13][19] = 1;

		map[14][2] = 1;
		map[14][18] = 1;
		map[14][19] = 1;

		map[15][2] = 1;
		map[15][14] = 1;
		map[15][15] = 1;

		map[16][8] = 1;
		map[16][9] = 1;
		map[16][14] = 1;
		map[16][15] = 1;
		map[16][16] = 1;

		map[17][4] = 1;
		map[17][5] = 1;
		map[17][9] = 1;
		map[17][19] = 1;
		map[17][20] = 1;

		map[18][5] = 1;
		map[18][9] = 1;
		map[18][10] = 1;
		map[18][19] = 1;
		map[18][20] = 1;

		map[19][9] = 1;
		map[19][10] = 1;
		map[19][13] = 1;
		map[19][14] = 1;
		map[19][15] = 1;
		map[19][20] = 1;

		map[20][3] = 1;
		map[20][6] = 1;
		map[20][15] = 1;
		map[20][20] = 1;

		map[21][3] = 1;
		map[21][6] = 1;
		map[21][15] = 1;
		map[21][16] = 1;

		map[22][6] = 1;
		map[22][7] = 1;
		map[22][15] = 1;
		map[22][16] = 1;

		break;

	case 5:
		//honeycomb

		//boundary attached walls
		for (int i = 1; i < 4; i++)
		{
			//vertical top
			map[6 * i][1] = 1;
			map[6 * i][2] = 1;
			map[6 * i][3] = 1;
			map[6 * i][4] = 1;

			//vertical bottom
			map[6 * i][21] = 1;
			map[6 * i][22] = 1;
			map[6 * i][23] = 1;
			map[6 * i][20] = 1;

			//horizontal left
			map[1][6 * i] = 1;
			map[2][6 * i] = 1;
			map[3][6 * i] = 1;
			map[4][6 * i] = 1;

			//horizontal right
			map[20][6 * i] = 1;
			map[21][6 * i] = 1;
			map[22][6 * i] = 1;
			map[23][6 * i] = 1;
		}

		//extension of middle walls
		map[12][5] = 1;
		map[12][6] = 1;
		map[12][18] = 1;
		map[12][19] = 1;

		map[5][12] = 1;
		map[6][12] = 1;
		map[18][12] = 1;
		map[19][12] = 1;

		//floating walls
		map[8][8] = 1;
		map[9][8] = 1;
		map[10][8] = 1;
		map[8][9] = 1;
		map[8][10] = 1;

		map[8][14] = 1;
		map[9][16] = 1;
		map[10][16] = 1;
		map[8][15] = 1;
		map[8][16] = 1;

		map[14][8] = 1;
		map[15][8] = 1;
		map[16][8] = 1;
		map[16][9] = 1;
		map[16][10] = 1;

		map[16][14] = 1;
		map[16][16] = 1;
		map[15][16] = 1;
		map[16][15] = 1;
		map[14][16] = 1;

		break;

	case 6:
		generateRandomMaze();
		break;

	default:
		throw(mapNumber);
		break;
	}
}

void Map::generateRandomMaze()
{
	assert(MAP_SIZE & 1);
	vector<vector<array<bool, 4>>> walls = generateRandomWalls();
	removeWalls(walls);

	for (int i = 0; i < MAP_SIZE / 2; ++i)
	{
		vector<bool> col1(MAP_SIZE, 0), col2(MAP_SIZE, 0);
		assert(walls[i][0][0]);
		col1[0] = col2[0] = 1;
		for (int j = 0; j < MAP_SIZE / 2; ++j)
		{
			if (walls[i][j][3])
				col1[2 * j] = col1[2 * j + 1] = col1[2 * j + 2] = 1;
			if (walls[i][j][2])
				col1[2 * j + 2] = col2[2 * j + 2] = 1;
		}
		map[2 * i] = col1;
		map[2 * i + 1] = col2;
	}
}

vector<vector<array<bool, 4>>> Map::generateRandomWalls()
{
	vector<vector<bool>> visited(MAP_SIZE / 2, vector<bool>(MAP_SIZE / 2, 0));
	vector<vector<array<bool, 4>>> walls(visited.size(), vector<array<bool, 4>>(visited[0].size(), {1, 1, 1, 1}));
	stack<pair<int, int>> partlyProcessed;
	pair<int, int> xy = {rand() % (MAP_SIZE / 2), rand() % (MAP_SIZE / 2)};

	while (true)
	{
		visited[xy.first][xy.second] = 1;
		vector<int> directions;
		if (xy.second != 0 && !visited[xy.first][xy.second - 1])
			directions.push_back(0);
		if (xy.first != MAP_SIZE / 2 - 1 && !visited[xy.first + 1][xy.second])
			directions.push_back(1);
		if (xy.second != MAP_SIZE / 2 - 1 && !visited[xy.first][xy.second + 1])
			directions.push_back(2);
		if (xy.first != 0 && !visited[xy.first - 1][xy.second])
			directions.push_back(3);

		if (directions.empty())
		{
			if (partlyProcessed.empty())
				break;
			xy = partlyProcessed.top();
			partlyProcessed.pop();
			continue;
		}

		if (directions.size() > 1)
			partlyProcessed.push(xy);
		int direction = directions[rand() % directions.size()];
		walls[xy.first][xy.second][direction] = 0;
		if (direction == 0)
			--xy.second;
		else if (direction == 1)
			++xy.first;
		else if (direction == 2)
			++xy.second;
		else
			--xy.first;
		walls[xy.first][xy.second][(direction + 2) % 4] = 0;
	}

	return walls;
}

void Map::removeWalls(vector<vector<array<bool, 4>>> &walls)
{
	int removed = 0;
	while (removed < (MAP_SIZE / 2) * (MAP_SIZE / 2) * REMOVE_PROB)
	{
		int row = rand() % (MAP_SIZE / 2), col = rand() % (MAP_SIZE / 2);
		vector<int> directions;
		if (row != 0 && walls[col][row][0])
			directions.push_back(0);
		if (col != MAP_SIZE / 2 - 1 && walls[col][row][1])
			directions.push_back(1);
		if (row != MAP_SIZE / 2 - 1 && walls[col][row][2])
			directions.push_back(2);
		if (col != 0 && walls[col][row][3])
			directions.push_back(3);
		for (int direction : directions)
		{
			if (rand() * 1.0 / RAND_MAX >= REMOVE_PROB)
				continue;
			++removed;
			walls[col][row][direction] = 0;
			if (direction == 0)
				--row;
			else if (direction == 1)
				++col;
			else if (direction == 2)
				++row;
			else
				--col;
			walls[col][row][(direction + 2) % 4] = 0;
		}
	}
}

void Map::setBasicMap()
{
	map.assign(MAP_SIZE, vector<bool>(MAP_SIZE, 0));
	//boundary wall creation
	for (int i = 0; i < MAP_SIZE; i++)
	{
		if (i == 0 || i == 24)
		{
			map[i] = vector<bool>(MAP_SIZE, 1);
		}
		else
		{
			map[i][0] = 1;
			map[i][24] = 1;
		}
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
