// main.cpp - Lesson 14: Many files
//
// Arena step: the Arena is split into files.
//   Dice.h / Dice.cpp         rollDice
//   Fighter.h / Fighter.cpp   Stance, stanceName, and the Fighter class
//   main.cpp                  the game itself (this file)
//
// Build all the .cpp files together. The headers are never listed: they
// get pulled in by #include.
//   Windows:  cl /nologo /std:c++20 /W4 /EHsc /Zi /fsanitize=address main.cpp Fighter.cpp Dice.cpp
//   Linux:    g++ -std=c++20 -Wall -Wextra -g -fsanitize=address,undefined main.cpp Fighter.cpp Dice.cpp -o arena

// Your own headers, in quotes. #include pastes the file's text right here,
// so this file now knows that Fighter and rollDice exist.
#include "Dice.h"
#include "Fighter.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

void printStatus(const Fighter& fighter) {
    std::cout << "  " << fighter.getName() << " HP " << fighter.getHp() << "/"
              << fighter.getMaxHp() << " [" << stanceName(fighter.getStance()) << "]\n";
}

bool anyAlive(const std::vector<Fighter>& fighters) {
    for (const auto& fighter : fighters) {
        if (fighter.isAlive()) {
            return true;
        }
    }
    return false;
}

// A perfect roll (the attacker's maximum) is a crushing blow: +5 damage.
int addCrushingBonus(int roll) {
    std::cout << "CRUSHING BLOW!\n";
    return roll + 5;
}

void strike(const Fighter& attacker, Fighter& defender) {
    if (defender.getStance() == Stance::Defending) {
        std::cout << defender.getName() << " blocks half the blow.\n";
    }
    int roll = attacker.rollDamage();
    if (roll == attacker.getMaxDamage()) {
        roll = addCrushingBonus(roll);
    }
    int damage = defender.takeDamage(roll);
    std::cout << attacker.getName() << " hits " << defender.getName() << " for "
              << damage << "!\n";

    if (damage >= 14 && defender.isAlive()) {
        defender.setStance(Stance::Stunned);
        std::cout << defender.getName() << " staggers. Stunned!\n";
    }
}

void healParty(std::vector<Fighter>& party, int amount) {
    for (auto& member : party) {
        member.heal(amount);
    }
    std::cout << "The party shares a potion (+" << amount << " HP each).\n";
}

// Pick a random party member who is still standing, and return a POINTER
// to them. If nobody is standing, return nullptr: "nobody".
// (Lesson 10 returned a reference here. A reference must always refer to
// someone, so it could not say "nobody". A pointer can.)
Fighter* pickTarget(std::vector<Fighter>& party) {
    if (!anyAlive(party)) {
        return nullptr;
    }
    while (true) {
        int index = rollDice(0, static_cast<int>(party.size()) - 1);
        if (party[index].isAlive()) {
            // & in front of a VALUE means "the address of".
            // (& after a TYPE, as in Fighter&, means "reference". Same
            // symbol, two jobs. Everyone finds that confusing at first.)
            return &party[index];
        }
    }
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
    const std::vector<Fighter> monsters{
        {"Troll", 70, 8, 10},
        {"Goblin", 50, 6, 12},
        {"Wolf", 50, 7, 13},
    };
    const std::vector<std::string> lootTable{"rusty sword", "gold coin",
                                             "goblin ear", "healing herb"};
    std::map<std::string, int> prices{
        {"rusty sword", 12}, {"gold coin", 10}, {"goblin ear", 3},
        {"healing herb", 5}, {"bread", 1},
    };

    for (Fighter monster : monsters) {
        std::cout << "\nA " << monster.getName() << " appears!\n";

        while (anyAlive(party) && monster.isAlive()) {
            std::cout << "\n";
            for (const auto& member : party) {
                printStatus(member);
            }
            std::cout << "  vs\n";
            printStatus(monster);
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
                std::cout << "The " << monster.getName() << " has "
                          << monster.getHp() << " HP left";
                // ALWAYS check a pointer before you follow it.
                // "if (pointer)" is true when it is not nullptr.
                if (monster.getTarget()) {
                    // -> follows the pointer and reaches a member.
                    // target->getName() means "go to the fighter at this
                    // address, and call getName() on it".
                    std::cout << " and is after " << monster.getTarget()->getName();
                }
                std::cout << ".\n";
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
                for (auto& member : party) {
                    if (!member.isAlive() || !monster.isAlive()) {
                        continue;
                    }
                    // Crushing blows can stun party members now, too.
                    if (member.getStance() == Stance::Stunned) {
                        std::cout << member.getName() << " shakes off the stun.\n";
                        member.setStance(Stance::Normal);
                        continue;
                    }
                    strike(member, monster);
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

            if (monster.isAlive()) {
                switch (monster.getStance()) {
                    case Stance::Stunned:
                        std::cout << monster.getName()
                                  << " shakes off the stun and does nothing.\n";
                        monster.setStance(Stance::Normal);
                        break;
                    case Stance::Normal:
                    case Stance::Defending: {
                        // No target yet, or the target has fallen? Pick a new one.
                        Fighter* target = monster.getTarget();
                        if (target == nullptr || !target->isAlive()) {
                            target = pickTarget(party);
                            monster.setTarget(target);
                            if (target) {
                                std::cout << monster.getName() << " turns toward "
                                          << target->getName() << ".\n";
                            }
                        }

                        if (target) {
                            // *target follows the pointer and gives you the
                            // Fighter itself, which is what strike() wants.
                            strike(monster, *target);
                            if (!target->isAlive()) {
                                std::cout << target->getName() << " falls!\n";
                                monster.setTarget(nullptr);   // nobody, again
                            }
                        }
                        if (rollDice(1, 3) == 1) {
                            monster.setStance(Stance::Defending);
                            std::cout << monster.getName() << " raises its guard.\n";
                        } else {
                            monster.setStance(Stance::Normal);
                        }
                        break;
                    }
                }
            }
        }

        if (!anyAlive(party)) {
            std::cout << "\nDefeat... the " << monster.getName() << " wins.\n";
            return 0;
        }

        std::string loot = lootTable[rollDice(0, 3)];
        inventory.push_back(loot);
        std::cout << "The " << monster.getName() << " drops a " << loot << ".\n";
    }

    std::cout << "\nVictory! The party cleared the arena with "
              << gold << " gold and " << inventory.size() << " items.\n";
    return 0;
}
