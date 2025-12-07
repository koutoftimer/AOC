import unittest
import argparse


def solution(data: str) -> None:
    pass


class Tests(unittest.TestCase):
    def test_example(self):
        data = """\
"""
        self.assertEqual(solution(data), "expected")

    def test_input(self):
        with open("input") as f:
            data = f.read()
            self.assertEqual(solution(data), "expected")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "input_file",
        type=argparse.FileType(mode="r"),
        help="File with test data",
    )
    args = parser.parse_args()
    print(solution(args.input_file.read()))
