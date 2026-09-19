#ifndef DEV_CHALLENGE_UTILITY_FUNCTIONS_H
#define DEV_CHALLENGE_UTILITY_FUNCTIONS_H
#include <vector>
#include <queue>
#include <utility>
#include <limits>
#include <algorithm>
#include <cmath>

using Coord = std::pair<int, int>;
using MapTemplate = std::vector<std::vector<int> >;

struct Node {
    int cost;
    Coord pos;

    bool operator>(const Node &other) const {
        return cost > other.cost;
    }
};

inline std::vector<Coord> shortestPath(
    const std::vector<std::vector<int> > &grid,
    Coord start,
    Coord goal) {
    int rows = grid.size();
    int cols = grid[0].size();

    const int INF = std::numeric_limits<int>::max();

    std::vector<std::vector<int> > dist(
        rows, std::vector<int>(cols, INF)
    );

    std::vector<std::vector<Coord> > parent(
        rows, std::vector<Coord>(cols, {-1, -1})
    );

    std::priority_queue<
        Node,
        std::vector<Node>,
        std::greater<Node>
    > pq;

    dist[start.first][start.second] = 0;
    pq.push({0, start});

    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();

        int cost = current.cost;
        int r = current.pos.first;
        int c = current.pos.second;

        // Ignore outdated queue entries
        if (cost != dist[r][c])
            continue;

        if (current.pos == goal)
            break;

        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if (nr < 0 || nr >= rows ||
                nc < 0 || nc >= cols)
                continue;

            int moveCost =
                    1 + std::abs(grid[r][c] - grid[nr][nc]);

            int newCost = cost + moveCost;

            if (newCost < dist[nr][nc]) {
                dist[nr][nc] = newCost;
                parent[nr][nc] = {r, c};

                pq.push({
                    newCost,
                    {nr, nc}
                });
            }
        }
    }

    if (dist[goal.first][goal.second] == INF)
        return {};

    std::vector<Coord> path;

    for (Coord cur = goal;
         cur != start;
         cur = parent[cur.first][cur.second]) {
        path.push_back(cur);
    }

    path.push_back(start);

    std::reverse(path.begin(), path.end());

    return path;
}

inline int calculatePathCost(
    const std::vector<std::vector<int> > &grid,
    const std::vector<Coord> &path) {
    int cost = 0;

    for (int i = 1; i < path.size(); ++i) {
        auto [r1, c1] = path[i - 1];
        auto [r2, c2] = path[i];

        cost += 1 + std::abs(grid[r1][c1] - grid[r2][c2]);
    }

    return cost;
}

inline int moveAlongPath(
    const std::vector<std::vector<int> > &grid,
    const std::vector<Coord> &path) {
    int cost = 0;

    for (int i = 1; i < path.size(); ++i) {
        auto [r1, c1] = path[i - 1];
        auto [r2, c2] = path[i];

        cost += 1 + std::abs(grid[r1][c1] - grid[r2][c2]);
    }

    return cost;
}

inline MapTemplate generateWorldMap(int mapSize_x, int mapSize_y, std::mt19937 &rng) {
    MapTemplate map(mapSize_x, std::vector<int>(mapSize_y));

    for (int row = 0; row < mapSize_x; ++row) {
        for (int col = 0; col < mapSize_y; ++col) {
            int minValue = 0;
            int maxValue = 2;

            // Left
            if (col > 0) {
                minValue = std::max(minValue, map[row][col - 1] - 1);
                maxValue = std::min(maxValue, map[row][col - 1] + 1);
            }

            // Top-left
            if (row > 0 && col > 0) {
                minValue = std::max(minValue, map[row - 1][col - 1] - 1);
                maxValue = std::min(maxValue, map[row - 1][col - 1] + 1);
            }

            // Top
            if (row > 0) {
                minValue = std::max(minValue, map[row - 1][col] - 1);
                maxValue = std::min(maxValue, map[row - 1][col] + 1);
            }

            // Top-right
            if (row > 0 && col < mapSize_y - 1) {
                minValue = std::max(minValue, map[row - 1][col + 1] - 1);
                maxValue = std::min(maxValue, map[row - 1][col + 1] + 1);
            }

            std::uniform_int_distribution<int> dist(minValue, maxValue);
            map[row][col] = dist(rng);
        }
    }

    return map;
}

inline MapTemplate spreadFood(int mapSize_x, int mapSize_y, int foodCount, std::mt19937 &rng) {
    {
        std::vector<std::vector<int> > map(mapSize_x, std::vector<int>(mapSize_y, 0));

        // Generate all possible cell indices
        std::vector<int> indices(mapSize_x * mapSize_y);
        std::iota(indices.begin(), indices.end(), 0);

        // Randomize their order
        std::shuffle(indices.begin(), indices.end(), rng);

        // Set the first x randomly selected cells to 1
        for (int i = 0; i < foodCount; ++i) {
            int row = indices[i] / mapSize_y;
            int col = indices[i] % mapSize_y;

            map[row][col] = 1;
        }

        return map;
    }
}

inline bool hasFood(MapTemplate &foodMap) {
    for (const auto &row: foodMap) {
        if (std::find(row.begin(), row.end(), 1) != row.end()) {
            return true;
        }
    }

    return false;
}
#endif //DEV_CHALLENGE_UTILITY_FUNCTIONS_H
