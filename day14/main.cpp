#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
#include <regex>
#include <set>
#include <string>
#include <engineDefinitions.hpp>
#include <unordered_map>
#include <utils.hpp>
#include <chrono>
#include <vector>
#include <raylib.h>

static const char* INPUT_FILE = "./day14/input.txt";
static const char* DEMO_FILE = "./day14/demo.txt";
static const bool USE_REAL_DATA = true;


struct Offset {
    s64 dx, dy;
};

struct Position {
    s64 x, y;
};

struct Velocity {
    s64 dx, dy;
};


s64 min(s64 a, s64 b) {
    if(a < b) return a;
    return b;
}

s64 max(s64 a, s64 b) {
    if(a > b) return a;
    return b;
}

s64 gcd(s64 a, s64 b) {
    while(b != 0) {
        s64 bb = b;
        b = a%b;
        a = bb;
    }
    return a;
}

s64 lcm(s64 a, s64 b) {
    return (a*b)/gcd(a,b);
}

void printGCD(s64 a, s64 b) {
    printf("GCD(%lld, %lld): %lld\n", a, b, gcd(a, b));
}

void printLCM(s64 a, s64 b) {
    printf("LCM(%lld, %lld): %lld\n", a, b, lcm(a, b));
}

struct Robot {
    Position pos;
    Velocity vel;

    void print() const {
        printf("p=%d,%d v=%d,%d\n", pos.x, pos.y, vel.dx, vel.dy);
    }
};

std::pair<u32, u32> getWitdhHeightGrid(const std::vector<Robot>& robots) {
    u32 width = 0, height = 0;
    for(const Robot& rob: robots) {
        if(rob.pos.x > width)width = rob.pos.x;
        if(rob.pos.y > height)height = rob.pos.y;
    }
    width++;
    height++;
    return std::pair<u32, u32>(width, height);
} 

void printMap(const std::vector<Robot>& robots) {
    auto gridDimension = getWitdhHeightGrid(robots);
    for(u32 y = 0; y < gridDimension.second; y++) {
        for(u32 x = 0; x < gridDimension.first; x++) {
            u32 totalRob = 0;
            for(const Robot& r: robots) {
                if(r.pos.x == x && r.pos.y == y)totalRob++;
            }

            if(totalRob == 0) {
                printf(".");
            }else{
                printf("%d", totalRob);
            }
        }
        printf("\n");
    }
}

u64 silver(const std::vector<Robot>& robots) {
    std::vector<Robot> robs = robots;
    s64 total = 0;

    auto gridDimension = getWitdhHeightGrid(robots);
    u32 quadrantWidth = gridDimension.first/2;
    u32 quadrantHeight = gridDimension.second/2;

    printMap(robs);
    printf("\n");
    printf("\n");

    u32 quadrantCount[4] = {0,0,0, 0};

    for(Robot& robot: robs) {
        // 100 turns
        robot.pos.x = (robot.pos.x+robot.vel.dx*100) % gridDimension.first;
        robot.pos.y = (robot.pos.y+robot.vel.dy*100) % gridDimension.second;

        if(robot.pos.x < 0) robot.pos.x = robot.pos.x + gridDimension.first;
        if(robot.pos.y < 0) robot.pos.y = robot.pos.y + gridDimension.second;

        if(robot.pos.x == quadrantWidth || robot.pos.y == quadrantHeight)continue;
        u32 quadIdx = 0;

        if(robot.pos.x > quadrantWidth)quadIdx +=1;
        if(robot.pos.y > quadrantHeight)quadIdx +=2;
        quadrantCount[quadIdx]++;
    }
    printMap(robs);
    for(u32 i = 0; i < 4; i++) {
        printf("Quadrant %d: %d robots\n", i, quadrantCount[i]);
    }

    total = quadrantCount[0]*quadrantCount[1]*quadrantCount[2]*quadrantCount[3];

    return total;
}

void writeImageRobots(const std::vector<Robot>& robots, Image& image) {
    auto gridDimension = getWitdhHeightGrid(robots);
    for(u32 y = 0; y < gridDimension.second; y++) {
        for(u32 x = 0; x < gridDimension.first; x++) {
            u32 totalRob = 0;
            for(const Robot& r: robots) {
                if(r.pos.x == x && r.pos.y == y)totalRob++;
            }
            u8* dataPtr = static_cast<u8*>(image.data);
            if(totalRob == 0) {
                dataPtr[y*gridDimension.first+x] = 0;
            }else{
                dataPtr[y*gridDimension.first+x] = 255;
            }
        }
    }
}

void gold(const std::vector<Robot>& robots) {
    std::vector<Robot> robs = robots;
    auto gridDimension = getWitdhHeightGrid(robots);

    Image image;
    image.data = new u8[gridDimension.first*gridDimension.second];
    for(u32 i = 0; i < gridDimension.first*gridDimension.second; i++)((u8*)image.data)[i] = 0;
    image.width = gridDimension.first;
    image.height = gridDimension.second;
    image.format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE;

    for(u32 i = 0; i < 10000; i++) {
        for(Robot& robot: robs) {
            // 100 turns
            robot.pos.x = (robot.pos.x+robot.vel.dx*1) % gridDimension.first;
            robot.pos.y = (robot.pos.y+robot.vel.dy*1) % gridDimension.second;

            if(robot.pos.x < 0) robot.pos.x = robot.pos.x + gridDimension.first;
            if(robot.pos.y < 0) robot.pos.y = robot.pos.y + gridDimension.second;
        }
        writeImageRobots(robs, image);
        std::string fileName = "robots/robots"+ std::to_string(i) +".png";
        ExportImage(image, fileName.c_str());
    }
}



inline u64 getTimeNanoSinceEpoch() {
    auto time_point_now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(time_point_now.time_since_epoch()).count();
}


int main() {
    printf("DAY14\n");
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

    std::vector<Robot> robots;
    std::string line;
    while(std::getline(inputFile, line)) {
        std::vector<std::string> posvel = splitString(line, " ");
        // Position
        std::vector<std::string> posvec = splitString(splitString(posvel[0], "p=")[1], ",");
        Position pos = {std::atoi(posvec[0].c_str()), std::atoi(posvec[1].c_str())};
        // velocity
        std::vector<std::string> velVec = splitString(splitString(posvel[1], "v=")[1], ",");
        Velocity vel = {std::atoi(velVec[0].c_str()), std::atoi(velVec[1].c_str())};

        Robot r = {pos, vel};
        robots.push_back(r);
    }

    for(const Robot& r: robots) {
        r.print();
    }

    
    printf("Time to parse input: %lfms\n", (f64)(getTimeNanoSinceEpoch() - parseInput) / 1e6);
    u64 timeFinishProblem = getTimeNanoSinceEpoch();
    printf("Silver: %lld\n", silver(robots));
    gold(robots);
    printf("Time to finish problem: %lfms\n", (f64)(getTimeNanoSinceEpoch() - timeFinishProblem) / 1e6);
}