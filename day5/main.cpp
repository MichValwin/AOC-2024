#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <regex>
#include <string>
#include <engineDefinitions.hpp>
#include <unordered_map>
#include <utils.hpp>
#include <chrono>

static const char* INPUT_FILE = "./day5/input.txt";
static const char* DEMO_FILE = "./day5/demo.txt";
static const bool USE_REAL_DATA = true;

void printList(std::vector<u32>& nums) {
    for(size_t i = 0; i < nums.size(); i++) {
        printf("%d", nums[i]);
        if(i < nums.size()-1)printf(",");
    }
}

bool isCorrect(std::unordered_map<u32, std::vector<u32>> prioAfter, std::unordered_map<u32, std::vector<u32>> prioBefore, std::vector<u32> pageLine) {
    for(u32 i = 0; i < pageLine.size(); i++)  {
        u32 page = pageLine[i];
        auto prioPost = prioAfter[page];
        auto priobef = prioBefore[page];
        // Check if the number is ordered correcly for this I need to check every number behind and also every posterior one
        
        if(i > 0) {
            // check preceding
            for(s32 j = i-1; j >= 0; j--) {
                u32 priorNumb = pageLine[j];
                
                auto it = std::find(prioPost.begin(), prioPost.end(), priorNumb);
                if(it != prioPost.end()) {
                    return false;
                }
            }
        }else if(i < pageLine.size()-1) {
            // Check posterior
            for(u32 j = i+1; j < pageLine.size(); j++) {
                u32 postNumber = pageLine[j];
                
                auto it = std::find(priobef.begin(), priobef.end(), postNumber);
                if(it != priobef.end()) {
                    return false;
                }
            }
        }
    }
    return true;
}

std::vector<std::vector<u32>> wrongLines(std::unordered_map<u32, std::vector<u32>> prioAfter, std::unordered_map<u32, std::vector<u32>> prioBefore, std::vector<std::vector<u32>> pageNumbers) {
    std::vector<std::vector<u32>> badPageLines;
    for(auto& pageLine: pageNumbers) {
        bool correctPageLine = true;
        for(u32 i = 0; i < pageLine.size(); i++)  {
            u32 page = pageLine[i];
            auto prioPost = prioAfter[page];
            auto priobef = prioBefore[page];
            // Check if the number is ordered correcly for this I need to check every number behind and also every posterior one
            
            if(i > 0) {
                // check preceding
                for(s32 j = i-1; j >= 0; j--) {
                    u32 priorNumb = pageLine[j];
                    
                    auto it = std::find(prioPost.begin(), prioPost.end(), priorNumb);
                    if(it != prioPost.end()) {
                        correctPageLine = false;
                    }
                }
            }else if(i < pageLine.size()-1) {
                // Check posterior
                for(u32 j = i+1; j < pageLine.size(); j++) {
                    u32 postNumber = pageLine[j];
                    
                    auto it = std::find(priobef.begin(), priobef.end(), postNumber);
                    if(it != priobef.end()) {
                        correctPageLine = false;
                    }
                }
            }
        }

        if(!correctPageLine) {
            badPageLines.emplace_back(pageLine);
        }
    }
    return badPageLines;
}

u32 getSilver(std::unordered_map<u32, std::vector<u32>> prioAfter, std::unordered_map<u32, std::vector<u32>> prioBefore, std::vector<std::vector<u32>> pageNumbers) {
    // I guess I can get the wrong rows easier. I can check every number and for every number I can check all of the posterior numbers and if there is some that should go precedng.
    u32 total = 0;
    for(auto& pageLine: pageNumbers) {
        bool correctPageLine = true;
        for(u32 i = 0; i < pageLine.size(); i++)  {
            u32 page = pageLine[i];
            auto prioPost = prioAfter[page];
            auto priobef = prioBefore[page];
            // Check if the number is ordered correcly for this I need to check every number behind and also every posterior one
            
            if(i > 0) {
                // check preceding
                for(s32 j = i-1; j >= 0; j--) {
                    u32 priorNumb = pageLine[j];
                    
                    auto it = std::find(prioPost.begin(), prioPost.end(), priorNumb);
                    if(it != prioPost.end()) {
                        correctPageLine = false;
                    }
                }
            }else if(i < pageLine.size()-1) {
                // Check posterior
                for(u32 j = i+1; j < pageLine.size(); j++) {
                    u32 postNumber = pageLine[j];
                    
                    auto it = std::find(priobef.begin(), priobef.end(), postNumber);
                    if(it != priobef.end()) {
                        correctPageLine = false;
                    }
                }
            }
        }

        if(correctPageLine) {
            u32 middle = pageLine[pageLine.size() / 2];
            printf("From: ");
            printList(pageLine);
            printf("  Middle: %d\n", middle);
            total += middle;
        }
    }

    return total;
}

u32 getGold(std::unordered_map<u32, std::vector<u32>> prioAfter, std::unordered_map<u32, std::vector<u32>> prioBefore, std::vector<std::vector<u32>> wrongPageLines) {
    u32 total = 0;

    // Time to sort them of course
    for(auto& pageLine: wrongPageLines) {

        while(!isCorrect(prioAfter, prioBefore, pageLine)) {
            // very dumb way of ordering

            for(u32 i = 0; i < pageLine.size(); i++)  {
                u32 page = pageLine[i];
                auto prioPost = prioAfter[page];
                auto priobef = prioBefore[page];


                if(i > 0) {
                    // check preceding
                    for(s32 j = i-1; j >= 0; j--) {
                        u32 priorNumb = pageLine[j];
                        
                        auto it = std::find(prioPost.begin(), prioPost.end(), priorNumb);
                        if(it != prioPost.end()) {
                            // It is not correct so change it with its posterior 
                            u32 pagePost = pageLine[j+1];
                            pageLine[j] = pagePost;
                            pageLine[j+1] = priorNumb;
                            break;
                        }
                    }
                }else if(i < pageLine.size()-1) {
                    // Check posterior
                    for(u32 j = i+1; j < pageLine.size(); j++) {
                        u32 postNumber = pageLine[j];
                        
                        auto it = std::find(priobef.begin(), priobef.end(), postNumber);
                        if(it != priobef.end()) {
                            u32 pageBef = pageLine[j-1];
                            pageLine[j] = pageBef;
                            pageLine[j-1] = postNumber;
                            break;
                        }
                    }
                }

            }
        }

        u32 middle = pageLine[pageLine.size() / 2];
        printf("From: ");
        printList(pageLine);
        printf("  Middle: %d\n", middle);
        total += middle;
    }

    return total;
}

inline u64 getTimeNanoSinceEpoch() {
    auto time_point_now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(time_point_now.time_since_epoch()).count();
}


int main() {
    printf("DAY5\n");
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

    u64 timeFinishProblem = getTimeNanoSinceEpoch();

    std::unordered_map<u32, std::vector<u32>> prioAfter;
    std::unordered_map<u32, std::vector<u32>> prioBefore;
    std::vector<std::vector<u32>> pageNumbers;

    bool readingPrio = true;
    std::string line;
    while(std::getline(inputFile, line)) {
        if(readingPrio) {
            if(line.empty()) {
                readingPrio = false;
                continue;
            }

            std::vector<std::string> prioline = splitString(line, "|");
            u32 prioA = std::atoi(prioline[0].c_str());
            u32 prioB = std::atoi(prioline[1].c_str());
            auto aftIt = prioAfter.find(prioA);
            if(aftIt != prioAfter.end()) {
                prioAfter[prioA].push_back(prioB);
            }else{
                std::vector<u32> p = {prioB};
                prioAfter.insert({prioA, p});
            }

            auto BefIt = prioBefore.find(prioB);
            if(BefIt != prioBefore.end()) {
                prioBefore[prioB].push_back(prioA);
            }else{
                std::vector<u32> p = {prioA};
                prioBefore.insert({prioB, p});
            }
        }else{
            std::vector<std::string> pageNumbersStr = splitString(line, ",");
            std::vector<u32> page;
            for(std::string& pageNum: pageNumbersStr) {
                page.emplace_back(std::atoi(pageNum.c_str()));
            }
            pageNumbers.emplace_back(page);
        }
    }

    // prioAfter has key fro a numbera and then a vector for all the numbers that go after it
    // priooBeofre has heky for a number and then a vector for the preceding ones

    printf("Time to parse input and build grid: %lfms\n", (f64)(getTimeNanoSinceEpoch() - parseInput) / 1e6);


    printf("Silver: %d\n", getSilver(prioAfter, prioBefore, pageNumbers));
    std::vector<std::vector<u32>> badPagesLines = wrongLines(prioAfter, prioBefore, pageNumbers);

    printf("Gold: %d\n", getGold(prioAfter, prioBefore, badPagesLines));
    printf("Time to finish problem: %lfms\n", (f64)(getTimeNanoSinceEpoch() - timeFinishProblem) / 1e6);
}