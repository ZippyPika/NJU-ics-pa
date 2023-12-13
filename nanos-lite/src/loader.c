#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

extern size_t ramdisk_read(void *buf, size_t offset, size_t len);

static uintptr_t loader(PCB *pcb, const char *filename) {
    Elf_Ehdr ehdr;
    ramdisk_read(&ehdr, 0, sizeof(ehdr));
    assert(memcmp(ehdr.e_ident, ELFMAG, SELFMAG) == 0);

    Elf_Phdr phdr;
    for (int i = 0; i < ehdr.e_phnum; i++) {
        // 计算 Program Header 的位置并读取它
        ramdisk_read(&phdr, ehdr.e_phoff + i * ehdr.e_phentsize, sizeof(phdr));
        if(phdr.p_type==PT_LOAD){
            memset((void*)phdr.p_vaddr+phdr.p_filesz,0,phdr.p_memsz-phdr.p_filesz);
        }
    }
    assert(0);
    return ehdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

