#include <assert.h>
#include <lib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef da_define(uint) Button;
typedef da_define(Button) ButtonRow;

da_define(char *) lights     = {0};
da_define(ButtonRow) buttons = {0};
da_define(ulong) joltage     = {0};

const char light_toggle      = '#' ^ '.';

ulong
extract_lights(char *start, char *end)
{
        ulong res = *start == '#';
        while (start < end) {
                res <<= 1;
                res |= *start == '#';
                ++start;
        }
        return res;
}

Button
extract_button(char *it)
{
        Button flips = {0};
        uint value;
        int nread;
        while ((nread = sscanf(it, "%u,", &value)) > 0) {
                da_append(flips, value);
                it += nread + 1;
        }
        return flips;
}

void
flip_lights(char *lights, uint row, uint index)
{
        ButtonRow *brow = buttons.items + row;
        Button *button  = brow->items + index;
        for (uint *flip = button->items; flip < button->items + button->size;
             ++flip) {
                lights[*flip] ^= light_toggle;
        }
}

uint
r(char *state, uint row, uint to_flip, uint steps)
{
        assert(row < lights.size);

        if (to_flip >= buttons.items[row].size) return INT32_MAX;
        if (strchr(state, '#') == NULL) return steps;

        uint a = r(strdup(state), row, to_flip + 1, steps);

        flip_lights(state, row, to_flip);
        if (strchr(state, '#') == NULL) return steps + 1;

        uint b = r(state, row, to_flip + 1, steps + 1);

        return MIN(a, b);
}

int
main(int argc, char *argv[])
{
        assert(argc == 2);
        FILE *f = fopen(argv[1], "r");
        assert(f);

        char data[100500] = {0};
        fread(data, sizeof(data), 1, f);

        char *it        = strchrnul(data, '[') + 1;
        char *const end = data + strlen(data);
        while (it < end) {
                // extract lights
                char *bits_end = strchrnul(it, ']');
                // SAFETY: bits_end guarantied to be > it
                da_append(lights, strndup(it, (ulong)(bits_end - it)));

                // extract buttons
                it                = strchrnul(it, '(') + 1;
                char *buttons_end = strchrnul(it, '{');
                ButtonRow row     = {0};
                while (it < buttons_end) {
                        da_append(row, extract_button(it));
                        it = strchrnul(it, '(') + 1;
                }
                da_append(buttons, row);

                // extract joltage
                // ...

                it = strchrnul(buttons_end, '[') + 1;
        }

        uint res = 0;
        for (uint row = 0; row < lights.size; ++row) {
                res += r(lights.items[row], row, 0, 0);
        }

        printf("res: %u\n", res);

        if (strcmp(argv[1], "test") == 0) {
                assert(res == 7);
        }
        if (strcmp(argv[1], "input") == 0) {
                assert(res == 517);
        }
}
