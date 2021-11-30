#include <immintrin.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "bad_atomic.h"
#include "ctx.h"
#include "nproc.h"

//#define ARRAY_SIZE(x)   (sizeof(x) / sizeof(*(x)))

#define ROUNDS          (1U << 24U)
//#define cpu_relax()     _mm_pause()

#ifndef cpu_relax
#define cpu_relax()     do{}while(0)
#endif

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
	}

	printf("%016" PRIx64 "\n", atomic_load(bad_atomic_u64(&ctx->ctr)));

	ctx_destroy(&ctx);
	return EXIT_SUCCESS;
}

static void *loop_32(atomic_u32_t *v)
{
	for (unsigned int i = 0U; i < ROUNDS - 1U; ++i) {
		atomic_fetch_add_explicit(&v[0], 1U, memory_order_release);
		atomic_fetch_add_explicit(&v[1], 1U, memory_order_release);
		cpu_relax();
	}

	return NULL;
}

static void *loop_64(atomic_u64_t *v)
{
	for (unsigned int i = 0U; i < ROUNDS - 1U; ++i) {
		atomic_fetch_add_explicit(v, UINT64_C(0x100000001),
		                          memory_order_release);
		cpu_relax();
	}

	return NULL;
}

static void *f(void *arg)
{
	struct ctx *ctx = (struct ctx *)arg;
	bool odd = (atomic_fetch_add_explicit(&bad_atomic_u32(&ctx->ctr)[0],
	                                      1U, memory_order_release) & 1U);
	atomic_fetch_add_explicit(&bad_atomic_u32(&ctx->ctr)[1],
	                          1U, memory_order_release);

	if (!ctx_wait(ctx))
		pthread_exit(NULL);

	if (odd) {
		loop_64(bad_atomic_u64(&ctx->ctr));
	} else {
		loop_32(bad_atomic_u32(&ctx->ctr));
	}

	return NULL;
}
