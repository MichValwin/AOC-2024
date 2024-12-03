#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <regex>
#include <string>
#include <engineDefinitions.hpp>
#include <utils.hpp>

static const char* INPUT_FILE = "./day3/input.txt";
static const char* DEMO_FILE = "./day3/demo.txt";
static const bool USE_REAL_DATA = true;

struct Mult {
    u32 a;
    u32 b;
};


u32 getSilver(std::vector<Mult>& mult) {
    u32 total = 0;
    for(u32 i = 0; i < mult.size(); i++) {
        total += mult[i].a*mult[i].b;
    }
    return total;
}

u32 getGold() {
    return 1;
}

int main() {
    printf("DAY3\n");
    if(USE_REAL_DATA) {
        printf("Using REAL INPUT\n");
    } else {
        printf("Using DEMO\n");
    }
    // Get input file
    std::string file = USE_REAL_DATA ? INPUT_FILE : DEMO_FILE;
    std::fstream inputFile = std::fstream(file);
    if(!inputFile.is_open()) {
        printf("Can't open %s\n", file.c_str());
        return -1;
    }

   
    std::vector<Mult> multiplic;

    std::string line;
     bool enable = true;
    while(std::getline(inputFile, line)) {
        //std::regex wordRegex("\\b(mul\\((\\d+,\\d+)\\)|do\\(\\))\\b");
        std::regex pattern(R"(mul\(\d+,\d+\)|don't\(\)|do\(\))");
        std::smatch match;
        std::regex_match(line, match, pattern);

        auto wordsBegin = std::sregex_iterator(line.begin(), line.end(), pattern);
        auto wordsEnd = std::sregex_iterator();


       
        printf("Found %d words\n", std::distance(wordsBegin, wordsEnd));
        for(std::sregex_iterator i = wordsBegin; i != wordsEnd; ++i) {
            std::smatch match = *i;
            
            
            s32 dont = match[0].str().find("don't()");
            s32 dooo = match[0].str().find("do()");
            
            if(dont >= 0) enable = false;
            if(dooo >= 0) enable = true;
            
            s32 mulI = match[0].str().find("mul(");
            if(mulI >= 0 && enable) {
                std::string mult = match[0].str();
                mult.erase(0,4);
                mult.erase(mult.size()-1, 1);
                printf("%s\n", mult.c_str());

                auto matchVec= splitString(mult, ",");
                u32 a = std::atoi(matchVec[0].c_str());
                u32 b = std::atoi(matchVec[1].c_str());
                multiplic.push_back({a,b});
            }
        }
    }

    printf("Silver: %d\n", getSilver(multiplic));
    printf("Gold: %d\n", getGold());
}