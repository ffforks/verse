/*
 * Miscellaneous utility routines for generic use throughout the code.
*/

/* Safe, buffer size limited, string copy. */
extern char *	v_strlcpy(char *dst, const char *src, size_t size);

typedef struct {
	uint32 seconds;
	uint32 fractions;
} VUtilTimer;

extern void	v_timer_start(VUtilTimer *timer);
extern double	v_timer_elapsed(const VUtilTimer *timer);
extern void	v_timer_print(const VUtilTimer *timer);

extern int	v_quat32_valid(const VNQuat32 *q);
extern int	v_quat64_valid(const VNQuat64 *q);
