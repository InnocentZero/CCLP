#ifndef CONTAINER
#define CONTAINER

#define CCAT2(x, y) x##_##y
#define FN_CREATE(x, y) CCAT2(x, y)

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
#endif // !CONTAINER
