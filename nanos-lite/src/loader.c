#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

extern int fs_open(const char *pathname, int flags, int mode);
extern size_t fs_read(int fd, void *buf, size_t len);
extern size_t fs_write(int fd, const void *buf, size_t len);
extern size_t fs_lseek(int fd, size_t offset, int whence);
extern int fs_close(int fd);

extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
static uintptr_t loader(PCB *pcb, const char *filename) {
    Elf_Ehdr ehdr;
    int fd=fs_open(filename,0,0);
    fs_read(fd,&ehdr,sizeof(ehdr));
    assert(memcmp(ehdr.e_ident, ELFMAG, SELFMAG) == 0);

    Elf_Phdr phdr[ehdr.e_phnum];
    fs_lseek(fd,ehdr.e_phoff,0);
    fs_read(fd,phdr,ehdr.e_phnum*sizeof(Elf_Phdr));
    for (int i = 0; i < ehdr.e_phnum; i++) {
        if(phdr[i].p_type==PT_LOAD){
            fs_lseek(fd,phdr[i].p_offset,0);
            fs_read(fd,(void*)phdr[i].p_vaddr,phdr[i].p_filesz);
            memset((void*)phdr[i].p_vaddr+phdr[i].p_filesz,0,phdr[i].p_memsz-phdr[i].p_filesz);
        }
    }
    return ehdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

