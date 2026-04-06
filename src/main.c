#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "device.h"
#include "system.h"
#include "user_sim.h"
#include "logger.h"

// IN TRẠNG THÁI 
void print_status(int hour, System *sys, Device devices[], int count) {

    printf("\n===== HOUR %d =====\n", hour);
    
    float battery_percent = 0.0;
    if (sys->capacity > 0) {
        battery_percent = (sys->battery / sys->capacity) * 100;
    }
    printf("Battery: %.2f (%.1f%%)\n", sys->battery, battery_percent);
    printf("Mode: %d\n", sys->mode);
    printf("Total Power: %.2f\n", sys->totalPower);
    printf("Warnings: %d\n", sys->warningCount);

    for (int i = 0; i < count; i++) {
        printf("%s | %.2fW | P%d | %s\n",
               devices[i].name,
               devices[i].power,
               devices[i].priority,
               devices[i].state == DEVICE_ON ? "ON" : "OFF");
    }
}

// MAIN
int main() {
    
    initLogger();
    srand(time(NULL));

    //TẠO DEVICE
    Device devices[30];
    int deviceCount = 0;
    loadDevices(devices, &deviceCount, 30);

    if (deviceCount == 0) {
        printf("Khong co thiet bi!\n");
        return 1;
    }

    // TẠO SYSTEM
    System sys;
    sys.warningCount = 0;
    sys.totalPower = 0;

    if (!load_system(&sys)) {
        printf("Khong mo duoc file system.txt\n");
        return 1; // kết thúc chương trình
    }

    // MÔ PHỎNG 24H
    int TOTAL_HOURS = 24;
    for (int hour = 1; hour <= TOTAL_HOURS; hour++) {

        // 1. user tác động
        simulate_user_actions(devices, deviceCount);

        // 2. system control
        control_devices(devices, deviceCount, &sys);

        // 3. update battery
        update_battery(&sys);

        // 4. mỗi 4 giờ thì sạc
        if (hour % 4 == 0) {
            charge_battery(&sys);
        }

        // 5. in trạng thái
        print_status(hour, &sys, devices, deviceCount);
    }

    // Tinh toan thiet bi tieu thu nhieu dien nhat
    float max_energy = -1.0;
    int max_idx = -1;
    for (int i = 0; i < deviceCount; i++) {
        if (devices[i].total_energy >= max_energy) {
            max_energy = devices[i].total_energy;
            max_idx = i;
        }
    }

    // In bao cao ra man hinh
    printf("\n=== BAO CAO TONG KET ===\n");
    printf("- Tong thoi gian hoat dong: %d gio\n", TOTAL_HOURS);
    printf("- Tong nang luong tieu thu: %.2f W\n", sys.total_energy_consumed);
    printf("- So lan chuyen che do: %d\n", sys.mode_change_count);
    if (max_idx != -1) {
        printf("- Thiet bi tieu thu nhieu dien nhat: %s (%.2f W)\n", devices[max_idx].name, max_energy);
    }
    printf("- Tong so lan canh bao: %d\n", sys.warningCount);
    printf("========================\n\n");

    // Xuat bao cao ra file txt
    FILE *f_report = fopen("baocao.txt", "w");
    if (f_report != NULL) {
        fprintf(f_report, "=== BAO CAO TONG KET ===\n");
        fprintf(f_report, "- Tong thoi gian hoat dong: %d gio\n", TOTAL_HOURS);
        fprintf(f_report, "- Tong nang luong tieu thu: %.2f W\n", sys.total_energy_consumed);
        fprintf(f_report, "- So lan chuyen che do: %d\n", sys.mode_change_count);
        if (max_idx != -1) {
            fprintf(f_report, "- Thiet bi tieu thu nhieu dien nhat: %s (%.2f W)\n", devices[max_idx].name, max_energy);
        }
        fprintf(f_report, "- Tong so lan canh bao: %d\n", sys.warningCount);
        fprintf(f_report, "========================\n");
        fclose(f_report);
        printf(">> Da xuat ket qua luu vao file 'baocao.txt' thanh cong\n");
    }

    return 0;
}