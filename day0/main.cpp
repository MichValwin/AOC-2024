#include <fstream>
#include <string>

#include <raylib.h>
#include <virtualResWindow.hpp>
#include <engineDefinitions.hpp>

#include <utils.hpp>

using namespace std;

static const char* INPUT_FILE = "./day0/input.txt";
static const char* DEMO_FILE = "./day0/demo.txt";
static const bool USE_REAL_DATA = false;

u32 getSilver(std::vector<std::string>& lines) {
    u32 result = 0;
    for(std::string& line: lines) {
        char first = 0;
        char last = 0;
        for(char d: line) {
            if(std::isdigit(d)) {
                if(first == 0)first = d;
                last = d;
            }
        }
        
        u32 number = (first-'0')*10 + (last-'0');
        result += number;
    }

    return result;
}

u32 getGold(std::vector<std::string>& lines) {
    const static std::string lookAtTableString[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    
    u32 gold = 0;
    for(std::string& line: lines) {
        char firstChar = '0';
        size_t firsPosition = std::string::npos;
        char lastChar = '0';
        size_t lastPosition = 0;

        for(size_t i = 0; i < line.length(); i++) {
            if(std::isdigit(line[i])) {
                if(firsPosition >= i) {
                    firsPosition = i;
                    firstChar = line[i];
                }
                if(lastPosition <= i) {
                    lastPosition = i;
                    lastChar = line[i];
                }
            }
        }
        // First occurrence of our written numbers
        for(u32 i = 0; i < 9; i++) {
            size_t pos = line.find(lookAtTableString[i]);
            if(pos != std::string::npos) {
                if(firsPosition >= pos) {
                    char c = (char)(i+1)+'0';
                    firsPosition = pos;
                    firstChar = c;
                }
            }
        }
        // Final occurrence
        for(u32 i = 0; i < 9; i++) {
            size_t pos = line.rfind(lookAtTableString[i]);
            if(pos != std::string::npos) {
                if(lastPosition <= pos) {
                    char c = (char)(i+1)+'0';
                    lastPosition = pos;
                    lastChar = c;
                }
            }
        }

        u32 num = (firstChar-'0')*10 + (lastChar-'0');
        gold += num;
    }


    return gold;
}

int main() {
    // Get input file
    fstream inputFile;
    string file = USE_REAL_DATA ? INPUT_FILE : DEMO_FILE;
    inputFile = fstream(file);
    if(!inputFile.is_open()) {
        printf("Can't open %s\n", file.c_str());
        exit(0);
    }

    std::vector<std::string> lines;
    // Parse
    std::string line;
    while(std::getline(inputFile, line)) {
        lines.push_back(line);
    }

    for(std::string& s: lines) {
        std::vector<std::string> v = splitString(s, "2");
        
    }

    printf("Silver: %d\n", getSilver(lines));
    printf("Gold: %d\n", getGold(lines));

    // Drawing test
    /*
    VirtualResWindow virtualWindow;
    virtualWindow.init(300, 300, 60);

    Font fontPixeloid12 = loadFontFronFntFormat("./resources/fonts/pix.fnt");
    Font fontPixeloid36 = loadFontFronFntFormat("./resources/fonts/pix36.fnt");

    while(!WindowShouldClose()) {
        virtualWindow.updateVirtualMouse();
        if(IsWindowResized()) {
            virtualWindow.updateScreen();
        }

        // Draw
        BeginTextureMode(virtualWindow.m_finalFBO);
            ClearBackground(GREEN);
            Camera2D screenSpaceCamera = { 0 }; // Screen space camera
            screenSpaceCamera.zoom = 1.0f;
            BeginMode2D(screenSpaceCamera);
                DrawRectangleRec({10, 10, 100, 100}, WHITE);

                DrawTextEx(fontPixeloid36, "Hello", {0, 36}, 36, 0, RED);

                DrawFPS(GetScreenWidth()-100, 0);
            EndMode2D();
        EndTextureMode();

        // After all sytems run and they draw to FBO
        // We need to draw the FBO to screen 
        virtualWindow.drawFBOToWindow();
    }
    */
}