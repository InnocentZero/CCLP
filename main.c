#include "cclp.h"

int main(int argc, char *argv[]) {
    struct param arr[] = {init_param('c', "char", "lmfao"),
                          init_param('f', "fly", "idk man")};

    struct opttable *opts = check_args(argc, argv, 2, arr);

    printf("%s\n", opts->table[0].val ? opts->table[0].val : "Check 1");
    printf("%s\n", opts->table[1].val ? opts->table[1].val : "Check 2");
    disp_help(opts, argv[0]);
    free_opt_table(opts);
    return EXIT_SUCCESS;
}
