#pragma once

#include <stdio.h>
#include <stdlib.h>

static_assert(sizeof(long) == 8);

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define da_append(array, value)                                            \
        do {                                                               \
                if (array.items == NULL) {                                 \
                        array.size     = 0;                                \
                        array.capacity = 10;                               \
                        array.items =                                      \
                            malloc(sizeof(*array.items) * array.capacity); \
                }                                                          \
                if (array.size + 1 > array.capacity) {                     \
                        array.capacity *= 2;                               \
                        array.items =                                      \
                            reallocarray(array.items, array.capacity,      \
                                         sizeof(*array.items));            \
                }                                                          \
                array.items[array.size++] = value;                         \
        } while (0)

#define panic(message)                              \
        do {                                        \
                fprintf(stderr, "%s\n", (message)); \
                exit(1);                            \
        } while (0)
