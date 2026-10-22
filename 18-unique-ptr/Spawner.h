// Spawner.h - creates monsters while the game runs.
#pragma once

#include "Monster.h"

#include <memory>   // std::unique_ptr, std::make_unique
#include <vector>

// A std::unique_ptr<Monster> is a pointer that OWNS the monster it points
// at. There is exactly one owner at a time. When the owner is destroyed (or
// erased from a vector), the monster is destroyed too: its destructor runs
// and its memory is freed. No "delete" to forget.
class Spawner {
public:
    // One random monster: a Goblin, a Troll or a Slime.
    std::unique_ptr<Monster> spawnRandom();

    // A whole wave of them. The vector owns every monster in it.
    std::vector<std::unique_ptr<Monster>> makeWave(int size);

    int getTotalSpawned() const { return totalSpawned; }

private:
    int totalSpawned = 0;
};
