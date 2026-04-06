#ifndef LOGGER_H
#define LOGGER_H

void initLogger();
void logEvent(char *level, char *msg);
void logEventF(const char *level, const char *format, ...);

#endif