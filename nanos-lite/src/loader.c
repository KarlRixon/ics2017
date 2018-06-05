#include "common.h"

#define DEFAULT_ENTRY ((void *)0x4000000)

extern void ramdisk_read(void *buf, off_t offset, size_t len);
extern size_t get_ramdisk_size();

void* new_page(void);

int fs_open(const char *pathname, int flags, int mode);
size_t fs_filesz(int fd);
ssize_t fs_read(int fd, void *buf, size_t len);
int fs_close(int fd);

uintptr_t loader(_Protect *as, const char *filename) {
  // TODO();
  //ramdisk_read(DEFAULT_ENTRY, 0, get_ramdisk_size());
  //return (uintptr_t)DEFAULT_ENTRY;

  int fd = fs_open(filename, 0, 0);
  fs_read(fd, 0, fs_filesz(fd));

  //int fd = fs_open(filename, 0, 0);
  //int i, pages = fs_filesz(fd) / PGSIZE + 1;
  //void *pa, *va;
		  
  //for (i = 0, va = DEFAULT_ENTRY; i < pages; ++i, va += PGSIZE) {
//	  pa = new_page();
//	  printf("pa: %x, va: %x", pa, va);
//	  printf(", read: %d\n", fs_read(fd, pa, PGSIZE));
//	  fs_read(fd, pa, PGSIZE);
//	  _map(as, va, pa);
  //}

  fs_close(fd);
  return (uintptr_t)DEFAULT_ENTRY;
}
