#include <iostream>
#include "../include/antworld.h"

/** @brief The main function that will run the game. If you are not using a IDE gui, this is the executable you want to target when you build
 */
int main() {
    /**
     * default seed. you can change this to any number you want. for as long as the seed is the same, the same "random" world will
     * always be generated. You can use this to test reproducibly while developing.
     */
    const uint32_t SEED = 12345;

    // once you're confident and want to begin testing on random seeds, you can comment out the above line
    // uncomment the following ones.
    // std::random_device rd;
    // uint32_t SEED = rd();

    const int MAX_SIMULATION_STEP_COUNT = 1000;
    AntWorld gameInstance = AntWorld(SEED);

    bool gameOver = false;
    int stepCount = 1;
    while (not gameOver && stepCount <= MAX_SIMULATION_STEP_COUNT) {
        gameOver = gameInstance.worldStep();
        stepCount++;
    }

    if (gameOver) {
        printf("GAME OVER!! Total score: %d\n", gameInstance.score);
    } else if (stepCount >= MAX_SIMULATION_STEP_COUNT) {
        printf("Game not finished. Hit maxmimum simulation step count");
    } else {
        printf("Termination reached for unknown reason.");
    }
}
