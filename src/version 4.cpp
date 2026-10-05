//
// Created by dusan on 9/15/26.
//

#include "../include/antworld.h"


/** @brief this is where you as the applicant will make use of the above functions to develop your solution.
 * here are some existing examples of how calling these functions works to help get you started!
 */
void AntWorld::forage() {

    int rows = this->terrainMap.size();
    int cols = this->terrainMap[0].size();

    static std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

    std::vector<Coord> claimedTargets;

    for (int i = 0; i < this->ants.size(); i++) {

        int currentRow = this->ants[i].position.first;
        int currentCol = this->ants[i].position.second;

        visited[currentRow][currentCol] = true;

        if (this->ants[i].carryingFood) {
            this->ants[i].returnHome(this->terrainMap, this->foodMap); //send ant home if True using the world's terrain and food maps
        }

        else {
            std::vector<Coord> visibleFood = this->ants[i].foodScan(this->foodMap); //ant looks around using itsmradius and returns detected food coords

            if (!visibleFood.empty()) {

                Coord target = visibleFood[0];
                int bestFoodDistance = rows + cols;

                for (int j = 0; j < visibleFood.size(); j++) {

                    int foodRowDistance = visibleFood[j].first - currentRow;

                    if (foodRowDistance < 0) {
                        foodRowDistance = -foodRowDistance;
                    }

                    int foodColDistance = visibleFood[j].second - currentCol;

                    if (foodColDistance < 0) {
                        foodColDistance = -foodColDistance;
                    }

                    int foodDistance = foodRowDistance + foodColDistance;

                    if (foodDistance < bestFoodDistance) {
                        bestFoodDistance = foodDistance;
                        target = visibleFood[j];
                    }
                }

                this->ants[i].move(this->terrainMap, target, this->foodMap); //ant moves toward the selected visible food
            }

            else {

                Coord destination = this->ants[i].position;

                int bestDistance = rows + cols;
                bool destinationFound = false;

                for (int row = 0; row < rows; row++) {

                    for (int col = 0; col < cols; col++) {

                        if (!visited[row][col]) {

                            bool alreadyClaimed = false;

                            for (int j = 0; j < claimedTargets.size(); j++) {

                                if (claimedTargets[j] == Coord(row, col)) {
                                    alreadyClaimed = true;
                                    break;
                                }
                            }

                            if (!alreadyClaimed) {

                                int rowDistance = row - currentRow;

                                if (rowDistance < 0) {
                                    rowDistance = -rowDistance;
                                }

                                int colDistance = col - currentCol;

                                if (colDistance < 0) {
                                    colDistance = -colDistance;
                                }

                                int distance = rowDistance + colDistance;

                                if (distance < bestDistance) {
                                    bestDistance = distance;
                                    destination = Coord(row, col);
                                    destinationFound = true;
                                }
                            }
                        }
                    }
                }

                if (destinationFound) {

                    claimedTargets.push_back(destination);
                    Coord nextPosition = this->ants[i].position;

                    //compare ant's current coordinates with destination coordinates and move a single block each time
                    if (nextPosition.first < destination.first) {
                        nextPosition.first++;
                    }
                    else if (nextPosition.first > destination.first) {
                        nextPosition.first--;
                    }
                    else if (nextPosition.second < destination.second) {
                        nextPosition.second++;
                    }
                    else if (nextPosition.second > destination.second) {
                        nextPosition.second--;
                    }

                    this->ants[i].move(this->terrainMap, nextPosition, this->foodMap);
                }
            }
        }
    }
}

/** You may insert any custom functions below **/