// main.cpp - Lesson 16: Functions as values
//
// Arena step: the fastest monster acts first. The wave is sorted by speed
// with a LAMBDA: a small function written right where it is needed.
// A second lambda counts kills and hands out loot.
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

    Goblin goblin;
    Troll troll;
    Slime slime;
    std::vector<Monster*> wave{&troll, &goblin, &slime};

    std::cout << "\nA Troll, a Goblin and a Slime enter the arena!\n";

    // Sort the wave so the fastest monster comes first.
    //
    // std::sort needs to know "should a come before b?". We answer with a
    // lambda: a function with no name, written right here.
    //
    //   JS:   wave.sort((a, b) => b.speed - a.speed);
    //   C++:  std::sort(wave.begin(), wave.end(),
    //                   [](const Monster* a, const Monster* b) {
    //                       return a->getSpeed() > b->getSpeed();
    //                   });
    //
    // [ ]  the capture list: which outside variables the lambda may use
    //      (none here)
    // ( )  the inputs, with types, like any function
    // { }  the body
    //
    // One difference from JS: C++ wants true/false ("a goes first?"),
    // not a positive or negative number.
    std::sort(wave.begin(), wave.end(), [](const Monster* a, const Monster* b) {
        return a->getSpeed() > b->getSpeed();
    });

    std::cout << "Turn order:";
    for (const Monster* monster : wave) {
        std::cout << " " << monster->getName() << " (" << monster->getSpeed() << ")";
    }
    std::cout << "\n";

    // A lambda can also be stored in a variable and called later.
    // This one runs whenever a monster falls. It needs three outside
    // variables, so it CAPTURES them:
    //   &kills, &inventory  by reference: the lambda changes the real ones
    //   &lootTable          by reference: no copy (it only reads it)
    // Without the &, the lambda would get its own private COPY, and the
    // real kills would never change. (Same rule as lesson 10!)
    int kills = 0;
    auto onMonsterDown = [&kills, &inventory, &lootTable](const Monster& monster) {
        ++kills;
        std::string loot = lootTable[rollDice(0, 3)];
        inventory.push_back(loot);
        std::cout << "The " << monster.getName() << " falls and drops a "
                  << loot << ". (Kills: " << kills << ")\n";
    };

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
                    onMonsterDown(*front);   // call the lambda like a function
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

        // The monsters' turn, fastest first (the wave is sorted).
        for (Monster* monster : wave) {
            if (monster->isAlive() && anyAlive(party)) {
                monster->takeTurn(party);
            }
        }
    }

    if (!anyAlive(party)) {
        std::cout << "\nDefeat... the party has fallen after " << kills
                  << " kill(s).\n";
        return 0;
    }
    std::cout << "\nVictory! " << kills << " monsters defeated, "
              << gold << " gold and " << inventory.size() << " items.\n";
    return 0;
}
