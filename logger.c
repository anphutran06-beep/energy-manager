#include <stdio.h>
#include <time.h>
#include "logger.h"

void logEvent(char *level, char *msg) {
    FILE *f = fopen("log.txt", "a");

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    fprintf(f, "[%02d:%02d:%02d] [%s] %s\n",
        tm->tm_hour, tm->tm_min, tm->tm_sec,
        level, msg);

    fclose(f);
}