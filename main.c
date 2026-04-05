#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "device.h"
#include "system.h"
#include "logger.h"

int main() {
    Device devices[30];
    int n;

    System sys;
    sys.battery = 100;
    sys.warningCount = 0;

    float totalEnergy = 0;
    int modeChanges = 0;
    int prevMode = -1;

    srand(time(NULL));

    loadDevices(devices, &n);

    if (n == 0) {
        printf("Khong co thiet bi!\n");
        return 0;
    }

    printf("=== BAT DAU MO PHONG ===\n");

    char *modeName[] = {"HIGH", "MEDIUM", "LOW", "CRITICAL"};

    for (int t = 0; t < 20; t++) {
        printf("\nTime %d\n", t);

        updateSystem(&sys, devices, n);

        if (sys.mode != prevMode) {
            modeChanges++;
            prevMode = sys.mode;
        }

        controlDevices(&sys, devices, n);
        checkWarning(&sys, devices, n);

        printf("Battery: %.2f\n", sys.battery);
        printf("Total Power: %.2f\n", sys.totalPower);
        printf("Mode: %s\n", modeName[sys.mode]);

        printf("\n%-10s %-10s %-10s %-10s\n", "Ten", "Power", "Priority", "State");
        printf("--------------------------------------------------\n");

        for (int i = 0; i < n; i++) {
            if (rand() % 10 < 2) {
                devices[i].state = !devices[i].state;
            }

            printf("%-10s %-10d %-10d %-10s\n",
                devices[i].name,
                devices[i].power,
                devices[i].priority,
                devices[i].state ? "ON" : "OFF");
        }

        totalEnergy += sys.totalPower;

        if (t % 5 == 0 && t != 0) {
            sys.battery += 10;
            if (sys.battery > 100) sys.battery = 100;
            logEvent("INFO", "Dang sac pin");
        }

        sys.battery -= sys.totalPower * 0.001;
        if (sys.battery < 0) sys.battery = 0;
    }

    int maxIdx = 0;
    for (int i = 1; i < n; i++) {
        if (devices[i].power > devices[maxIdx].power)
            maxIdx = i;
    }

    printf("\n=== BAO CAO ===\n");
    printf("So thiet bi: %d\n", n);
    printf("Tong nang luong tieu thu: %.2f\n", totalEnergy);
    printf("So lan chuyen mode: %d\n", modeChanges);
    printf("Thiet bi tieu thu nhieu nhat: %s\n", devices[maxIdx].name);
    printf("So lan canh bao: %d\n", sys.warningCount);

    logEvent("INFO", "Ket thuc mo phong");

    return 0;
}