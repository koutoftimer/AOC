import unittest
import argparse


def solution(data: str) -> int:
    value = 50
    zero = 0

    for line in data.splitlines():
        direction, distance = line[0], int(line[1:])
        if direction == "L":
            distance = -distance
        value = (value + 100 + distance) % 100
        zero += value == 0

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
        self.assertEqual(solution(data), 3)

    def test_input(self):
        with open("input") as f:
            data = f.read()
            self.assertEqual(solution(data), 1071)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "input_file",
        type=argparse.FileType(mode="r"),
        help="File with test data",
    )
    args = parser.parse_args()
    print(solution(args.input_file.read()))
