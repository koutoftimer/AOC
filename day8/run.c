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

// Disjoint Union Sets (DSU) with size optimization is the most suitable version
// for this task.
typedef struct {
        uint *parent;
        uint *size;
} DSU;

DSU dsu = {0};

void
dsu_make_set(uint v)
{
        assert(dsu.parent != NULL);
        assert(dsu.size != NULL);

        dsu.parent[v] = v;
        dsu.size[v]   = 1;
}

uint
dsu_find_set(uint v)
{
        assert(dsu.parent != NULL);

        if (dsu.parent[v] == v) {
                return v;
        }
        return dsu.parent[v] = dsu_find_set(dsu.parent[v]);
}

void
dsu_union_sets(uint a, uint b)
{
        assert(dsu.parent != NULL);
        assert(dsu.size != NULL);

        a = dsu_find_set(a);
        b = dsu_find_set(b);

        if (a == b) return;

        if (dsu.size[a] > dsu.size[b]) swap(a, b);
        dsu.parent[a] = b;
        dsu.size[b] += dsu.size[a];
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

        dsu.parent = malloc(boxes.size * sizeof(*dsu.parent));
        dsu.size   = malloc(boxes.size * sizeof(*dsu.size));
        for (uint i = 0; i < boxes.size; ++i) dsu_make_set(i);

        long prev_distance = 0;
        long res;
        for (;;) {
                Pair c = distances.items[prev_distance++].pair;
                dsu_union_sets(c.a, c.b);
                uint root = dsu_find_set(c.a);
                if (dsu.size[root] != boxes.size) continue;
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
