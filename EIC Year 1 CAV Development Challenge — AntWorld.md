# EIC Year 1 CAV Development Challenge

## What is EcoCAR?

EcoCAR is an Advanced Vehicle Technology Competition (AVTC) that challenges university teams to research, design, and
implement next-generation automotive technologies. Students work across a variety of technical disciplines to develop
systems that are ultimately integrated and tested on a real vehicle.

## What is CAV?

The **Connected and Automated Vehicle (CAV) sub-team** is responsible for all autonomous functionality on the vehicle.

In previous competitions, our four primary autonomous features were:

- Cooperative Adaptive Cruise Control
- Lane Centering Control
- Automatic Intersection Navigation
- Auto Park

A new competition means new challenges, new technologies, and plenty of opportunities to innovate. In this competition,
autonomous features will be developed and tested for both **touring** and **track** modes.

Beyond individual autonomous features, CAV works across a wide range of technical areas, including computer vision,
sensor fusion, low-level controls, Ethernet and CAN communication, embedded systems programming, optimization,
automation, simulation, and more.

If you are interested in the intersection of **software, mechatronics, and AI**, CAV is the team for you.

# Your Challenge: AntWorld

This year's development challenge puts you in control of a **colony of ants exploring an unknown environment, searching
for food, and returning it to their home**.

At first, managing an ant colony may seem completely unrelated to developing software for an autonomous vehicle.
However, the fundamental problems—and the software engineering principles used to solve them—are surprisingly similar.

| AntWorld                         | CAV Development                                                          |
|----------------------------------|--------------------------------------------------------------------------|
| Game environment                 | Vehicle simulation environment                                           |
| Observe → Decide → Act → Update  | Perception → Decision-making → Control → Updated vehicle state           |
| Exploring an unknown map         | Collecting and interpreting sensor data                                  |
| Choosing an ant's next action    | Selecting vehicle behaviours, target speeds, lanes, trajectories, etc.   |
| Managing ant energy and movement | Accounting for range, physical constraints, and system limitations       |
| Coordinating multiple ants       | Coordinating components and behaviours within a larger autonomous system |

Most importantly, **the same fundamental programming and problem-solving principles apply to both.**

We are not necessarily looking for the person who discovers the mathematically perfect ant-foraging strategy. We want to
see how you approach an unfamiliar problem, structure your software, develop and test an algorithm, and improve your
solution.

# Structure of AntWorld

AntWorld consists of two primary components:

1. Ant colony — The group of manipulatable actors.
2. World map — 3 layers that represent the environment your ants must explore.

## The Ant Colony

Every `Ant` within the ant colony can be controlled individually.

Each ant begins the game with:

- A randomized amount of energy
- A limited viewing radius of the overall map
- The ability to move through the environment
- The ability to interact with pheromones (hint markers)
- The ability to collect and return food

### Movement

Ants may move around the map as long as they have enough energy to perform the requested movement.

The terrain is not necessarily flat, so the cost of travelling between locations may depend on both the distance
travelled and the change in terrain height. Efficient movement and route selection may therefore be important to your
strategy.

### Pheromones

Ants can drop and remove pheromones.

Pheromones can be detected by other ants at greater distances than their standard viewing range. This gives you a
mechanism for communicating information between ants.

For example, an ant could use pheromones to indicate that it has discovered food or to help guide other ants toward an
area of interest.

How—or whether—you use pheromones is up to you.

### Food and Scoring

Ants search the map for food. Once food is collected, it must be returned to the colony's home location to score points.

The game instance keeps track of information including:

- Total points
- Home coordinates
- Number of ants

Your goal is to develop a strategy that allows your colony to collect and return as much food as possible before its
available resources are exhausted.

# The World Map

The world map represents the environment in which your colony operates. It consists of **three two-dimensional layers
with
identical dimensions**.

| Layer             | Description                                                                                                                                                                                                  |
|-------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| **Terrain Map**   | A 2D array where each row and column represents a coordinate and the stored value represents the terrain height at that location. Neighbouring terrain cells have a maximum absolute height difference of 1. |
| **Food Map**      | A 2D array initialized to `0`. Random locations contain a value of `1`, indicating that food is present at that coordinate.                                                                                  |
| **Pheromone Map** | A 2D array representing pheromone locations across the map.                                                                                                                                                  |

Together, these layers describe the environment available to your ants. However, an individual ant does **not**
necessarily have access to the entire map. Your algorithm must make decisions using the information available through
the provided interfaces and each ant's observations.

# Your Task

The main game loop is driven by:

`AntWorld::step()`

Each step calls:

`AntWorld::forage()`

before updating the game state and checking whether the game has ended.

The game ends when `isGameOver()` determines that either:

- All available food has been collected, **or**
- All ants have run out of energy.

## `AntWorld::forage()`

**This is where the core of your solution will live.**

You are responsible for implementing the logic that determines what your colony does during each step of the simulation.

A useful way to think about your algorithm is:

### 1. Search

Observe the surroundings and available information for one or more ants.

What does each ant currently know? Is there visible food? Are there nearby pheromones? Has this area already been
explored? How much energy does the ant have remaining?

### 2. Decide

Use that information to determine what each ant should do next.

This is where you can implement your own strategy. Your ants might explore, pursue known food, return home, communicate
with other ants using pheromones, or perform some entirely different behaviour.

### 3. Act

Call the appropriate provided function to execute the desired action for the appropriate ant.

The AntWorld framework contains the necessary update functionality for each supported action. Your responsibility is to
determine **which actions should be taken and when**.

This **Search → Decide → Act** loop is intentionally similar to the structure used in autonomous systems: observe the
environment, determine the desired behaviour, execute an action, and repeat using the newly updated state.

# What Code Can You Modify?

You have significant freedom in how you implement your solution.

You may add functions as necessary, provided your changes **do not violate the spirit of the challenge or
bypass the intended game mechanics**. Any work done by the applicant should be added to the **`solution.cpp`** code
file.

You are encouraged to build upon the provided framework.

For example, if your strategy requires dividing the colony into groups with different responsibilities, you are free to
add functionality that helps manage those groups. Similarly, you may add supporting algorithms, data structures, helper
functions, or internal state that make your strategy easier to implement.

You may also add initialization functionality outside the main game loop. For example, your strategy could perform
one-time setup at the beginning of the game before the colony begins foraging.

These examples are intended only to demonstrate the freedom you have when designing your solution. **They are not hints
toward a preferred strategy.**

Research the problem, experiment with different approaches, and apply your own ideas.

Building new functionality on top of an existing codebase is a major part of developing real CAV software, and this
challenge is designed to give you an opportunity to demonstrate exactly that.

# What We're Looking For

There is no single correct solution to AntWorld.

Your final score matters, but **how you arrive at that score matters too**. We are interested in seeing how you:

- Approach and break down an unfamiliar problem
- Navigate and understand an existing C++ codebase
- Design algorithms and data structures
- Write clean, understandable, and maintainable code
- Account for constraints and edge cases
- Test and debug your implementation
- Make engineering trade-offs
- Iterate on your initial approach
- Research and apply ideas independently

A simple, well-designed solution that you understand and can explain is more valuable than an unnecessarily complicated
solution.

Be creative, experiment, and have fun with it.

    For any questions, please contact ***petrod6@mcmaster.ca*** via Microsoft Teams. Keep your messages concise and specific, and please allow up to **24 hours** for a response.