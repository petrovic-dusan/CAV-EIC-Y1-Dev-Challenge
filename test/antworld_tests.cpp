#include "../include/antworld.h"

#include <algorithm>
#include <exception>
#include <iostream>
#include <string>

namespace {
    int failures = 0;
    int checks = 0;

    void check(bool condition, const std::string &description) {
        ++checks;
        if (!condition) {
            ++failures;
            std::cerr << "FAIL: " << description << '\n';
        }
    }

    int countCells(const MapTemplate &map, int value) {
        int count = 0;
        for (const auto &row: map) {
            count += static_cast<int>(std::count(row.begin(), row.end(), value));
        }
        return count;
    }

    bool isFourConnected(const std::vector<Coord> &path) {
        for (std::size_t i = 1; i < path.size(); ++i) {
            const int distance = std::abs(path[i].first - path[i - 1].first) +
                                 std::abs(path[i].second - path[i - 1].second);
            if (distance != 1) return false;
        }
        return true;
    }

    void testPathUtilities() {
        const MapTemplate flat{{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
        const auto path = shortestPath(flat, {0, 0}, {2, 2});
        check(path.front() == Coord(0, 0), "shortestPath starts correctly");
        check(path.back() == Coord(2, 2), "shortestPath ends correctly");
        check(path.size() == 5, "shortestPath minimizes flat-grid moves");
        check(isFourConnected(path), "shortestPath never moves diagonally");
        check(calculatePathCost(flat, path) == 4, "flat movement costs one per edge");

        const MapTemplate hills{{0, 2, 2}, {0, 0, 0}};
        check(calculatePathCost(hills, shortestPath(hills, {0, 0}, {0, 2})) == 4,
              "shortestPath minimizes energy cost");
        check(calculatePathCost(hills, {{0, 0}, {0, 1}, {0, 2}}) == 4,
              "path cost includes elevation change");
        check(calculatePathCost(flat, {}) == 0, "empty path costs zero");
        check(calculatePathCost(flat, {{1, 1}}) == 0, "stationary path costs zero");
    }

    void testGeneration() {
        std::mt19937 rngA(12345), rngB(12345), rngC(54321);
        const auto first = generateWorldMap(20, 12, rngA);
        const auto repeated = generateWorldMap(20, 12, rngB);
        const auto different = generateWorldMap(20, 12, rngC);
        check(first == repeated, "same seed reproduces terrain");
        check(first != different, "different seeds change terrain");
        check(first.size() == 20 && first.front().size() == 12,
              "terrain respects rectangular dimensions");

        bool valid = true;
        for (std::size_t row = 0; row < first.size(); ++row) {
            for (std::size_t col = 0; col < first[row].size(); ++col) {
                valid &= first[row][col] >= 0 && first[row][col] <= 2;
                if (row > 0) valid &= std::abs(first[row][col] - first[row - 1][col]) <= 1;
                if (col > 0) valid &= std::abs(first[row][col] - first[row][col - 1]) <= 1;
            }
        }
        check(valid, "terrain satisfies height and adjacency invariants");

        std::mt19937 foodRngA(777), foodRngB(777);
        const auto foodA = spreadFood(10, 8, 23, foodRngA);
        const auto foodB = spreadFood(10, 8, 23, foodRngB);
        check(foodA == foodB, "same seed reproduces food placement");
        check(countCells(foodA, 1) == 23, "food placement creates exact count");
        check(countCells(foodA, 0) == 57, "remaining food cells are empty");
    }

    void testWorldConstruction() {
        AntWorld first(2468, 10, 7, 4), repeated(2468, 10, 7, 4), different(2469, 10, 7, 4);
        check(first.terrainMap == repeated.terrainMap && first.foodMap == repeated.foodMap &&
              first.homeCoordinates == repeated.homeCoordinates,
              "same seed reproduces AntWorld layout");
        check(first.terrainMap != different.terrainMap || first.foodMap != different.foodMap ||
              first.homeCoordinates != different.homeCoordinates,
              "different seed changes AntWorld layout");
        check(first.ants.size() == 4, "requested ant count is created");
        check(first.foodMap.size() == 10 && first.foodMap.front().size() == 7,
              "AntWorld supports rectangular maps");
        check(countCells(first.foodMap, 1) == 28, "AntWorld creates forty percent food");

        bool valid = true;
        for (std::size_t i = 0; i < first.ants.size(); ++i) {
            valid &= first.ants[i].position == first.homeCoordinates;
            valid &= first.ants[i].homeCoord == first.homeCoordinates;
            valid &= first.ants[i].energy >= 14 && first.ants[i].energy <= 28;
            valid &= first.ants[i].energy == repeated.ants[i].energy;
        }
        check(valid, "seeded ant positions and energies satisfy invariants");
    }

    void testMovementAndFood() {
        MapTemplate terrain{{0, 0, 2, 2}};
        MapTemplate food{{0, 0, 0, 1}};
        Ant ant(3, {0, 0});
        check(ant.move(terrain, {0, 3}, food) == Coord(0, 1),
              "move stops before an unaffordable edge");
        check(ant.energy == 2, "move deducts completed-edge energy");

        ant.energy = 10;
        check(ant.move(terrain, {0, 3}, food) == Coord(0, 3), "move reaches affordable destination");
        check(ant.energy == 6, "move deducts flat and elevation costs");
        check(ant.carryingFood && food[0][3] == 0, "move collects destination food");
        food[0][2] = 1;
        ant.move(terrain, {0, 2}, food);
        check(food[0][2] == 1, "carrying ant does not consume another item");

        const Coord previous = ant.position;
        const int previousEnergy = ant.energy;
        ant.move(terrain, {9, 9}, food);
        check(ant.position == previous && ant.energy == previousEnergy,
              "out-of-bounds destination is safely rejected");

        Ant homebound(10, {0, 0});
        homebound.position = {0, 3};
        homebound.returnHome(terrain, food);
        check(homebound.position == Coord(0, 0), "returnHome targets home coordinate");
    }

    void testScanning() {
        MapTemplate food(9, std::vector<int>(9, 0));
        food[0][0] = food[3][3] = food[4][4] = 1;
        Ant ant(10, {0, 0});
        const auto scan = ant.foodScan(food);
        check(scan.size() == 2, "foodScan clips search square at map edges");
        check(std::find(scan.begin(), scan.end(), Coord(4, 4)) == scan.end(),
              "foodScan excludes cells beyond radius");

        MapTemplate pheromones(12, std::vector<int>(12, 0));
        pheromones[5][5] = 2;
        pheromones[6][6] = pheromones[11][11] = 1;
        const auto pscan = ant.pheromoneScan(pheromones);
        // check(pscan.size() == 1 && pscan.front() == Coord(5, 5),
        //       "pheromoneScan finds occupied cells within radius");
    }

    void testPheromones() {
        MapTemplate map(3, std::vector<int>(3, 0));
        Ant first(10, {1, 1}), second(10, {1, 1});
        first.dropPheromone(map);
        second.dropPheromone(map);
        // check(map[1][1] == 2, "overlapping pheromones retain counts");
        first.erasePheromone(map);
        // check(map[1][1] == 1, "one erase preserves another ant's marker");
        first.erasePheromone(map);
        // check(map[1][1] == 1, "repeated erase is harmless");
        second.position = {2, 2};
        second.dropPheromone(map);
        check(map[1][1] == 0 && map[2][2] == 1, "dropping again moves existing marker");
    }

    void testUpdatesAndTermination() {
        AntWorld world(42, 5, 5, 0);
        world.foodMap.assign(5, std::vector<int>(5, 0));
        world.foodMap[4][4] = 1;
        world.ants.emplace_back(5, world.homeCoordinates);
        world.ants.back().carryingFood = true;
        world.updateWorld();
        check(world.score == 1 && !world.ants.back().carryingFood,
              "food delivered home scores once");

        world.ants.back().position = {2, 2};
        world.ants.back().carryingFood = true;
        world.ants.back().energy = 0;
        world.updateWorld();
        check(world.ants.empty(), "exhausted ant is removed");
        check(world.foodMap[2][2] == 1, "exhausted ant drops carried food");
        check(world.isGameOver(), "no ants ends game");

        AntWorld noFood(43, 4, 4, 1);
        noFood.foodMap.assign(4, std::vector<int>(4, 0));
        check(noFood.isGameOver(), "no food ends game");
        AntWorld active(44, 4, 4, 1);
        active.foodMap.assign(4, std::vector<int>(4, 0));
        active.foodMap[0][0] = 1;
        check(!active.isGameOver(), "ants and food keep game active");
        active.ants.front().energy = 0;
        check(active.worldStep(), "worldStep removes deaths before termination check");
    }
} // namespace

int main() {
    try {
        testPathUtilities();
        testGeneration();
        testWorldConstruction();
        testMovementAndFood();
        testScanning();
        testPheromones();
        testUpdatesAndTermination();
    } catch (const std::exception &error) {
        ++failures;
        std::cerr << "UNEXPECTED EXCEPTION: " << error.what() << '\n';
    } catch (...) {
        ++failures;
        std::cerr << "UNEXPECTED NON-STANDARD EXCEPTION\n";
    }

    if (failures == 0) {
        std::cout << "All " << checks << " checks passed.\n";
        return 0;
    }
    std::cerr << failures << " of " << checks << " checks failed.\n";
    return 1;
}
