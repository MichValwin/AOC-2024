#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <regex>
#include <set>
#include <string>
#include <engineDefinitions.hpp>
#include <unordered_map>
#include <unordered_set>
#include <utils.hpp>
#include <chrono>

static const char* INPUT_FILE = "./day9/input.txt";
static const char* DEMO_FILE = "./day9/demo.txt";
static const bool USE_REAL_DATA = true;

struct Disk {
    u32* disk;
    u64 totalSpace;

    void print() const{
        for(u32 j = 0; j < totalSpace; j++) {
            if(disk[j] == UINT32_MAX) {
                printf("%c", '.');
            }else{
                printf("%c", disk[j] + '0');
            }
        }
        printf("\n");
    }
};

u64 getSilver(const Disk& d) {
    Disk dc;
    dc.totalSpace = d.totalSpace;
    dc.disk = new u32[dc.totalSpace];
    for(u32 i = 0; i < dc.totalSpace; i++) {
        dc.disk[i] = d.disk[i];
    }

    u64 left = 0;
    u64 right = dc.totalSpace-1;

    while(left < right) {
        u32 lastPosId = dc.disk[right];
        if(lastPosId == UINT32_MAX){
            right--;
            continue;
        }

        while(dc.disk[left] != UINT32_MAX && left < right) {
            left++;
        }

        if(left >= right) break;
        dc.disk[left] = lastPosId;
        dc.disk[right] = UINT32_MAX;
    }

    // After ordering
    //dc.print();


    u64 checksum = 0;
    for(u32 i = 0; i < dc.totalSpace; i++) {
        u32 id = dc.disk[i];
        if(id == UINT32_MAX) continue;
        checksum += i*id;
    }


    return checksum;
}

u64 getGold(const Disk& d) {
    Disk dc;
    dc.totalSpace = d.totalSpace;
    dc.disk = new u32[dc.totalSpace];
    for(u32 i = 0; i < dc.totalSpace; i++) {
        dc.disk[i] = d.disk[i];
    }

    s64 ls = 0;
    s64 rs = 0;

    s64 fRight = dc.totalSpace-1;
    s64 fLeft = fRight;

    std::unordered_set<u32> fileIDMoved;

    while(fRight >= 0) {
        // get file dimensions
        u32 lastPosId = dc.disk[fRight];
        if(lastPosId == UINT32_MAX){
            fRight--;
            continue;
        }
        fLeft = fRight;

        while(dc.disk[fLeft-1] == lastPosId) {
            fLeft--;
        }
        s64 totalFile = fRight - fLeft + 1;
        printf("trying to move: ");
        for(u32 i = fLeft; i <= fRight; i++) {
            printf("%c", dc.disk[i] + '0');
        }
        printf(" of size: %lld\n", totalFile);

        //Get space
        s64 available = 0;
        ls = 0;
        rs = 0;
        while(available < totalFile && rs < fLeft) {
            while(dc.disk[ls] != UINT32_MAX) ls++;
            rs = ls;
            while(dc.disk[rs+1] == UINT32_MAX)rs++;
            if(rs >= fLeft)break;

            available = rs - ls + 1;
            //printf("Availabe block of %lld\n", available);
            if(available < totalFile)ls++;
        }
        
        if(available >= totalFile) {
            // Move
            for(u32 i = 0; i < totalFile; i++) {
                dc.disk[ls+i] = dc.disk[fLeft+i];
                dc.disk[fLeft+i] = UINT32_MAX;
            }
            //dc.print();
        }

        // move idx
        ls = 0;
        fRight = fLeft-1;
    }

    // After ordering
    //dc.print();


    u64 checksum = 0;
    for(u32 i = 0; i < dc.totalSpace; i++) {
        u32 id = dc.disk[i];
        if(id == UINT32_MAX) continue;
        checksum += i*id;
    }


    return checksum;
}


inline u64 getTimeNanoSinceEpoch() {
    auto time_point_now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(time_point_now.time_since_epoch()).count();
}



int main() {
    printf("DAY9\n");
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

    u64 totalSpace = 0;
    std::string line;
    std::getline(inputFile, line);
    // Get size
    for(const char& c: line) {
        u8 i = c - '0';
        totalSpace += i;
    }

    Disk disk;
    disk.disk = new u32[totalSpace];
    disk.totalSpace = totalSpace;
    u32 i = 0;
    u32 id = 0;
    u32 isData = true;
    for(const char& c: line) {
        u8 n = c - '0';
        for(u32 j = 0; j < n; j++) {
            if(isData)  {
                disk.disk[i] = id;
            }else{
                disk.disk[i] = UINT32_MAX;
            }
            i++;
        }

        if(isData)  {
            id++;
        }else{
            
        }

        isData = !isData;
    }
    disk.print();



    printf("Total space needed: %lld\n", totalSpace);

    //paintGrid(grid.width, grid.height, allAntennasByType, allAntinode);

    printf("Time to parse input and build grid: %lfms\n", (f64)(getTimeNanoSinceEpoch() - parseInput) / 1e6);
    u64 timeFinishProblem = getTimeNanoSinceEpoch();
    printf("Silver: %lld\n", getSilver(disk));
    printf("Gold: %lld\n", getGold(disk));
    printf("Time to finish problem: %lfms\n", (f64)(getTimeNanoSinceEpoch() - timeFinishProblem) / 1e6);

    delete disk.disk;
}