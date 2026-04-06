#include <stdio.h>
#include <string.h>
#include "system.h"
#include "logger.h"

// Lấy thông tin System
int load_system(System *sys) {
    FILE *f = fopen("docs/system.txt", "r");

    if (f == NULL) {
        return 0;
    }

    sys->mode_change_count = 0;
    sys->total_energy_consumed = 0;

    char key[50];
    float value;

    while (fscanf(f, "%49[^=]=%f\n", key, &value) == 2) {

        if (strcmp(key, "battery") == 0) {
            sys->battery = value;
            sys->capacity = value;
        }
        else if (strcmp(key, "max_power") == 0) {
            sys->maxPower = value;
        }
    }

    fclose(f);
    return 1;
}

// CẬP NHẬT PIN
void update_battery(System *sys) {

    // giả sử mỗi vòng là 1 giờ
    float consumption = sys->totalPower;

    sys->battery -= consumption;

    // không cho âm
    if (sys->battery < 0) {
        sys->battery = 0;
    }
}

// SẠC PIN
void charge_battery(System *sys) {

    printf(">>> Charging...\n");

    sys->battery += 0.2*(sys->capacity); 

    if (sys->battery > sys->capacity) {
        sys->battery = sys->capacity;
    }
}

// TÍNH TỔNG CÔNG SUẤT
float calculate_total_power(Device devices[], int count) {

    float total = 0;
    for (int i = 0; i < count; i++) {
        if (devices[i].state == DEVICE_ON) {
            total += devices[i].power;
        }
    }
    return total;
}


// ĐIỀU KHIỂN THIẾT BỊ
void control_devices(Device devices[], int count, System *sys) {
    if (sys->capacity <= 0) return;

    int old_mode = sys->mode;

    // XÁC ĐỊNH MODE THEO LƯỢNG PIN
    float battery_percent = (sys->battery/sys->capacity)*100;
    if (battery_percent > 60) {
        sys->mode = MODE_HIGH;
    } else if (battery_percent > 30) {
        sys->mode = MODE_MEDIUM;
    } else if (battery_percent > 10) {
        sys->mode = MODE_LOW;
    } else {
        sys->mode = MODE_CRITICAL;
    }

    if (old_mode != sys->mode) {
        logEventF("WARNING", "Che do cua System thay doi: %d -> %d", old_mode, sys->mode);
        sys->mode_change_count++;
        
        if (sys->mode == MODE_CRITICAL) {
            logEventF("CRITICAL", "Muc pin nguy hiem!");
        }
    }

    // ĐIỀU CHỈNH THEO MODE
    for (int i = 0; i < count; i++) {

        // MODE_HIGH → hoạt động bình thường
        if (sys->mode == MODE_MEDIUM) {
            if (devices[i].priority == PRIORITY_OPTIONAL && devices[i].state == DEVICE_ON) {
                devices[i].state = DEVICE_OFF;
                logEventF("INFO", "System: Tat ep buoc thiet bi [%s] do an toan pin", devices[i].name);
            }
        }
        else if (sys->mode == MODE_LOW) {
            if (devices[i].priority >= PRIORITY_NORMAL && devices[i].state == DEVICE_ON) {
                devices[i].state = DEVICE_OFF;
                logEventF("INFO", "System: Tat ep buoc thiet bi [%s] do an toan pin", devices[i].name);
            }
        }
        else if (sys->mode == MODE_CRITICAL) {
            if (devices[i].priority != PRIORITY_ESSENTIAL && devices[i].state == DEVICE_ON) {
                devices[i].state = DEVICE_OFF;
                logEventF("INFO", "System: Tat ep buoc thiet bi [%s] do an toan pin", devices[i].name);
            }
        }
    }

    // KIỂM TRA NGƯỠNG CÔNG SUẤT
    float total = calculate_total_power(devices, count);

    if (total > (sys->maxPower)) {

        sys->warningCount++;
        logEventF("WARNING", "Vuot dung luong MaxPower. Canh bao tang len %d", sys->warningCount);

        // tắt dần từ priority thấp nhất đến khi dưới ngưỡng
        int done = 0;

        for (int p = PRIORITY_OPTIONAL; p > PRIORITY_ESSENTIAL && !done; p--) {
            for (int i = 0; i < count; i++) {
                if (devices[i].priority == p && devices[i].state == DEVICE_ON) {
                    devices[i].state = DEVICE_OFF;
                    logEventF("INFO", "System: Tat ep buoc thiet bi [%s] do qua tai", devices[i].name);
                    total -= devices[i].power;

                    if (total <= sys->maxPower) {
                        done = 1;
                        break;
                    }
                }
            }
        }
    }

    sys->totalPower = calculate_total_power(devices, count);
    
    // Cộng dồn điện tiêu thụ chu kỳ
    sys->total_energy_consumed += sys->totalPower;
    for (int i = 0; i < count; i++) {
        if (devices[i].state == DEVICE_ON) {
            devices[i].total_energy += devices[i].power;
        }
    }
}