#include <fs.h>

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);
extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
  size_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};
static int FD_MAX = 4;
size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, invalid_write},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, invalid_write},
#include "files.h"
};

int fs_open(const char *pathname, int flags, int mode){
    for(int i=0;i<=FD_MAX;i++){
        if(strcmp(pathname,file_table[i].name)==0){
            file_table[i].open_offset=0;
            return i;
        }
    }
    panic("file %s not found",pathname);
    return -1;
}
int fs_close(int fd){
    return 0;
}
size_t fs_read(int fd, void *buf, size_t len){
    if(fd<=2){
        Log("read from stdin/stdout/stderr");
        return 0;
    }
    //assert(file_table[fd].open_offset+len<=file_table[fd].size);
    if(file_table[fd].open_offset+len>file_table[fd].size){
        Log("read beyond file size");
        return -1;
    }
    ramdisk_read(buf,file_table[fd].disk_offset+file_table[fd].open_offset,len);
    file_table[fd].open_offset+=len;
    return len;
}
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence){
    if(fd<=2){
        Log("lsee from stdin/stdout/stderr");
        return 0;
    }
    switch(whence){
        case SEEK_SET:file_table[fd].open_offset=offset;break;
        case SEEK_CUR:file_table[fd].open_offset+=offset;break;
        case SEEK_END:file_table[fd].open_offset=file_table[fd].size+offset;break;
        default:assert(0);
    }
    if(file_table[fd].open_offset>file_table[fd].size){
        Log("lseek beyond file size");
        return -1;
    }
    if(file_table[fd].open_offset<0){
        Log("lseek before file start");
        return -1;
    }
    return file_table[fd].open_offset;
}


void init_fs() {
  // TODO: initialize the size of /dev/fb
  FD_MAX = sizeof(file_table) / sizeof(file_table[0]);
}
