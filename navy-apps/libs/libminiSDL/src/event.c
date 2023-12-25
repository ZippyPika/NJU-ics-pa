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




const int buf_size=1024; //TODO
char buf[1024];
int SDL_WaitEvent(SDL_Event *event) {
    while(SDL_PollEvent(event));
    // while(!NDL_PollEvent(buf,buf_size));
    // char *p=buf;
    // if(strncmp(p,"kd",2)==0){
    //     event->type=SDL_KEYDOWN;
    //     p+=3;
    // }
    // else{
    //     event->type=SDL_KEYUP;
    //     p+=3;
    // }
    // int i=0;
    // while(p[i]!='\n'){
    //     i++;
    // }
    // p[i]='\0';
    // for(int j=0;j<sizeof(keyname)/sizeof(keyname[0]);j++){
    //     if(strcmp(p,keyname[j])==0){
    //         event->key.keysym.sym=j;
    //         break;
    //     }
    // }
    // return 1;
}

static unsigned char keystate[sizeof(keyname) / sizeof(keyname[0])];

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
        keystate[ev->key.keysym.sym] = ev->key.type == SDL_KEYDOWN;
        return 1;
    }
    return 0;
}
int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}


uint8_t* SDL_GetKeyState(int *numkeys) {
    return keystate;
}