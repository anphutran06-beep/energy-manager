#include <stdio.h>
#include "system.h"
#include "logger.h"


// 🔥 TÍNH TỔNG CÔNG SUẤT
float calculate_total_power(Device devices[], int count) {
    float total = 0;

    for (int i = 0; i < count; i++) {
        if (devices[i].state == DEVICE_ON) {
            total += devices[i].power;
        }
    }

    return total;
}


// 🔥 ĐIỀU KHIỂN HỆ THỐNG
void control_devices(Device devices[], int count, System *sys) {

    // ✅ 1. RESET trạng thái (quan trọng)
    for (int i = 0; i < count; i++) {
        devices[i].state = DEVICE_ON;
    }

    // ✅ 2. XÁC ĐỊNH MODE theo pin
    if (sys->battery > 60) {
        sys->mode = MODE_HIGH;
    }
    else if (sys->battery > 30) {
        sys->mode = MODE_MEDIUM;
    }
    else if (sys->battery > 10) {
        sys->mode = MODE_LOW;
    }
    else {
        sys->mode = MODE_CRITICAL;
    }

    // ✅ 3. ĐIỀU KHIỂN THEO MODE
    for (int i = 0; i < count; i++) {

        // MODE_MEDIUM → tắt thiết bị không quan trọng
        if (sys->mode == MODE_MEDIUM) {
            if (devices[i].priority == PRIORITY_OPTIONAL) {
                devices[i].state = DEVICE_OFF;
            }
        }

        // MODE_LOW → chỉ giữ thiết bị thiết yếu
        else if (sys->mode == MODE_LOW) {
            if (devices[i].priority != PRIORITY_ESSENTIAL) {
                devices[i].state = DEVICE_OFF;
            }
        }

        // MODE_CRITICAL → chỉ giữ thiết bị thiết yếu (nghiêm ngặt)
        else if (sys->mode == MODE_CRITICAL) {
            if (devices[i].priority != PRIORITY_ESSENTIAL) {
                devices[i].state = DEVICE_OFF;
            }
        }
    }

    // ✅ 4. TÍNH công suất sau khi điều khiển
    float total = calculate_total_power(devices, count);

    // ✅ 5. KIỂM TRA QUÁ TẢI
    if (total > MAX_POWER) {

        logEvent("WARNING", "Vuot nguong cong suat!");
        sys->warningCount++;

        // 🔥 Tắt dần từ thiết bị ít quan trọng nhất
        for (int p = PRIORITY_OPTIONAL; p >= PRIORITY_ESSENTIAL; p--) {
            for (int i = 0; i < count; i++) {

                if (devices[i].priority == p &&
                    devices[i].state == DEVICE_ON) {

                    devices[i].state = DEVICE_OFF;
                    total -= devices[i].power;

                    // log lại
                    char msg[100];
                    sprintf(msg, "Tat thiet bi: %s", devices[i].name);
                    logEvent("INFO", msg);

                    if (total <= MAX_POWER) {
                        sys->totalPower = total;
                        return;
                    }
                }
            }
        }
    }

    // ✅ 6. CẬP NHẬT công suất cuối
    sys->totalPower = total;
}