#include "../include/color_printf.h"
#include <stdarg.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
    #include <io.h>
    #define isatty _isatty
    #define fileno _fileno
#else
    #include <unistd.h> /* isatty for POSIX */
#endif

/* Map ANSI color names to Windows console attributes */
#ifdef _WIN32
static WORD get_windows_color(const char *color)
{
    if (strcmp(color, C_BLACK) == 0) return WIN_BLACK;
    if (strcmp(color, C_RED) == 0) return WIN_RED;
    if (strcmp(color, C_GREEN) == 0) return WIN_GREEN;
    if (strcmp(color, C_YELLOW) == 0) return WIN_YELLOW;
    if (strcmp(color, C_BLUE) == 0) return WIN_BLUE;
    if (strcmp(color, C_MAGENTA) == 0) return WIN_MAGENTA;
    if (strcmp(color, C_CYAN) == 0) return WIN_CYAN;
    if (strcmp(color, C_WHITE) == 0) return WIN_WHITE;
    return WIN_WHITE; /* default */
}
#endif

void cprintf(const char *color, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    
    int use_color = 0;
    if (color != NULL && color[0] != '\0') {
        /* Only use color when stdout is a TTY */
        use_color = isatty(fileno(stdout));
    }
    
    if (use_color) {
#ifdef _WIN32
        /* Windows-specific color handling */
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        WORD savedAttributes;
        
        /* Save current text attributes */
        GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
        savedAttributes = consoleInfo.wAttributes;
        
        /* Set new color */
        SetConsoleTextAttribute(hConsole, get_windows_color(color));
        
        /* Print text */
        vprintf(fmt, ap);
        
        /* Restore original color */
        SetConsoleTextAttribute(hConsole, savedAttributes);
#else
        /* POSIX (Linux/macOS) - use ANSI escape codes */
        fputs(color, stdout);
        vfprintf(stdout, fmt, ap);
        fputs(C_RESET, stdout);
#endif
    } else {
        /* No color output */
        vfprintf(stdout, fmt, ap);
    }
    
    va_end(ap);
}