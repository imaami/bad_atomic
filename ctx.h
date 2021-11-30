#ifndef BAD_ATOMIC_CTX_H_
#define BAD_ATOMIC_CTX_H_

#include "bad_atomic.h"

#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>

struct ctx {
	bad_atomic_t      ctr;
	pthread_barrier_t run;
	size_t            num;
	pthread_t         thd[];
};

extern struct ctx *ctx_create(size_t nthreads);
extern void ctx_destroy(struct ctx **ctx_pptr);

extern bool ctx_exec(struct ctx *ctx);
extern bool ctx_wait(struct ctx *ctx);

#endif /* BAD_ATOMIC_CTX_H_ */
