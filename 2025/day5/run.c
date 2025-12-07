#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static_assert(sizeof(long) == 8);

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct {
        long start, end;
} Range;

int
cmp(const void *a, const void *b)
{
        long value = ((Range *)a)->start - ((Range *)b)->start;
        return value > 0 ? 1 : (value < 0 ? -1 : 0);
}

int
main(int argc, char *argv[])
{
        assert(argc == 2);
        FILE *f = fopen(argv[1], "r");
        assert(f);

        Range ranges[2000] = {0};
        uint ranges_len    = 0;

        char *line;
        size_t n;
        for (; (getline(&line, &n, f)) != 1;) {
                long start, end;
                sscanf(line, "%ld-%ld\n", &start, &end);
                assert(start <= end);
                ranges[ranges_len++] = (Range){.start = start, .end = end};
        }

        qsort(ranges, ranges_len, sizeof(*ranges), cmp);

        uint left = 0, right = 1;
        for (; right < ranges_len; ++right) {
                // merge
                if (ranges[right].start - 1 <= ranges[left].end) {
                        ranges[left].end =
                            MAX(ranges[right].end, ranges[left].end);
                        continue;
                }
                // cmpress
                ranges[++left] = ranges[right];
        }

        long res = 0;
        for (uint i = 0; i <= left; ++i) {
                res += ranges[i].end - ranges[i].start + 1;
        }

        printf("res: %ld\n", res);
        if (strcmp(argv[1], "test") == 0) {
                assert(res == 14);
        }
        if (strcmp(argv[1], "input") == 0) {
                assert(res == 344260049617193);
        }
}
