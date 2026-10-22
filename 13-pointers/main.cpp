// main.cpp - Lesson 13: Pointers, honestly
//
// Arena step: a monster now has a TARGET: the party member it is after.
// The target is a pointer, because it can be "nobody" (nullptr): at the
// start of a fight, and after the target falls.
//
// (The crushing-blow bug from lesson 12 is fixed here.)

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

class Fighter {
public:
    Fighter(const std::string& startName, int startHp, int minDmg, int maxDmg) {
        name = startName;
        hp = startHp;
        maxHp = startHp;
        minDamage = minDmg;
        maxDamage = maxDmg;
    }

    const std::string& getName() const { return name; }
    int getHp() const { return hp; }
    int getMaxHp() const { return maxHp; }
    bool isAlive() const { return hp > 0; }
    Stance getStance() const { return stance; }

    void setStance(Stance newStance) { stance = newStance; }

    // The target is a POINTER: it holds the ADDRESS of another Fighter.
    // "Fighter*" reads as "pointer to a Fighter".
    // nullptr means "points at nobody". Unlike a reference (lesson 10), a
    // pointer can be empty, and can be changed to point somewhere else.
    Fighter* getTarget() const { return target; }
    void setTarget(Fighter* newTarget) { target = newTarget; }

    int getMaxDamage() const { return maxDamage; }
    int rollDamage() const { return rollDice(minDamage, maxDamage); }

    // The only way to lower HP: defending halves it, and HP stops at 0.
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

    // The only way to raise HP: the fallen stay fallen, max is max.
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
    Fighter* target = nullptr;   // nobody yet
};

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
// (Lesson 12's bug was here: this function used to call takeDamage too.)
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
