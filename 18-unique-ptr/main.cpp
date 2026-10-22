// main.cpp - Lesson 18: Who owns this?   (Text Arena v1.0)
//
// Arena step: monsters are no longer fixed local objects. A Spawner creates
// three waves of random monsters WHILE THE GAME RUNS. Each wave is a
//   std::vector<std::unique_ptr<Monster>>
// which OWNS its monsters. When a monster dies, it is erased from the wave,
// and erasing its owner destroys it ("The Goblin fades away.").
// No "new", no "delete", no leaks.
//
// Build:
//   Windows:  cl /nologo /std:c++20 /W4 /EHsc /Zi /fsanitize=address main.cpp Fighter.cpp Monster.cpp Spawner.cpp Dice.cpp
//   Linux:    g++ -std=c++20 -Wall -Wextra -g -fsanitize=address,undefined main.cpp Fighter.cpp Monster.cpp Spawner.cpp Dice.cpp -o arena

#include "Dice.h"
#include "Fighter.h"
#include "Monster.h"
#include "Spawner.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

// A shorter name for a long type. "Wave" now means "a vector of owned
// monsters". (Unreal code does the same kind of thing all the time.)
using Wave = std::vector<std::unique_ptr<Monster>>;

void healParty(std::vector<Fighter>& party, int amount) {
    for (auto& member : party) {
        member.heal(amount);
    }
    std::cout << "The party shares a potion (+" << amount << " HP each).\n";
}

class RoundBanner {
public:
    RoundBanner(int roundNumber) {
        number = roundNumber;
        std::cout << "--- Round " << number << " begins ---\n";
    }

    ~RoundBanner() {
        std::cout << "--- Round " << number << " over ---\n";
    }

private:
    int number = 0;
};

// The first monster still standing, or nullptr.
//
// The wave OWNS the monsters. This function only wants to point at one for
// a moment, so it returns a plain Monster* (lesson 13). monster.get()
// borrows the address from the unique_ptr without taking ownership.
// Rule of thumb: unique_ptr OWNS, a plain pointer only LOOKS.
Monster* firstAlive(const Wave& wave) {
    for (const auto& monster : wave) {   // "auto&": never copy an owner
        if (monster->isAlive()) {
            return monster.get();
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
    const int numberOfWaves = 3;
    std::vector<std::string> inventory{"bread"};
    const std::vector<std::string> lootTable{"rusty sword", "gold coin",
                                             "goblin ear", "healing herb"};
    std::map<std::string, int> prices{
        {"rusty sword", 12}, {"gold coin", 10}, {"goblin ear", 3},
        {"healing herb", 5}, {"bread", 1},
    };

    int kills = 0;
    int roundCount = 0;
    auto onMonsterDown = [&kills, &inventory, &lootTable](const Monster& monster) {
        ++kills;
        std::string loot = lootTable[rollDice(0, 3)];
        inventory.push_back(loot);
        std::cout << "The " << monster.getName() << " falls and drops a "
                  << loot << ". (Kills: " << kills << ")\n";
    };

    Spawner spawner;

    for (int waveNumber = 1; waveNumber <= numberOfWaves; ++waveNumber) {
        // The spawner builds the monsters and hands the whole wave over.
        // From this line on, "wave" is their one and only owner.
        Wave wave = spawner.makeWave(waveNumber);   // 1, then 2, then 3 monsters

        // Fastest first. The lambda now receives the owners, by const
        // reference: look at them, never copy them.
        std::sort(wave.begin(), wave.end(),
                  [](const std::unique_ptr<Monster>& a, const std::unique_ptr<Monster>& b) {
                      return a->getSpeed() > b->getSpeed();
                  });

        std::cout << "\n=== Wave " << waveNumber << " of " << numberOfWaves << ":";
        for (const auto& monster : wave) {
            std::cout << " " << monster->getName();
        }
        std::cout << " ===\n";

        while (anyAlive(party) && !wave.empty()) {
            std::cout << "\n";
            for (const auto& member : party) {
                printStatus(member);
            }
            std::cout << "  vs\n";
            for (const auto& monster : wave) {
                printStatus(*monster);
            }
            std::cout << "Gold " << gold << "  Potions " << potions << "  > ";

            std::string line;
            if (!std::getline(std::cin, line)) {
                std::cout << "\nNo more input. Goodbye.\n";
                return 0;   // the wave is destroyed on the way out
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
            }
            if (verb == "look") {
                for (const auto& monster : wave) {
                    std::cout << "The " << monster->getName() << " (speed "
                              << monster->getSpeed() << ")";
                    if (monster->getTarget()) {
                        std::cout << " is after " << monster->getTarget()->getName();
                    }
                    std::cout << ".\n";
                }
                continue;
            }
            if (verb == "inventory" || verb == "inv") {
                std::cout << "You carry " << inventory.size() << " item(s):\n";
                for (const auto& item : inventory) {
                    std::cout << "  - " << item << "\n";
                }
                continue;
            }
            if (verb == "sell") {
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
            }

            bool isAction = verb == "attack" || verb == "run" ||
                            (verb == "use" && object == "potion");
            if (!isAction) {
                std::cout << "Unknown command '" << line
                          << "'. Try: attack, use potion, run, sell <item>, look, inventory\n";
                continue;
            }

            if (verb == "use" && potions == 0) {
                std::cout << "You have no potions left!\n";
                continue;   // checked BEFORE the round starts: no banner
            }

            ++roundCount;
            RoundBanner banner(roundCount);

            if (verb == "run") {
                std::cout << "The party runs for the exit!\n";
                return 0;
            }

            if (verb == "attack") {
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
                        onMonsterDown(*front);
                    }
                }
            } else {
                potions -= 1;
                healParty(party, potionHeal);
            }

            for (const auto& monster : wave) {
                if (monster->isAlive() && anyAlive(party)) {
                    monster->takeTurn(party);
                }
            }

            // Remove the fallen. std::erase_if (C++20) erases every element
            // for which the lambda says true. Erasing a unique_ptr destroys
            // the monster it owns, so each fallen monster's destructor runs
            // right here and prints "fades away". (Unreal's TArray has the
            // same idea: RemoveAll with a lambda.)
            std::erase_if(wave, [](const std::unique_ptr<Monster>& monster) {
                return !monster->isAlive();
            });
        }

        if (!anyAlive(party)) {
            std::cout << "\nDefeat... the party falls in wave " << waveNumber
                      << " after " << kills << " kill(s).\n";
            return 0;
        }

        std::cout << "\nWave " << waveNumber << " cleared!";
        if (waveNumber < numberOfWaves) {
            std::cout << " The party rests: everyone is back on their feet at full HP,\n"
                      << "and the potion bag is refilled.\n";
            for (auto& member : party) {
                member.restoreFull();
            }
            potions = 3;
        } else {
            std::cout << "\n";
        }
    }

    std::cout << "\nVICTORY! Text Arena v1.0 complete. " << kills << " of "
              << spawner.getTotalSpawned() << " monsters defeated, " << gold
              << " gold, " << inventory.size() << " items.\n";
    return 0;
}
