// break-it.cpp - a kill counter whose lambda captures BY VALUE.
// ("mutable" is needed only so the lambda may change its own copy.)
// Build it and run it. Count the kills.
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> fallen{"Goblin", "Troll", "Slime"};
    int kills = 0;

    auto onMonsterDown = [kills](const std::string& name) mutable {
        ++kills;
        std::cout << "  " << name << " falls. (Kills inside the lambda: "
                  << kills << ")\n";
    };

    for (const auto& name : fallen) {
        onMonsterDown(name);
    }

    std::cout << "Total kills: " << kills << "\n";
    return 0;
}
