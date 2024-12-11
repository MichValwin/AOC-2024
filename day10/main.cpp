#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <regex>
#include <set>
#include <stack>
#include <string>
#include <engineDefinitions.hpp>
#include <unordered_map>
#include <unordered_set>
#include <utils.hpp>
#include <chrono>

static const char* INPUT_FILE = "./day10/bigboy.txt";
static const char* DEMO_FILE = "./day10/demo.txt";
static const bool USE_REAL_DATA = true;


struct Grid {
    size_t width, height;
    char* grid;

    void initialize(size_t _width, size_t _height) {
        width = _width;
        height = _height;
        grid = new char[_width*_height];
    }

    void writeAllPoints() {
        for(size_t y = 0; y < height; y++) {
            for(size_t x = 0; x < width; x++) {
                grid[y*width + x] = '.';
            }
        }
    }

    bool isPositionAllowed(u32 x, u32 y) const {
        if(x < 0 || x >= width || y < 0 || y >= height ) return false;
        return true;
    }

    char get(u32 idx) const {
        return grid[idx];
    }

    char get(u32 x, u32 y) const {
        return grid[y*width + x];
    }

    void set(u32 x, u32 y, char ch) {
        grid[y*width + x] = ch;
    }

    u32 countChar(char ch) const {
        u32 total = 0;
        for(size_t y = 0; y < height; y++) {
            for(size_t x = 0; x < width; x++) {
                if(grid[y*width + x] == ch) total++;
            }
        }
        return total;
    }

    void printToPrintf() const {
        for(size_t y = 0; y < height; y++) {
            for(size_t x = 0; x < width; x++) {
                printf("%c", grid[y*width + x]);
            }
            printf("\n");
        }
    }

    Grid copy() const {
        Grid gCopy;
        gCopy.initialize(width, height);
        for(u32 y = 0; y < height; y++) {
            for(u32 x = 0; x < width; x++) {
                char c = get(x, y);
                gCopy.set(x, y, c);
            }
        }
        return gCopy;
    }

};

struct Direction {
    s8 x, y;
};

struct Position {
    u32 x, y;

    bool operator<(const Position& o) const {
        if(x != o.x) return x < o.x;
        return y < o.y;
    }

    bool operator==(const Position& o) const {
        return x == o.x && y == o.y;
    }
};


u64 getSilver(const Grid& grid) {
    u32 totalTrailHeads = 0;
    const static Direction DIRS[] = {
        {0, -1},
        {1, 0},
        {0, 1},
        {-1, 0}
    };

    for(u32 y = 0; y < grid.height; y++) {
        for(u32 x = 0; x < grid.width; x++) {
            if(grid.get(x, y) == '0') {
                // Is a trailhead
                // Get all the ways to a 9
                u32 numTrails = 0;

                std::stack<Position> toVisit;
                std::set<Position> visited;
                toVisit.push({x, y});

                while(!toVisit.empty()) {
                    Position p = toVisit.top();
                    toVisit.pop();

                    if(visited.find(p) != visited.end()) continue;
                    visited.insert(p);
                    
                    // If we reach said level, it is a trail
                    char actualLevel = grid.get(p.x, p.y);
                    if(actualLevel == '9') {
                        //printf("Reach 9: {%d, %d}\n", p.x, p.y);
                        numTrails++;
                        continue;
                    }

                    for(u8 i = 0; i < 4; i++) {
                        Position pNext = {p.x+DIRS[i].x, p.y+DIRS[i].y};
                        
                        if(!grid.isPositionAllowed(pNext.x, pNext.y)) continue;

                        char nextLevel = grid.get(pNext.x, pNext.y);

                        if(nextLevel == actualLevel+1) {
                            toVisit.emplace(pNext);
                        }
                    }
                }


                totalTrailHeads += numTrails;
            }
        }
    }

    return totalTrailHeads;
}

u64 getGold(const Grid& grid) {
    u32 totalTrailHeads = 0;
    const static Direction DIRS[] = {
        {0, -1},
        {1, 0},
        {0, 1},
        {-1, 0}
    };

    for(u32 y = 0; y < grid.height; y++) {
        for(u32 x = 0; x < grid.width; x++) {
            if(grid.get(x, y) == '0') {
                // Is a trailhead
                // Get all the ways to a 9
                u32 numTrails = 0;

                std::stack<Position> toVisit;
                toVisit.push({x, y});

                while(!toVisit.empty()) {
                    Position p = toVisit.top();
                    toVisit.pop();
                    
                    // If we reach said level, it is a trail
                    char actualLevel = grid.get(p.x, p.y);
                    if(actualLevel == '9') {
                        //printf("Reach 9: {%d, %d}\n", p.x, p.y);
                        numTrails++;
                        continue;
                    }

                    for(u8 i = 0; i < 4; i++) {
                        Position pNext = {p.x+DIRS[i].x, p.y+DIRS[i].y};
                        
                        if(!grid.isPositionAllowed(pNext.x, pNext.y)) continue;

                        char nextLevel = grid.get(pNext.x, pNext.y);

                        if(nextLevel == actualLevel+1) {
                            toVisit.emplace(pNext);
                        }
                    }
                }


                totalTrailHeads += numTrails;
            }
        }
    }

    return totalTrailHeads;
}


inline u64 getTimeNanoSinceEpoch() {
    auto time_point_now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(time_point_now.time_since_epoch()).count();
}



int main() {
    printf("DAY10\n");
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

    std::vector<std::string> gridStr;
    std::string line;
    while(std::getline(inputFile, line)) {
        gridStr.emplace_back(line);
    }
    Grid grid;
    grid.initialize(gridStr[0].size(), gridStr.size());
    // Get the guard and remove it from the grid
    for(u32 y = 0; y < gridStr.size(); y++) {
        for(u32 x = 0; x < gridStr[y].size(); x++) {
            char c = gridStr[y][x];
            grid.set(x, y, c);
        }
    }
    //grid.printToPrintf();


    printf("Time to parse input and build grid: %lfms\n", (f64)(getTimeNanoSinceEpoch() - parseInput) / 1e6);
    u64 timeFinishProblem = getTimeNanoSinceEpoch();
    printf("Silver: %lld\n", getSilver(grid));
    printf("Gold: %lld\n", getGold(grid));
    printf("Time to finish problem: %lfms\n", (f64)(getTimeNanoSinceEpoch() - timeFinishProblem) / 1e6);
}