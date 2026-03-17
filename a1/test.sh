g++ -std=c++11 -O2 a1.cpp -o a1.out

for f in tests/*.txt; do
  name=$(basename "$f")
  cat "$f" | ./a1.out > outputs/$name
done