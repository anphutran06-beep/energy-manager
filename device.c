#include <stdio.h>
#include "device.h"

#include <stdio.h>
#include "device.h"

void loadDevices(Device d[], int *n) {
    FILE *f = fopen("config.txt", "r");

    if (f == NULL) {
        printf("Khong mo duoc file config.txt\n");
        *n = 0;
        return;
    }

    fscanf(f, "%d\n", n); // 👈 QUAN TRỌNG (ăn luôn newline)

    for (int i = 0; i < *n; i++) {
        fscanf(f, "%s %d %d",
               d[i].name,
               &d[i].power,
               &d[i].priority);

        d[i].state = 1;
    }

    fclose(f);
}