#include "arch/cc.h"
#include <sys/time.h>

u32_t sys_now(void) {
	struct timeval time;
	gettimeofday(&time, NULL);
	return time.tv_sec * 1000;
}
