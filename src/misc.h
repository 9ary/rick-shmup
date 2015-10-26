#ifndef INCLUDE_MISC_H
#define INCLUDE_MISC_H

/*
 * Miscellaneous macros
 */

/*
 * Silence warnings about unused variables/arguments
 * This is a no-op
 */
#define UNUSED(expr) (void)(expr)

/*
 * Math
 */
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define in_range(x, a, b) (((x) >= (a)) && ((x) < (b)))

#endif
