#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static_assert(sizeof(long) == 8);

typedef struct {
        long from, to;
} Range;

bool
in_ranges(Range *ranges, int ranges_len, long value)
{
        for (int i = 0; i < ranges_len; ++i) {
                if (value >= ranges[i].from && value <= ranges[i].to)
                        return true;
        }
        return false;
}

int
main(int argc, char *argv[])
{
        assert(argc == 2);
        FILE *f = fopen(argv[1], "r");
        assert(f);

        Range ranges[2000] = {0};
        int ranges_len     = 0;
        long from, to;

        char *line;
        size_t n;
        for (; (getline(&line, &n, f)) != 1;) {
                sscanf(line, "%ld-%ld\n", &from, &to);
                assert(from <= to);
                ranges[ranges_len++] = (Range){.from = from, .to = to};
        }

        int res = 0;
        for (; fscanf(f, "%ld\n", &from) != EOF;) {
                res += in_ranges(ranges, ranges_len, from);
        }

        printf("res: %d\n", res);
        if (strcmp(argv[1], "test") == 0) {
                assert(res == 3);
        }
        if (strcmp(argv[1], "input") == 0) {
                assert(res < 1000);
                assert(res == 529);
        }
}
