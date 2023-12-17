#include <stdio.h>
#include <assert.h>
#include <sys/time.h>

int main() {
  struct timeval tv;
  // struct timezone tz;
  gettimeofday(&tv, NULL);
  __uint64_t ms = 1000;
  printf("ms = %ld sec=%ld usec=%ld \n", ms,tv.tv_sec,tv.tv_usec);
  while (1) {
    while ((tv.tv_sec * 1000000 + tv.tv_usec ) < ms*1000) {
      gettimeofday(&tv, NULL);
    }
    ms += 1001;
    printf("ms = %d sec=%d usec=%d \n", ms,tv.tv_sec,tv.tv_usec);
  }
}
