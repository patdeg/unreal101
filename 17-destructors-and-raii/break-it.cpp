// break-it.cpp - make the banner with "new", the old-fashioned way.
// Build it, run it, and look for "Round 3 over".
#include <iostream>

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

void playRound() {
    // "new" builds the banner somewhere else in memory (the "heap") and
    // hands back a pointer to it. It does NOT belong to this { } block.
    RoundBanner* banner = new RoundBanner(3);
    std::cout << "Ayla attacks the goblin.\n";
    // (no "delete banner;" anywhere)
}

int main() {
    playRound();
    std::cout << "Back in main. The round should be over by now...\n";
    return 0;
}
