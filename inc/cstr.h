#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef CSTR

#ifndef __BUFCHK_PRESENT
#define __BUFCHK_PRESENT
#define BUFCHK(buf)                                                            \
	do {                                                                   \
		if (buf == NULL) {                                             \
			fprintf(stderr,                                        \
				"malloc/calloc failed at line %d; function "   \
				"%s; file %s\n",                               \
				__LINE__, __func__, __FILE__);                 \
			exit(EXIT_FAILURE);                                    \
		}                                                              \
	} while (0)
#endif // !__BUFCHK_PRES
struct cstr {
	size_t len;
	char *buf;
	size_t capacity;
};

typedef struct cstr cstr;

cstr cpy_str(size_t bufsize, const char *buf);
cstr *alloc_str(size_t bufsize, const char *buf);
cstr ref_str(size_t bufsize, char *buf);
cstr readline(FILE *fs);
cstr readdelim(FILE *fs, int delim);
void append_raw_str(cstr *curr, const char *buf, const size_t len);
void append_str(cstr *curr, const cstr app);
void append_char(cstr *curr, const char c);
static inline void freestr(cstr *str)
{
	free(str->buf);
	str->buf = NULL;
	str->len = 0;
	str->capacity = 0;
};
#endif // !CSTR
