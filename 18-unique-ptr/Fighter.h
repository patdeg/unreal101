// Fighter.h - what every fighter has in common, hero or monster.
#pragma once

#include <string>
#include <vector>

enum class Stance {
    Normal,
    Defending,
    Stunned,
};

std::string stanceName(Stance stance);

class Fighter {
public:
    Fighter(const std::string& startName, int startHp, int minDmg, int maxDmg);
    int rollDamage() const;
    int takeDamage(int amount);
    void heal(int amount);

    const std::string& getName() const { return name; }
    int getHp() const { return hp; }
    int getMaxHp() const { return maxHp; }
    int getMaxDamage() const { return maxDamage; }
    bool isAlive() const { return hp > 0; }
    Stance getStance() const { return stance; }
    void setStance(Stance newStance) { stance = newStance; }
    Fighter* getTarget() const { return target; }
    void setTarget(Fighter* newTarget) { target = newTarget; }

    // New in lesson 18: a full rest between waves. Back to maximum HP and a
    // Normal stance, even for a fighter who had fallen.
    void restoreFull() { hp = maxHp; stance = Stance::Normal; }

private:
    std::string name;
    int hp = 0;
    int maxHp = 0;
    int minDamage = 0;
    int maxDamage = 0;
    Stance stance = Stance::Normal;
    Fighter* target = nullptr;
};

// Helpers that both the game and the monsters use. They moved here from
// main.cpp, so Monster.cpp can use them too.
void printStatus(const Fighter& fighter);
bool anyAlive(const std::vector<Fighter>& fighters);
void strike(const Fighter& attacker, Fighter& defender);
