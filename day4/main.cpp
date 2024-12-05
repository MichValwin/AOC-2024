#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <regex>
#include <string>
#include <engineDefinitions.hpp>
#include <utils.hpp>
#include <chrono>

static const char* INPUT_FILE = "./day4/input.txt";
static const char* DEMO_FILE = "./day4/demo.txt";
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

    void printToPrintf() const {
        for(size_t y = 0; y < height; y++) {
            for(size_t x = 0; x < width; x++) {
                printf("%c", grid[y*width + x]);
            }
            printf("\n");
        }
    }
};

struct Direction {
    s8 x, y;
};

struct Position {
    u32 x, y;
};

struct Word {
    std::string word;
    std::vector<Direction> directions; 
};

void setWordToAnotherGrid(const Grid& gridOrigin, Grid& gridOut, Position p, Direction d) {
    for(u8 i = 0; i < 4; i++) {
        char c = gridOrigin.get(p.x, p.y);
        gridOut.set(p.x, p.y, c);
        p.x += d.x;
        p.y += d.y;
    }
}

void setMasX(const Grid& gridOrigin, Grid& gridOut, Position p, const std::vector<Direction>& d) {
    char c = gridOrigin.get(p.x, p.y);
    gridOut.set(p.x, p.y, c);

    for(u8 i = 0; i < d.size(); i++) {
        Position nextPos = {p.x+d[i].x, p.y+d[i].y};
        c = gridOrigin.get(nextPos.x, nextPos.y);
        gridOut.set(nextPos.x, nextPos.y, c);
    }
}

u32 getSilver(const Grid& grid) {
    Word word = {
    "XMAS",
    {
                {1, 0},     // right
                {-1, 0},    // left
                {0, 1},     // up
                {0, -1},    // down
                {1, 1},     // right-up
                {1, -1},    // right-down
                {-1, 1},    // left-up
                {-1, -1}    // left-down
                }
    };

    /*
    Grid gdebug;
    gdebug.initialize(grid.width, grid.height);
    gdebug.writeAllPoints();
    */

    u32 total = 0;

    for(u32 y = 0; y < grid.height; y++) {
        for(u32 x = 0; x < grid.width; x++) {

            char ch = grid.get(x, y);
            if(ch == word.word[0]) {

                // Check in every direction
                for(u8 i = 0; i < word.directions.size(); i++) {
                    Position nextPos = {x, y};
                    const Direction d = word.directions[i];
                    u8 wordIdx = 1;


                    while(wordIdx < word.word.size()) {
                        nextPos.x += d.x;
                        nextPos.y += d.y;
                        if(!grid.isPositionAllowed(nextPos.x, nextPos.y)) break;

                        if(grid.get(nextPos.x, nextPos.y) == word.word[wordIdx]) {
                            wordIdx++;
                        }else {
                            break;
                        }

                        if(wordIdx == word.word.size()) {
                            //setWordToAnotherGrid(grid, gdebug, {x, y}, d);
                            total++;
                        }
                    }
                }
            }

        }
    }

    //gdebug.printToPrintf();

    return total;
}

u32 getGold(const Grid& grid) {
    /*
    Grid gdebug;
    gdebug.initialize(grid.width, grid.height);
    gdebug.writeAllPoints();
    */

    u32 total = 0;

    std::vector<Direction> dirs = {
        {1, 1},
        {-1, 1}
    };

    for(u32 y = 0; y < grid.height; y++) {
        for(u32 x = 0; x < grid.width; x++) {

            char ch = grid.get(x, y);
            if(ch == 'A') {
                
                // Check in every direction
                u8 checkdSides = 0;
                for(u8 i = 0; i < dirs.size(); i++) {
                    const Direction d = dirs[i];

                    Position nextPos = {x+d.x, y+d.y};
                    if(!grid.isPositionAllowed(nextPos.x, nextPos.y)) break;
                    char c1 = grid.get(nextPos.x, nextPos.y);

                    Direction dRevers = dirs[i];
                    dRevers.x = -dRevers.x;
                    dRevers.y = -dRevers.y;
                    nextPos = {x+dRevers.x, y+dRevers.y};
                    if(!grid.isPositionAllowed(nextPos.x, nextPos.y)) break;
                    char c2 = grid.get(nextPos.x, nextPos.y);

                    u8 totNumCharsEq = 0;
                    if(c1 == 'M' || c2 == 'M')totNumCharsEq++;
                    if(c1 == 'S' || c2 == 'S')totNumCharsEq++;
                    if(totNumCharsEq == 2)checkdSides++;
                }

                if(checkdSides == 2) {
                    total += 1;
                    std::vector<Direction> dirs = {
                        {1,1},
                        {1, -1},
                        {-1, -1},
                        {-1, 1}
                    };
                    //setMasX(grid, gdebug, {x, y}, dirs);
                }
                
            }

        }
    }

    //gdebug.printToPrintf();

    return total;
}

inline u64 getTimeNanoSinceEpoch() {
    auto time_point_now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(time_point_now.time_since_epoch()).count();
}


int main() {
    printf("DAY4\n");
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
    Grid grid;
    std::vector<std::string> gridStr;
    std::string line;
    while(std::getline(inputFile, line)) {
        gridStr.emplace_back(line);
    }
    grid.initialize(gridStr[0].size(), gridStr.size());
    for(u32 y = 0; y < gridStr.size(); y++) {
        for(u32 x = 0; x < gridStr[y].size(); x++) {
            char c = gridStr[y][x];
            grid.set(x, y, c);
        }
    }
    //grid.printToPrintf();
    printf("Time to parse input and build grid: %lfms\n", (f64)(getTimeNanoSinceEpoch() - parseInput) / 1e6);


    printf("Silver: %d\n", getSilver(grid));
    printf("Gold: %d\n", getGold(grid));
    printf("Time to finish problem: %lfms\n", (f64)(getTimeNanoSinceEpoch() - timeFinishProblem) / 1e6);
}