#include <stdio.h>

typedef struct{
    char name[50];
    int power;        // cong suat(W)
    int priority;     // 1 (cao) -> 5 (thap)
    int state;        // 0: OFF, 1: ON
    int mode;         // 0: nomal, 1: eco
}Device;
typedef struct{
    float battery;    // %
    int charging;     // 1: dang sac, 0: dang xa
    float total_power;// tong cong suat
}System;


int main() {
    printf("Hello GitHub!\n");
    return 0;
}