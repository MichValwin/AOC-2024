#pragma once

#include <cassert>
#include <cstdint>
#include <raylib.h>

// Normal definitions
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using s8 = int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;

using f32 = float;
using f64 = double;


#define LOGGIN_ENABLED
#define RAYLIB_LOGGING_ENABLED


/* Raylib logging
LOG_ALL = 0,        // Display all logs
LOG_TRACE,          // Trace logging, intended for internal use only
LOG_DEBUG,          // Debug logging, used for internal debugging, it should be disabled on release builds
LOG_INFO,           // Info logging, used for program execution info
LOG_WARNING,        // Warning logging, used on recoverable failures
LOG_ERROR,          // Error logging, used on unrecoverable failures
LOG_FATAL,          // Fatal logging, used to abort program: exit(EXIT_FAILURE)
LOG_NONE            // Disable logging
*/

#ifdef LOGGIN_ENABLED
    #ifdef RAYLIB_LOGGING_ENABLED
        #define LOG(level, ...) \
            TraceLog(level, __VA_ARGS__);
    #else
        #define LOG(level, ...) \
            printf(__VA_ARGS__);
    #endif
#else
    #define LOG(level, message)
#endif