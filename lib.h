#pragma once

#include <stdio.h>
#include <stdlib.h>

static_assert(sizeof(long) == 8);
static_assert(sizeof(ulong) == 8);
static_assert(sizeof(int) == 4);
static_assert(sizeof(uint) == 4);

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/**
 * Macro allows to append values to dynamically allocated arrays.
 * This macro works correctly with zero initialized arrays.
 *
 * `array` is a stuct with the following fields:
 *
 *      struct {
 *              ElementType * items;
 *              size_t        capacity;
 *              size_t        size;
 *      }
 */
#define da_append(array, value)                                             \
        do {                                                                \
                static_assert(_Generic(array.size, size_t: 1, default: 0)); \
                static_assert(                                              \
                    _Generic(array.capacity, size_t: 1, default: 0));       \
                static_assert(                                              \
                    _Generic(*array.items, typeof(value): 1, default: 0));  \
                if (array.items == NULL) {                                  \
                        array.size     = 0;                                 \
                        array.capacity = 10;                                \
                        array.items =                                       \
                            malloc(sizeof(*array.items) * array.capacity);  \
                }                                                           \
                if (array.size + 1 > array.capacity) {                      \
                        array.capacity *= 2;                                \
                        array.items =                                       \
                            reallocarray(array.items, array.capacity,       \
                                         sizeof(*array.items));             \
                }                                                           \
                array.items[array.size++] = value;                          \
        } while (0)

/**
 * This macro helps defining dynamic arrays for ergonomic reasons.
 *
 * Example:
 *
 *     da_define(int) array = {0};
 */
#define da_define(type)          \
        struct {                 \
                type *items;     \
                size_t size;     \
                size_t capacity; \
        }

#define da_foreach(array, it, expression)          \
        for (typeof(array.items) it = array.items; \
             it < array.items + array.size; ++it)  \
                expression;

#define panic(message)                              \
        do {                                        \
                fprintf(stderr, "%s\n", (message)); \
                exit(1);                            \
        } while (0)

#define swap(a, b)                                 \
        do {                                       \
                typeof(a) __swap_tmp = a;          \
                a                    = b;          \
                b                    = __swap_tmp; \
        } while (0)
