#include <assert.h>
#include <lib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
        char **items;
        size_t capacity;
        size_t size;
} Lines;

Lines lines = {0};

typedef struct {
        long *items;
        size_t capacity;
        size_t size;
} Visited;

Visited visited = {0};

#define PACK(y, x) (((y) << 32) + (x))

bool
is_visited(long y, long x)
{
        const long PACKED = PACK(y, x);
        for (uint i = 0; i < visited.size; ++i) {
                if (visited.items[i] == PACKED) return true;
        }
        return false;
}

void
r(long y, long x)
{
        assert(y >= 0);
        // SAFETY: y has assert, x have conditional check for negative values
        if ((ulong)y >= lines.size || x < 0 ||
            (ulong)x >= strlen(lines.items[0]) || is_visited(y, x)) {
                return;
        }

        const char current = lines.items[y][x];

        if (current == '.') {
                r(y + 1, x);
        } else if (current == 'S') {
                r(y + 1, x);
        } else if (current == '^') {
                da_append(visited, PACK(y, x));
                r(y, x - 1);
                r(y, x + 1);
        }
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

        r(0, strchr(lines.items[0], 'S') - *lines.items);
        ulong res = visited.size;

        printf("res: %lu\n", res);

        if (strcmp(argv[1], "test") == 0) {
                assert(res == 21);
        }
        if (strcmp(argv[1], "input") == 0) {
                assert(res == 1613);
        }
}
