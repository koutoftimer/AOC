#include <assert.h>
#include <lib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct {
        int x;
        int y;
        long timelines;
} Entry;

da_define(Entry) dp     = {0};
da_define(char *) lines = {0};

bool
contains(long y, long x)
{
        for (uint i = 0; i < dp.size; ++i) {
                if (dp.items[i].x == x && dp.items[i].y == y) return true;
        }
        return false;
}

uint
dp_index(long y, long x)
{
        for (uint i = 0; i < dp.size; ++i) {
                if (dp.items[i].x == x && dp.items[i].y == y) return i;
        }

        panic("unreachable");
}

long
r(long y, long x)
{
        assert(y >= 0);

        // SAFETY: y has assert, x have conditional check for negative values
        if ((ulong)y >= lines.size || x < 0 ||
            (ulong)x >= strlen(lines.items[0])) {
                return 1;
        }

        if (contains(y, x)) {
                return dp.items[dp_index(y, x)].timelines;
        }

        const char current = lines.items[y][x];

        if (current == '.' || current == 'S') {
                return r(y + 1, x);

        } else if (current == '^') {
                long timelines = r(y, x - 1) + r(y, x + 1);
                Entry entry    = {
                       .x         = (int)x,
                       .y         = (int)y,
                       .timelines = timelines,
                };
                da_append(dp, entry);
                return timelines;
        }

        panic("unreachable");
}

int
main(int argc, char *argv[])
{
        assert(argc == 2);
        FILE *f = fopen(argv[1], "r");
        assert(f);

        char line[200];
        while (fscanf(f, "%s\n", line) != EOF) {
                da_append(lines, strdup(line));
        }

        long res = r(0, strchr(lines.items[0], 'S') - *lines.items);

        printf("res: %ld\n", res);

        if (strcmp(argv[1], "test") == 0) {
                assert(res == 40);
        }
        if (strcmp(argv[1], "input") == 0) {
                assert(res == 48021610271997);
        }
}
