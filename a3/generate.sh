mkdir -p tests

index_file="tests/index.txt"
: > "$index_file"

i=1
buffer=""

flush_case() {
  if [[ -z "$buffer" ]]; then
    return
  fi

  name="t${i}.txt"
  printf "%s\n" "$buffer" > "tests/$name"
  printf "%s\n" "$name" >> "$index_file"

  buffer=""
  ((i++))
}

while IFS= read -r line || [[ -n "$line" ]]; do
  if [[ "$line" =~ ^[[:space:]]*# ]]; then
    continue
  fi

  if [[ "$line" =~ ^[[:space:]]*$ ]]; then
    flush_case
    continue
  fi

  if [[ -z "$buffer" ]]; then
    buffer="$line"
  else
    buffer+=$'\n'"$line"
  fi
done < all_tests.txt

flush_case
