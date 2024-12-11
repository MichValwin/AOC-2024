#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <string>
#include <engineDefinitions.hpp>
#include <unordered_map>
#include <utils.hpp>
#include <chrono>

static const char* INPUT_FILE = "./day11/input.txt";
static const char* DEMO_FILE = "./day11/demo.txt";
static const bool USE_REAL_DATA = true;

struct StoneStep {
    u64 stone;
    u32 steps;

    bool operator==(const StoneStep& o) const {
        return stone == o.stone && steps == o.steps;
    }
};

struct HashStoneStep {
    size_t operator()(const StoneStep& s) const {
        return std::hash<u64>()(s.stone) ^ (std::hash<u64>()(s.steps) << 1);
    }
};

bool evenNumDigits(u64 num) {
    return std::to_string(num).size() % 2 == 0;
}

std::vector<u64> stoneBlink(u64 stone) {
    std::vector<u64> stones;
    if(stone == 0) {
        stones.push_back(1);
    }else if(evenNumDigits(stone)) {
        std::string nStr = std::to_string(stone);
        std::string s1 = nStr.substr(0, nStr.size()/2);
        std::string s2 = nStr.substr(nStr.size()/2, nStr.size());
        
        stones.push_back(std::atoll(s1.c_str()));
        stones.push_back(std::atoll(s2.c_str()));
    }else{
        stones.push_back(stone*2024);
    }
    return stones;
}

u64 stoneCountAfBlink(u64 stone, std::unordered_map<StoneStep, u64, HashStoneStep>& memo, u32 blinkCount, u32 endBlink) {
    if(blinkCount == endBlink) return 1;

    u64 totalStones = 0;
    std::vector<u64> stonesRes = stoneBlink(stone);
    for(const u64& st: stonesRes) {
        StoneStep forMem = {st, endBlink - blinkCount};

        if(memo.find(forMem) == memo.end()) {
            u64 nStones = stoneCountAfBlink(st, memo, blinkCount+1, endBlink);
            memo.insert({forMem, nStones});
            totalStones += nStones;
        }else{
            //printf("Found: %lld for steps: %d\n", forMem.stone, forMem.steps);
            totalStones += memo[forMem];
        }
    }
    return totalStones;
}



u64 getSteps(const std::vector<u64>& stones, u32 steps) {
    // (stone, numSteps) --> numStones
    std::unordered_map<StoneStep, u64, HashStoneStep> memo;

    u64 total = 0;
    for(const u64& st: stones)
        total += stoneCountAfBlink(st, memo, 0, steps);

    return total;
}


inline u64 getTimeNanoSinceEpoch() {
    auto time_point_now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(time_point_now.time_since_epoch()).count();
}


int main() {
    printf("DAY11\n");
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

    std::vector<u64> stones;
    std::string line;
    std::getline(inputFile, line);
    std::vector<std::string> stonesStr = splitString(line, " ");
    for(const std::string& s: stonesStr) {
        stones.push_back(std::atoll(s.c_str()));
    }
    for(const u64& num: stones) {
        printf("%d ", num);
    }
    printf("\n");

    printf("Time to parse input and build grid: %lfms\n", (f64)(getTimeNanoSinceEpoch() - parseInput) / 1e6);
    u64 timeFinishProblem = getTimeNanoSinceEpoch();
    printf("Silver: %lld\n", getSteps(stones, 25));
    printf("Gold: %lld\n", getSteps(stones, 75));
    printf("Time to finish problem: %lfms\n", (f64)(getTimeNanoSinceEpoch() - timeFinishProblem) / 1e6);
}