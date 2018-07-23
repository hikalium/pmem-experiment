#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <sys/io.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>

#define PFN_MASK_SIZE 8

void *virt2phy(const void *virtaddr) {
  // http://mmi.hatenablog.com/entry/2017/05/01/215921
  unsigned long page, physaddr, virt_pfn;
  off_t offset;
  int page_size = sysconf(_SC_PAGESIZE);
  int fd, retval;

  printf("page_size = 0x%X\n", page_size);

  fd = open("/proc/self/pagemap", O_RDONLY);
  if (fd < 0) {
    printf("error: open");
    return NULL;
  }

  virt_pfn = (unsigned long)virtaddr / page_size;
  offset = sizeof(uint64_t) * virt_pfn;
  if (lseek(fd, offset, SEEK_SET) == (off_t)-1) {
    printf("error: lseek\n");
    close(fd);
    return NULL;
  }

  retval = read(fd, &page, PFN_MASK_SIZE);
  close(fd);
  if (retval < 0) {
    printf("error: read\n");
    return NULL;
  } else if (retval != PFN_MASK_SIZE) {
    printf("error: read2\n");
    return NULL;
  }

  if ((page & 0x7fffffffffffffULL) == 0) {
    printf("pfn == 0\n");
    printf("page = %016lX\n", page);
    return NULL;
  }

  physaddr = ((page & 0x7fffffffffffffULL) * page_size) +
             ((unsigned long)virtaddr % page_size);

  return (void *)physaddr;
}

int main(int argc, char *argv[]) {
  printf("File: %s\n", argv[1]);

  int fd = open(argv[1], O_RDWR);
  if (fd == -1) {
    printf("Open failed.\n");
    return 1;
  }
  printf("Opened.\n");
  void *addr = mmap(NULL, 8453619712, PROT_READ | PROT_WRITE,
                    MAP_FILE | MAP_SHARED | MAP_LOCKED, fd, 0);

  printf("VirtAddr: %p\n", addr);
  printf("PhysAddr: %p\n", virt2phy(addr));

  return 0;
}
