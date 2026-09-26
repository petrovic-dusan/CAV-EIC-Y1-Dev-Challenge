//
// Created by dusan on 9/15/26.
//

#include "../include/antworld.h"

void AntWorld::forage() {
    #include "../include/antworld.h"
    #include <set>
    #include <climits>

    // =========================================================
    // SHARED STATE between partners 
    // =========================================================

    struct WorldModel {
        MapTemplate visited;          // 0 = unseen, 1 = seen
        std::set<Coord> knownFood;    // food locations any ant has ever seen
    };

    static WorldModel g_model;
    static bool g_initialized = false;

    /** One-time setup: sizes the shared knowledge to the map and resets it. */
    void initSolution(AntWorld &world) {
        int rows = world.terrainMap.size();
        int cols = world.terrainMap[0].size();

        g_model.visited = MapTemplate(rows, std::vector<int>(cols, 0));
        g_model.knownFood.clear();

        g_initialized = true;
    }

    // =========================================================
    // PERSON A: Navigation & Safety — STUBS, replace on individual branch 
    // =========================================================

    void updateKnowledge(Ant &ant, AntWorld &world) {
        // TODO (Person A): record visible food + mark visited cells
    }

    bool canAfford(Ant &ant, AntWorld &world, Coord target) {
        return true; // TODO (Person A): real safety check
    }

    // =========================================================
    // PERSON B: Strategy & Decision-Making — STUBS, replace on individual branch 
    // =========================================================

    bool chooseFoodTarget(Ant &ant, AntWorld &world, std::set<Coord> &claimed, Coord &outTarget) {
        return false; // TODO (Person B): real food selection
    }

    Coord chooseExploreTarget(Ant &ant, AntWorld &world) {
        return world.homeCoordinates; // TODO (Person B): real explore logic
    }

    // =========================================================
    // forage() - connecting both halves 
    // =========================================================

    void AntWorld::forage() {
        if (!g_initialized) initSolution(*this);

        std::set<Coord> claimedThisStep;

        for (auto &ant : this->ants) {
            updateKnowledge(ant, *this);

            Coord target;
            Coord foodTarget;
            if (ant.carryingFood) {
                target = this->homeCoordinates;
            } else if (chooseFoodTarget(ant, *this, claimedThisStep, foodTarget)
                    && canAfford(ant, *this, foodTarget)) {
                target = foodTarget;
            } else {
                target = chooseExploreTarget(ant, *this);
            }

            if (target != this->homeCoordinates && !canAfford(ant, *this, target)) {
                target = this->homeCoordinates;
            }

            ant.move(this->terrainMap, target, this->foodMap);
        }
    }
}

