/*
 * Utility functions.
*/

#include <stdio.h>

#include "verse_header.h"
#include "v_network.h"
#include "v_util.h"

/* Safe string copy. Copies from <src> to <dst>, not using more than <size>
 * bytes of destination space. Always 0-terminates the destination. Returns
 * the beginning of the destination string.
*/
char * v_strlcpy(char *dst, const char *src, size_t size)
{
	char	*base = dst;

	if(size == 0)
		return NULL;
	for(size--; size > 0 && *src != '\0'; size--)
		*dst++ = *src++;
	*dst = '\0';

	return base;
}

void v_timer_start(VUtilTimer *timer)
{
	v_n_get_current_time(&timer->seconds, &timer->fractions);
}

double v_timer_elapsed(const VUtilTimer *timer)
{
	uint32 cur_seconds, cur_fractions;

	v_n_get_current_time(&cur_seconds, &cur_fractions);
	return (double)(cur_seconds - timer->seconds) + ((double)cur_fractions - (double)timer->fractions) / (double) 0xffffffff;
}

void v_timer_print(const VUtilTimer *timer)
{
	uint32 cur_seconds, cur_fractions;

	v_n_get_current_time(&cur_seconds, &cur_fractions);
	printf("%f", (double)(cur_seconds - timer->seconds) + ((double)cur_fractions - (double)timer->fractions) / (double) 0xffffffff);
}
