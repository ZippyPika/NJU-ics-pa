#include <stdio.h>
#include <assert.h>
#include <sys/time.h>

int main() {
  struct timeval tv;
  // struct timezone tz;
  gettimeofday(&tv, NULL);
  __uint64_t ms = 500;
  while (1) {
    while ((tv.tv_sec * 1000 + tv.tv_usec / 1000) < ms) {
      gettimeofday(&tv, NULL);
    }
    //ms += 500;
    printf("ms = %lld sec=%ld usec=%ld \n", ms,tv.tv_sec,tv.tv_usec);
  }
}
