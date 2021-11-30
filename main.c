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

	for (i = 0; i < ctx->num; ++i) {
		if (pthread_create(&ctx->thd[i], NULL, f, (void *)ctx)) {
			ctx_destroy(&ctx);
			return EXIT_FAILURE;
		}
	}

	for (i = 0; i < ctx->num; ++i) {
		void *ret = NULL;
		pthread_join(ctx->thd[i], &ret);
		printf("%2u\t%08" PRIx32 "\n", i, (uint32_t)(uintptr_t)ret);
	}

	printf("%016" PRIx64 "\n", atomic_load(bad_atomic_u64(&ctx->ctr)));

	ctx_destroy(&ctx);
	return EXIT_SUCCESS;
}

static void *f(void *arg)
{
	struct ctx *ctx = (struct ctx *)arg;
	uint64_t i = atomic_fetch_add_explicit(bad_atomic_u64(&ctx->ctr), 1U,
	                                       memory_order_release);
	uint32_t u32 = atomic_fetch_or_explicit(&bad_atomic_u32(&ctx->ctr)[1],
	                                        UINT32_C(1) << i,
	                                        memory_order_release);
	u32 |= UINT32_C(1) << i;
	return (void *)(uintptr_t)u32;
}
