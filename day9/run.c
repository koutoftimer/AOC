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

bool
on_line(Pos corner)
{
        for (uint k = 0; k < red_tiles.size; ++k) {
                Pos from         = red_tiles.items[k],
                    to           = red_tiles.items[(k + 1) % red_tiles.size];

                Pos top_left     = {MIN(from.x, to.x), MIN(from.y, to.y)},
                    bottom_right = {MAX(from.x, to.x), MAX(from.y, to.y)};

                if (top_left.x == bottom_right.x && top_left.x == corner.x &&
                    top_left.y <= corner.y && corner.y <= bottom_right.y) {
                        return true;

                } else if (top_left.y == bottom_right.y &&
                           top_left.y == corner.y && top_left.x <= corner.x &&
                           bottom_right.x <= corner.x) {
                        return true;
                }
        }
        return false;
}

bool
crosses_path(Pos a, Pos b)
{
        if (a.x > b.x) swap(a.x, b.x);
        if (a.y > b.y) swap(a.y, b.y);

        for (uint k = 0; k < red_tiles.size; ++k) {
                Pos c = red_tiles.items[k],
                    d = red_tiles.items[(k + 1) % red_tiles.size];

                if (c.x > d.x) swap(c.x, d.x);
                if (c.y > d.y) swap(c.y, d.y);

                if (a.x == b.x && c.y == d.y && c.x < a.x && a.x < d.x &&
                    a.y <= c.y && c.y <= b.y) {
                        return true;
                }
                if (a.y == b.y && c.x == d.x && c.y < a.y && a.y < d.y &&
                    a.x <= c.x && c.x <= b.x) {
                        return true;
                }
        }
        return false;
}

bool
possible(uint i, uint j)
{
        Pos a = red_tiles.items[i], b = red_tiles.items[j];
        if (a.y > b.y) swap(a.y, b.y);

        // rectangle corner not on red "sides"
        Pos corners[] = {
            a,
            {b.x, a.y},
            {a.x, b.y},
            b,
        };
        for (uint c = 0; c < sizeof(corners) / sizeof(*corners); ++c) {
                if (!on_line(corners[c])) return false;
        }

        // red "line" crosses rectangle
        for (uint c = 0; c < 4; ++c) {
                if (crosses_path(corners[c], corners[(c + 1) % 4]))
                        return false;
        }

        return true;
}

long
square(uint i, uint j)
{
        assert(i < j);

        if (!possible(i, j)) return 0;

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

        FILE *svg = fopen("path.svg", "w");
        fprintf(svg, "<?xml version=\"1.0\" standalone=\"no\"?>\n");
        fprintf(svg,
                "<svg version=\"1.1\" width=\"100000\" height=\"100000\" "
                "xmlns=\"http://www.w3.org/2000/svg\">\n"
                "<rect x=\"0\" y=\"0\" width=\"100000\" height=\"100000\" "
                "fill=\"white\" />");
        fprintf(svg,
                "<polygon fill=\"green\" stroke=\"red\" stroke-width=\"1\" "
                "points=\"");
        da_foreach(red_tiles, it, { fprintf(svg, "%ld, %ld ", it->x, it->y); });
        fprintf(svg, "\"/>\n</svg>");
        exit(1);

        // qsort(red_tiles.items, red_tiles.size, sizeof(*red_tiles.items),
        //       cmp_red_tiles);

        long res = -1;

        for (uint i = 0; i < red_tiles.size; ++i) {
                for (uint j = i + 1; j < red_tiles.size; ++j) {
                        long s = square(i, j);
                        res    = MAX(s, res);
                }
        }

        printf("%ld\n", res);

        if (strcmp(argv[1], "test") == 0) {
                assert(res == 24);
        }
        if (strcmp(argv[1], "input") == 0) {
                assert(res < 4695337232);
                assert(res > 114207960);
        }
}
