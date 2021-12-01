#include "ctx.h"

#include <stdlib.h>

struct ctx *ctx_create(size_t nthreads)
{
	if (nthreads < 1U || nthreads > 32U)
		return NULL;

	struct ctx *ctx = calloc((7U + offsetof(struct ctx, thd)
	                          + nthreads * sizeof(*ctx->thd))
	                         & (SIZE_MAX ^ 7U), 1U);
	if (!ctx)
		return NULL;

	bad_atomic_init(&ctx->ctr, 0U);
	ctx->num = nthreads;

	return ctx;
}

void ctx_destroy(struct ctx **ctx_pptr)
{
	if (ctx_pptr && *ctx_pptr) {
		free(*ctx_pptr);
		*ctx_pptr = NULL;
	}
}
