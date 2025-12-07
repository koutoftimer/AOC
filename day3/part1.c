#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static_assert(sizeof(long) == 8);

int
main(int argc, char *argv[])
{
        assert(argc == 2);
        FILE *f = fopen(argv[1], "r");
        assert(f);

        char line[200] = {0};
        long sum       = 0;
        while (fscanf(f, "%s", line) != EOF) {
                // SAFETY: line is never > 127 bytes long
                int len       = (int)strlen(line);
                long tail_max = line[len - 1];
                long max      = INT64_MIN;
                for (int i = len - 2; i >= 0; --i) {
                        long cur = line[i] * 10 + tail_max - '0' * 11;
                        if (cur > max) max = cur;
                        if (line[i] > tail_max) tail_max = line[i];
                }
                sum += max;
        }

        printf("sum: %ld\n", sum);
        if (strcmp(argv[1], "test") == 0) {
                assert(sum == 357);
        }
        if (strcmp(argv[1], "input") == 0) {
        }
}
