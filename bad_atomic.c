#include "bad_atomic.h"

void bad_atomic_init(bad_atomic_t *ptr,
                     uint64_t      val)
{
	atomic_init(bad_atomic_u64(ptr), val);
}

void bad_atomic_init_u32(bad_atomic_t *ptr,
                         uint32_t      val0,
                         uint32_t      val1)
{
	atomic_init(&bad_atomic_u32(ptr)[0], val0);
	atomic_init(&bad_atomic_u32(ptr)[1], val1);
}
