for f in tests/*.txt; do
  name=$(basename "$f")
  diff expected/$name outputs/$name || echo "FAIL: $name"
done
echo "compared all files"