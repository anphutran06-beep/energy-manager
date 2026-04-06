#include <stdio.h>
#include "device.h"

void loadDevices(Device devices[], int* deviceCount, int max_size) {
    FILE *f = fopen("docs/devices.txt", "r");

    if (f == NULL) {
        printf("Khong mo duoc file config.txt\n");
        *deviceCount = 0;
        return;
    }

    fscanf(f, "%d\n", deviceCount);
    if (*deviceCount > max_size) {
        *deviceCount = max_size;
    }

    for (int i = 0; i < *deviceCount; i++) {
        fscanf(f, "%49s %f %d",
               devices[i].name,
               &devices[i].power,
               &devices[i].priority);

        devices[i].state = DEVICE_ON;
        devices[i].total_energy = 0;
    }

    fclose(f);
}