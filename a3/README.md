Testing Workflow:

This mirrors the `a1` / `a2` layout, but `compare` uses a Python validator instead of plain `diff` because Assignment 3 allows multiple valid `PART2_EDGES` answers.

1. Add or edit matrices in `all_tests.txt`
2. Run `bash generate.sh`
3. Run `python3 generate.py`
4. Run `bash test.sh`
5. Run `bash compare.sh`

Folder layout:

- `all_tests.txt`: master list of matrix test cases separated by blank lines
- `tests/`: one generated `matrix.txt` input per test
- `expected/`: generated reference summaries for each test
- `outputs/`: program output captured from your `a3.cpp` or `main.cpp`

Input format:

Each test block in `all_tests.txt` is a full `matrix.txt` file:

- First line: `n`
- Next `n` lines: the `n x n` probability matrix
- Blank lines separate tests
- Lines starting with `#` are ignored by `generate.sh`

Validation rules:

- Output must contain exactly 3 lines:
  `PART1:`
  `PART2_EDGES:`
  `PART2_PROB:`
- Probabilities must be printed with exactly 6 decimal places.
- Returned edges must be valid positive directed non-self edges from the original matrix.
- Duplicate edges are rejected.
- `PART1` is recomputed independently with a max-probability Dijkstra reference.
- `PART2_PROB` is checked twice:
  against the probability obtained by removing the returned edges, and
  against the exact optimal answer from brute-forcing all 3-edge removals on these small test graphs.

Notes:

- `test.sh` compiles `main.cpp` if it exists, otherwise it compiles `a3.cpp`.
- The `n = 2` test is kept on purpose. Since that graph has fewer than 3 positive directed edges, the validator relaxes the edge-count rule for that case and requires all available positive edges to be removed.
