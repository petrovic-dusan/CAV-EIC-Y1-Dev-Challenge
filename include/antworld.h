#ifndef DEV_CHALLENGE_ANTWORLD_H
#define DEV_CHALLENGE_ANTWORLD_H

#include <vector>
#include  <random>
#include "utility_functions.h"
//
// Created by dusan on 9/4/26.
//

class Ant {
public:
    Ant(int initEnergy, Coord homeCoordinates);

    std::vector<Coord> foodScan(MapTemplate &foodMap);

    std::vector<Coord> pheromoneScan(MapTemplate &pheromoneMap);

    Coord move(MapTemplate &terrainMap, Coord dest, MapTemplate &foodMap);

    void dropPheromone(MapTemplate &foodMap);

    void erasePheromone(MapTemplate &pheromoneMap);

    Coord returnHome(MapTemplate &terrainMap, MapTemplate &foodMap);

    int energy{0};

    Coord homeCoord = Coord(-1, -1);
    Coord position = Coord(-1, -1);

    int foodRadius{3};
    int pheromoneRadius{5};
    bool pheromoneDropped{false};
    Coord pheromonePosition = Coord(-1, -1);
    bool carryingFood{false};
};

class AntWorld {
public:
    AntWorld(uint32_t seed, int mapSize_x = 15, int mapSize_y = 15, int antCount = 8);

    bool worldStep();

    void forage();

    void updateWorld();

    bool isGameOver();

    MapTemplate terrainMap;
    MapTemplate foodMap;
    MapTemplate pheromoneMap;

    std::vector<Ant> ants = {};
    Coord homeCoordinates = Coord(-1, -1);

    int score = 0;

private:
    std::mt19937 rng;
};


#endif //DEV_CHALLENGE_ANTWORLD_H
