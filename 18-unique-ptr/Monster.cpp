// Monster.cpp - how each kind of monster takes its turn.
#include "Monster.h"

#include "Dice.h"

#include <iostream>

// Pick a random party member who is still standing, or nullptr if nobody is.
static Fighter* pickTarget(std::vector<Fighter>& party) {
    if (!anyAlive(party)) {
        return nullptr;
    }
    while (true) {
        int index = rollDice(0, static_cast<int>(party.size()) - 1);
        if (party[index].isAlive()) {
            return &party[index];
        }
    }
}

// A child's constructor must build its parent part first. The ": Fighter(...)"
// after the closing parenthesis does exactly that: it passes the name, HP and
// damage up to Fighter's constructor.
Monster::Monster(const std::string& startName, int startHp, int minDmg, int maxDmg,
                 int startSpeed)
    : Fighter(startName, startHp, minDmg, maxDmg) {
    speed = startSpeed;
}

Monster::~Monster() {
    std::cout << "The " << getName() << " fades away.\n";
}

// The default turn, for any monster that does not override it.
void Monster::takeTurn(std::vector<Fighter>& party) {
    if (recoverFromStun()) {
        return;
    }
    attackTarget(party);
    chooseNextStance();
}

bool Monster::recoverFromStun() {
    if (getStance() != Stance::Stunned) {
        return false;
    }
    std::cout << getName() << " shakes off the stun and does nothing.\n";
    setStance(Stance::Normal);
    return true;
}

void Monster::attackTarget(std::vector<Fighter>& party) {
    Fighter* victim = getTarget();
    if (victim == nullptr || !victim->isAlive()) {
        victim = pickTarget(party);
        setTarget(victim);
        if (victim) {
            std::cout << getName() << " turns toward " << victim->getName() << ".\n";
        }
    }
    if (victim) {
        // Inside a member function, "this" is a pointer to the monster whose
        // turn it is. *this follows it (lesson 13) and gives the monster
        // itself, which strike() takes as its attacker.
        strike(*this, *victim);
        if (!victim->isAlive()) {
            std::cout << victim->getName() << " falls!\n";
            setTarget(nullptr);
        }
    }
}

void Monster::chooseNextStance() {
    if (rollDice(1, 3) == 1) {
        setStance(Stance::Defending);
        std::cout << getName() << " raises its guard.\n";
    } else {
        setStance(Stance::Normal);
    }
}

// ---- Goblin: weak, fast, and stabs twice ----

Goblin::Goblin() : Monster("Goblin", 30, 2, 6, 8) {}

// A child's destructor runs FIRST, then the parent's (~Monster). If
// ~Monster were not virtual, deleting a Goblin through a Monster pointer
// would skip this line. Try it: that is Try it yourself 3.
Goblin::~Goblin() {
    std::cout << "The Goblin's dagger clatters to the floor.\n";
}

void Goblin::takeTurn(std::vector<Fighter>& party) {
    if (recoverFromStun()) {
        return;
    }
    attackTarget(party);
    attackTarget(party);   // a second quick stab
    chooseNextStance();
}

// ---- Troll: slow and tough, and heals a little every turn ----

Troll::Troll() : Monster("Troll", 45, 6, 9, 2) {}

void Troll::takeTurn(std::vector<Fighter>& party) {
    if (getHp() < getMaxHp()) {
        heal(3);
        std::cout << "The Troll's wounds close a little (+3 HP).\n";
    }
    // Then fight like any monster: call the PARENT's version.
    // (Unreal writes this as Super::takeTurn(...).)
    Monster::takeTurn(party);
}

// ---- Slime: when badly hurt, it splits and glues itself back together ----

Slime::Slime() : Monster("Slime", 35, 4, 7, 4) {}

void Slime::takeTurn(std::vector<Fighter>& party) {
    if (!hasSplit && getHp() < getMaxHp() / 2) {
        hasSplit = true;
        heal(10);
        std::cout << "The Slime splits in two... and the halves squish back "
                     "together, bigger! (+10 HP)\n";
        return;   // splitting takes its whole turn
    }
    Monster::takeTurn(party);
}
