#include "map.hpp"

vector<vector<int>> Map::map(50, vector<int>(50, 0));

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
        map[-16][-16]=1;
        map[-16][-17]=1;
        map[-16][-18]=1;
        map[-16][-19]=1;
        map[-17][-16]=1;
        map[-18][-16]=1;
        map[-19][-16]=1;





        break;

    default:
        throw (mapNumber);
        break;
    }

}

bool Map::setBasicMap()
{
    if (map.size()==50 && map[0].size()==50)
    {
        //boundary wall creation
        for (int i = 0; i < 50; i++)
        {
            if (i==0 || i==49)
            {
                map[i]=vector<int>(50,1);
            }
            else
            {
                map[i][0]=1;
                map[i][-1]=1;
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
        map[-2][-2]=3;
        map[-2][-3]=3;
        map[-2][-4]=3;
        map[-3][-2]=3;
        map[-3][-3]=3;
        map[-3][-4]=3;

        return true;
    }
    else
    {
        return false;
    }
    
    
}