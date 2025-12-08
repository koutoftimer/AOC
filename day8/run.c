/**
 * Part 2 may have some improvements, according to `perf` over unoptimized
 * build:
 *
 *   * `dfs` takes 75% of time, which means, having "disjoint set union" will
 *   improve performance a lot.
 *
 *   See: https://cp-algorithms.com/data_structures/disjoint_set_union.html
 *
 *   The only difference will be meta information attached to the root that
 *   contains amount of boxes in the circuit.
 */
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

typedef struct {
        long distance;
        Pair pair;
} Distance;

int
compare_distances(const void *a, const void *b)
{
        Distance arg1 = *(const Distance *)a;
        Distance arg2 = *(const Distance *)b;

        if (arg1.distance < arg2.distance) return -1;
        if (arg1.distance > arg2.distance) return 1;
        return 0;
}

int
main(int argc, char *argv[])
{
        assert(argc == 2);
        FILE *f = fopen(argv[1], "r");
        assert(f);

        Pos pos;
        while (fscanf(f, "%ld,%ld,%ld\n", &pos.x, &pos.y, &pos.z) != EOF) {
                da_append(boxes, pos);
        }

        da_define(Distance) distances = {0};
        for (uint i = 0; i < boxes.size; ++i) {
                for (uint j = i + 1; j < boxes.size; ++j) {
                        Distance elem = {
                            distance(boxes.items[i], boxes.items[j]),
                            (Pair){i, j},
                        };
                        da_append(distances, elem);
                }
        }
        qsort(distances.items, distances.size, sizeof(*distances.items),
              compare_distances);

        connections        = calloc(boxes.size, sizeof(da_define(uint)));
        long prev_distance = 0;
        long res;
        for (;;) {
                Pair c = distances.items[prev_distance++].pair;
                da_append(connections[c.a], c.b);
                da_append(connections[c.b], c.a);
                uint connected_boxes = dfs(c.a);
                visited.size         = 0;
                if (connected_boxes != boxes.size) continue;
                res = boxes.items[c.a].x * boxes.items[c.b].x;
                break;
        }

        printf("res: %lu\n", res);

        if (strcmp(argv[1], "test") == 0) {
                assert(res == 25272);
        }
        if (strcmp(argv[1], "input") == 0) {
                assert(res == 3926518899);
        }
}
