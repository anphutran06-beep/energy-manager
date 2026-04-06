#ifndef SYSTEM_H
#define SYSTEM_H

// SYSTEM MODE
#define MODE_HIGH     0
#define MODE_MEDIUM   1
#define MODE_LOW      2
#define MODE_CRITICAL 3

#include "device.h"

typedef struct {
    float battery;     // pin hiện tại
    float capacity;    // tổng dung lượng pin
    float totalPower;  // công xuát tổng của các thiết bị
    float maxPower;    // công suất tối đa của hệ thống
    int warningCount;
    int mode;
    int mode_change_count;
    float total_energy_consumed;
} System;

int load_system(System *sys);
void update_battery(System *sys);
void charge_battery(System *sys);
float calculate_total_power(Device devices[], int count);
void control_devices(Device devices[], int count, System *sys);

#endif