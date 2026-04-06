#include <stdlib.h>
#include <time.h>
#include "user_sim.h"
#include "logger.h"

// mô phỏng hành vi người dùng
void simulate_user_actions(Device devices[], int count) {

    for (int i = 0; i < count; i++) {
        // giữ cho thiết bị thiết yếu luôn bật
        if (devices[i].priority== PRIORITY_ESSENTIAL){
            continue;
        }
        if (rand() % 10 < 2) {

            if (devices[i].state == DEVICE_ON) {
                devices[i].state = DEVICE_OFF;
                logEventF("INFO", "User: Thiet bi [%s] tat", devices[i].name);
            } else {
                devices[i].state = DEVICE_ON;
                logEventF("INFO", "User: Thiet bi [%s] bat", devices[i].name);
            }
        }
    }
}