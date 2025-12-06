#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{
        assert(argc == 2);

        FILE *f = fopen(argv[1], "r");
        char direction;
        int64_t distance, value = 50, old_value, password = 0;

        while (fscanf(f, "%c%ld\n", &direction, &distance) != EOF) {
                assert(direction == 'L' || direction == 'R');
                assert(distance > 0);

                // full circles
                password += distance / 100;
                distance %= 100;
                assert(distance < 100);

                old_value = value;
                value += direction == 'L' ? -distance : distance;
                if (old_value != 0 && (value <= 0 || value >= 100)) ++password;
                value = (value + 100) % 100;
                assert(value >= 0);
                assert(value < 100);
        }

        printf("password: %ld\n", password);
}
