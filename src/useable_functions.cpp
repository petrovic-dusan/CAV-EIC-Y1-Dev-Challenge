//
// Created by dusan on 9/15/26.
//

#include "../include/antworld.h"

/** @brief Checks all squares within foodRadius blocks of itself.
 *
 * @param foodMap the food layer of the world map
 *
 * @return vector of coordinates of locations in that range that have food
 */
std::vector<Coord> Ant::foodScan(MapTemplate &foodMap) {
    std::vector<Coord> foodLocations = {};
    for (int i = this->position.first - this->foodRadius; i <= this->position.first + this->foodRadius; ++i) {
        for (int j = this->position.second - this->foodRadius; j <= this->position.second + this->foodRadius; ++j) {
            if (i < 0 || i >= foodMap.size() || j < 0 || j >= foodMap[0].size()) {
                continue;
            } else {
                if (foodMap[i][j] == 1) {
                    foodLocations.emplace_back(i, j);
                }
            }
        }
    }
    return foodLocations;
}

/** @brief Checks all squares within pheromoneRadius blocks of itself.
 *
 * @param pheromoneMap the pheromone layer of the world map
 *
 * @return vector of coordinates of locations in that range that have a pheromone marker
 */
std::vector<Coord> Ant::pheromoneScan(MapTemplate &pheromoneMap) {
    std::vector<Coord> pheromoneLocations = {};
    for (int i = this->position.first - this->pheromoneRadius; i <= this->position.first + this->pheromoneRadius; ++i) {
        for (int j = this->position.second - this->pheromoneRadius; j <= this->position.second + this->pheromoneRadius;
             ++j) {
            if (i < 0 || i >= pheromoneMap.size() || j < 0 || j >= pheromoneMap[0].size()) {
                continue;
            } else {
                if (pheromoneMap[i][j] == 1) {
                    pheromoneLocations.emplace_back(i, j);
                }
            }
        }
    }
    return pheromoneLocations;
}

/** @brief this function computes the most energy efficient route from the ant's current position to target destination.
 * it will follow the shortest path for as long as it has the energy to do so. When the ant reaches it's final position,
 * if food exists, it will pick it up.
 *
 * @param terrainMap terrain layer of the world map
 * @param dest coordinates of destination
 * @param foodMap food layer of the world map
 *
 * @return coordinates of final ant position. can be used to double check it's final position
 */
Coord Ant::move(MapTemplate &terrainMap, Coord dest, MapTemplate &foodMap) {
    if (dest.first >= terrainMap.size() || dest.second >= terrainMap[0].size()) {
        printf("illegal move: attempted to move to %d, %d in %d, %d grid space", dest.first, dest.second,
               terrainMap.size(), terrainMap[0].size());
        return this->position;
    }
    std::vector<Coord> path = shortestPath(terrainMap, this->position, dest);

    for (int i = 1; i < path.size(); ++i) {
        auto [r1, c1] = path[i - 1];
        auto [r2, c2] = path[i];

        int cost = 1 + std::abs(terrainMap[r1][c1] - terrainMap[r2][c2]);
        if (cost > energy) {
            break;
        }
        this->position = path[i];
        energy -= cost;
    }

    if (foodMap[this->position.first][this->position.second] == 1 && !this->carryingFood) {
        foodMap[this->position.first][this->position.second] = 0;
        this->carryingFood = true;
    }

    return this->position;
}

/** @brief removes existing pheromone and drops one at the ant's current location
 *
 * @param pheromoneMap pheromone layer of world map
 */
void Ant::dropPheromone(MapTemplate &pheromoneMap) {
    if (this->pheromoneDropped) {
        erasePheromone(pheromoneMap);
    }

    pheromoneMap[this->position.first][this->position.second] = 1;
    this->pheromonePosition = this->position;
    this->pheromoneDropped = true;
}

/** @brief removes ant's existing pheromone
 *
* @param pheromoneMap pheromone layer of world map
 */
void Ant::erasePheromone(MapTemplate &pheromoneMap) {
    if (this->pheromoneDropped) {
        pheromoneMap[this->pheromonePosition.first][this->pheromonePosition.second] = 0;
        this->pheromonePosition = Coord(-1, -1);
        this->pheromoneDropped = false;
    }
}

/** @brief basically the move function, with the destination preset to the home base coordinates
 *
 * @param terrainMap terrain layer of the world map
 * @param foodMap food layer of the world map
 *
 * @return coordinates of final ant position. can be used to double check it's final position
 */
Coord Ant::returnHome(MapTemplate &terrainMap, MapTemplate &foodMap) {
    return this->move(terrainMap, this->homeCoord, foodMap);
}
