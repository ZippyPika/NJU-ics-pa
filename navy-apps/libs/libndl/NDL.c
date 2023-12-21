#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/time.h>
static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;
static int convas_w = 0, convas_h = 0;
struct timeval tv_start;

int event_fd;
uint32_t NDL_GetTicks() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec - tv_start.tv_sec) * 1000 + (tv.tv_usec - tv_start.tv_usec) / 1000;
}

int NDL_PollEvent(char *buf, int len) {
    int ret=read(event_fd,buf,len);
    return ret;
}

void NDL_OpenCanvas(int *w, int *h) {
    if (getenv("NWM_APP")) {
        int fbctl = 4;
        fbdev = 5;
        screen_w = *w; screen_h = *h;
        char buf[64];
        int len = sprintf(buf, "%d %d", screen_w, screen_h);
        // let NWM resize the window and create the frame buffer
        write(fbctl, buf, len);
        while (1) {
            // 3 = evtdev
            int nread = read(3, buf, sizeof(buf) - 1);
            if (nread <= 0) continue;
            buf[nread] = '\0';
            if (strcmp(buf, "mmap ok") == 0) break;
        }
        close(fbctl);
    }
    if(*w==0 && *h==0){
        *w=screen_w;
        *h=screen_h;
    }
    convas_w=*w;
    convas_h=*h;
    printf("screen size %d %d\n",*w,*h);
    return ;
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
    int fd=open("/dev/fb",0,0);
    printf("draw rect %d %d %d %d\n",x,y,w,h);
    for(int i=0;i<h && y+i<convas_h;i++)
    {
        lseek(fd,(y+i)*screen_w+x,SEEK_SET);
        write(fd,pixels+i*w,w*4);
    }
    close(fd);
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

static void init_vga(){
    int buf_size=1024;
    char buf[buf_size];
    int fd=open("/proc/dispinfo",0,0);
    int ret=read(fd,buf,buf_size);
    //assert(p);
    //printf("1111\n");
    //printf("%s\n",buf);
    assert(ret<buf_size);
    close(fd);
    int i=0;
    int w=0,h=0;
    assert(strncmp(buf,"WIDTH:",6)==0);
    i=6;
    while(buf[i]!='\n'){
        w=w*10+buf[i]-'0';
        i++;
    }

    assert(strncmp(buf+i+1,"HEIGHT:",7)==0);
    i+=8;
    while(buf[i]!='\n'){
        h=h*10+buf[i]-'0';
        i++;
    }
    screen_w=w;
    screen_h=h;
}
int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
    event_fd=open("/dev/events",0,0);
    gettimeofday(&tv_start, NULL);
    init_vga();
  return 0;
}

void NDL_Quit() {
    close(event_fd);
    return;
}
