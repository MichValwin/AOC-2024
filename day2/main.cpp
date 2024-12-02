#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <string>
#include <engineDefinitions.hpp>
#include <utils.hpp>

static const char* INPUT_FILE = "./day2/input.txt";
static const char* DEMO_FILE = "./day2/demo.txt";
static const bool USE_REAL_DATA = true;

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

u32 getSilver(std::vector<std::vector<u32>>& reports) {
    u32 safeTotal = std::accumulate(reports.begin(), reports.end(), 0, [](u32 acc, const std::vector<u32>& report){
        return acc + isSafe(report);
    });
    return safeTotal;
}

u32 getGold(const std::vector<std::vector<u32>>& reports) {
    u32 safeTotal = 0;
    for(size_t i = 0; i < reports.size(); i++) {
        u8 safe  = isSafe(reports[i]);

        if(safe) {
            //printf("Safe\n");
        }else{
            // Bruteforcing
            for(size_t j = 0; j < reports[i].size(); j++) {
                std::vector<u32> newWihout(reports[i]);
                newWihout.erase(newWihout.begin()+j);
                safe = isSafe(newWihout);
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

    std::vector<std::vector<u32>> reports;
    // Parse
    std::string line;
    while(std::getline(inputFile, line)) {
        std::vector<std::string> reportStr = splitString(line, " ");
        std::vector<u32> report;
        for(std::string& r: reportStr) {
            int num = std::atoi(r.c_str());
            if(num != 0)  report.emplace_back(num); // Bigboy sanitize
        }
        reports.emplace_back(report);
    }

    printf("Silver: %d\n", getSilver(reports));
    printf("Gold: %d\n", getGold(reports));
}