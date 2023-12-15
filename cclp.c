#include "cclp.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

inline void assertparams(const char *opt) {
    if (opt[0] != '-' || opt[1] == '\0') {
        fprintf(stderr, "Invalid option: %s", opt);
        exit(EXIT_FAILURE);
    }
}

inline struct param init_param(char sparam, char *lparam) {
    struct param c = {.sparam = sparam, .lparam = lparam, .deps = NULL};
    return c;
}

// pointers to the dependencies as the rest of the arguments
void init_param_deps(struct param *opt, int ndeps, struct param *deps) {
    struct deplist **iter = &(opt->deps);
    for (int i = 0; i < ndeps; i++) {
        *iter = malloc(sizeof(**iter));
        (*iter)->dep = (deps + i);
        *iter = (*iter)->next;
    }
    *iter = NULL;
}

void init_opt_table(struct opttable *tbl, int params, struct param *arr) {
    tbl->table = (struct param *)malloc(params * sizeof(*(tbl->table)));
    tbl->present = (bool *)calloc(params, sizeof(*(tbl->present)));
    tbl->size = params;
    for (int i = 0; i < params; i++) {
        tbl->table[i] = arr[i];
    }
}

bool *sentry(struct opttable *tbl, char param) {
    bool *ptr = NULL;
    for (int i = 0; i < tbl->size; i++) {
        if (tbl->table[i].sparam == param) {
            ptr = tbl->present + i;
            break;
        }
    }
    return ptr;
}

bool *lentry(struct opttable *tbl, const char *param) {
    bool *ptr = NULL;
    for (int i = 0; i < tbl->size; i++) {
        if (strcmp(tbl->table[i].lparam, param) == 0) {
            ptr = tbl->present + i;
            break;
        }
    }
    return ptr;
}

// supply the sizes of arrays and arrays of dependent options as variadic
// parameters, NULL if none
struct opttable *check_args(int ioargc, char *ioargv[], int params,
                            struct param *arr, ...) {
    struct opttable *tbl = (struct opttable *)malloc(sizeof(*tbl));
    init_opt_table(tbl, params, arr);
    va_list vaiter;
    va_start(vaiter, arr);
    for (int i = 0; i < params; i++) {
        init_param_deps(tbl->table + i, va_arg(vaiter, int),
                        va_arg(vaiter, struct param *));
    }
    va_end(vaiter);
    for (int i = 1; i < ioargc; i++) {
        assertparams(ioargv[i]);
        if (strncmp(ioargv[i], "--", 3) == 0) {
            break;
        } else if (strncmp(ioargv[i], "--", 2) == 0) {
            char *ptr = ioargv[i];
            ptr++, ptr++;
            *(lentry(tbl, ptr)) = true;
        } else {
            char *ptr = ioargv[i];
            ptr++;
            while (*ptr != '\0') {
                *(sentry(tbl, *ptr)) = true;
                ptr++;
            }
        }
    }
    return tbl;
}
