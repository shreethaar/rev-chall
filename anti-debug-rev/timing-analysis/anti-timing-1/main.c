#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

unsigned long get_time_microseconds() {
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC_RAW, &ts) != 0) {
        perror("clock_gettime");
        return 0;
    }
    return ts.tv_sec * 1000000UL + ts.tv_nsec / 1000;
}

int is_numeric(const char *str) {
    for (size_t i = 0; i < strlen(str); ++i) {
        if (str[i] < '0' || str[i] > '9') {
            return 0;
        }
    }
    return 1;
}

int main() {
    printf("Hello, World!\n");

    unsigned long begin = 0;

    while (1) {
        begin = get_time_microseconds();

        DIR *dir = opendir("/proc");
        if (dir == NULL) {
            perror("opendir");
            return 1;
        }

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (is_numeric(entry->d_name)) {
                printf("%s ", entry->d_name);
                fflush(stdout);
            }
        }
        closedir(dir);

        usleep(1000000); // sleep for 1 second

        unsigned long end = get_time_microseconds();
        unsigned long duration = end - begin;

        if (duration > 1010000) {
            fprintf(stderr, "Debugging attempt detected (%lu us)\n", duration);
            break;
        }
    }

    return 0;
}
