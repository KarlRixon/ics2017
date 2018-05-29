#include "common.h"
#include "syscall.h"

//extern ssize_t fs_read(int fd, void *buf, size_t len);
//extern ssize_t fs_write(int fd, const void *buf, size_t len);
//extern int fs_open(const char *pathname, int flags, int mode);
//extern off_t fs_lseek(int fd, off_t offset, int whence);
//extern int fs_close(int fd);

static inline _RegSet* sys_none(_RegSet *r) {
	SYSCALL_ARG1(r) = 1;
	return NULL;
}

static inline _RegSet* sys_exit(_RegSet *r) {
	_halt(SYSCALL_ARG2(r));
	return NULL;
}

static inline _RegSet* sys_write(_RegSet *r) {
	Log("!");
	if(SYSCALL_ARG2(r) == 1 || SYSCALL_ARG2(r) == 2){
		for(int i = 0; i < SYSCALL_ARG2(r); i++) {
			_putc(((char*)SYSCALL_ARG3(r))[i]);
		}
	SYSCALL_ARG1(r) = SYSCALL_ARG4(r);
	}
	return NULL;
}

_RegSet* do_syscall(_RegSet *r) {
  uintptr_t a[4];
  a[0] = SYSCALL_ARG1(r);
  a[1] = SYSCALL_ARG2(r);
  a[2] = SYSCALL_ARG3(r);
  a[3] = SYSCALL_ARG4(r);

  switch (a[0]) {
	case SYS_none:
		return sys_none(r);
	case SYS_exit:
		return sys_exit(r);
	case SYS_write:
		return sys_write(r);

    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
