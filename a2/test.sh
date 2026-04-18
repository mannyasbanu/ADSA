mkdir -p outputs

g++ -std=c++11 -O2 a2.cpp -o a2.out

for f in tests/*.txt; do
  name=$(basename "$f")
  ./a2.out < "$f" > "outputs/$name"
done
