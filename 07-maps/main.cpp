// main.cpp - Lesson 07: Look things up by name
//
// Arena step: every piece of loot has a gold price, kept in a std::map.
// New command:   sell <item>     (for example: sell gold coin)

#include <algorithm>   // std::find
#include <iostream>
#include <map>         // std::map, C++'s dictionary
#include <random>
#include <string>
#include <vector>

int rollDice(int min, int max) {
    static std::mt19937 generator{std::random_device{}()};
    std::uniform_int_distribution<int> distribution{min, max};
    return distribution(generator);
}

int main() {
    std::string heroName;
    std::cout << "Name your hero: ";
    if (!std::getline(std::cin, heroName) || heroName.empty()) {
        heroName = "Ayla";
    }

    int heroHp = 60;
    int gold = 0;
    const int potionHeal = 10;
    std::vector<std::string> inventory{"bread"};
    const std::vector<std::string> monsters{"Goblin", "Wolf", "Goblin Chief"};
    const std::vector<std::string> lootTable{"rusty sword", "gold coin",
                                             "goblin ear", "healing herb"};

    // A map stores KEY -> VALUE pairs. Here: item name -> price in gold.
    //   JS:   const prices = { "rusty sword": 12, "gold coin": 10, ... };
    //   C++:  std::map<std::string, int> prices{ {"rusty sword", 12}, ... };
    // Both types are fixed: keys are always strings, values always ints.
    // (A std::map also keeps its keys sorted A to Z.)
    std::map<std::string, int> prices{
        {"rusty sword", 12},
        {"gold coin", 10},
        {"goblin ear", 3},
        {"healing herb", 5},
        {"bread", 1},
    };

    for (const auto& monsterName : monsters) {
        int monsterHp = 30;
        std::cout << "\nA " << monsterName << " appears!\n";

        while (heroHp > 0 && monsterHp > 0) {
            std::cout << "\n" << heroName << " HP " << heroHp << "  Gold " << gold
                      << "   " << monsterName << " HP " << monsterHp << "\n";
            std::cout << "> ";

            std::string line;
            if (!std::getline(std::cin, line)) {
                std::cout << "\nNo more input. Goodbye.\n";
                return 0;
            }

            std::string verb = line;
            std::string object;
            std::size_t space = line.find(' ');
            if (space != std::string::npos) {
                verb = line.substr(0, space);
                object = line.substr(space + 1);
            }

            if (verb.empty()) {
                continue;
            } else if (verb == "look") {
                std::cout << "A " << monsterName << " with " << monsterHp
                          << " HP glares at " << heroName << ".\n";
                continue;
            } else if (verb == "inventory" || verb == "inv") {
                std::cout << "You carry " << inventory.size() << " item(s):\n";
                for (const auto& item : inventory) {
                    std::cout << "  - " << item << "\n";
                }
                continue;
            } else if (verb == "sell") {
                // Step 1: does the shop buy this at all?
                // contains() ASKS. It never changes the map. (C++20)
                //   JS:   object in prices
                if (!prices.contains(object)) {
                    std::cout << "The shop does not buy '" << object << "'.\n";
                    continue;
                }

                // Step 2: do we have one? std::find walks the vector and
                // gives back a "bookmark" (an iterator) at the first match.
                // If there is no match, the bookmark equals inventory.end(),
                // which means "past the last item". Same idea as npos in
                // lesson 06: a special "not found" answer.
                auto found = std::find(inventory.begin(), inventory.end(), object);
                if (found == inventory.end()) {
                    std::cout << "You don't have a " << object << ".\n";
                    continue;
                }

                // erase removes the item the bookmark points at.
                //   JS:   inventory.splice(index, 1)
                inventory.erase(found);

                // Reading with [ ] is safe here, because we checked
                // contains() first. See break-it.cpp for when it is not.
                gold += prices[object];
                std::cout << "Sold the " << object << " for " << prices[object]
                          << " gold.\n";
                continue;   // shopping is free, the monster waits politely
            } else if (verb == "attack") {
                int damage = rollDice(8, 16);
                monsterHp -= damage;
                std::cout << heroName << " hits for " << damage << "!\n";
            } else if (verb == "use" && object == "potion") {
                heroHp += potionHeal;
                std::cout << heroName << " drinks a potion (+" << potionHeal << ").\n";
            } else if (verb == "use") {
                std::cout << "You can't use '" << object << "' here.\n";
                continue;
            } else {
                std::cout << "Unknown command '" << verb
                          << "'. Try: attack, use potion, sell <item>, look, inventory\n";
                continue;
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

        std::string loot = lootTable[rollDice(0, 3)];
        inventory.push_back(loot);
        std::cout << "The " << monsterName << " drops a " << loot << ".\n";
    }

    std::cout << "\nVictory! " << heroName << " cleared the arena with "
              << gold << " gold and " << inventory.size() << " items.\n";
    return 0;
}
