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
        long z;
} Pos;

da_define(Pos) boxes = {0};
long **distances;

long
distance(Pos a, Pos b)
{
        return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) +
               (a.z - b.z) * (a.z - b.z);
}

// indexes of connected boxes, a < b
typedef struct {
        uint a;
        uint b;
} Pair;

Pair
closest(long min_distance)
{
        Pair res = {0, 1};
        for (uint i = 0; i < boxes.size; ++i) {
                for (uint j = i + 1; j < boxes.size; ++j) {
                        if (distances[i][j] < distances[res.a][res.b] &&
                            distances[i][j] > min_distance) {
                                res = (Pair){i, j};
                        }
                }
        }
        return res;
}

// connectivity list
da_define(uint) *connections = {0};

da_define(uint) visited;
da_define(uint) circuits;

uint
dfs(uint i)
{
        da_foreach(visited, it, {
                if (*it == i) return 0;
        });
        da_append(visited, i);
        uint res = 1;
        for (uint j = 0; j < connections[i].size; ++j) {
                res += dfs(connections[i].items[j]);
        }
        return res;
}

int
compare_ints(const void *a, const void *b)
{
        uint arg1 = *(const uint *)a;
        uint arg2 = *(const uint *)b;

        if (arg1 < arg2) return 1;
        if (arg1 > arg2) return -1;
        return 0;
}

int
main(int argc, char *argv[])
{
        assert(argc == 2);
        FILE *f = fopen(argv[1], "r");
        assert(f);

        long x, y, z;
        while (fscanf(f, "%ld,%ld,%ld\n", &x, &y, &z) != EOF) {
                Pos pos = {x, y, z};
                da_append(boxes, pos);
        }

        distances = malloc(sizeof(*distances) * boxes.size);
        for (ulong i = 0; i < boxes.size; ++i) {
                distances[i] = malloc(sizeof(**distances) * boxes.size);
                for (ulong j = i + 1; j < boxes.size; ++j) {
                        distances[i][j] =
                            distance(boxes.items[i], boxes.items[j]);
                }
        }

        int iterations = 0;
        if (strcmp(argv[1], "test") == 0) {
                iterations = 10;

        } else if (strcmp(argv[1], "input") == 0) {
                iterations = 1000;
        }

        connections        = calloc(boxes.size, sizeof(da_define(uint)));
        long prev_distance = 0;
        for (int i = 0; i < iterations; ++i) {
                Pair c        = closest(prev_distance);
                prev_distance = distances[c.a][c.b];
                da_append(connections[c.a], c.b);
                da_append(connections[c.b], c.a);
        }

        // make circuits
        for (uint i = 0; i < boxes.size; ++i) {
                uint number_of_boxes = dfs(i);
                if (!number_of_boxes) continue;
                da_append(circuits, number_of_boxes);
        }

        qsort(circuits.items, circuits.size, sizeof(*circuits.items),
              compare_ints);

        ulong res =
            (ulong)circuits.items[0] * circuits.items[1] * circuits.items[2];
        printf("res: %lu\n", res);

        if (strcmp(argv[1], "test") == 0) {
                assert(res == 40);
        }
        if (strcmp(argv[1], "input") == 0) {
                assert(res == 72150);
        }
}
