#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <regex>
#include <string>
#include <engineDefinitions.hpp>
#include <utils.hpp>
#include <chrono>

static const char* INPUT_FILE = "./day3/bigboy.txt";
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

struct ParseToken {
    std::string token;
    u32 index;
    bool specialTokenParsed;
    u32 specialTokenAt = 0;
};

inline u64 getTimeNanoSinceEpoch() {
    auto time_point_now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(time_point_now.time_since_epoch()).count();
}


int main() {
    printf("DAY3\n");
    u64 timeStartFetch = getTimeNanoSinceEpoch();
    if(USE_REAL_DATA) {
        printf("Using REAL INPUT\n");
    } else {
        printf("Using DEMO\n");
    }
    // Get input file
    std::string file = USE_REAL_DATA ? INPUT_FILE : DEMO_FILE;
    std::ifstream inputFile(file, std::ios::binary);
    if(!inputFile.is_open()) {
        printf("Can't open %s\n", file.c_str());
        return -1;
    }

    inputFile.seekg(0, std::ios_base::end);
    size_t fileSize = inputFile.tellg();
    inputFile.seekg(0, std::ios_base::beg);

    std::vector<char> buffer(fileSize);
    if (!inputFile.read(buffer.data(), fileSize)) {
        printf("Error reading file\n");
        return -1;
    }
    printf("Time to read file: %lfms\n", (f64)(getTimeNanoSinceEpoch() - timeStartFetch) / 1e6);



    
    ParseToken mulToken = {"mul(\1,\1)", 0, false, 0};
    ParseToken doToken = {"do()", 0, false, 0};
    ParseToken dontToken = {"don't()", 0, false, 0};

    u32 mulA = 0, mulB = 0;
    u64 total = 0;
    u64 timeFinishProblem = getTimeNanoSinceEpoch();
    bool enabled = true;

    for(size_t i = 0; i < fileSize; i++) {
        char c = buffer[i];

        // Mul
        if(mulToken.token[mulToken.index] == c) {
            if(mulToken.index == 0) {
                mulA = 0;
                mulB = 0;
            }
            mulToken.index++;
        }else if(mulToken.token[mulToken.index] == '\1') {
            if(c >= '0' && c <= '9') {
                mulToken.specialTokenParsed = true;
                if(mulToken.specialTokenAt == 0) {
                    mulA = (mulA*10) + c-'0';
                }else{
                    mulB = (mulB*10) + c-'0';
                }
            }else{
                if(mulToken.specialTokenParsed && (mulToken.token[mulToken.index+1]) == c) {
                    if(mulToken.index+1 != mulToken.token.size()-1) {
                        mulToken.index+=2;
                        mulToken.specialTokenAt++;
                    }else{
                        if(enabled)total += mulA*mulB;
                        mulToken.specialTokenAt = 0;
                        mulToken.index = 0;
                        mulA = 0;
                        mulB = 0;
                    }
                   
                }else{
                    mulToken.index = 0;
                    mulA = 0;
                    mulB = 0;
                    mulToken.specialTokenAt = 0;
                }
            }
        }else{
            mulToken.index = 0;
            mulA = 0;
            mulB = 0;
            mulToken.specialTokenAt = 0;
        }

        if(dontToken.token[dontToken.index] == c) {
            if(dontToken.index == dontToken.token.size()-1) {
                dontToken.index = 0;
                enabled = false;
            }
            dontToken.index++;
        }else{
            dontToken.index = 0;
        }

        if(doToken.token[doToken.index] == c) {
            if(doToken.index == doToken.token.size()-1) {
                doToken.index = 0;
                enabled = true;
            }
            doToken.index++;
        }else{
            doToken.index = 0;
        }

        

    }
    
    printf("Time to finish problem: %lfms\n", (f64)(getTimeNanoSinceEpoch() - timeFinishProblem) / 1e6);
    printf("Total Silver: %lld\n", total);

    /*
    std::string line;
     bool enable = true;
    while(std::getline(inputFile, line)) {
        //std::regex wordRegex("\\b(mul\\((\\d+,\\d+)\\)|do\\(\\))\\b");
        std::regex pattern(R"(mul\(\d+,\d+\)|don't\(\)|do\(\))");
        std::smatch match;
        std::regex_match(line, match, pattern);

        auto wordsBegin = std::sregex_iterator(line.begin(), line.end(), pattern);
        auto wordsEnd = std::sregex_iterator();


       
        //printf("Found %d words\n", std::distance(wordsBegin, wordsEnd));
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
                //printf("%s\n", mult.c_str());

                auto matchVec= splitString(mult, ",");
                u32 a = std::atoi(matchVec[0].c_str());
                u32 b = std::atoi(matchVec[1].c_str());
                multiplic.push_back({a,b});
            }
        }
    }
    

    printf("Silver: %d\n", getSilver(multiplic));
    printf("Gold: %d\n", getGold());
    */
}