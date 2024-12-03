#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <string>
#include <chrono>
#include <engineDefinitions.hpp>
#include <utils.hpp>


static const char* INPUT_FILE = "./day2/bigboy.txt";
static const char* DEMO_FILE = "./day2/demo.txt";
static const bool USE_REAL_DATA = true;

struct ReportSafety {
    bool safe;
    size_t idxA;
    size_t idxB;
};

bool isSafe(const std::vector<u32>& report) {
    bool desc = false;
    if(report[0] > report[1]) desc = true;

    for(size_t i = 0; i < report.size()-1; i++) {
        s32 a = report[i], b = report[i+1];
        if((desc && a < b) || (!desc && a > b)) {
            return false;
        }
        s32 diff = std::abs(a - b);
        if(diff == 0 || diff > 3) {
            return false;
        }
    }

    return true;
}

ReportSafety isSafeWithReport(const std::vector<u32>& report) {
    bool desc = false;
    if(report[0] > report[1]) desc = true;

    for(size_t i = 0; i < report.size()-1; i++) {
        s32 a = report[i], b = report[i+1];
        if((desc && a < b) || (!desc && a > b)) {
            return {false, i, i+1};
        }
        s32 diff = std::abs(a - b);
        if(diff == 0 || diff > 3) {
            return {false, i, i+1};
        }
    }

    return {true};
}

bool isSafewithout(const std::vector<u32>& report, size_t indexNoCheck) {
    bool desc = false;

    u32 firstIdxDesc = 0;
    u32 secondIdxDesc = 1;
    if(indexNoCheck == 0) {
        firstIdxDesc = 1;
        secondIdxDesc = 2;
    }
    if(indexNoCheck == 1) {
        firstIdxDesc = 0;
        secondIdxDesc = 2;
    }

    if(report[firstIdxDesc] > report[secondIdxDesc]) desc = true;

    for(size_t i = 0; i < report.size()-1; i++) {
        if(i == indexNoCheck)continue;

        size_t idx1 = i, idx2 = i+1;
        if(i+1 == indexNoCheck) {
            if(i+2 >= report.size()) {
                return true; // index to not check is the end. We have checked the last ones, so the report is "safe"
            } 
            idx2 = i+2;
        }

        s32 a = report[idx1], b = report[idx2];
        if((desc && a < b) || (!desc && a > b)) {
            return false;
        }
        s32 diff = std::abs(a - b);
        if(diff == 0 || diff > 3) {
            return false;
        }
    }

    return true;
}


inline u64 getTimeNanoSinceEpoch() {
    auto time_point_now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(time_point_now.time_since_epoch()).count();
}


u32 getSilver(std::vector<std::vector<u32>>& reports) {
    u32 safeTotal = std::accumulate(reports.begin(), reports.end(), 0, [](u32 acc, const std::vector<u32>& report){
        return acc + isSafe(report);
    });
    return safeTotal;
}

u32 getGold(const std::vector<std::vector<u32>>& reports) {
    u32 safeTotal = 0;
    for(size_t i = 0; i < reports.size(); i++) {
        u8 safe = isSafe(reports[i]);
        
        if(safe) {
            //printf("Safe\n");
        }else{
            // Bruteforcing
            for(size_t j = 0; j < reports[i].size(); j++) {
                safe = isSafewithout(reports[i], j);
                if(safe) {
                    //printf("Safe but removing index: %lld\n", j);
                    break;
                }
            }            
        }

        if(!safe) {
            //printf("Unsafe\n");
        }

        safeTotal += safe;
    }

    return safeTotal;
}

int main() {
    printf("DAY2\n");
    u64 timeStartFetch = getTimeNanoSinceEpoch();
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
    printf("Time to read file: %lfms\n", (f64)(getTimeNanoSinceEpoch() - timeStartFetch) / 1e6);


    u64 timeStartParsing = getTimeNanoSinceEpoch();
    std::vector<std::vector<u32>> reports;
    // Parse
    std::string line;
    while(std::getline(inputFile, line)) {
        if(line.rfind(" ") == line.size()-1) line.pop_back(); // remove trailing space
        std::vector<std::string> reportStr = splitString(line, " ");
        std::vector<u32> report;
        for(std::string& r: reportStr) {
            int num = std::atoi(r.c_str());
            report.emplace_back(num);
        }
        reports.emplace_back(report);
    }
    printf("Time to parse text: %lfms\n", (f64)(getTimeNanoSinceEpoch() - timeStartParsing) / 1e6);

    u64 timeStartSilver = getTimeNanoSinceEpoch();
    printf("Silver: %d\n", getSilver(reports));
    printf("Time to silver: %lfms\n", (f64)(getTimeNanoSinceEpoch() - timeStartSilver) / 1e6);

    u64 timeStartGold = getTimeNanoSinceEpoch();
    printf("Gold: %d\n", getGold(reports));
    printf("Time to gold: %lfms\n", (f64)(getTimeNanoSinceEpoch() - timeStartGold) / 1e6);
    printf("\n");
    printf("Time total: %lfms\n", (f64)(getTimeNanoSinceEpoch() - timeStartFetch) / 1e6);
}