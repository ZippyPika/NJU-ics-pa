#include <stdio.h>
#include <assert.h>
#include <sys/time.h>

int main() {
  struct timeval tv;
  struct timeval st;
  // struct timezone tz;
  gettimeofday(&st, NULL);
  __uint64_t ms = 500;
  gettimeofday(&tv, NULL);
  while (1) {
    while (((tv.tv_sec-st.tv_sec) * 1000 + (tv.tv_usec-st.tv_usec) / 1000) < ms) {
      gettimeofday(&tv, NULL);
    }
    st=tv;
    //ms += 500;
    printf("ms = %ld sec=%ld usec=%ld \n", ms,tv.tv_sec,tv.tv_usec);
  }
}
