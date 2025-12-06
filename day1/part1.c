#include <assert.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  assert(argc == 2);

  FILE *f = fopen(argv[1], "r");
  char direction;
  int distance, value = 50, password = 0;

  while (fscanf(f, "%c%d\n", &direction, &distance) != EOF) {
    assert(direction == 'L' || direction == 'R');
    value = (value + (direction == 'L' ? -distance : distance)) % 100;
    password += value == 0;
  }

  printf("password: %d\n", password);
}
