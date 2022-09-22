/*
 * utils.h
 *
 *  Created on: Sep 21, 2022
 *      Author: maluz
 */

#ifndef INC_TYPES_UTILS_H_
#define INC_TYPES_UTILS_H_

#include <stdbool.h>

// https://www.reddit.com/r/C_Programming/comments/wjoghd/implementing_an_optional_type_in_c/
#define optional(type) struct { \
	bool present; \
	type value; \
}

// https://stackoverflow.com/questions/3437404/min-and-max-in-c
#define max(a,b)           \
({                         \
	__typeof__ (a) _a = (a); \
	__typeof__ (b) _b = (b); \
	_a > _b ? _a : _b;       \
})

#define min(a,b)           \
({                         \
	__typeof__ (a) _a = (a); \
	__typeof__ (b) _b = (b); \
	_a < _b ? _a : _b;       \
})

#define lambda(lambda$_ret, lambda$_args, lambda$_body) \
({ \
	lambda$_ret lambda$__anon$ lambda$_args \
		lambda$_body \
	lambda$__anon$; \
})

#endif /* INC_TYPES_UTILS_H_ */
