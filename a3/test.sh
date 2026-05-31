mkdir -p outputs

source_file="a3.cpp"
if [[ -f "main.cpp" ]]; then
  source_file="main.cpp"
fi

g++ -std=c++17 -O2 -Wall "$source_file" -o a3.out

original_exists=0
original_contents=""

if [[ -f "matrix.txt" ]]; then
  original_exists=1
  original_contents="$(cat matrix.txt)"
fi

while IFS= read -r name; do
  [[ -z "$name" ]] && continue

  cat "tests/$name" > matrix.txt
  ./a3.out > "outputs/$name"
done < tests/index.txt

if [[ "$original_exists" -eq 1 ]]; then
  printf "%s\n" "$original_contents" > matrix.txt
fi
