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
        int phoff = ehdr.e_phoff + i * ehdr.e_phentsize;
        ramdisk_read(&phdr, phoff, sizeof(phdr));

        // 打印 segment 的信息
        printf("Segment %d: type=%d, offset=0x%x, vaddr=0x%x, paddr=0x%x, filesz=%d, memsz=%d\n",
               i, phdr.p_type, phdr.p_offset, phdr.p_vaddr, phdr.p_paddr, phdr.p_filesz, phdr.p_memsz);
    }
    assert(0);
    return 0;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

