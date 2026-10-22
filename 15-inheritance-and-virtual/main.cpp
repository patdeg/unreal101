// main.cpp - Lesson 15: One interface, many monsters
//
// Arena step: three kinds of monster, each with its own way of fighting,
// all in the arena AT ONCE:
//   Goblin  - fast, stabs twice
//   Troll   - slow and tough, regenerates
//   Slime   - splits when badly hurt
// The game holds them as Monster* pointers, and never needs to know which
// kind is which. Each one's own takeTurn runs anyway. That is "virtual".
//
// Build:
//   Windows:  cl /nologo /std:c++20 /W4 /EHsc /Zi /fsanitize=address main.cpp Fighter.cpp Monster.cpp Dice.cpp
//   Linux:    g++ -std=c++20 -Wall -Wextra -g -fsanitize=address,undefined main.cpp Fighter.cpp Monster.cpp Dice.cpp -o arena

#include "Dice.h"
#include "Fighter.h"
#include "Monster.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

void healParty(std::vector<Fighter>& party, int amount) {
    for (auto& member : party) {
        member.heal(amount);
    }
    std::cout << "The party shares a potion (+" << amount << " HP each).\n";
}

// The first monster in the wave that is still standing, or nullptr.
Monster* firstAlive(const std::vector<Monster*>& wave) {
    for (Monster* monster : wave) {
        if (monster->isAlive()) {
            return monster;
        }
    }
    return nullptr;
}

int main() {
    std::vector<Fighter> party{
        {"Ayla", 40, 8, 16},
        {"Bram", 50, 5, 10},
    };

    std::cout << "Name your hero: ";
    std::string typedName;
    if (std::getline(std::cin, typedName) && !typedName.empty()) {
        party[0] = Fighter{typedName, 40, 8, 16};
    }
    std::cout << party[0].getName() << " recruits " << party[1].getName()
              << ". A party of two!\n";

    int gold = 0;
    int potions = 3;
    const int potionHeal = 20;
    std::vector<std::string> inventory{"bread"};
    const std::vector<std::string> lootTable{"rusty sword", "gold coin",
                                             "goblin ear", "healing herb"};
    std::map<std::string, int> prices{
        {"rusty sword", 12}, {"gold coin", 10}, {"goblin ear", 3},
        {"healing herb", 5}, {"bread", 1},
    };

    // Three ordinary objects, of three different classes...
    Goblin goblin;
    Troll troll;
    Slime slime;

    // ...and one list that holds them all, as pointers to their PARENT type.
    // A Monster* can point at a Goblin, because a Goblin IS a Monster.
    std::vector<Monster*> wave{&goblin, &troll, &slime};

    std::cout << "\nA Goblin, a Troll and a Slime enter the arena!\n";

    while (anyAlive(party) && firstAlive(wave) != nullptr) {
        std::cout << "\n";
        for (const auto& member : party) {
            printStatus(member);
        }
        std::cout << "  vs\n";
        for (const Monster* monster : wave) {
            if (monster->isAlive()) {
                printStatus(*monster);   // a Monster can go where a Fighter& is wanted
            }
        }
        std::cout << "Gold " << gold << "  Potions " << potions << "  > ";

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
            for (const Monster* monster : wave) {
                if (!monster->isAlive()) {
                    continue;
                }
                std::cout << "The " << monster->getName() << " (speed "
                          << monster->getSpeed() << ")";
                if (monster->getTarget()) {
                    std::cout << " is after " << monster->getTarget()->getName();
                }
                std::cout << ".\n";
            }
            continue;
        } else if (verb == "inventory" || verb == "inv") {
            std::cout << "You carry " << inventory.size() << " item(s):\n";
            for (const auto& item : inventory) {
                std::cout << "  - " << item << "\n";
            }
            continue;
        } else if (verb == "sell") {
            if (!prices.contains(object)) {
                std::cout << "The shop does not buy '" << object << "'.\n";
                continue;
            }
            auto found = std::find(inventory.begin(), inventory.end(), object);
            if (found == inventory.end()) {
                std::cout << "You don't have a " << object << ".\n";
                continue;
            }
            inventory.erase(found);
            gold += prices[object];
            std::cout << "Sold the " << object << " for " << prices[object]
                      << " gold.\n";
            continue;
        } else if (verb == "attack") {
            // Every living party member strikes the first monster standing.
            for (auto& member : party) {
                Monster* front = firstAlive(wave);
                if (!member.isAlive() || front == nullptr) {
                    continue;
                }
                if (member.getStance() == Stance::Stunned) {
                    std::cout << member.getName() << " shakes off the stun.\n";
                    member.setStance(Stance::Normal);
                    continue;
                }
                strike(member, *front);
                if (!front->isAlive()) {
                    std::string loot = lootTable[rollDice(0, 3)];
                    inventory.push_back(loot);
                    std::cout << "The " << front->getName() << " falls and drops a "
                              << loot << ".\n";
                }
            }
        } else if (verb == "use" && object == "potion") {
            if (potions == 0) {
                std::cout << "You have no potions left!\n";
                continue;
            }
            potions -= 1;
            healParty(party, potionHeal);
        } else if (verb == "use") {
            std::cout << "You can't use '" << object << "' here.\n";
            continue;
        } else {
            std::cout << "Unknown command '" << verb
                      << "'. Try: attack, use potion, sell <item>, look, inventory\n";
            continue;
        }

        // The monsters' turn. This loop does not know or care which kind
        // each monster is. monster->takeTurn(...) runs the Goblin's version
        // for the goblin, the Troll's for the troll, and so on, because
        // takeTurn is virtual.
        for (Monster* monster : wave) {
            if (monster->isAlive() && anyAlive(party)) {
                monster->takeTurn(party);
            }
        }
    }

    if (!anyAlive(party)) {
        std::cout << "\nDefeat... the party has fallen.\n";
        return 0;
    }
    std::cout << "\nVictory! The party cleared the arena with "
              << gold << " gold and " << inventory.size() << " items.\n";
    return 0;
}
