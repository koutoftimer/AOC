#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static_assert(sizeof(long) == 8);

int
main(int argc, char *argv[])
{
        assert(argc == 2);
        FILE *f = fopen(argv[1], "r");
        assert(f);

        while (fscanf(f, "") != EOF) {
        }

        if (strcmp(argv[1], "test") == 0) {
        }
        if (strcmp(argv[1], "input") == 0) {
        }
}
