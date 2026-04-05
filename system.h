#ifndef SYSTEM_H
#define SYSTEM_H

#include "device.h"

typedef struct {
    float battery;
    float totalPower;
    int warningCount;
    int mode; // 0: high, 1: medium, 2: low, 3: critical
} System;

void updateSystem(System *sys, Device d[], int n);
void controlDevices(System *sys, Device d[], int n);
void checkWarning(System *sys, Device d[], int n);

#endif