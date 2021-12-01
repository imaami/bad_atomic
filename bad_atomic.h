#ifndef BAD_ATOMIC_H_
#define BAD_ATOMIC_H_

#include <stdatomic.h>
#include <stdint.h>

#define bad_atomic_u64(ptr)     (&(ptr)->u64)
#define bad_atomic_u32(ptr)     (&(ptr)->u32[0])

typedef _Atomic uint64_t atomic_u64_t;
typedef _Atomic uint32_t atomic_u32_t;
typedef union bad_atomic bad_atomic_t;

union bad_atomic {
	atomic_u64_t u64;
	atomic_u32_t u32[2];
};

extern void bad_atomic_init(bad_atomic_t *ptr,
                            uint64_t      val);

extern void bad_atomic_init_u32(bad_atomic_t *ptr,
                                uint32_t      val0,
                                uint32_t      val1);

#endif /* BAD_ATOMIC_H_ */
