// main.cpp - Lesson 05: Lists that know their type
//
// Arena step: the hero now fights three monsters in a row, and every win
// drops loot into an inventory. Both lists are std::vector.

#include <iostream>
#include <random>
#include <string>
#include <vector>   // std::vector, C++'s growable list

int rollDice(int min, int max) {
    static std::mt19937 generator{std::random_device{}()};
    std::uniform_int_distribution<int> distribution{min, max};
    return distribution(generator);
}

int main() {
    const std::string heroName = "Ayla";
    int heroHp = 60;
    const int potionHeal = 10;

    // A vector is like a JavaScript array, with one big difference:
    // the type in the angle brackets < > says what it holds, and it can
    // ONLY hold that. This one holds strings, and nothing else, ever.
    //
    //   JS:   let inventory = ["bread"];
    //   C++:  std::vector<std::string> inventory{"bread"};
    std::vector<std::string> inventory{"bread"};

    // "const" works on a whole vector too: nobody can add to this list or
    // change it. The monsters you will face, in order.
    const std::vector<std::string> monsters{"Goblin", "Wolf", "Goblin Chief"};

    // What a beaten monster can drop.
    const std::vector<std::string> lootTable{"rusty sword", "gold coin",
                                             "goblin ear", "healing herb"};

    // Range-for: "for each monster name in the list".
    //   JS:   for (const name of monsters) { ... }
    //   C++:  for (const auto& name : monsters) { ... }
    // "auto" lets the compiler fill in the type (std::string here).
    // "const" means we only look at it. The "&" means "do not make a copy";
    // lesson 10 is all about that. For now, read "const auto&" as "look at
    // each one".
    for (const auto& monsterName : monsters) {
        int monsterHp = 30;   // every monster starts fresh
        std::cout << "\nA " << monsterName << " appears!\n";

        while (heroHp > 0 && monsterHp > 0) {
            std::cout << "\n" << heroName << " HP " << heroHp
                      << "   " << monsterName << " HP " << monsterHp << "\n";
            std::cout << "(a)ttack, (h)eal or (i)nventory? ";

            char choice = ' ';
            if (!(std::cin >> choice)) {
                std::cout << "\nNo more input. Goodbye.\n";
                return 0;
            }

            if (choice == 'i') {
                // .size() tells you how many items are in the vector.
                // (JS: inventory.length)
                std::cout << "You carry " << inventory.size() << " item(s):\n";
                for (const auto& item : inventory) {
                    std::cout << "  - " << item << "\n";
                }
                continue;   // looking at your bag is free: the monster waits
            }

            if (choice == 'a') {
                int damage = rollDice(8, 16);
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

        if (heroHp <= 0) {
            std::cout << "\nDefeat... the " << monsterName << " wins.\n";
            return 0;
        }

        // [ ] reads one item by its position, starting at 0, like JS.
        // lootTable has 4 items, so the positions are 0, 1, 2 and 3.
        std::string loot = lootTable[rollDice(0, 3)];

        // push_back adds to the end. (JS: inventory.push(loot))
        inventory.push_back(loot);
        std::cout << "The " << monsterName << " drops a " << loot << ".\n";
    }

    std::cout << "\nVictory! " << heroName << " cleared the arena with "
              << inventory.size() << " items:\n";
    for (const auto& item : inventory) {
        std::cout << "  - " << item << "\n";
    }
    return 0;
}
