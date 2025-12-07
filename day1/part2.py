import unittest
import argparse


def solution(data: str) -> int:
    value = 50
    zero = 0

    for line in data.splitlines():
        direction, distance = line[0], int(line[1:])
        if direction == "L":
            distance = -distance
        value += distance
        zero += value == 0 or not 0 <= value < 100
        value %= 100

    return zero


class Tests(unittest.TestCase):
    def test_example(self):
        data = """\
L68
L30
R48
L5
R60
L55
L1
L99
R14
L82
"""
        self.assertEqual(solution(data), 6)

    def test_input(self):
        with open("input") as f:
            data = f.read()
            self.assertEqual(solution(data), 6700)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "input_file",
        type=argparse.FileType(mode="r"),
        help="File with test data",
    )
    args = parser.parse_args()
    print(solution(args.input_file.read()))
