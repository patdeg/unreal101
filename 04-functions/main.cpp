// main.cpp - Lesson 04: Functions with types
//
// Arena step: damage is no longer fixed. A "dice machine" rolls it.

#include <iostream>
#include <random>   // C++'s random number tools
#include <string>

// A function says what goes IN and what comes OUT, with types:
//
//   int rollDice(int min, int max)
//   ^   ^        ^
//   |   |        two whole numbers go in
//   |   the name
//   a whole number comes out
//
// This function must be written ABOVE the place where you call it. The
// compiler reads top to bottom and has to know a name before you use it.
//
// The three lines inside are a recipe. Treat them as a dice machine for now:
//   - "generator" is the machine, seeded from the computer's real randomness
//   - "static" keeps the machine alive between calls
//   - "distribution" turns raw randomness into a number from min to max
int rollDice(int min, int max) {
    static std::mt19937 generator{std::random_device{}()};
    std::uniform_int_distribution<int> distribution{min, max};
    return distribution(generator);
}

int main() {
    const std::string heroName = "Ayla";
    const std::string monsterName = "Goblin";
    int heroHp = 40;
    int monsterHp = 30;
    const int potionHeal = 10;

    std::cout << heroName << " faces a " << monsterName << "!\n";

    while (heroHp > 0 && monsterHp > 0) {
        std::cout << "\n" << heroName << " HP " << heroHp
                  << "   " << monsterName << " HP " << monsterHp << "\n";
        std::cout << "(a)ttack or (h)eal? ";

        char choice = ' ';
        if (!(std::cin >> choice)) {
            std::cout << "\nNo more input. Goodbye.\n";
            return 0;
        }

        if (choice == 'a') {
            int damage = rollDice(8, 16);   // a call: values in, a value out
            monsterHp -= damage;
            std::cout << heroName << " hits for " << damage << "!\n";
        } else if (choice == 'h') {
            heroHp += potionHeal;
            std::cout << heroName << " drinks a potion (+" << potionHeal << ").\n";
        } else {
            std::cout << "That is not a move. You lose your turn!\n";
        }

        if (monsterHp > 0) {
            int damage = rollDice(3, 9);
            heroHp -= damage;
            std::cout << monsterName << " hits for " << damage << "!\n";
        }
    }

    if (heroHp > 0) {
        std::cout << "\nVictory! " << heroName << " wins.\n";
    } else {
        std::cout << "\nDefeat... the " << monsterName << " wins.\n";
    }
    return 0;
}
