#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long normalized_half(long number);

long invalid(long half);

int
main(int argc, char *argv[])
{
        assert(argc == 2);
        FILE *f = fopen(argv[1], "r");
        assert(f);
        assert(sizeof(long) == 8);

        long sum = 0, from, to;
        while (fscanf(f, "%ld-%ld,", &from, &to) != EOF) {
                long low_half  = normalized_half(from);
                long high_half = normalized_half(to);
                for (long value = invalid(low_half);
                     low_half <= high_half && value <= to;
                     value = invalid(++low_half)) {
                        if (value >= from) sum += value;
                }
        }

        printf("sum: %ld\n", sum);
        if (strcmp(argv[1], "test") == 0) {
                assert(sum == 1227775554);
        }
}

long
invalid(long half)
{
        char buf[20] = {0};
        sprintf(buf, "%ld", half);
        assert(atol(buf) == half);

        // SAFETY: len of buf will not exced int
        int len = (int)strlen(buf);
        // SAFETY: strlen returns posivite values
        memcpy(buf + len, buf, (ulong)len);
        // SAFETY: len of buf will not exced int
        assert((int)strlen(buf) == 2 * len);

        return atol(buf);
}

long
normalized_half(long number)
{
        char buf[20] = {0};
        sprintf(buf, "%ld", number);
        // SAFETY: ulong `number` can't overflow int size
        int len = (int)strlen(buf);
        if (len & 1) {
                buf[0] = '1';
                for (int i = 1; i <= len / 2; ++i) buf[i] = '0';
                buf[len / 2 + 1] = 0;
        } else {
                buf[len / 2] = 0;
        }

        return atol(buf);
}
