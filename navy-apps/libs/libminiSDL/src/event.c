#include <NDL.h>
#include <SDL.h>
#include <string.h>
#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

int SDL_PushEvent(SDL_Event *ev) {
  return 0;
}




const int buf_size=32; //TODO
char buf[32];
int SDL_WaitEvent(SDL_Event *event) {
    while(!NDL_PollEvent(buf,buf_size));
    char *p=buf;
    if(strncmp(p,"kd",2)==0){
        event->type=SDL_KEYDOWN;
        p+=3;
    }
    else{
        event->type=SDL_KEYUP;
        p+=3;
    }
    int i=0;
    while(p[i]!='\n'){
        i++;
    }
    p[i]='\0';
    for(int j=0;j<sizeof(keyname)/sizeof(keyname[0]);j++){
        if(strcmp(p,keyname[j])==0){
            event->key.keysym.sym=j;
            break;
        }
    }
    return 1;
}
int SDL_PollEvent(SDL_Event *ev) {
    if(NDL_PollEvent(buf,buf_size)){
        char *p=buf;
        if(strncmp(p,"kd",2)==0){
            ev->type=SDL_KEYDOWN;
            p+=3;
        }
        else{
            ev->type=SDL_KEYUP;
            p+=3;
        }
        int i=0;
        while(p[i]!='\n'){
            i++;
        }
        p[i]='\0';
        for(int j=0;j<sizeof(keyname)/sizeof(keyname[0]);j++){
            if(strcmp(p,keyname[j])==0){
                ev->key.keysym.sym=j;
                break;
            }
        }
        return 1;
    }
    return 0;
}
int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

static unsigned char keystate[sizeof(keyname) / sizeof(keyname[0])];

uint8_t* SDL_GetKeyState(int *numkeys) {
    SDL_Event ev;
    if (SDL_PollEvent(&ev) == 1 && ev.key.type == SDL_KEYDOWN) {
        keystate[ev.key.keysym.sym] = 1;
    } else {
        memset(keystate, 0, sizeof(keystate));
    }
    return keystate;
}