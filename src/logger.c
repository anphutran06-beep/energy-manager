#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include "logger.h"

void initLogger() {
    FILE *f = fopen("log.txt", "w");
    if (f) fclose(f);
}

void logEvent(char *level, char *msg) {
    FILE *f = fopen("log.txt", "a");

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    fprintf(f, "[%02d:%02d:%02d] [%s] %s\n",
        tm->tm_hour, tm->tm_min, tm->tm_sec,
        level, msg);

    fclose(f);
}

void logEventF(const char *level, const char *format, ...) {
    FILE *f = fopen("log.txt", "a");
    if (!f) return;

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    fprintf(f, "[%02d:%02d:%02d] [%s] ",
        tm->tm_hour, tm->tm_min, tm->tm_sec,
        level);

    va_list args;
    va_start(args, format);
    vfprintf(f, format, args);
    va_end(args);

    fprintf(f, "\n");
    fclose(f);
}