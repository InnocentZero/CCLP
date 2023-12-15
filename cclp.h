#include <stdbool.h>
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

inline struct param init_param(char sparam, char *lparam);

void init_param_deps(struct param *opt, int ndeps, struct param *deps);

inline void assertparams(const char *opt);

void init_opt_table(struct opttable *tbl, int params, struct param *arr);

bool *sentry(struct opttable *tbl, char param);

bool *lentry(struct opttable *tbl, const char *param);

struct opttable *check_args(int ioargc, char *ioargv[], int params,
                            struct param *arr, ...);

#endif // !CCLP
