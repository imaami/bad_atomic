#ifndef BAD_ATOMIC_CTX_H_
#define BAD_ATOMIC_CTX_H_

#include "bad_atomic.h"

#include <pthread.h>
#include <stddef.h>

struct ctx {
	bad_atomic_t ctr;
	size_t       num;
	pthread_t    thd[];
};

extern struct ctx *ctx_create(size_t nthreads);
extern void ctx_destroy(struct ctx **ctx_pptr);

#endif /* BAD_ATOMIC_CTX_H_ */
