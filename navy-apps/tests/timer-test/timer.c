#include <stdio.h>

#define HAS_NDL

#ifndef HAS_NDL
#include <assert.h>
#include <sys/time.h>

void gettimeofday_test() {
    printf("gettimeofday test start...\n");

    struct timeval init;
    struct timeval now;

    assert(gettimeofday(&init, NULL) == 0);
    time_t init_sec = init.tv_sec;
    suseconds_t init_usec = init.tv_usec;

    size_t times = 1;

    while (1) {
        assert(gettimeofday(&now, NULL) == 0);
        time_t now_sec = now.tv_sec;
        suseconds_t now_usec = now.tv_usec;
        uint64_t time_gap = (now_sec - init_sec) * 1000000 + (now_usec - init_usec); // unit: us
        if (time_gap > 500000 * times) {
            printf("Half a second passed, %u time(s)\n", times);
            times++;
        }
    }
}
#else
#include <NDL.h>

void NDL_GetTicks_test() {
	NDL_Init(0);
    printf("NDL_GetTicks test start...\n");

    uint32_t init = NDL_GetTicks();
    size_t times = 1;

    while (1) {
        uint32_t now = NDL_GetTicks();
        uint32_t time_gap = now - init;
        if (time_gap > 500 * times) {
            printf("Half a second passed, %u time(s)\n", times);
            times++;
        }
    }
    NDL_Quit();
}
#endif

int main() {
#ifndef HAS_NDL
    gettimeofday_test();
#else
    NDL_GetTicks_test();
#endif
}