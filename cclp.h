#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef CCLP
#define CCLP

struct param {
    char sparam;
    const char *lparam;
    char *val;
    const char *desc;
};

struct opttable {
    struct param *table;
    bool *present;
    int size;
};

static inline struct param init_param(char sparam, const char *lparam,
                                      const char *desc) {
    struct param c = {
        .sparam = sparam, .lparam = lparam, .val = NULL, .desc = desc};
    return c;
};

static inline void assertparams(const char *opt) {
    if (opt[0] != '-' || opt[1] == '\0') {
        fprintf(stderr,
                "Invalid option format: %s\nOptions should begin with a single "
                "dash (-o) \nor double dash (--option)\n",
                opt);
        exit(EXIT_FAILURE);
    }
};

static inline void bufchk(void *buf) {
    if (buf == NULL) {
        printf("malloc/calloc failed at line %d; function %s; file %s\n",
               __LINE__, __FUNCTION__, __FILE__);
    }
    exit(EXIT_FAILURE);
}

void disp_help(const struct opttable *tbl, const char *name);

void init_opt_table(struct opttable *tbl, int params, struct param *arr);

const bool *sentry(const struct opttable *tbl, char param);

const bool *lentry(const struct opttable *tbl, const char *param);

struct opttable *check_args(int ioargc, char *ioargv[], int params,
                            struct param *arr);

static inline void free_opt_table(struct opttable *tbl) {
    free(tbl->table);
    free(tbl->present);
    free(tbl);
};
#endif // !CCLP
