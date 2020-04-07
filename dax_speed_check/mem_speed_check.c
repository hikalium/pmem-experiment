#include <stdint.h>
#include <stdio.h>

#include <sys/time.h>

#ifdef CONFIG_X86_64
/* Using 64-bit values saves one instruction clearing the high half of low */
#define DECLARE_ARGS(val, low, high) unsigned long low, high
#define EAX_EDX_VAL(val, low, high) ((low) | (high) << 32)
#define EAX_EDX_RET(val, low, high) "=a"(low), "=d"(high)
#else
#define DECLARE_ARGS(val, low, high) unsigned long long val
#define EAX_EDX_VAL(val, low, high) (val)
#define EAX_EDX_RET(val, low, high) "=A"(val)
#endif

#define TSC_CLOCK_PER_SEC (3600UL * 1000 * 1000)
#define INV_OF_NANO_SEC (1000UL * 1000 * 1000)

static __always_inline uint64_t rdtsc(void) {
  uint64_t val;
  asm volatile("rdtsc" : "=A"(val));
  return val;
}

#define CACHE_SIZE  (8192 * 1024)
#define LINE_SIZE   64
#define STRIDE      (LINE_SIZE / sizeof(uint64_t))
#define ARRAY_SIZE  (CACHE_SIZE * STRIDE)

uint64_t array[ARRAY_SIZE];
uint64_t dummy_array[ARRAY_SIZE];

int main(int argc, char *argv[]) {
  uint64_t t0, t1;
  volatile uint64_t sum;
  for (int i = 0; i < 5; i++) {
    for (int i = 0; i < ARRAY_SIZE; i += STRIDE) {
      sum = dummy_array[i];
    }
    t0 = rdtsc();
    for (int i = 0; i < ARRAY_SIZE; i += STRIDE) {
      sum = array[i];
    }
    t1 = rdtsc();

    printf("sum: %lu\n", sum);
    printf("%lu ns\n", (t1 - t0) * INV_OF_NANO_SEC / TSC_CLOCK_PER_SEC);
    printf("%lu ns / access\n",
           (t1 - t0) * INV_OF_NANO_SEC / TSC_CLOCK_PER_SEC / (ARRAY_SIZE / STRIDE));
  }

  return 0;
}
