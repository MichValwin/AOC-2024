#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <queue>
#include <set>
#include <string>
#include <engineDefinitions.hpp>
#include <unordered_map>
#include <utils.hpp>
#include <chrono>

static const char* INPUT_FILE = "./day12/input.txt";
static const char* DEMO_FILE = "./day12/demo.txt";
static const bool USE_REAL_DATA = false;



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

inline u64 getTimeNanoSinceEpoch() {
    auto time_point_now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(time_point_now.time_since_epoch()).count();
}

u64 silver(const Grid& grid) {
    std::set<Position> visited;
    u64 total = 0;

    Direction DIRS[4] = {
        {0, -1},
        {1, 0},
        {0, 1},
        {-1, 0}
    };

    for(u32 y = 0; y < grid.height; y++) {
        for(u32 x = 0; x < grid.width; x++) {
            char c = grid.get(x, y);
            Position p = {x, y};
            if(visited.find(p) == visited.end()) {
                visited.insert(p);

                // Get area and perimeter
                std::queue<Position> toVisit;
                toVisit.emplace(p);
                u32 area = 0, perimeter = 0;

                while(!toVisit.empty()) {
                    Position pc = toVisit.front();
                    toVisit.pop();
                    visited.insert(pc);

                    area++;
                    for(u8 d = 0; d < 4; d++) {
                        Direction dir = DIRS[d];
                        Position nextP = {pc.x+dir.x, pc.y+dir.y};
                        if(grid.isPositionAllowed(nextP.x, nextP.y)) {
                            char cn = grid.get(nextP.x, nextP.y);
                            if(cn == c) {
                                if(visited.find(nextP) == visited.end()) {
                                    toVisit.push(nextP);
                                }
                            }else{
                                perimeter++;
                            }
                        }else{
                            // Distinct, this one is a perimeter
                            perimeter++;
                        }
                    }
                }
                total += area*perimeter;
                printf("A region of %c plants with price %d * %d = %d\n", c, area, perimeter, area*perimeter);
            }
        }
    }

    return total;
}

u64 gold(const Grid& grid) {
    return 1;
}

int main() {
    printf("DAY12\n");
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

    std::string line;
    std::vector<std::string> gridStr;
    while(std::getline(inputFile, line)) {
        gridStr.push_back(line);
    }

    Grid  grid;
    grid.initialize(gridStr[0].size(), gridStr.size());
    for(u32 y = 0; y < gridStr.size(); y++) {
        for(u32 x = 0; x < grid.width; x++) {
            grid.set(x, y, gridStr[y][x]);
        }
    }
    grid.printToPrintf();


    printf("Time to parse input and build grid: %lfms\n", (f64)(getTimeNanoSinceEpoch() - parseInput) / 1e6);
    u64 timeFinishProblem = getTimeNanoSinceEpoch();
    printf("Silver: %lld\n", silver(grid));
    printf("Gold: %lld\n", gold(grid));
    printf("Time to finish problem: %lfms\n", (f64)(getTimeNanoSinceEpoch() - timeFinishProblem) / 1e6);
}