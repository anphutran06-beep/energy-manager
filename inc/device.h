#ifndef DEVICE_H
#define DEVICE_H

// DEVICE STATE 
#define DEVICE_OFF 0
#define DEVICE_ON  1

// PRIORITY
#define PRIORITY_ESSENTIAL 1
#define PRIORITY_IMPORTANT 2
#define PRIORITY_NORMAL    3
#define PRIORITY_OPTIONAL  4

typedef struct{
    char name[50];
    float power;
    int priority;
    int state;
    float total_energy;
}Device;

void loadDevices(Device devices[], int* deviceCount, int max_size);

#endif