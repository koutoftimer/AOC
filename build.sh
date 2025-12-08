# Usage: sh path/to/build.sh [path/to/solution.c] && ./a.out <path/to/test>


FLAGS=(-std=gnu23)
# turn on debug build
FLAGS=(-ggdb3)
# forbid silent type conversion that may loose precision
FLAGS+=(-Wconversion)
FLAGS+=(-Wunused)
FLAGS+=(-Wall -Wextra -pedantic)
# make all warnings errros
FLAGS+=(-Werror)

solution="$1"
if [[ "$solution" == "" ]]
then
  solution="run.c"
fi

gcc -I.. -I. ${FLAGS[@]} "$solution"
