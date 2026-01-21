#ifndef COLOR_PRINTF_H
#define COLOR_PRINTF_H

#include <stdio.h>

/* Detect platform */
#ifdef _WIN32
    #define COLOR_PRINTF_WINDOWS 1
#else
    #define COLOR_PRINTF_WINDOWS 0
#endif

/* Foreground color ANSI escape codes (simple 8-color set) */
#define C_RESET   "\x1b[0m"
#define C_BLACK   "\x1b[30m"
#define C_RED     "\x1b[31m"
#define C_GREEN   "\x1b[32m"
#define C_YELLOW  "\x1b[33m"
#define C_BLUE    "\x1b[34m"
#define C_MAGENTA "\x1b[35m"
#define C_CYAN    "\x1b[36m"
#define C_WHITE   "\x1b[37m"

/* Windows-specific color codes (WinAPI) */
#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    
    /* Windows console color constants */
    #define WIN_BLACK       0
    #define WIN_DARK_BLUE   FOREGROUND_BLUE
    #define WIN_DARK_GREEN  FOREGROUND_GREEN
    #define WIN_DARK_CYAN   FOREGROUND_GREEN | FOREGROUND_BLUE
    #define WIN_DARK_RED    FOREGROUND_RED
    #define WIN_DARK_MAGENTA FOREGROUND_RED | FOREGROUND_BLUE
    #define WIN_DARK_YELLOW FOREGROUND_RED | FOREGROUND_GREEN
    #define WIN_GRAY        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
    #define WIN_DARK_GRAY   FOREGROUND_INTENSITY
    #define WIN_BLUE        FOREGROUND_BLUE | FOREGROUND_INTENSITY
    #define WIN_GREEN       FOREGROUND_GREEN | FOREGROUND_INTENSITY
    #define WIN_CYAN        FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
    #define WIN_RED         FOREGROUND_RED | FOREGROUND_INTENSITY
    #define WIN_MAGENTA     FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY
    #define WIN_YELLOW      FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
    #define WIN_WHITE       FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
#endif

/*
 * cprintf: printf with foreground-only color support.
 * - color: one of the C_* macros above (const char*), or NULL for no color.
 * - fmt, ...: standard printf format and args.
 * Behavior: emits color escape before the formatted output and resets after.
 * If stdout is not a TTY, color codes are omitted.
 */
void cprintf(const char *color, const char *fmt, ...);

#endif /* COLOR_PRINTF_H */