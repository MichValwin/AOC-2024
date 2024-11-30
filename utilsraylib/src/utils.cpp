#include "utils.hpp"
#include <string>
#include <cstring>

Vector3 getMiddlePosition(const Rectangle& bounds) {
	return {bounds.x + bounds.width/2, bounds.y + bounds.height/2, 0};
}

f32 limitFloat(f32 toLimit, f32 minLimit, f32 maxLimit) {
	if(toLimit > maxLimit) {
		toLimit = maxLimit;
	}else if(toLimit < minLimit) {
		toLimit = minLimit;
	}
	return toLimit;
}

f32 approach(f32 x, f32 step, f32 valueToApproach) {
	if(x < valueToApproach) {
		x = std::min(x+step, valueToApproach);
	}else if(x > valueToApproach) {
		x = std::max(x-step, valueToApproach);
	}
	return x;
}

std::vector<std::string> splitString(const std::string& str, const std::string& delimiter) {
	std::vector<std::string> strings;

	size_t start = 0;
	size_t end = str.find(delimiter);
	while (end != -1) {
		strings.push_back(str.substr(start, end - start));
		start = end + delimiter.size();
		end = str.find(delimiter, start);
	}
	strings.push_back(str.substr(start, end - start));

	return strings;
}

std::string getFileExtension(const char* path) {
	std::string pathStr = path;
	size_t dotPos = pathStr.rfind('.');
	if (dotPos != std::string::npos) {
		return pathStr.substr(dotPos);
	}
	return "";
}

std::string substring(const char* str, u32 start, u32 end) {
	return std::string(str + start, str + end);
}



bool startWith(const char* str, const char* start) {
	while(*start) {
		if(*start++ != *str++) {
			return false;
		}
	}
	return true;
}

s32 indexOf(const char* str, const char* substr) {
    const char* ptr = strstr(str, substr);
    if (ptr != NULL) {
        return ptr - str;
    }
    return -1;
}

s32 lastIndexOf(const char* str, const char* substr) {
	const char* ptr = str;
	const char* lastPtr = NULL;

	while ((ptr = strstr(ptr, substr)) != NULL) {
		lastPtr = ptr;
		ptr++;
	}

	if (lastPtr != NULL) {
		return lastPtr - str;
	}

	return -1;
}

s32 lastIndexOf(const char* str, char ch) {
	s32 lastIndex = -1;

	s32 i = 0;
	while (*str != '\0') {
		if(*str == ch)lastIndex = i;
		str++;
		i++;
	}
	
	return lastIndex;
}

std::string extractString(const char* line, const char* searchText) {
	size_t start = indexOf(line, searchText);
	size_t lengthStart = strlen(searchText);
	start += lengthStart + 1;

	s32 end = start;
	const char* lineAtPos = line + end;
	while(*lineAtPos != ' '){
		end++;
		lineAtPos++;
	}

	if(end != start) {
		return substring(line, start, end);
	}else{
		return "";
	}
}

u32 extractNumber(const char* line, const char* searchText) {
	size_t index = indexOf(line, searchText);
	size_t length = strlen(searchText);
	char number[10];
	index += length + 1;

	s32 i = 0;
	const char* lineAtPos = line + index;
	while(*lineAtPos != ' ' && *lineAtPos != '\0') {
		number[i++] = *lineAtPos;
		lineAtPos++;
	}
	number[i] = '\0';

	s32 ret = atoi(number);

	return ret;
}

Font loadFontFronFntFormat(const char* path) {
	char* fontData = LoadFileText(path);
	// Split text with new lines
	std::vector<std::string> lines = splitString(fontData, "\n");

	/* Header
		info face="Pixeloid Sans" size=36 bold=0 italic=0 charset="" unicode=0 stretchH=100 smooth=1 aa=1 padding=1,1,1,1 spacing=-2,-2
		common lineHeight=45 base=36 scaleW=1024 scaleH=1024 pages=1 packed=0
		page id=0 file="pix36.png"
		chars count=647
	*/
	std::string name = extractString(lines[0].c_str(), "face=");
	s32 size = extractNumber(lines[0].c_str(), "size");
	s32 bold = extractNumber(lines[0].c_str(), "bold");
	s32 italic = extractNumber(lines[0].c_str(), "italic");
	s32 unicode = extractNumber(lines[0].c_str(), "unicode");
	s32 stretchH = extractNumber(lines[0].c_str(), "stretchH");
	s32 smooth = extractNumber(lines[0].c_str(), "smooth");
	s32 aa = extractNumber(lines[0].c_str(), "aa");
	std::string padding = extractString(lines[0].c_str(), "padding");
	std::string spacing = extractString(lines[0].c_str(), "spacing");
	
	s32 lineHeight = extractNumber(lines[1].c_str(), "lineHeight");
	s32 base = extractNumber(lines[1].c_str(), "base");
	s32 scaleW = extractNumber(lines[1].c_str(), "scaleW");
	s32 scaleH = extractNumber(lines[1].c_str(), "scaleH");
	s32 pages = extractNumber(lines[1].c_str(), "pages");
	s32 packed = extractNumber(lines[1].c_str(), "packed");

	s32 id = extractNumber(lines[2].c_str(), "id");
	std::string file = extractString(lines[2].c_str(), "file");
	file.erase(std::remove(file.begin(), file.end(), '"'), file.end());
	
	s32 count = extractNumber(lines[3].c_str(), "count");

	GlyphInfo* glyphs = (GlyphInfo *)RL_MALLOC(count*sizeof(GlyphInfo));
    Rectangle* recs = (Rectangle *)RL_MALLOC(count*sizeof(Rectangle));

	s32 i = 0;
	for(std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it) {
		// Char loading
		if(startWith(it->c_str(), "char ")) {
			//printf("%s\n", *it);
			//Extract info
			s32 id = extractNumber(it->c_str(), "id");
			s32 x = extractNumber(it->c_str(), "x");
			s32 y = extractNumber(it->c_str(), "y");
			s32 width = extractNumber(it->c_str(), "width");
			s32 height = extractNumber(it->c_str(), "height");
			s32 xoffset = extractNumber(it->c_str(), "xoffset");
			s32 yoffset = extractNumber(it->c_str(), "yoffset");
			s32 xadvance = extractNumber(it->c_str(), "xadvance");

			GlyphInfo glyph;
			glyph.value = id;
			glyph.advanceX = xadvance;
			glyph.offsetX = xoffset;
			glyph.offsetY = yoffset;
			glyphs[i] = glyph;

			recs[i] = {(f32)x, (f32)y, (f32)width, (f32)height};
			i++;
		}
	}

	// CAUTION: There should be only ONE Atlas and it should be besides the .fnt file
	s32 indexLastPath = lastIndexOf(path, '/');
	std::string pathToFile = substring(path, 0, indexLastPath+1);
	std::string texturePath = pathToFile + file; 
	Texture fontTexture = LoadTexture(texturePath.c_str());
	
	Font font = {
		size,
		count,
		0,
		fontTexture,
		recs,
		glyphs
	};
	return font;
}