#include <bitset>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <regex>
#include <set>
#include <stack>
#include <string>
#include <engineDefinitions.hpp>
#include <unordered_map>
#include <unordered_set>
#include <utils.hpp>
#include <chrono>
#include <vector>
#include <raylib.h>

static const char* INPUT_FILE = "./day19/input.txt";
static const char* DEMO_FILE = "./day19/demo.txt";
static const bool USE_REAL_DATA = true ;

bool recursiveSliding(const std::unordered_set<std::string>& towelPatterns, const std::string& design, u32 start) {
    u32 count = 1;
   
    bool found = false;
    while(!found && count <= design.size()) {
        std::string first = design.substr(start, count);
        if(towelPatterns.find(first) != towelPatterns.end()) {
            // We have one, check next
            std::string second = design.substr(start+count);
            if(second.empty() || recursiveSliding(towelPatterns, second, 0)) {
                found = true;
            }
        }
        count++;
    }

    return found;
}

s64 recursiveSlidingAll(const std::unordered_set<std::string>& towelPatterns, const std::string& design, u32 start, std::unordered_map<std::string, s64>& memo) {
    u32 count = 1;
    s64 forms = 0;

    if(memo.find(design) != memo.end()) {
        return memo[design];
    }

    while(count <= design.size()) {
        std::string first = design.substr(start, count);
        if(towelPatterns.find(first) != towelPatterns.end()) {
            // We have one, check next
            std::string second = design.substr(start+count);
            
            if(second.empty()) {
                forms++;
            }else{
                s64 types = recursiveSlidingAll(towelPatterns, second, 0, memo);
                memo[second] = types;
                forms += types;
            }
        }
        count++;
    }
    return forms;
}

void silver(const std::unordered_set<std::string>& towelPatterns, const std::vector<std::string>& designs) {
    u32 total = 0;
    for(const std::string& design: designs) {
        if(recursiveSliding(towelPatterns, design, 0)) {
            total++;
        }else{
            printf("%s is impossible.\n", design.c_str());
        }
    }
    
    printf("Silver: %d\n", total);
}


void gold(const std::unordered_set<std::string>& towelPatterns, const std::vector<std::string>& designs) {
    s64 total = 0;
    std::unordered_map<std::string, s64> memo;
    for(const std::string& design: designs) {
        s64 forms = recursiveSlidingAll(towelPatterns, design, 0, memo);
        printf("%s can be made %lld different ways\n", design.c_str(), forms);
        total += forms;
    }
    
    printf("Gold: %lld\n", total);
}

inline u64 getTimeNanoSinceEpoch() {
    auto time_point_now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(time_point_now.time_since_epoch()).count();
}


int main() {
    printf("DAY19\n");
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
    
    std::unordered_set<std::string> towelPatterns;
    std::vector<std::string> designs;
    
    std::string line;
    std::getline(inputFile, line);
    std::vector<std::string> patternsSplit = splitString(line, ", ");
    for(const std::string& p: patternsSplit)towelPatterns.insert(p);

    std::getline(inputFile, line);
    while(std::getline(inputFile, line)) {
        designs.emplace_back(line);
    }
    
    printf("Time to parse input: %lfms\n", (f64)(getTimeNanoSinceEpoch() - parseInput) / 1e6);
    u64 timeFinishProblem = getTimeNanoSinceEpoch();
    //silver(towelPatterns, designs);
    gold(towelPatterns, designs);
    printf("Time to finish problem: %lfms\n", (f64)(getTimeNanoSinceEpoch() - timeFinishProblem) / 1e6);
}