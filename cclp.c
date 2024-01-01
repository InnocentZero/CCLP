#include "cclp.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void disp_help(const struct opttable *tbl, const char *name)
{
	fprintf(stderr, "Usage: %s <arguments>\n", name);
	for (int i = 0; i < tbl->size; i++) {
		fprintf(stderr, "-%c, --%s: %s\n", (tbl->table + i)->sparam,
			(tbl->table + i)->lparam, (tbl->table + i)->desc);
		fprintf(stderr, "    --no-%1$s: Negate/unset --%1$s.\n",
			(tbl->table + i)->lparam);
	}
	fprintf(stderr,
		"Some flags may take additional key=value style "
		"options\n.Refer to the documentation for more detail.\n\n");
	exit(EXIT_FAILURE);
}

void init_opt_table(struct opttable *tbl, int params, struct param *arr)
{
	tbl->table = (struct param *)malloc(params * sizeof(*(tbl->table)));
	BUFCHK(tbl->table);
	tbl->present = (bool *)calloc(params, sizeof(*(tbl->present)));
	BUFCHK(tbl->present);
	tbl->size = params;
	for (int i = 0; i < params; i++) {
		tbl->table[i] = arr[i];
	}
}

const bool *sentry(const struct opttable *tbl, char param)
{
	bool *ptr = NULL;
	for (int i = 0; i < tbl->size; i++) {
		if (tbl->table[i].sparam == param) {
			ptr = tbl->present + i;
			break;
		}
	}
	return ptr;
}

bool *sentry__(struct opttable *tbl, char param)
{
	bool *ptr = NULL;
	for (int i = 0; i < tbl->size; i++) {
		if (tbl->table[i].sparam == param) {
			ptr = tbl->present + i;
			break;
		}
	}
	return ptr;
};
const bool *lentry(const struct opttable *tbl, const char *param)
{
	bool *ptr = NULL;
	for (int i = 0; i < tbl->size; i++) {
		if (strcmp(tbl->table[i].lparam, param) == 0) {
			ptr = tbl->present + i;
			break;
		}
	}
	return ptr;
}

char *lopt(const struct opttable *tbl, const char *param)
{
	int i = 0;
	while (i < tbl->size && strcmp(tbl->table[i].lparam, param) != 0) {
		i++;
	}
	return i < tbl->size ? tbl->table[i].val : NULL;
}

struct opts {
	bool *entry;
	struct param *param;
};

struct opts lentry__(struct opttable *tbl, char *param, const char *paramend)
{
	bool *boolptr;
	struct param *paramptr;
	int len = paramend - param + 1;
	char *buf = (char *)malloc(len * sizeof(*buf));
	BUFCHK(buf);
	int i = 0;
	for (i = 0; i < len - 1; i++) {
		buf[i] = *param++;
	}
	buf[len - 1] = '\0';
	i = 0;
	while (i < tbl->size && strcmp(tbl->table[i].lparam, buf)) {
		i++;
	}
	boolptr = i < tbl->size ? tbl->present + i : NULL;
	paramptr = i < tbl->size ? tbl->table + i : NULL;
	free(buf);
	struct opts opt = {.entry = boolptr, .param = paramptr};
	return opt;
};

struct opttable *check_args(int ioargc, char *ioargv[], int params,
			    struct param *arr)
{
	struct opttable *tbl = (struct opttable *)malloc(sizeof(*tbl));
	BUFCHK(tbl);
	init_opt_table(tbl, params, arr);
	for (int i = 1; i < ioargc; i++) {
		assertparams(ioargv[i]);
		if (strncmp(ioargv[i], "--help", 7) == 0 ||
		    strncmp(ioargv[i], "-h", 2) == 0) {
			disp_help(tbl, ioargv[0]);
		} else if (strncmp(ioargv[i], "--", 3) == 0) {
			break;
		} else if (strncmp(ioargv[i], "--", 2) == 0) {
			char *ptr = ioargv[i];
			ptr += 2;
			bool negate = (strncmp(ptr, "no-", 3) == 0);
			negate ? ptr += 3 : 0;
			char *ptr2 = ptr;
			while (ptr2 != NULL && *ptr2 != '=' && *ptr2 != '\0') {
				ptr2++;
			}
			struct opts pres = lentry__(tbl, ptr, ptr2);
			if (pres.entry) {
				*(pres.entry) = !negate;
				pres.param->val =
				    *(ptr2) == '=' ? ptr2 + 1 : NULL;
			}
		} else {
			char *ptr = ioargv[i];
			ptr++;
			while (*ptr != '\0') {
				bool *pres = sentry__(tbl, *ptr);
				pres ? *pres = true : 0;
				ptr++;
			}
		}
	}
	return tbl;
}
