#include <nterm.h>
#include <stdarg.h>
#include <unistd.h>
#include <SDL.h>

char handle_key(SDL_Event *ev);
void extern_app_run(const char *app_path);

static void sh_printf(const char *format, ...) {
  static char buf[256] = {};
  va_list ap;
  va_start(ap, format);
  int len = vsnprintf(buf, 256, format, ap);
  va_end(ap);
  term->write(buf, len);
}

static void sh_banner() {
  sh_printf("Built-in Shell in NTerm (NJU Terminal)\n\n");
}

static void sh_prompt() {
  sh_printf("sh> ");
}


static void sh_handle_cmd(const char *cmd) {
    char c='\n';
    if(cmd==NULL || cmd==""+'\n'+'\0') return;
    printf("cmd: %s\n", cmd);
    char *file_buf = (char *)malloc((strlen(cmd) + 1) * sizeof(char));
    memset(file_buf, 0, strlen(cmd) + 1);
    file_buf=strncpy(file_buf,cmd,strlen(cmd)-1);
    execvp(file_buf, NULL);                 
    //assert(0);
   // execvp(file_buf, NULL);
    return;
}

void builtin_sh_run() {
  sh_banner();
  sh_prompt();

    setenv("PATH", "/bin", 0);
  while (1) {
    SDL_Event ev;
    if (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_KEYUP || ev.type == SDL_KEYDOWN) {
        const char *res = term->keypress(handle_key(&ev));
        if (res) {
          sh_handle_cmd(res);
          sh_prompt();
        }
      }
    }
    refresh_terminal();
  }
}
