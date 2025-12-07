#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long normalized_part(long number, int chunk_len);

long invalid(long part, int chunks);

int
cmp(const void *a, const void *b)
{
        long res = *(long *)a - *(long *)b;
        return res > 0 ? 1 : (res < 0 ? -1 : 0);
}

int
main(int argc, char *argv[])
{
        assert(argc == 2);
        FILE *f = fopen(argv[1], "r");
        assert(f);
        assert(sizeof(long) == 8);

        long sum = 0, from, to;
        long values[100000];
        int values_size = 0;
        while (fscanf(f, "%ld-%ld,", &from, &to) != EOF) {
                char number[40] = {20};
                sprintf(number, "%ld", from);
                // SAFETY: no ulong can overflow the int size
                int len = (int)strlen(number);
                for (int chunk_len = 1; chunk_len <= (len + 1) / 2;
                     ++chunk_len) {
                        long low_part = normalized_part(from, chunk_len);
                        int chunks    = (len + chunk_len - 1) / chunk_len;
                        if (chunks < 2) chunks = 2;
                        for (long value         = invalid(low_part, chunks);
                             value <= to; value = invalid(++low_part, chunks)) {
                                if (value < from) continue;
                                values[values_size++] = value;
                        }
                }

                sprintf(number, "%ld", to);
                // SAFETY: no ulong can overflow the int size
                len = (int)strlen(number);
                for (int chunk_len = 1; chunk_len <= (len + 1) / 2;
                     ++chunk_len) {
                        long high_part = normalized_part(to, chunk_len);
                        int chunks     = (len + chunk_len - 1) / chunk_len;
                        if (chunks < 2) chunks = 2;
                        for (long value = invalid(high_part, chunks);
                             value <= to;
                             value = invalid(++high_part, chunks)) {
                                if (value < from) continue;
                                values[values_size++] = value;
                        }
                }
        }

        // SAFETY: values_size will not overflow
        assert(values_size > 0);
        qsort(values, (uint)values_size, sizeof(*values), cmp);
        int j = 0;
        for (int i = 1; i < values_size; ++i) {
                if (values[i] != values[j]) ++j;
                values[j] = values[i];
        }
        for (int i = 0; i <= j; ++i) {
                sum += values[i];
        }

        printf("sum: %ld\n", sum);
        if (strcmp(argv[1], "test") == 0) {
                assert(sum == 4174379265);
        }
        if (strcmp(argv[1], "input") == 0) {
                assert(sum < 44143124812);
        }
}

long
invalid(long part, int chunks)
{
        // int(part * chunks)
        char buf[40] = {0};
        sprintf(buf, "%ld", part);
        assert(atol(buf) == part);

        // SAFETY: buf will never be so long to overflow
        int len = (int)strlen(buf);
        for (int i = len; i < len * chunks; ++i) {
                buf[i] = buf[i - len];
        }
        // SAFETY: buf will never be so long to overflow
        assert((int)strlen(buf) == chunks * len);

        return atol(buf);
}

long
normalized_part(long number, int chunk_len)
{
        assert(chunk_len > 0);

        char buf[40] = {0};
        sprintf(buf, "%ld", number);
        // SAFETY: buf will never be so long to overflow
        int len = (int)strlen(buf);

        if (len % chunk_len == 0) {
                buf[chunk_len] = 0;
        } else {
                buf[0] = '1';
                for (int i = 1; i < chunk_len; ++i) buf[i] = '0';
                buf[chunk_len] = 0;
        }

        // SAFETY: buf will never be so long to overflow
        assert((int)strlen(buf) == chunk_len);

        return atol(buf);
}
