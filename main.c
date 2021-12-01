#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "bad_atomic.h"
#include "ctx.h"
#include "nproc.h"

#define ARRAY_SIZE(x)   (sizeof(x) / sizeof(*(x)))
#define force_inline    static inline __attribute__((always_inline))

static void *f(void *arg);

int main(void)
{
	struct ctx *ctx = ctx_create(nproc());
	if (!ctx)
		return EXIT_FAILURE;

	unsigned int i;
	bad_atomic_t a;
	pthread_t t[32];

	bad_atomic_init(&a, 0U);

	for (i = 0; i < ARRAY_SIZE(t); ++i) {
		printf("%2u\n", i);
		if (pthread_create(&t[i], NULL, f, (void *)&a)) {
			ctx_destroy(&ctx);
			return EXIT_FAILURE;
		}
	}

	for (i = 0; i < ARRAY_SIZE(t); ++i) {
		void *ret = NULL;
		pthread_join(t[i], &ret);
		printf("%2u\t%08" PRIx32 "\n", i, (uint32_t)(uintptr_t)ret);
	}

	printf("%016" PRIx64 "\n", atomic_load(bad_atomic_u64(&a)));

	ctx_destroy(&ctx);
	return EXIT_SUCCESS;
}

static void *f(void *arg)
{
	bad_atomic_t *a = (bad_atomic_t *)arg;
	uint64_t i = atomic_fetch_add_explicit(bad_atomic_u64(a), 1U,
	                                       memory_order_release);
	uint32_t u32 = atomic_fetch_or_explicit(&bad_atomic_u32(a)[1],
	                                        UINT32_C(1) << i,
	                                        memory_order_release);
	u32 |= UINT32_C(1) << i;
	return (void *)(uintptr_t)u32;
}
