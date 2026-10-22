// main.cpp - Lesson 10: Copies vs references
//
// Arena step: the hero recruits Bram. The two of them form a PARTY, stored
// in a std::vector<Fighter>. "use potion" now heals the whole party, and
// functions work on the real fighters, not on copies.
//
// The rule of this lesson, in three lines:
//   Fighter  f   a COPY. Changes to f do not touch the original.
//   Fighter& f   a REFERENCE: another name for the original. Changes stick.
//   const Fighter& f   the original, read-only: no copy, no changes.

#include <algorithm>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

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
    Stance stance = Stance::Normal;
};

std::string stanceName(Stance stance) {
    switch (stance) {
        case Stance::Normal:
            return "Normal";
        case Stance::Defending:
            return "Defending";
        case Stance::Stunned:
            return "Stunned";
    }
    return "?";
}

int rollDice(int min, int max) {
    static std::mt19937 generator{std::random_device{}()};
    std::uniform_int_distribution<int> distribution{min, max};
    return distribution(generator);
}

// const Fighter& = "let me LOOK at the real fighter".
// No copy is made (fast), and the compiler forbids changes (safe).
// This is the normal way to pass anything bigger than a number.
void printStatus(const Fighter& fighter) {
    std::cout << "  " << fighter.name << " HP " << fighter.hp
              << " [" << stanceName(fighter.stance) << "]\n";
}

// Is anyone in this group still standing? We only look, so const&.
bool anyAlive(const std::vector<Fighter>& fighters) {
    for (const auto& fighter : fighters) {
        if (fighter.hp > 0) {
            return true;
        }
    }
    return false;
}

// The attacker is only looked at (const&). The defender gets hurt, so it
// must be the REAL defender (&). If this said "Fighter defender", the
// damage would land on a copy and vanish when the function ends.
void strike(const Fighter& attacker, Fighter& defender) {
    int damage = rollDice(attacker.minDamage, attacker.maxDamage);
    if (defender.stance == Stance::Defending) {
        damage = damage / 2;
        std::cout << defender.name << " blocks half the blow.\n";
    }
    defender.hp -= damage;
    std::cout << attacker.name << " hits " << defender.name << " for "
              << damage << "!\n";

    if (damage >= 14 && defender.hp > 0) {
        defender.stance = Stance::Stunned;
        std::cout << defender.name << " staggers. Stunned!\n";
    }
}

// Heal everyone who is still standing.
// The vector comes in by reference (&), so we change the real party.
// Inside, "auto&" makes each "member" a reference to the real fighter.
// Try removing either & and see what happens (that is the Break it).
void healParty(std::vector<Fighter>& party, int amount) {
    for (auto& member : party) {
        if (member.hp > 0) {
            member.hp += amount;
        }
    }
    std::cout << "The party shares a potion (+" << amount << " HP each).\n";
}

// A function can also RETURN a reference: "here is the real one".
// The monster picks a random party member who is still standing.
// (The caller must make sure someone is alive, or this loops forever.)
Fighter& pickVictim(std::vector<Fighter>& party) {
    while (true) {
        // party.size() is a std::size_t; rollDice wants an int.
        // static_cast<int>(...) converts it on purpose, so the compiler
        // knows we meant it.
        int index = rollDice(0, static_cast<int>(party.size()) - 1);
        if (party[index].hp > 0) {
            return party[index];
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
        party[0].name = typedName;
    }
    std::cout << party[0].name << " recruits " << party[1].name << ". A party of two!\n";

    int gold = 0;
    int potions = 3;
    const int potionHeal = 20;
    std::vector<std::string> inventory{"bread"};
    const std::vector<Fighter> monsters{
        {"Goblin", 50, 6, 12},
        {"Wolf", 50, 7, 13},
        {"Goblin Chief", 90, 9, 13},
    };
    const std::vector<std::string> lootTable{"rusty sword", "gold coin",
                                             "goblin ear", "healing herb"};
    std::map<std::string, int> prices{
        {"rusty sword", 12}, {"gold coin", 10}, {"goblin ear", 3},
        {"healing herb", 5}, {"bread", 1},
    };

    // Here we WANT a copy: each fight gets a fresh monster, and the list
    // of monster templates stays untouched.
    for (Fighter monster : monsters) {
        std::cout << "\nA " << monster.name << " appears!\n";

        while (anyAlive(party) && monster.hp > 0) {
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
                std::cout << "The " << monster.name << " hits for "
                          << monster.minDamage << "-" << monster.maxDamage << ".\n";
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
                // Every living party member strikes. "auto&": the real member.
                for (auto& member : party) {
                    if (member.hp > 0 && monster.hp > 0) {
                        strike(member, monster);
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

            if (monster.hp > 0) {
                switch (monster.stance) {
                    case Stance::Stunned:
                        std::cout << monster.name << " shakes off the stun and does nothing.\n";
                        monster.stance = Stance::Normal;
                        break;
                    case Stance::Normal:
                    case Stance::Defending: {
                        // "victim" is a reference: another name for the
                        // real party member that pickVictim chose.
                        Fighter& victim = pickVictim(party);
                        strike(monster, victim);
                        if (victim.hp <= 0) {
                            std::cout << victim.name << " falls!\n";
                        }
                        if (rollDice(1, 3) == 1) {
                            monster.stance = Stance::Defending;
                            std::cout << monster.name << " raises its guard.\n";
                        } else {
                            monster.stance = Stance::Normal;
                        }
                        break;
                    }
                }
            }
        }

        if (!anyAlive(party)) {
            std::cout << "\nDefeat... the " << monster.name << " wins.\n";
            return 0;
        }

        std::string loot = lootTable[rollDice(0, 3)];
        inventory.push_back(loot);
        std::cout << "The " << monster.name << " drops a " << loot << ".\n";
    }

    std::cout << "\nVictory! The party cleared the arena with "
              << gold << " gold and " << inventory.size() << " items.\n";
    return 0;
}
