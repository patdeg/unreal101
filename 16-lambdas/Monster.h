// Monster.h - one base class, Monster, and three kinds of monster.
#pragma once

#include "Fighter.h"

#include <vector>

// "class Monster : public Fighter" means: a Monster IS a Fighter, plus more.
// It inherits everything a Fighter has (name, HP, takeDamage, ...) and adds
// a speed and a takeTurn function.
//   JS:   class Monster extends Fighter { ... }
class Monster : public Fighter {
public:
    Monster(const std::string& startName, int startHp, int minDmg, int maxDmg,
            int startSpeed);

    int getSpeed() const { return speed; }

    // "virtual" means: a child class may REPLACE this function with its own
    // version, and the right version runs even when you only hold a
    // Monster* pointer. This is the most important word in this lesson.
    virtual void takeTurn(std::vector<Fighter>& party);

    // Building blocks that every kind of monster can use in its turn.
    bool recoverFromStun();                        // true = this turn is lost
    void attackTarget(std::vector<Fighter>& party);
    void chooseNextStance();

private:
    int speed = 0;
};

// Each child says what it is, and "override"s takeTurn with its own way of
// fighting. "override" asks the compiler to CHECK that there really is a
// virtual function with this exact name and inputs in the parent.

class Goblin : public Monster {
public:
    Goblin();
    void takeTurn(std::vector<Fighter>& party) override;   // stabs twice
};

class Troll : public Monster {
public:
    Troll();
    void takeTurn(std::vector<Fighter>& party) override;   // regenerates
};

class Slime : public Monster {
public:
    Slime();
    void takeTurn(std::vector<Fighter>& party) override;   // splits, once

private:
    bool hasSplit = false;
};
