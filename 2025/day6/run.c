#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "../lib.h"

static_assert(sizeof(long) == 8);

long
add(long a, long b)
{
        return a + b;
}

long
mult(long a, long b)
{
        return a * b;
}

long
value(ulong col, char *data, ulong step, ulong file_size)
{
        long res = 0;
        for (; col < file_size; col += step) {
                if (!isdigit(data[col])) continue;
                res = res * 10 + data[col] - '0';
        }
        return res;
}

int
main(int argc, char *argv[])
{
        assert(argc == 2);
        FILE *f = fopen(argv[1], "r");
        assert(f);

        struct stat st;
        assert(stat(argv[1], &st) == 0);

        char *data = malloc((size_t)st.st_size + 1);
        assert(fread(data, sizeof(*data), (size_t)st.st_size, f) > 0);

        ulong line_size = (ulong)(strchr(data, '\n') - data);
        ulong newlines  = ((ulong)st.st_size) / line_size;
        assert((line_size + 1) * newlines == (ulong)st.st_size);

        char *op             = MIN(strchr(data, '*'), strchr(data, '+'));
        char *const OP_START = op;
        long grand_total     = 0;
        while (op - data < st.st_size) {
                long sub_total         = *op == '*' ? 1 : 0;
                typeof(mult) *operator = *op == '*' ? mult : add;
                char *next_op =
                    MIN(strchrnul(op + 1, '*'), strchrnul(op + 1, '+'));
                for (char *col = op; col < next_op; ++col) {
                        long v = value((ulong)(col - OP_START), data,
                                       line_size + 1, (ulong)st.st_size);
                        if (v == 0) continue;
                        sub_total = operator(sub_total, v);
                }
                op = next_op;
                grand_total += sub_total;
        }

        printf("grand_total: %ld\n", grand_total);
        if (strcmp(argv[1], "test") == 0) {
                assert(grand_total == 3263827);
        }
        if (strcmp(argv[1], "input") == 0) {
                assert(grand_total == 11708563470209);
        }
}
