#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static_assert(sizeof(long) == 8);

int
first_max_element(char *line, int from, int to)
{
        char max = line[from];
        int ret  = from;
        for (int i = from + 1; i <= to; ++i) {
                if (line[i] > max) {
                        max = line[i];
                        ret = i;
                }
        }
        return ret;
}

int
main(int argc, char *argv[])
{
        assert(argc == 2);
        FILE *f = fopen(argv[1], "r");
        assert(f);

        char line[200] = {0};
        long sum       = 0;
        while (fscanf(f, "%s", line) != EOF) {
                int len      = strlen(line);
                char max[13] = {first_max_element(line, 0, len - 12)};
                for (int i = 1; i < 12; ++i) {
                        max[i] = first_max_element(line, max[i - 1] + 1,
                                                   len - 12 + i);
                }
                for (int i = 0; i < 12; ++i) max[i] = line[max[i]];
                sum += atol(max);
        }

        printf("sum: %ld\n", sum);
        if (strcmp(argv[1], "test") == 0) {
                assert(sum == 3121910778619);
        }
        if (strcmp(argv[1], "input") == 0) {
                assert(sum == 169019504359949);
        }
}
