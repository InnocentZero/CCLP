#include "cstr.h"
#include <stdlib.h>
#include <string.h>

cstr cpy_str(size_t bufsize, const char *buf)
{
	cstr str = {.len = bufsize};
	str.buf = (char *)malloc(bufsize * sizeof(*(str.buf)));
	BUFCHK(str.buf);
	strncpy(str.buf, buf, bufsize);
	return str;
}

cstr *alloc_str(size_t bufsize, const char *buf)
{
	cstr *str = malloc(sizeof(*str));
	BUFCHK(str);
	str->len = bufsize;
	str->buf = (char *)malloc(bufsize * sizeof(*buf));
	BUFCHK(str->buf);
	strncpy(str->buf, buf, bufsize);
	return str;
}

cstr ref_str(size_t bufsize, char *buf)
{
	cstr str = {.len = bufsize};
	str.buf = buf;
	return str;
}

cstr readline(FILE *fs)
{
	cstr s = {.buf = NULL};
	ssize_t res = getline(&(s.buf), &(s.len), fs);
	if (res == -1) {
		free(s.buf);
		s.buf = NULL;
		s.len = 0;
	} else {
		s.buf[res - 1] = '\0';
		s.len = res - 1;
	}
	return s;
}

cstr readdelim(FILE *fs, int delim)
{
	cstr s = {.buf = NULL, .len = 0};
	ssize_t res = getdelim(&(s.buf), &(s.len), delim, fs);
	if (res == -1) {
		free(s.buf);
		s.buf = NULL;
		s.len = 0;
	} else {
		s.buf[res - 1] = '\0';
		s.len = res - 1;
	}
	return s;
}
