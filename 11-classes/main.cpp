// main.cpp - Lesson 11: Classes guard their data
//
// Arena step: Fighter becomes a CLASS. Its HP is private: only the
// Fighter's own functions can change it, and they make sure HP never drops
// below 0 and never rises above the maximum.

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

// A class is a struct with walls.
//
//   JS:   class Fighter {
//             #hp;                         // private field
//             constructor(name, hp) { ... }
//             takeDamage(amount) { ... }   // method
//         }
//
// "public:" is the door: anyone may call these.
// "private:" is the vault: only code INSIDE the class can touch these.
// (In a struct everything is public by default; in a class, private.)
class Fighter {
public:
    // The CONSTRUCTOR has the same name as the class and no return type.
    // It runs once, when a Fighter is made, and sets it up properly.
    // From now on, nobody can make a Fighter without giving these four.
    Fighter(const std::string& startName, int startHp, int minDmg, int maxDmg) {
        name = startName;
        hp = startHp;
        maxHp = startHp;
        minDamage = minDmg;
        maxDamage = maxDmg;
    }

    // Member functions ("methods") can read and change the private data.
    //
    // The "const" at the end is a promise: "this function only looks".
    // You need it to call the function through a const Fighter&
    // (lesson 10), because the compiler only allows promised-safe calls
    // on something it must not change.
    const std::string& getName() const { return name; }
    int getHp() const { return hp; }
    int getMaxHp() const { return maxHp; }
    bool isAlive() const { return hp > 0; }
    Stance getStance() const { return stance; }

    void setStance(Stance newStance) { stance = newStance; }

    int rollDamage() const { return rollDice(minDamage, maxDamage); }

    // The ONLY way to lower HP. It enforces the rules in one place:
    // a defending fighter takes half, and HP never goes below 0.
    // It returns how much damage really landed.
    int takeDamage(int amount) {
        if (stance == Stance::Defending) {
            amount = amount / 2;
        }
        hp -= amount;
        if (hp < 0) {
            hp = 0;
        }
        return amount;
    }

    // The ONLY way to raise HP. The fallen stay fallen, and nobody goes
    // above their maximum.
    void heal(int amount) {
        if (!isAlive()) {
            return;
        }
        hp += amount;
        if (hp > maxHp) {
            hp = maxHp;
        }
    }

private:
    std::string name;
    int hp = 0;
    int maxHp = 0;
    int minDamage = 0;
    int maxDamage = 0;
    Stance stance = Stance::Normal;
};   // classes end with a semicolon too

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

// strike no longer does the HP math itself. It ASKS the defender to take
// damage, and the defender applies its own rules.
void strike(const Fighter& attacker, Fighter& defender) {
    if (defender.getStance() == Stance::Defending) {
        std::cout << defender.getName() << " blocks half the blow.\n";
    }
    int damage = defender.takeDamage(attacker.rollDamage());
    std::cout << attacker.getName() << " hits " << defender.getName() << " for "
              << damage << "!\n";

    if (damage >= 14 && defender.isAlive()) {
        defender.setStance(Stance::Stunned);
        std::cout << defender.getName() << " staggers. Stunned!\n";
    }
}

void healParty(std::vector<Fighter>& party, int amount) {
    for (auto& member : party) {
        member.heal(amount);   // the class decides what "heal" is allowed to do
    }
    std::cout << "The party shares a potion (+" << amount << " HP each).\n";
}

Fighter& pickVictim(std::vector<Fighter>& party) {
    while (true) {
        int index = rollDice(0, static_cast<int>(party.size()) - 1);
        if (party[index].isAlive()) {
            return party[index];
        }
    }
}

int main() {
    // Each {...} here calls the constructor.
    std::vector<Fighter> party{
        {"Ayla", 40, 8, 16},
        {"Bram", 50, 5, 10},
    };

    std::cout << "Name your hero: ";
    std::string typedName;
    if (std::getline(std::cin, typedName) && !typedName.empty()) {
        // The name is private now, so we cannot write party[0].name = ...
        // Instead, replace the whole fighter with a new one.
        party[0] = Fighter{typedName, 40, 8, 16};
    }
    std::cout << party[0].getName() << " recruits " << party[1].getName()
              << ". A party of two!\n";

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
                          << monster.getHp() << " HP left.\n";
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
                    if (member.isAlive() && monster.isAlive()) {
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

            if (monster.isAlive()) {
                switch (monster.getStance()) {
                    case Stance::Stunned:
                        std::cout << monster.getName()
                                  << " shakes off the stun and does nothing.\n";
                        monster.setStance(Stance::Normal);
                        break;
                    case Stance::Normal:
                    case Stance::Defending: {
                        Fighter& victim = pickVictim(party);
                        strike(monster, victim);
                        if (!victim.isAlive()) {
                            std::cout << victim.getName() << " falls!\n";
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
