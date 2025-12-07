#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static_assert(sizeof(long) == 8);

enum { GRID_SIZE = 150 };
char grid[GRID_SIZE][GRID_SIZE];

int
count_neighbours(int i, int j)
{
        int res = 0;
        for (int di = -1; di < 2; ++di) {
                for (int dj = -1; dj < 2; ++dj) {
                        if (!di && !dj) continue;
                        res += grid[i + di][j + dj] == '@';
                }
        }
        return res;
}

int
main(int argc, char *argv[])
{
        assert(argc == 2);
        FILE *f = fopen(argv[1], "r");
        assert(f);

        memset(grid, '.', sizeof(grid));
        int line = 0;
        while (fscanf(f, "%s", &grid[++line][1]) != EOF) {
        }

        int res = 0;
        // input is always a square, so that width and height = line
        for (int i = 1; i < line; ++i) {
                for (int j = 1; j < line; ++j) {
                        if (grid[i][j] != '@') continue;
                        int count = count_neighbours(i, j);
                        assert(count < 9);
                        res += count < 4;
                }
        }

        printf("res: %d\n", res);
        if (strcmp(argv[1], "test") == 0) {
                assert(res == 13);
        }
        if (strcmp(argv[1], "input") == 0) {
                assert(res == 1393);
        }
}
