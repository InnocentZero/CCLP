#include "cstr.h"
#include <stdlib.h>
#include <string.h>

cstr cpy_str(size_t bufsize, const char *buf)
{
	cstr str = {.len = bufsize, .capacity = 2 * bufsize};
	str.buf = (char *)malloc(2 * bufsize * sizeof(*(str.buf)));
	BUFCHK(str.buf);
	strncpy(str.buf, buf, bufsize);
	return str;
}

cstr *alloc_str(size_t bufsize, const char *buf)
{
	cstr *str = malloc(sizeof(*str));
	BUFCHK(str);
	str->len = bufsize;
	str->capacity = 2 * bufsize;
	str->buf = (char *)malloc(2 * bufsize * sizeof(*buf));
	BUFCHK(str->buf);
	strncpy(str->buf, buf, bufsize);
	return str;
}

cstr ref_str(size_t bufsize, char *buf)
{
	cstr str = {.len = bufsize, .capacity = 0};
	str.buf = buf;
	return str;
}

cstr readline(FILE *fs)
{
	cstr s = {.buf = NULL, .len = 0, .capacity = 0};
	ssize_t res = getline(&(s.buf), &(s.len), fs);
	if (res == -1) {
		free(s.buf);
		s.buf = NULL;
		s.len = 0;
	} else {
		s.buf[res - 1] = '\0';
		s.len = res - 1;
		s.capacity = 2 * (res - 1);
		char *tmp = realloc(s.buf, s.capacity);
		BUFCHK(tmp);
		s.buf = tmp;
	}
	return s;
}

cstr readdelim(FILE *fs, int delim)
{
	cstr s = {.buf = NULL, .len = 0, .capacity = 0};
	ssize_t res = getdelim(&(s.buf), &(s.len), delim, fs);
	if (res == -1) {
		free(s.buf);
		s.buf = NULL;
		s.len = 0;
	} else {
		s.buf[res - 1] = '\0';
		s.len = res - 1;
		s.capacity = 2 * (res - 1);
		char *tmp = realloc(s.buf, s.capacity);
		BUFCHK(tmp);
		s.buf = tmp;
	}
	return s;
}

static void double_capacity(cstr *s)
{
	s->capacity *= 2;
	char *tmp = realloc(s->buf, s->capacity);
	BUFCHK(tmp);
	s->buf = tmp;
};

void append_raw_str(cstr *curr, const char *restrict buf, const size_t len)
{
	if (curr->capacity == 0) {
	} else if (curr->len + len >= curr->capacity) {
		double_capacity(curr);
	}
	strncat(curr->buf, buf, curr->len + len);
	curr->len += len;
}

void append_str(cstr *curr, const cstr app)
{
	if (curr->capacity == 0) {
	} else if (curr->len + app.len >= curr->capacity) {
		double_capacity(curr);
	}
	strncat(curr->buf, app.buf, curr->len + app.len);
	curr->len += app.len;
}

void append_char(cstr *curr, const char c)
{
	if (curr->len >= curr->capacity) {
		double_capacity(curr);
	}
	curr->buf[curr->len] = c;
	curr->len++;
}
