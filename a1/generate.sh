mkdir -p tests

i=1
while IFS= read -r line; do
  [[ -z "$line" || "$line" =~ ^# ]] && continue
  echo "$line" > tests/t$i.txt
  ((i++))
done < all_tests.txt