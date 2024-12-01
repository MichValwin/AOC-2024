#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <regex>
#include <string>
#include <engineDefinitions.hpp>

#include <unordered_map>
#include <utils.hpp>

using namespace std;

static const char* INPUT_FILE = "./day1/input.txt";
static const char* DEMO_FILE = "./day1/demo.txt";
static const bool USE_REAL_DATA = true;

struct PuzzleInput {
    std::vector<u32> left;
    std::vector<u32> right;
};

u32 getSilver(PuzzleInput& puzzle) {
    std::sort(puzzle.left.begin(), puzzle.left.end(), [](u32 a, u32 b) {
        return a < b;
    });
    std::sort(puzzle.right.begin(), puzzle.right.end(), [](u32 a, u32 b) {
        return a < b;
    });

    u32 totalDistance = 0;
    for(size_t i = 0; i < puzzle.left.size(); i++) {
        printf("left: %d, right: %d\n", puzzle.left[i], puzzle.right[i] );
        totalDistance += std::abs((float)puzzle.left[i] - (float)puzzle.right[i]);
    }
    return totalDistance;
}

u32 getGold(PuzzleInput& puzzle) {
    std::unordered_map<u32, u32> rightMap;
    for(u32 right: puzzle.right) {
        if (rightMap.find(right) != rightMap.end()) {
            rightMap[right] = rightMap[right] + 1;
        }else{
            rightMap.insert({right, 1});
        }
    }
    
    u32 total = 0;
    for(u32 left: puzzle.left) {
        if(rightMap.find(left) != rightMap.end()) {
            total += left * rightMap[left];
        }
    }

    return total;
}


void parseString(const std::string& input, PuzzleInput& puzzle) {
    std::regex pattern(R"((\d+)   (\d+))");
    std::smatch matches;

    if (std::regex_search(input, matches, pattern) && matches.size() == 3) {
        puzzle.left.push_back(std::stoi(matches[0]));
        puzzle.left.push_back(std::stoi(matches[1]));
    } else {
        throw std::invalid_argument("Input string does not match the expected format");
    }
}



int main() {
    // Get input file
    fstream inputFile;
    string file = USE_REAL_DATA ? INPUT_FILE : DEMO_FILE;
    inputFile = fstream(file);
    if(!inputFile.is_open()) {
        printf("Can't open %s\n", file.c_str());
        exit(0);
    }

    std::vector<std::string> lines;

    PuzzleInput puzzle;

    // Parse
    std::string line;
    while(std::getline(inputFile, line)) {
        //parseString(line, puzzle);
        std::vector<std::string> b = splitString(line, "   ");
        printf("line: %s\n", line.c_str());
        puzzle.left.push_back(std::stoi(b[0]));
        puzzle.right.push_back(std::stoi(b[1]));
    }

    printf("Silver: %d\n", getSilver(puzzle));
    printf("Gold: %d\n", getGold(puzzle));
}