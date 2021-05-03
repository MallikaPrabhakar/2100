#include "map.hpp"

vector<vector<int>> Map::map(MAP_SIZE, vector<int>(MAP_SIZE, 0));

void Map::setMap(int mapNumber)
{
    bool validMap=setBasicMap();
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
        map[15][15]=1;
        map[15][16]=1;
        map[15][17]=1;
        map[15][18]=1;
        map[16][15]=1;
        map[17][15]=1;
        map[18][15]=1;
        map[33][33]=1;
        map[33][32]=1;
        map[33][31]=1;
        map[33][30]=1;
        map[32][33]=1;
        map[31][33]=1;
        map[30][33]=1;
        break;

    default:
        throw (mapNumber);
        break;
    }

}

bool Map::setBasicMap()
{
    if (map.size()==MAP_SIZE && map[0].size()==MAP_SIZE)
    {
        //boundary wall creation
        for (int i = 0; i < MAP_SIZE; i++)
        {
            if (i==0 || i==49)
            {
                map[i]=vector<int>(MAP_SIZE,1);
            }
            else
            {
                map[i][0]=1;
                map[i][49]=1;
            }         
            
        }
        //home base of player 1
        map[1][1]=2;
        map[1][2]=2;
        map[1][3]=2;
        map[2][1]=2;
        map[2][2]=2;
        map[2][3]=2;

        //home base for player 2
        map[48][48]=3;
        map[48][47]=3;
        map[48][46]=3;
        map[47][48]=3;
        map[47][47]=3;
        map[47][46]=3;

        return true;
    }
    else
    {
        return false;
    }
    
    
}