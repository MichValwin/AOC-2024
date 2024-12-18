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

static const char* INPUT_FILE = "./day15/input.txt";
static const char* DEMO_FILE = "./day15/demo.txt";
static const bool USE_REAL_DATA = true;

struct Direction {
    s8 x, y;

    void print() const {
        if(x == 0 && y == -1) {
            printf("^");
        }else if(x == 1 && y == 0) {
            printf(">");
        }else if(x == 0 && y == 1) {
            printf("v");
        }else {
            printf("<");
        }
    }
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

    char get(const Position& p) const {
        return grid[p.y*width + p.x];
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

     void printToPrintfWidthPlayer(const Position& player) const {
        for(size_t y = 0; y < height; y++) {
            for(size_t x = 0; x < width; x++) {
                char c = grid[y*width + x];
                if(player.x == x && player.y == y) c = '@';
                printf("%c", c);
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




bool moveNext(Grid& grid, const Position& p, const Direction& d) {
    char current = grid.get(p);
    Position nextPos = {p.x+d.x, p.y+d.y};
    char next = grid.get(nextPos);

    if(next == '.') {
        // can move
        grid.set(p.x, p.y, '.');
        grid.set(nextPos.x, nextPos.y, current);
        return true;
    }else if(next == '#') {
        return false;
    }else if(next == 'O' || next == '[' || next == ']') {
        // Get box position
        if(moveNext(grid, nextPos, d)) {
            grid.set(p.x, p.y, '.');
            grid.set(nextPos.x, nextPos.y, current);
            return true;
        }else{
            return false;
        }
    }
}

bool canMoveGold(Grid& grid, const Position& p, const Direction& d) {
    Position nextPos = {p.x+d.x, p.y+d.y};
    char next = grid.get(nextPos);

    if(next == '.') {
        return true;
    }else if(next == '#') {
        return false;
    }else if(next == '[' || next == ']') {
        // Get box position
        Position b1;
        Position b2;
        if(next == '[') {
            b1 = nextPos;
            b2 = nextPos; b2.x+=1;
        }else{
            b2 = nextPos;
            b1 = nextPos; b1.x-=1;
        }
        bool canMoveB1 = canMoveGold(grid, b1, d);
        bool canMoveB2 = canMoveGold(grid, b2, d);

        if(canMoveB1 && canMoveB2) return true;
        return false;
    }
    printf("SHOULD NOT BE HERE\n");
    return false;
}

void nextMoveVertical(Grid& grid, const Position& p, const Direction& d) {
    char current = grid.get(p);
    Position nextPos = {p.x+d.x, p.y+d.y};
    char next = grid.get(nextPos);
    
    if(next == '.') {
        // Move
        grid.set(p.x, p.y, '.');
        grid.set(nextPos.x, nextPos.y, current);
    }else if(next == '[' || next == ']') {
        // Get box position
        Position b1;
        Position b2;
        if(next == '[') {
            b1 = nextPos;
            b2 = nextPos; b2.x+=1;
        }else{
            b2 = nextPos;
            b1 = nextPos; b1.x-=1;
        }
        nextMoveVertical(grid, b1, d);
        nextMoveVertical(grid, b2, d);
        grid.set(p.x, p.y, '.');
        grid.set(nextPos.x, nextPos.y, current);
    }
}



void silver(const Grid& grid, const std::vector<Direction>& directions) {
    Grid gc;
    gc.initialize(grid.width, grid.height);

    Position rob;
    for(s32 y = 0; y < grid.height; y++) {
        for(s32 x = 0; x < grid.width; x++) {
            char c = grid.get(x, y);
            if(c == '@') {
                rob = {x, y};
                gc.set(x, y, '.');
            }else{
                gc.set(x, y, c);
            }       
        }
    }

    gc.printToPrintfWidthPlayer(rob);

    for(const Direction d: directions) {
        //d.print(); printf("\n");
        Position nextPos = {rob.x+d.x, rob.y+d.y};
        bool moved = moveNext(gc, rob, d);
        if(moved) rob = nextPos;
        //gc.printToPrintfWidthPlayer(rob);
    }

    gc.printToPrintfWidthPlayer(rob);

    // Count boxes points
    s64 boxesPoints = 0;
    for(s32 y = 0; y < gc.height; y++) {
        for(s32 x = 0; x < gc.width; x++) {
            if(gc.get(x, y) == 'O') boxesPoints += y*100+x;
        }
    }
    printf("Silver: %lld\n", boxesPoints);


    delete[] gc.grid;
}

void gold(const Grid& grid, const std::vector<Direction>& directions) {
    // Enlarge
    Grid gc;
    gc.initialize(grid.width*2, grid.height);

    Position rob;
    for(s32 y = 0; y < grid.height; y++) {
        for(s32 x = 0; x < grid.width; x++) {
            char c = grid.get(x, y);
            if(c == '@') {
                rob = {x*2, y};
                gc.set(x*2, y, '.');
                gc.set(x*2+1, y, '.');
            }
            if(c == '#') {
                gc.set(x*2, y, '#');
                gc.set(x*2+1, y, '#');
            }
            if(c == 'O') {
                gc.set(x*2, y, '[');
                gc.set(x*2+1, y, ']');
            }
            if(c == '.') {
                gc.set(x*2, y, '.');
                gc.set(x*2+1, y, '.');
            }
        }
    }

     for(const Direction d: directions) {
        d.print(); printf("\n");
        Position nextPos = {rob.x+d.x, rob.y+d.y};
        if(d.x != 0) {
            // moving in horizontal
            bool moved = moveNext(gc, rob, d);
            if(moved) rob = nextPos;
        }else{
            // vertical
            bool canMove = canMoveGold(gc, rob,d);
            if(canMove) {
                nextMoveVertical(gc, rob, d);
                rob = nextPos;
            }
        }
        //gc.printToPrintfWidthPlayer(rob);
        //gc.printToPrintf();
    }

    gc.printToPrintfWidthPlayer(rob);

    // Count boxes points
    s64 boxesPoints = 0;
    for(s32 y = 0; y < gc.height; y++) {
        for(s32 x = 0; x < gc.width; x++) {
            if(gc.get(x, y) == '[') boxesPoints += y*100+x;
        }
    }
    printf("Gold: %lld\n", boxesPoints);
}

inline u64 getTimeNanoSinceEpoch() {
    auto time_point_now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(time_point_now.time_since_epoch()).count();
}


int main() {
    printf("DAY15\n");
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
    
    std::vector<Direction> directions;

    Grid grid;
    u32 width = 0, height = 1;
    std::string line;
    std::getline(inputFile, line);
    width = line.size();

    while(std::getline(inputFile, line)) {
        if(line.empty()) {
            break;
        }
        height++;
    }

    inputFile.clear(); // Clear any error flags
    inputFile.seekg(0, std::ios::beg); // Move the get pointer to the beginning
    grid.initialize(width, height);

    bool processGrid = true;
    u32 y = 0;
    while(std::getline(inputFile, line)) {
        if(line.empty()) {
            processGrid = false;
            std::getline(inputFile, line);
        }
        if(processGrid) {
            for(u32 x = 0; x < grid.width; x++) {
                grid.set(x, y, line[x]);
            }
            y++;
        }else{
            for(const char c: line) {
                switch(c) {
                    case '^':
                        directions.push_back({0, -1});
                        break;
                    case '>':
                        directions.push_back({1, 0});
                        break;
                    case 'v':
                        directions.push_back({0, 1});
                        break;
                    case '<':
                        directions.push_back({-1, 0});
                        break;
                }
            }
        }
    }
    grid.printToPrintf();
    printf("\n");
    for(const Direction& d: directions) {
        d.print();
    }
    printf("\n");

    
    printf("Time to parse input: %lfms\n", (f64)(getTimeNanoSinceEpoch() - parseInput) / 1e6);
    u64 timeFinishProblem = getTimeNanoSinceEpoch();
    //silver(grid, directions);
    gold(grid, directions);
    printf("Time to finish problem: %lfms\n", (f64)(getTimeNanoSinceEpoch() - timeFinishProblem) / 1e6);
}