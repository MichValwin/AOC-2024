#include <algorithm>
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

static const char* INPUT_FILE = "./day8/input.txt";
static const char* DEMO_FILE = "./day8/demo.txt";
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

Position getDistance(const Position& a, const Position& b) {
    Position dist = {a.x - b.x, a.y - b.y};
    return dist;
}

void paintGrid(u32 width, u32 height, const std::unordered_map<char, std::vector<Position>>& allAntennasByType, const std::set<Position>& allAntinode) {
    Grid grid;
    grid.initialize(width, height);
    grid.writeAllPoints();

    for(const Position& anti: allAntinode) {
        grid.set(anti.x, anti.y, '#');
    }

    for(const auto& ant: allAntennasByType) {
        for(const Position& p: ant.second) {
            grid.set(p.x, p.y, ant.first);
        }
    }

    grid.printToPrintf();
    delete grid.grid;
}


u64 getSilver(const Grid& grid, const std::unordered_map<char, std::vector<Position>>& allAntennasByType) {
    std::set<Position> allAntinode;

    for(const auto& pair: allAntennasByType) {
        char antennaType = pair.first;
        std::vector<Position> allPositions = pair.second;
        for(u32 i = 0; i < allPositions.size(); i++) {
            Position p = allPositions[i];
            for(u32 j = 0; j < allPositions.size(); j++) {
                if(j == i) continue;
                Position o = allPositions[j];
                Position dist = getDistance(p, o); // can Be negative
                dist.x *= 2;
                dist.y *= 2;

                Position antinode = {o.x + dist.x, o.y + dist.y};
                if(grid.isPositionAllowed(antinode.x, antinode.y)){
                    allAntinode.insert(antinode);
                }
            }
        }
    }

    paintGrid(grid.width, grid.height, allAntennasByType, allAntinode);

    return allAntinode.size();
}

u64 getGold(const Grid& grid, const std::unordered_map<char, std::vector<Position>>& allAntennasByType) {
    std::set<Position> allAntinode;

    for(const auto& pair: allAntennasByType) {
        char antennaType = pair.first;
        std::vector<Position> allPositions = pair.second;
        for(u32 i = 0; i < allPositions.size(); i++) {
            Position p = allPositions[i];
            for(u32 j = 0; j < allPositions.size(); j++) {
                if(j == i) continue;
                Position o = allPositions[j];
                Position dist = getDistance(p, o); // can Be negative

                Position antinode = {o.x + dist.x, o.y + dist.y};
                while(grid.isPositionAllowed(antinode.x, antinode.y)) {
                    allAntinode.insert(antinode);
                    antinode = {antinode.x + dist.x, antinode.y + dist.y};
                }
            }
        }
    }

    paintGrid(grid.width, grid.height, allAntennasByType, allAntinode);

    return allAntinode.size();
}


inline u64 getTimeNanoSinceEpoch() {
    auto time_point_now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(time_point_now.time_since_epoch()).count();
}





int main() {
    printf("DAY8\n");
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

    std::unordered_map<char, std::vector<Position>> allAntennasByType;

    Grid grid;
    grid.initialize(gridStr[0].size(), gridStr.size());
    for(u32 y = 0; y < gridStr.size(); y++) {
        for(u32 x = 0; x < gridStr[y].size(); x++) {
            char c = gridStr[y][x];
            grid.set(x, y, c);
            if(c != '.') {
                // Add antenna
                if(allAntennasByType.find(c) != allAntennasByType.end()) {
                    // Add new antenna
                    allAntennasByType[c].push_back({x, y});
                }else{
                    allAntennasByType[c] = std::vector<Position>();
                    allAntennasByType[c].push_back({x, y});
                }
            }
        }
    }

    //paintGrid(grid.width, grid.height, allAntennasByType, allAntinode);

    printf("Time to parse input and build grid: %lfms\n", (f64)(getTimeNanoSinceEpoch() - parseInput) / 1e6);
    u64 timeFinishProblem = getTimeNanoSinceEpoch();
    printf("Silver: %lld\n", getSilver(grid, allAntennasByType));
    printf("Gold: %lld\n", getGold(grid, allAntennasByType));
    printf("Time to finish problem: %lfms\n", (f64)(getTimeNanoSinceEpoch() - timeFinishProblem) / 1e6);
}