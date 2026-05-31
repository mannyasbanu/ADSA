import heapq
import math
import os
import re
import sys


EDGE_LINE_RE = re.compile(r"^PART2_EDGES:(?: \(\d+,\d+\))*$")
PROB_LINE_RE = re.compile(r"^(PART1|PART2_PROB): ([0-9]+\.[0-9]{6})$")
EDGE_RE = re.compile(r"\((\d+),(\d+)\)")
TOLERANCE = 1e-6


def iter_test_names():
    with open("tests/index.txt", "r", encoding="utf-8") as index_file:
        for line in index_file:
            name = line.strip()
            if name:
                yield name


def load_matrix(path):
    with open(path, "r", encoding="utf-8") as test_file:
        rows = [line.strip() for line in test_file if line.strip()]

    n = int(rows[0])
    matrix = [[float(value) for value in row.split()] for row in rows[1:]]

    if len(matrix) != n:
        raise ValueError(f"{path}: expected {n} matrix rows, found {len(matrix)}")

    for row in matrix:
        if len(row) != n:
            raise ValueError(f"{path}: matrix row has length {len(row)}, expected {n}")

    return matrix


def load_expected(path):
    values = {}

    with open(path, "r", encoding="utf-8") as expected_file:
        for line in expected_file:
            key, value = line.rstrip("\n").split(": ", 1)
            values[key] = value

    values["PART1"] = float(values["PART1"])
    values["PART2_PROB"] = float(values["PART2_PROB"])
    values["AVAILABLE_POSITIVE_EDGES"] = int(values["AVAILABLE_POSITIVE_EDGES"])
    values["REQUIRED_REMOVALS"] = int(values["REQUIRED_REMOVALS"])
    return values


def positive_edges(matrix):
    n = len(matrix)
    edges = set()

    for i in range(n):
        for j in range(n):
            if i != j and matrix[i][j] > 0.0:
                edges.add((i + 1, j + 1))

    return edges


def solve_part1(matrix):
    n = len(matrix)
    best = [0.0] * n
    best[0] = 1.0
    heap = [(-1.0, 0)]

    while heap:
        neg_prob, node = heapq.heappop(heap)
        prob = -neg_prob

        if prob + 1e-15 < best[node]:
            continue

        for to, edge_prob in enumerate(matrix[node]):
            if to == node or edge_prob <= 0.0:
                continue

            next_prob = prob * edge_prob
            if next_prob > best[to] + 1e-15:
                best[to] = next_prob
                heapq.heappush(heap, (-next_prob, to))

    return best[-1]


def remove_edges(matrix, edges):
    clone = [row[:] for row in matrix]

    for u, v in edges:
        clone[u - 1][v - 1] = 0.0

    return clone


def nearly_equal(a, b):
    return abs(a - b) <= TOLERANCE


def parse_output(path):
    with open(path, "r", encoding="utf-8") as output_file:
        lines = output_file.read().splitlines()

    if len(lines) != 3:
        raise AssertionError(f"expected exactly 3 output lines, found {len(lines)}")

    part1_match = PROB_LINE_RE.fullmatch(lines[0])
    if not part1_match or part1_match.group(1) != "PART1":
        raise AssertionError("line 1 must be exactly 'PART1: <probability>' with 6 decimals")

    if not EDGE_LINE_RE.fullmatch(lines[1]):
        raise AssertionError("line 2 must be exactly 'PART2_EDGES: (u,v) ...'")

    part2_match = PROB_LINE_RE.fullmatch(lines[2])
    if not part2_match or part2_match.group(1) != "PART2_PROB":
        raise AssertionError("line 3 must be exactly 'PART2_PROB: <probability>' with 6 decimals")

    edges = [(int(u), int(v)) for u, v in EDGE_RE.findall(lines[1])]
    return float(part1_match.group(2)), edges, float(part2_match.group(2))


def validate_case(name):
    matrix = load_matrix(os.path.join("tests", name))
    expected = load_expected(os.path.join("expected", name))
    printed_part1, removed_edges, printed_part2 = parse_output(os.path.join("outputs", name))

    valid_edges = positive_edges(matrix)
    required_removals = expected["REQUIRED_REMOVALS"]
    n = len(matrix)

    if len(removed_edges) != required_removals:
        raise AssertionError(
            f"expected {required_removals} returned edges, found {len(removed_edges)}"
        )

    if len(set(removed_edges)) != len(removed_edges):
        raise AssertionError("duplicate edges are not allowed")

    for u, v in removed_edges:
        if not (1 <= u <= n and 1 <= v <= n):
            raise AssertionError(f"edge ({u},{v}) is outside the 1..{n} node range")
        if u == v:
            raise AssertionError(f"edge ({u},{v}) is a self-loop")
        if (u, v) not in valid_edges:
            raise AssertionError(f"edge ({u},{v}) is not a positive directed edge in the input")

    recomputed_part1 = solve_part1(matrix)
    if not nearly_equal(printed_part1, recomputed_part1):
        raise AssertionError(
            f"PART1 mismatch: printed {printed_part1:.6f}, recomputed {recomputed_part1:.6f}"
        )

    if not nearly_equal(printed_part1, expected["PART1"]):
        raise AssertionError(
            f"PART1 is not optimal: printed {printed_part1:.6f}, expected {expected['PART1']:.6f}"
        )

    recomputed_part2 = solve_part1(remove_edges(matrix, removed_edges))
    if not nearly_equal(printed_part2, recomputed_part2):
        raise AssertionError(
            f"PART2_PROB mismatch: printed {printed_part2:.6f}, recomputed {recomputed_part2:.6f}"
        )

    if not nearly_equal(printed_part2, expected["PART2_PROB"]):
        raise AssertionError(
            f"PART2_PROB is not optimal: printed {printed_part2:.6f}, expected {expected['PART2_PROB']:.6f}"
        )


def main():
    failures = []

    for name in iter_test_names():
        try:
            validate_case(name)
            print(f"PASS: {name}")
        except Exception as exc:
            failures.append((name, str(exc)))
            print(f"FAIL: {name} - {exc}")

    if failures:
        sys.exit(1)

    print("compared all files")


if __name__ == "__main__":
    main()
