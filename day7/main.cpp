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

static const char* INPUT_FILE = "./day7/input.txt";
static const char* DEMO_FILE = "./day7/demo.txt";
static const bool USE_REAL_DATA = true;

struct MathOp {
    s64 result;
    std::vector<u32> operands;

    void print() const {
        printf("%lld: ", result);
        for(size_t i = 0; i < operands.size(); i++) {
            printf("%d", operands[i]);
            if(i < operands.size()-1)printf(" ");
        }
    };
};

s64 recursive(const MathOp& op, s64 res, u8 idx) {
    if(op.operands.size() == idx) return res;

    u32 next = op.operands[idx];
    if(recursive(op, res+next, idx+1) == op.result) {
        return op.result;
    }else if(recursive(op, res*next, idx+1) == op.result) {
        return op.result;
    }else{
        return 0;
    }
}

u64 concadenate(u64 a, u64 b) {
    std::string conc = std::to_string(a) + std::to_string(b);
    return std::atoll(conc.c_str());
}

s64 recursiveGold(const MathOp& op, u64 res, u8 idx) {
    if(op.operands.size() == idx) return res;

    u32 next = op.operands[idx];
    if(recursiveGold(op, res+next, idx+1) == op.result) {
        return op.result;
    }else if(recursiveGold(op, res*next, idx+1) == op.result) {
        return op.result;
    }else if(recursiveGold(op, concadenate(res,next), idx+1) == op.result) {
        return op.result;
    }else{
        return 0;
    }
}

bool bruteforceOperationSilver(const MathOp& op) {
    return recursive(op, op.operands[0], 1) == op.result;
}

bool bruteforceOperationGold(const MathOp& op) {
    return recursiveGold(op, op.operands[0], 1) == op.result;
}

u64 getSilver(const std::vector<MathOp>& ops) {
    u64 total = 0;
    for(const MathOp& op: ops) {
        if(bruteforceOperationSilver(op)) total += op.result;
    }
    return total;
}

size_t getGold(const std::vector<MathOp>& ops) {
    u64 total = 0;
    for(const MathOp& op: ops) {
        if(bruteforceOperationGold(op)) total += op.result;
    }
    return total;
}

inline u64 getTimeNanoSinceEpoch() {
    auto time_point_now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(time_point_now.time_since_epoch()).count();
}






int main() {
    printf("DAY7\n");
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

    std::vector<MathOp> operations;     

    std::string line;
    while(std::getline(inputFile, line)) {
        auto resultOperands = splitString(line, ": ");
        s64 result = atoll(resultOperands[0].c_str());
        std::vector<std::string> operandsStr = splitString(resultOperands[1], " ");

        std::vector<u32> operands(operandsStr.size());
        std::transform(operandsStr.begin(), operandsStr.end(), operands.begin(), [](const std::string& op) {
            return std::atoi(op.c_str());
        });

        MathOp operation = {result, operands};
        operations.emplace_back(operation);
    }

    for(const MathOp& m: operations) {
        m.print();
        printf("\n");
    }


    printf("Time to parse input and build grid: %lfms\n", (f64)(getTimeNanoSinceEpoch() - parseInput) / 1e6);
    u64 timeFinishProblem = getTimeNanoSinceEpoch();
    printf("Silver: %lld\n", getSilver(operations));
    printf("Gold: %lld\n", getGold(operations));
    printf("Time to finish problem: %lfms\n", (f64)(getTimeNanoSinceEpoch() - timeFinishProblem) / 1e6);
}