#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <string>
#include <engineDefinitions.hpp>
#include <unordered_map>
#include <utils.hpp>

static const char* INPUT_FILE = "./day1/input.txt";
static const char* DEMO_FILE = "./day1/demo.txt";
static const bool USE_REAL_DATA = true;

struct PuzzleInput {
    std::vector<u64> left;
    std::vector<u64> right;
};

u64 getSilver(PuzzleInput& puzzle) {
    u64 totalDistance = 0;
    for(size_t i = 0; i < puzzle.left.size(); i++) {
        totalDistance += std::abs((f64)puzzle.left[i] - (f64)puzzle.right[i]);
    }
    return totalDistance;
}

u64 getGold(PuzzleInput& puzzle) {
    std::unordered_map<u64, u64> rightMap;
    for(u64 right: puzzle.right) {
        auto it = rightMap.find(right);
        if (it != rightMap.end()) {
            rightMap[right] = it->second + 1;
        }else{
            rightMap.insert({right, 1});
        }
    }
    
    u64 total = 0;
    for(u64 left: puzzle.left) {
        auto it = rightMap.find(left);
        if(it != rightMap.end()) {
            total += left * it->second;
        }
    }

    return total;
}

int main() {
    // Get input file
    std::string file = USE_REAL_DATA ? INPUT_FILE : DEMO_FILE;
    std::fstream inputFile = std::fstream(file);
    if(!inputFile.is_open()) {
        printf("Can't open %s\n", file.c_str());
        exit(0);
    }

    std::vector<std::string> lines;

    PuzzleInput puzzle;
    // Parse
    std::string line;
    while(std::getline(inputFile, line)) {
        std::vector<std::string> b = splitString(line, "   ");
        puzzle.left.emplace_back(std::stoi(b[0]));
        puzzle.right.emplace_back(std::stoi(b[1]));
    }

    // Sort for 1º part
    std::sort(puzzle.left.begin(), puzzle.left.end(), [](u64 a, u64 b) {return a < b;});
    std::sort(puzzle.right.begin(), puzzle.right.end(), [](u64 a, u64 b) {return a < b;});

    printf("Silver: %lld\n", getSilver(puzzle));
    printf("Gold: %lld\n", getGold(puzzle));
}