/*
 * Utility functions.
*/

#include <stdio.h>

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
