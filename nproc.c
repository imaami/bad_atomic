#include "nproc.h"

#include <sched.h>
#include <sys/types.h>
#include <unistd.h>

size_t nproc(void)
{
#ifdef CPU_COUNT
	cpu_set_t mask;
	CPU_ZERO(&mask);

	if (!sched_getaffinity(getpid(), sizeof(mask), &mask))
		return (size_t)CPU_COUNT(&mask);
#endif // CPU_COUNT

#ifdef _SC_NPROCESSORS_ONLN
	long n = sysconf(_SC_NPROCESSORS_ONLN);

	if (n > 0)
		return (size_t)n;
#endif // _SC_NPROCESSORS_ONLN

	return 0U;
}
