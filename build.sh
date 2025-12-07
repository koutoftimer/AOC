FLAGS=(-std=gnu23)
# turn on debug build
FLAGS=(-ggdb3)
# forbid silent type conversion that may loose precision
FLAGS+=(-Wconversion)
FLAGS+=(-Wunused)
FLAGS+=(-Wall -Wextra -pedantic)
# make all warnings errros
FLAGS+=(-Werror)

gcc -I.. ${FLAGS[@]} run.c
