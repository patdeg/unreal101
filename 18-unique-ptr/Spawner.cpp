// Spawner.cpp - how monsters are made.
#include "Spawner.h"

#include "Dice.h"

#include <utility>   // std::move

std::unique_ptr<Monster> Spawner::spawnRandom() {
    ++totalSpawned;
    // std::make_unique<Goblin>() creates a new Goblin and wraps it in a
    // unique_ptr, all in one step. This replaces the raw "new" from
    // lesson 17. A unique_ptr<Goblin> converts to a unique_ptr<Monster>,
    // because a Goblin is a Monster (lesson 15).
    switch (rollDice(1, 3)) {
        case 1:
            return std::make_unique<Goblin>();
        case 2:
            return std::make_unique<Troll>();
        default:
            return std::make_unique<Slime>();
    }
}

std::vector<std::unique_ptr<Monster>> Spawner::makeWave(int size) {
    std::vector<std::unique_ptr<Monster>> wave;
    for (int i = 0; i < size; ++i) {
        std::unique_ptr<Monster> monster = spawnRandom();

        // There can be only one owner. "wave.push_back(monster)" would try
        // to COPY the owner, and C++ refuses (see break-it.cpp).
        // std::move(monster) HANDS OVER ownership instead: afterwards the
        // vector owns the monster, and "monster" here is empty (nullptr).
        wave.push_back(std::move(monster));
    }
    // Returning the vector hands the whole wave to the caller. No copies:
    // C++ moves it automatically when you return a local variable.
    return wave;
}
