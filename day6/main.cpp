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

static const char* INPUT_FILE = "./day6/input.txt";
static const char* DEMO_FILE = "./day6/demo.txt";
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
};

const Direction DIRS_TO_ROTATE90[] = {
    {0, -1}, // UP
    {1, 0}, // RIGHT
    {0, 1}, // DOWN
    {-1, 0} // LEFT
};

enum DIRECTION {
    UP = 0, RIGHT, DOWN, LEFT
};

struct Guard {
    Position p;
    u8 d;

    bool operator<(const Guard& o) const {
        // Pos
        if(p.x != o.p.x) return p.x < o.p.x;
        if(p.y != o.p.y) return p.y < o.p.y;
        // Dir
        return d < o.d;
    }

    bool operator==(const Guard& o) const {
        return p.x == o.p.x && p.y == o.p.y && d == o.d;
    }
};

struct Guardhash {
    std::size_t operator()(const Guard& g) const noexcept {
        // Combine the hash values of the members
        return (std::hash<int>()(g.p.x) ^
                std::hash<int>()(g.p.y) << 1) ^
                std::hash<int>()(g.d);
    }
};

struct GuardEqual {
    bool operator()(const Guard& a, const Guard& b) const {
        return a.p.x == b.p.x && a.p.y == b.p.y && a.d == b.d;
    }
};



inline u64 getTimeNanoSinceEpoch() {
    auto time_point_now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(time_point_now.time_since_epoch()).count();
}


void rotate(Guard& g) {
    g.d = (g.d+1) % 4;
}

void moveGuard(const Grid& grid, Guard& guard) {
    Position nextPos;
    Direction dir = DIRS_TO_ROTATE90[guard.d];
    nextPos.x = guard.p.x+dir.x;
    nextPos.y = guard.p.y+dir.y;

    if(grid.isPositionAllowed(nextPos.x, nextPos.y)) {
        char c = grid.get(nextPos.x, nextPos.y);
        if(c == '#') {
            nextPos = guard.p;
            rotate(guard);
        }
        if(c == 'O') {
            nextPos = guard.p;
            rotate(guard);
        }
    }
    guard.p = nextPos;
}

u32 getSilver(const Grid& grid, const Guard& guard) {
    Grid gCopy = grid.copy();
    Guard guardC = {guard.p, guard.d}; 

    while(grid.isPositionAllowed(guardC.p.x, guardC.p.y)) {
        gCopy.set(guardC.p.x, guardC.p.y, 'X');
        moveGuard(gCopy, guardC);
    }
    //gCopy.printToPrintf();

    u32 total = gCopy.countChar('X');

    return total;   
}

size_t getGold(const Grid& grid, const Guard& guard) {
    Grid gCopy = grid.copy();
    std::vector<Position> obstacles;

    for(u32 y = 0; y < gCopy.height; y++) {
        for(u32 x = 0; x < gCopy.width; x++) {
            Position pObs = {x, y};
            if(grid.get(x, y) == '#' || (guard.p.x == x && guard.p.y == y)) continue; // Cant be in a obstruction or in the guard initial pos

            Guard guardC = {guard.p, guard.d};
            std::unordered_set<Guard, Guardhash, GuardEqual> positionsGuardHasbeen;

            gCopy.set(x, y, 'O');
            while(grid.isPositionAllowed(guardC.p.x, guardC.p.y)) {
                if(positionsGuardHasbeen.find(guardC) == positionsGuardHasbeen.end()) {
                    positionsGuardHasbeen.insert(guardC);
                    moveGuard(gCopy, guardC);
                }else{
                    obstacles.emplace_back(pObs);
                    break;
                }
            }
            gCopy.set(x, y, '.');
        }
    }
    
    return obstacles.size();
}

int main() {
    printf("DAY6\n");
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


    Grid grid;
    std::vector<std::string> gridStr;
    std::string line;
    while(std::getline(inputFile, line)) {
        gridStr.emplace_back(line);
    }
    grid.initialize(gridStr[0].size(), gridStr.size());
    Guard guard;
    // Get the guard and remove it from the grid
    for(u32 y = 0; y < gridStr.size(); y++) {
        for(u32 x = 0; x < gridStr[y].size(); x++) {
            char c = gridStr[y][x];
            if(c == '^') {
                guard = {{x, y}, UP};
                grid.set(x, y, '.');
            }else{
                grid.set(x, y, c);
            }
        }
    }
    //grid.printToPrintf();

    printf("Time to parse input and build grid: %lfms\n", (f64)(getTimeNanoSinceEpoch() - parseInput) / 1e6);

    u64 timeFinishProblem = getTimeNanoSinceEpoch();
    printf("Silver: %d\n", getSilver(grid, guard));
    printf("Gold: %lld\n", getGold(grid, guard));
    printf("Time to finish problem: %lfms\n", (f64)(getTimeNanoSinceEpoch() - timeFinishProblem) / 1e6);
}