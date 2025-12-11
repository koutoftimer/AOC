#include <assert.h>
#include <lib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { MAP_SIZE = 1009 };

uint keys[MAP_SIZE]              = {0};
da_define(uint) values[MAP_SIZE] = {0};

uint
hash(char *name)
{
        return ((uint)name[0] << 0) + ((uint)name[1] << 8) +
               ((uint)name[2] << 16);
}

uint
hashmap_index(uint key)
{
        uint index = key % MAP_SIZE;
        while (keys[index] != 0 && keys[index] != key) {
                index = (index + 1) % MAP_SIZE;
        }
        return index;
}

void
hashmap_add(uint key, uint value)
{
        uint index  = hashmap_index(key);
        keys[index] = key;
        da_append(values[index], value);
}

uint
r(uint key)
{
        if (key == hash("out")) return 1;

        uint index = hashmap_index(key);
        uint sum   = 0;
        da_foreach(values[index], it, sum += r(*it));
        return sum;
}

int
main(int argc, char *argv[])
{
        assert(argc == 2);
        FILE *f = fopen(argv[1], "r");
        assert(f);

        char *line;
        size_t n;
        while (getline(&line, &n, f) > 0) {
                uint key = hash(line);
                line += 4;
                while (line) {
                        line++;
                        uint value = hash(line);
                        hashmap_add(key, value);
                        line = strchr(line, ' ');
                }
        }

        uint res = r(hash("you"));
        printf("res: %u\n", res);

        if (strcmp(argv[1], "test") == 0) {
                assert(res == 5);
        }
        if (strcmp(argv[1], "input") == 0) {
                assert(res == 448);
        }
}
