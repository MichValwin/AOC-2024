#include <bitset>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
#include <regex>
#include <set>
#include <stack>
#include <string>
#include <engineDefinitions.hpp>
#include <unordered_map>
#include <utils.hpp>
#include <chrono>
#include <vector>
#include <raylib.h>

static const char* INPUT_FILE = "./day18/input.txt";
static const char* DEMO_FILE = "./day18/demo.txt";
static const bool USE_REAL_DATA = true ;



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
    s32 x, y;

    bool operator<(const Position& o) const {
        if(x != o.x) return x < o.x;
        return y < o.y;
    }

    bool operator==(const Position& o) const {
        return x == o.x && y == o.y;
    }
};
struct PositionHash {
    size_t operator()(const Position& p) const {
        return std::hash<int>()(p.x) ^ (std::hash<int>()(p.y) << 1);
    }
};

void printMap(s32 width, s32 height, const std::vector<Position>& positionsBytes, s32 bytesUsed, const std::set<Position>& path) {
    for(s32 y = 0; y < height; y++) {
        for(s32 x = 0; x < width; x++) {
            char c = '.';
            if(path.find({x, y}) != path.end()) {
                c = 'O';
            }
            for(s32 i = 0; i < bytesUsed; i++) {
                if(positionsBytes[i].x == x && positionsBytes[i].y == y){
                    if(c == 'O'){
                        c = '?';
                    }else{ 
                        c = '#';
                    }
                } 
            }
            printf("%c", c);
        }
        printf("\n");
    }
}


s64 silver(const std::vector<Position>& positionsBytes) {
    Position start = {0,0};
    Position end = {6,6};
    s32 WIDTH = 7;
    s32 HEIGHT = 7;
    s32 BYTES_FALL = 12;
    if(USE_REAL_DATA) {
        start = {0,0};
        end = {70,70};
        WIDTH = 71;
        HEIGHT = 71;
        BYTES_FALL = 1024;
    }
    

    Direction DIRS[4] = {
        {0, -1},
        {1, 0},
        {0, 1},
        {-1, 0}
    };

    std::set<Position> blocks;
    for(s32 i = 0; i < BYTES_FALL; i++) blocks.insert(positionsBytes[i]);

    s64 visitedTotal = 0;

    std::set<Position> visited;
    std::queue<Position> toVisit;
    std::unordered_map<Position, Position, PositionHash> parentNode;
    std::set<Position> path;
    toVisit.emplace(start);

    while(!toVisit.empty()) {
        Position p = toVisit.front();
        toVisit.pop();
        if(visited.find(p) != visited.end()) continue;
        visited.insert(p);

        if(p.x == end.x && p.y == end.y) {
            Position node = p;
            path.emplace(node);
            while(node != start) {
                node = parentNode[node];
                path.emplace(node);
                visitedTotal++;
            }
            printf("FIN\n");
            break;
        }

        for(u8 i = 0; i < 4; i++) {
            Direction dir = DIRS[i];
            Position next = {p.x+dir.x, p.y+dir.y};
            if(next.x >= 0 && next.x < WIDTH && next.y >= 0 && next.y < HEIGHT) {
                if(blocks.find(next) == blocks.end()) {
                    toVisit.emplace(next);
                    parentNode.insert({next, p});
                }
            }
        }
    }

    printMap(WIDTH, HEIGHT, positionsBytes, BYTES_FALL, path);

    return visitedTotal;
}


void gold(const std::vector<Position>& positionsBytes) {
    Position start = {0,0};
    Position end = {6,6};
    s32 WIDTH = 7;
    s32 HEIGHT = 7;
    s32 BYTES_FALL = 12;
    if(USE_REAL_DATA) {
        start = {0,0};
        end = {70,70};
        WIDTH = 71;
        HEIGHT = 71;
        BYTES_FALL = 1024;
    }
    

    Direction DIRS[4] = {
        {0, -1},
        {1, 0},
        {0, 1},
        {-1, 0}
    };

    bool canComplete = true;
    while(canComplete) {
        canComplete = false;
        std::set<Position> blocks;
        for(s32 i = 0; i < BYTES_FALL; i++) blocks.insert(positionsBytes[i]);


        std::set<Position> visited;
        std::stack<Position> toVisit;
        toVisit.emplace(start);

        while(!toVisit.empty()) {
            Position p = toVisit.top();
            toVisit.pop();
            if(visited.find(p) != visited.end()) continue;
            visited.insert(p);

            if(p.x == end.x && p.y == end.y) {
                canComplete = true;
                break;
            }

            for(u8 i = 0; i < 4; i++) {
                Direction dir = DIRS[i];
                Position next = {p.x+dir.x, p.y+dir.y};
                if(next.x >= 0 && next.x < WIDTH && next.y >= 0 && next.y < HEIGHT) {
                    if(blocks.find(next) == blocks.end()) {
                        toVisit.emplace(next);
                    }
                }
            }
        }

        if(canComplete)BYTES_FALL++;
    }

    printf("Position: {%d,%d} blocks the exit\n", positionsBytes[BYTES_FALL-1].x, positionsBytes[BYTES_FALL-1].y);
}

inline u64 getTimeNanoSinceEpoch() {
    auto time_point_now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(time_point_now.time_since_epoch()).count();
}


int main() {
    printf("DAY18\n");
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
    
    std::vector<Position> bytesFalling;
    std::string line;
    while(std::getline(inputFile, line)) {
        std::vector<std::string> numsLine = splitString(line, ",");
        Position p = {std::atoi(numsLine[0].c_str()), std::atoi(numsLine[1].c_str())};
        bytesFalling.emplace_back(p);
    }
    
    printf("Time to parse input: %lfms\n", (f64)(getTimeNanoSinceEpoch() - parseInput) / 1e6);
    u64 timeFinishProblem = getTimeNanoSinceEpoch();
    printf("Silver: %lld\n", silver(bytesFalling));
    gold(bytesFalling);
    printf("Time to finish problem: %lfms\n", (f64)(getTimeNanoSinceEpoch() - timeFinishProblem) / 1e6);
}