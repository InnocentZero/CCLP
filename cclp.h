#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef CCLP
#define CCLP

struct param {
    char sparam;
    char *lparam;
    struct deplist *deps;
};

struct deplist {
    struct param *dep;
    struct deplist *next;
};

struct opttable {
    struct param *table;
    bool *present;
    int size;
};

static inline struct param init_param(char sparam, char *lparam) {
    struct param c = {.sparam = sparam, .lparam = lparam, .deps = NULL};
    return c;
};

void init_param_deps(struct param *opt, int ndeps, struct param *deps);

static inline void assertparams(const char *opt) {
    if (opt[0] != '-' || opt[1] == '\0') {
        fprintf(stderr, "Invalid option: %s", opt);
        exit(EXIT_FAILURE);
    }
};

void init_opt_table(struct opttable *tbl, int params, struct param *arr);

bool *sentry(struct opttable *tbl, char param);

bool *lentry(struct opttable *tbl, const char *param);

struct opttable *check_args(int ioargc, char *ioargv[], int params,
                            struct param *arr, ...);

#endif // !CCLP
