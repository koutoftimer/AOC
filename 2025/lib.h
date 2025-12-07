#pragma once

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
                        typeof(array.items) items =                        \
                            malloc(sizeof(*array.items) * array.capacity); \
                        memcpy(items, array.items,                         \
                               array.size * sizeof(*array.items));         \
                        array.items = items;                               \
                }                                                          \
                array.items[array.size++] = value;                         \
        } while (0)
