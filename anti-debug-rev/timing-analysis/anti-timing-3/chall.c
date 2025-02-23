#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#define VARIANCE_THRESHOLD 1e-8
#define MAX_TIME_DIFF 300

typedef struct {
    char *flag;
    int debug_detected;
    double timing_samples[10];
} TimingProtectedChallenge;

void xor_encode(char *str, int len) {
    for (int i = 0; i < len; i++) {
        str[i] ^= 0x33;
    }
}

double precise_timing() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

double precise_timing() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
}

double timing_check(TimingProtectedChallenge *challenge) {
    double samples[10];
    for (int i = 0; i < 10; i++) {
        double start = precise_timing();

        int sum = 0;
        for (int j = 0; j < 1000; j++) {
            sum += j * j;
        }
        double end = precise_timing();
        samples[i] = end - start;
    }


    double avg = 0;
    for (int i = 0; i < 10; i++) {
        avg += samples[i];
    }
    avg /= 10;

    double variance = 0;
    for (int i = 0; i < 10; i++) {
        variance += (samples[i] - avg) * (samples[i] - avg);
    }
    variance /= 10;


    if (variance > VARIANCE_THRESHOLD) {
        challenge->debug_detected = 1;
    }

    memcpy(challenge->timing_samples, samples, sizeof(samples));
    return variance;
}

void slow_print(const char *text) {
    for (int i = 0; i < strlen(text); i++) {
        putchar(text[i]);
        fflush(stdout);
        usleep(200000);  
    }
}

void reveal_flag(TimingProtectedChallenge *challenge) {
    if (challenge->debug_detected) {
        char fake_flag[] = "CTF{n1c3_try_d3bugg3r}";
        slow_print(fake_flag);
        return;
    }
    double base_time = precise_timing();

    for (int i = 0; i < strlen(challenge->flag); i++) {
        double current_time = precise_timing();
        double time_diff = current_time - base_time;
        if (time_diff < 0 || time_diff > MAX_TIME_DIFF) {
            printf("\nTime manipulation detected!");
            return;
        }
        char c = challenge->flag[i] ^ 0x33;
        putchar(c);
        fflush(stdout);
        usleep(100000 + rand() % 200000);  
    }
}

int main() {
    char real_flag[] = "CTF{t1m1ng_4n4ly515_15_k3y}";
    xor_encode(real_flag, strlen(real_flag));

    TimingProtectedChallenge challenge;
    challenge.flag = real_flag;
    challenge.debug_detected = 0;
    double variance = timing_check(&challenge);
    printf("Revealing flag...\n");
    reveal_flag(&challenge);
    printf("\n");

    return 0;
}
