#pragma once
#include <raylib.h>
#include "engineDefinitions.hpp"
#include <string>
#include <vector>

Vector3 getMiddlePosition(const Rectangle& bounds);
f32 limitFloat(f32 toLimit, f32 minLimit, f32 maxLimit);
f32 approach(f32 x, f32 step, f32 valueToApproach);

// String related functions
s32 indexOf(const char* str, const char* substr);
s32 lastIndexOf(const char* str, char ch);
std::string getFileExtension(const char* path);
std::string substring(const char* str, u32 start, u32 end);
bool startWith(const char* str, const char* start);
std::vector<std::string> splitString(const std::string& str, const std::string& delimiter);

u32 extractNumber(const char* line, const char* searchText);
std::string extractString(const char* line, const char* searchText);

Font loadFontFronFntFormat(const char* path);