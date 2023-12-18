#include "cclp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void disp_help(const struct opttable *tbl, const char *name) {
    printf("Usage: %s <arguments>", name);
    for (int i = 0; i < tbl->size; i++) {
        printf("-%c, --%s: %s\n", (tbl->table + i)->sparam,
               (tbl->table + i)->lparam, (tbl->table + i)->desc);
    }
    fputc('\n', stdout);
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
                            struct param *arr) {
    struct opttable *tbl = (struct opttable *)malloc(sizeof(*tbl));
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
            bool *pres = lentry(tbl, ptr);
            pres ? *pres = !negate : 0;
        } else {
            char *ptr = ioargv[i];
            ptr++;
            while (*ptr != '\0') {
                bool *pres = sentry(tbl, *ptr);
                pres ? *pres = true : 0;
                ptr++;
            }
        }
    }
    return tbl;
}

void free_opt_table(struct opttable *tbl) {
    free(tbl->table);
    free(tbl->present);
    free(tbl);
}
