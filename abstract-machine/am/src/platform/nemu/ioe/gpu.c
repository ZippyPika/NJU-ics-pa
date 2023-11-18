#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
  int i;
  int w = 400;  // TODO: get the correct width
  int h = 300;  // TODO: get the correct height
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < w * h; i ++) fb[i] = i;
  outl(SYNC_ADDR, 1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
    uint32_t x=inl(VGACTL_ADDR);
    uint16_t h=x&0xffff;
    uint16_t w=(x>>16);
    *cfg = (AM_GPU_CONFIG_T) {
        .present = true, .has_accel = false,
        .width = w, .height = h,
        .vmemsz = 0
    };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
    int x=ctl->x,y=ctl->y,h=ctl->h,w=ctl->w;
    uint32_t *pixels=ctl->pixels;
    uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
    int i,j;
    int all_w=inl(VGACTL_ADDR)>>16;
    for(i=0;i<h;i++){
        for(j=0;j<w;j++){
            fb[(y+i)*all_w+x+j]=pixels[i*w+j];
        }
    }
    if (ctl->sync) {
        outl(SYNC_ADDR, 1);
    }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
