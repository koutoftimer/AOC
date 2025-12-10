#include <assert.h>
#include <lib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
        long x;
        long y;
} Pos;

da_define(Pos) red_tiles;

int
cmp_red_tiles(const void *l, const void *r)
{
        Pos a = *(Pos *)l;
        Pos b = *(Pos *)r;

        if (a.x < b.x) return -1;
        if (a.x > b.x) return 1;

        if (a.y < b.y) return -1;
        if (a.y > b.y) return 1;

        return 0;
}

long
square(uint i, uint j)
{
        assert(i < j);

        long dx = labs(red_tiles.items[j].x - red_tiles.items[i].x);
        long dy = labs(red_tiles.items[j].y - red_tiles.items[i].y);

        return (1 + dx) * (1 + dy);
}

int
main(int argc, char *argv[])
{
        assert(argc == 2);
        FILE *f = fopen(argv[1], "r");
        assert(f);

        Pos pos;
        while (fscanf(f, "%ld,%ld\n", &pos.x, &pos.y) != EOF) {
                da_append(red_tiles, pos);
        }
        qsort(red_tiles.items, red_tiles.size, sizeof(*red_tiles.items),
              cmp_red_tiles);

        long res = -1;

        for (uint i = 0; i < red_tiles.size; ++i) {
                for (uint j = i + 1; j < red_tiles.size; ++j) {
                        long s = square(i, j);
                        res    = MAX(s, res);
                }
        }

        printf("%ld\n", res);

        if (strcmp(argv[1], "test") == 0) {
                assert(res == 50);
        }
        if (strcmp(argv[1], "input") == 0) {
                assert(res == 4737096935);
        }
}
