#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
#include <regex>
#include <set>
#include <string>
#include <engineDefinitions.hpp>
#include <unordered_map>
#include <utils.hpp>
#include <chrono>

static const char* INPUT_FILE = "./day13/input.txt";
static const char* DEMO_FILE = "./day13/demo.txt";
static const bool USE_REAL_DATA = true;


struct Offset {
    s64 dx, dy;
};

struct Position {
    s64 x, y;
};

struct Prize {
    Offset buttA;
    Offset buttB;
    s64 x, y;

    void print() const{
        printf("Prize: x=%d, y=%d. ButtA: x+%d, y+%d. ButtB: x+%d, y+%d\n", x, y, buttA.dx, buttA.dy, buttB.dx, buttB.dy);
    }
};


s64 min(s64 a, s64 b) {
    if(a < b) return a;
    return b;
}

s64 max(s64 a, s64 b) {
    if(a > b) return a;
    return b;
}

/* B rutefocing
u64 silver(const std::vector<Prize>& prizes) {
    s64 total = 0;

    for (const Prize& prize: prizes) {
        s64 maxButtA = min(prize.x/prize.buttA.dx, prize.y/prize.buttA.dy);
        s64 maxButtB = min(prize.x/prize.buttB.dx, prize.y/prize.buttB.dy);
        
        for(s64 bButt = maxButtB; bButt >= 0; bButt--) {
            for(s64 aButt = 0; aButt < maxButtA; aButt++) {
                if(prize.x == prize.buttA.dx*aButt + prize.buttB.dx*bButt  
                    && prize.y == prize.buttA.dy*aButt + prize.buttB.dy*bButt) {
                    total += 3*aButt + 1*bButt;
                }
            }
        }

        //printf("Max ButtA: %d, Max buttB: %d\n", maxButtA, maxButtB);

    }

    return total;
}
*/

s64 gcd(s64 a, s64 b) {
    while(b != 0) {
        s64 bb = b;
        b = a%b;
        a = bb;
    }
    return a;
}

s64 lcm(s64 a, s64 b) {
    return (a*b)/gcd(a,b);
}

void printGCD(s64 a, s64 b) {
    printf("GCD(%lld, %lld): %lld\n", a, b, gcd(a, b));
}

void printLCM(s64 a, s64 b) {
    printf("LCM(%lld, %lld): %lld\n", a, b, lcm(a, b));
}

u64 gold(const std::vector<Prize>& prizes) {
    std::vector<Prize> prizesMill = prizes;
    for(Prize& prizeM: prizesMill) {
        prizeM.x += 10000000000000;
        prizeM.y += 10000000000000;
    }

    s64 total = 0;

    for (const Prize& prize: prizesMill) {
        // Solve for AButt
        f64 determinant = (prize.buttA.dx*prize.buttB.dy) - (prize.buttA.dy*prize.buttB.dx);
        f64 aPress = (prize.x*prize.buttB.dy - prize.y*prize.buttB.dx) / determinant;
        f64 bPress = (prize.y*prize.buttA.dx - prize.x*prize.buttA.dy) / determinant;

        if(aPress-(s64)aPress == 0 && bPress-(s64)bPress == 0) {
            total += 3*aPress+1*bPress;
        }

    }

    return total;
}

u64 silver(const std::vector<Prize>& prizes) {
    s64 total = 0;

    for (const Prize& prize: prizes) {
        // Solve for AButt
        f64 determinant = (prize.buttA.dx*prize.buttB.dy) - (prize.buttA.dy*prize.buttB.dx);
        f64 aPress = (prize.x*prize.buttB.dy - prize.y*prize.buttB.dx) / determinant;
        f64 bPress = (prize.y*prize.buttA.dx - prize.x*prize.buttA.dy) / determinant;

        if(aPress-(s64)aPress == 0 && bPress-(s64)bPress == 0) {
            total += 3*aPress+1*bPress;
        }
    }

    return total;
}



inline u64 getTimeNanoSinceEpoch() {
    auto time_point_now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(time_point_now.time_since_epoch()).count();
}


int main() {
    printf("DAY13\n");
    u64 parseInput = getTimeNanoSinceEpoch();
    if(USE_REAL_DATA) {
        printf("Using REAL INPUT from: %s\n", INPUT_FILE);
    } else {
        printf("Using DEMO from: %s\n", DEMO_FILE);
    }
    // Get input file
    std::string file = USE_REAL_DATA ? INPUT_FILE : DEMO_FILE;
    std::fstream inputFile = std::fstream(file);
    if(!inputFile.is_open()) {
        printf("Can't open %s\n", file.c_str());
        return -1;
    }

    std::vector<Prize> prizes;
    std::string line;
    while(std::getline(inputFile, line)) {
        Prize prize;

        // Button A
        std::regex pattern(R"(X\+(\d+), Y\+(\d+))");
        std::smatch matches;
        if (std::regex_search(line, matches, pattern)) {
            prize.buttA.dx = std::atoi(matches[1].str().c_str());
            prize.buttA.dy = std::atoi(matches[2].str().c_str());
        }
        // Button B
        std::getline(inputFile, line);
        if (std::regex_search(line, matches, pattern)) {
            prize.buttB.dx = std::atoi(matches[1].str().c_str());
            prize.buttB.dy = std::atoi(matches[2].str().c_str());
        }

        // Prize
        std::getline(inputFile, line);
        std::regex prizePattern(R"(X=(\d+), Y=(\d+))");
        if (std::regex_search(line, matches, prizePattern)) {
            prize.x = std::atoi(matches[1].str().c_str());
            prize.y = std::atoi(matches[2].str().c_str());
        }
        
        prizes.push_back(prize);
        std::getline(inputFile, line);
    }

    for(const Prize& p: prizes) {
        p.print();
    }
    
    printf("Time to parse input: %lfms\n", (f64)(getTimeNanoSinceEpoch() - parseInput) / 1e6);
    u64 timeFinishProblem = getTimeNanoSinceEpoch();
    printf("Silver: %lld\n", silver(prizes));
    printf("Gold: %lld\n", gold(prizes));
    printf("Time to finish problem: %lfms\n", (f64)(getTimeNanoSinceEpoch() - timeFinishProblem) / 1e6);
}