#ifndef DEVICE_H
#define DEVICE_H

typedef struct{
    char name[50];
    int power;
    int priority;
    int state;
}Device;

void loadDevices(Device d[], int* n);

#endif