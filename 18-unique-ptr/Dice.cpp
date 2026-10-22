// Dice.cpp - the "kitchen": HOW the dice machine works.
//
// A .cpp file DEFINES: it gives the bodies. It includes its own header
// first, so the compiler can check that the promise and the body match.
#include "Dice.h"

#include <random>

int rollDice(int min, int max) {
    static std::mt19937 generator{std::random_device{}()};
    std::uniform_int_distribution<int> distribution{min, max};
    return distribution(generator);
}
