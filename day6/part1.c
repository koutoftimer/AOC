#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "../lib.h"

static_assert(sizeof(long) == 8);

int
main(int argc, char *argv[])
{
        assert(argc == 2);
        FILE *f = fopen(argv[1], "r");
        assert(f);

        // struct stat st;
        // assert(stat(argv[1], &st) != 0);
        //
        // char *data = calloc(st.st_size + 1, sizeof(*data));
        // assert(fread(data, sizeof(*data), st.st_size, f) < 1);
        //
        // char *it = MIN(strchrnul(data, '+'), strchrnul(data, '*'));
        // while (it < data + st.st_size) {
        // }

        struct {
                long *items;
                size_t capacity;
                size_t size;
        } data = {0};

        {
                long element;
                int ret;
                while ((ret = fscanf(f, "%ld", &element)) > 0) {
                        da_append(data, element);
                }
        }

        struct {
                char *items;
                size_t capacity;
                size_t size;
        } operations = {0};

        {
                char operation;
                while (fscanf(f, "%c", &operation) > 0) {
                        if (operation == '+' || operation == '*') {
                                da_append(operations, operation);
                        }
                }
        }

        long grand_total = 0;
        for (uint op = 0; op < operations.size; ++op) {
                long sub_total = 0;
                if (operations.items[op] == '+') {
                        for (size_t i = op; i < data.size;
                             i += operations.size) {
                                sub_total += data.items[i];
                        }
                } else if (operations.items[op] == '*') {
                        sub_total = 1;
                        for (size_t i = op; i < data.size;
                             i += operations.size) {
                                sub_total *= data.items[i];
                        }
                }
                grand_total += sub_total;
        }

        printf("grand_total: %ld\n", grand_total);
        if (strcmp(argv[1], "test") == 0) {
                assert(grand_total == 4277556);
        }
        if (strcmp(argv[1], "input") == 0) {
                assert(grand_total == 8108520669952);
        }
}
