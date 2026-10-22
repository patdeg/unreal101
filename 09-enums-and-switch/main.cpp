// main.cpp - Lesson 09: Named choices
//
// Arena step: every fighter has a STANCE.
//   Normal     - fights as usual
//   Defending  - takes half damage this turn
//   Stunned    - loses its next turn (a big hit of 14+ stuns a monster)

#include <algorithm>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

// An "enum class" is a type with a fixed list of named values.
//
//   JS has no enums. People write stance = "defending" and hope nobody
//   types "defnding". In C++ a Stance can ONLY be one of these three names,
//   and a typo is a compile error.
//
// You always write the type name in front: Stance::Defending. That is the
// "class" part of "enum class": the names live inside Stance and cannot
// clash with anything else.
enum class Stance {
    Normal,
    Defending,
    Stunned,
};

struct Fighter {
    std::string name;
    int hp = 0;
    int minDamage = 0;
    int maxDamage = 0;
    Stance stance = Stance::Normal;   // everyone starts in Normal
};

// Turn a Stance into text for printing. std::cout does not know how to
// print a Stance by itself, so we tell it.
//
// A switch picks one "case" by value. It is the same idea as JavaScript's
// switch, but it only works on whole-number-like things: ints, chars and
// enums. (Not strings!) Each case ends with return or break.
std::string stanceName(Stance stance) {
    switch (stance) {
        case Stance::Normal:
            return "Normal";
        case Stance::Defending:
            return "Defending";
        case Stance::Stunned:
            return "Stunned";
    }
    return "?";   // never reached, but keeps every compiler happy
}

int rollDice(int min, int max) {
    static std::mt19937 generator{std::random_device{}()};
    std::uniform_int_distribution<int> distribution{min, max};
    return distribution(generator);
}

int main() {
    // Fields we leave out (stance) keep their default value.
    Fighter hero{"Ayla", 60, 8, 16};

    std::cout << "Name your hero: ";
    std::string typedName;
    if (std::getline(std::cin, typedName) && !typedName.empty()) {
        hero.name = typedName;
    }

    int gold = 0;
    int potions = 3;
    const int potionHeal = 15;
    std::vector<std::string> inventory{"bread"};

    const std::vector<Fighter> monsters{
        {"Goblin", 30, 4, 9},
        {"Wolf", 30, 5, 10},
        {"Goblin Chief", 50, 6, 12},
    };
    const std::vector<std::string> lootTable{"rusty sword", "gold coin",
                                             "goblin ear", "healing herb"};
    std::map<std::string, int> prices{
        {"rusty sword", 12}, {"gold coin", 10}, {"goblin ear", 3},
        {"healing herb", 5}, {"bread", 1},
    };

    // A fresh copy of each monster for each fight (see lesson 08).
    for (Fighter monster : monsters) {
        std::cout << "\nA " << monster.name << " appears!\n";

        while (hero.hp > 0 && monster.hp > 0) {
            std::cout << "\n" << hero.name << " HP " << hero.hp << "  Gold " << gold
                      << "  Potions " << potions
                      << "   " << monster.name << " HP " << monster.hp
                      << " [" << stanceName(monster.stance) << "]\n";
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
                std::cout << "A " << monster.name << " with " << monster.hp
                          << " HP. It hits for " << monster.minDamage << "-"
                          << monster.maxDamage << ". Stance: "
                          << stanceName(monster.stance) << ".\n";
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
                int damage = rollDice(hero.minDamage, hero.maxDamage);

                // The monster's stance changes how much the hit hurts.
                switch (monster.stance) {
                    case Stance::Defending:
                        damage = damage / 2;   // int division: 15 / 2 is 7
                        std::cout << "The " << monster.name << " blocks half the blow.\n";
                        break;   // without break, C++ falls into the next case!
                    case Stance::Normal:
                    case Stance::Stunned:      // two cases can share one block
                        break;
                }

                monster.hp -= damage;
                std::cout << hero.name << " hits for " << damage << "!\n";

                // A big hit stuns the monster for one turn.
                if (damage >= 14 && monster.hp > 0) {
                    monster.stance = Stance::Stunned;
                    std::cout << "The " << monster.name << " staggers. Stunned!\n";
                }
            } else if (verb == "use" && object == "potion") {
                if (potions == 0) {
                    std::cout << "You have no potions left!\n";
                    continue;
                }
                potions -= 1;
                hero.hp += potionHeal;
                std::cout << hero.name << " drinks a potion (+" << potionHeal << ").\n";
            } else if (verb == "use") {
                std::cout << "You can't use '" << object << "' here.\n";
                continue;
            } else {
                std::cout << "Unknown command '" << verb
                          << "'. Try: attack, use potion, sell <item>, look, inventory\n";
                continue;
            }

            if (monster.hp > 0) {
                // The monster's turn, decided by its stance.
                switch (monster.stance) {
                    case Stance::Stunned:
                        std::cout << "The " << monster.name
                                  << " shakes off the stun and does nothing.\n";
                        monster.stance = Stance::Normal;
                        break;
                    case Stance::Normal:
                    case Stance::Defending: {
                        // Braces around a case let it have its own variables.
                        int damage = rollDice(monster.minDamage, monster.maxDamage);
                        hero.hp -= damage;
                        std::cout << monster.name << " hits for " << damage << "!\n";

                        // Pick the stance for next turn: 1 time in 3, defend.
                        if (rollDice(1, 3) == 1) {
                            monster.stance = Stance::Defending;
                            std::cout << "The " << monster.name << " raises its guard.\n";
                        } else {
                            monster.stance = Stance::Normal;
                        }
                        break;
                    }
                }
            }
        }

        if (hero.hp <= 0) {
            std::cout << "\nDefeat... the " << monster.name << " wins.\n";
            return 0;
        }

        std::string loot = lootTable[rollDice(0, 3)];
        inventory.push_back(loot);
        std::cout << "The " << monster.name << " drops a " << loot << ".\n";
    }

    std::cout << "\nVictory! " << hero.name << " cleared the arena with "
              << gold << " gold and " << inventory.size() << " items.\n";
    return 0;
}
