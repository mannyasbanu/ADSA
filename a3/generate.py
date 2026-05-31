import heapq
import itertools
import math
import os


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
    matrix = []

    for row in rows[1:]:
        matrix.append([float(value) for value in row.split()])

    if len(matrix) != n:
        raise ValueError(f"{path}: expected {n} matrix rows, found {len(matrix)}")

    for row in matrix:
        if len(row) != n:
            raise ValueError(f"{path}: matrix row has length {len(row)}, expected {n}")

    return matrix


def positive_edges(matrix):
    n = len(matrix)
    edges = []

    for i in range(n):
        for j in range(n):
            if i != j and matrix[i][j] > 0.0:
                edges.append((i + 1, j + 1))

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


def format_edges(edges):
    if not edges:
        return "NONE"
    return " ".join(f"({u},{v})" for u, v in edges)


def exact_part2(matrix):
    edges = positive_edges(matrix)
    removal_count = min(3, len(edges))

    best_prob = math.inf
    best_choice = ()

    for choice in itertools.combinations(edges, removal_count):
        prob = solve_part1(remove_edges(matrix, choice))
        if prob + 1e-15 < best_prob:
            best_prob = prob
            best_choice = choice

    return removal_count, best_prob, best_choice


def main():
    os.makedirs("expected", exist_ok=True)

    for name in iter_test_names():
        matrix = load_matrix(os.path.join("tests", name))
        part1 = solve_part1(matrix)
        removal_count, part2, optimal_edges = exact_part2(matrix)
        edge_count = len(positive_edges(matrix))

        output_path = os.path.join("expected", name)
        with open(output_path, "w", encoding="utf-8") as expected_file:
            expected_file.write(f"PART1: {part1:.6f}\n")
            expected_file.write(f"PART2_PROB: {part2:.6f}\n")
            expected_file.write(f"AVAILABLE_POSITIVE_EDGES: {edge_count}\n")
            expected_file.write(f"REQUIRED_REMOVALS: {removal_count}\n")
            expected_file.write(f"ONE_OPTIMAL_SET: {format_edges(optimal_edges)}\n")


if __name__ == "__main__":
    main()
