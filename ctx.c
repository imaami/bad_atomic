#include "ctx.h"

#include <stdlib.h>

#define sizeof_member(T, m) sizeof(((T *)0)->m)

__attribute__((pure))
force_inline size_t sizeof_ctx(size_t nthreads)
{
	return (7U + offsetof(struct ctx, thd) +
	        nthreads * sizeof_member(struct ctx, thd[0]))
	       & (SIZE_MAX ^ 7U);
}

struct ctx *ctx_create(size_t nthreads)
{
	if (nthreads < 1U || nthreads > 32U)
		return NULL;

	struct ctx *ctx = calloc(1U, sizeof_ctx(nthreads));
	if (!ctx)
		return NULL;

	if (pthread_barrier_init(&ctx->run, NULL, nthreads)) {
		free(ctx);
		return NULL;
	}

	bad_atomic_init(&ctx->ctr, 0U);
	ctx->num = nthreads;

	return ctx;
}

void ctx_destroy(struct ctx **ctx_pptr)
{
	if (ctx_pptr && *ctx_pptr) {
		pthread_barrier_destroy(&(*ctx_pptr)->run);
		free(*ctx_pptr);
		*ctx_pptr = NULL;
	}
}

bool ctx_exec(struct ctx *ctx __attribute__((unused)))
{
	return false;
}

bool ctx_wait(struct ctx *ctx)
{
	if (!ctx)
		return false;
	int ret = pthread_barrier_wait(&ctx->run);
	return !ret || (PTHREAD_BARRIER_SERIAL_THREAD == ret);
}
