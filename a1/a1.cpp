#include <iostream>
#include <vector>
using namespace std;

//! HELPER FUNCTIONS !//

// TRIM : remove trailing zeros from reversed vector
void trim(vector<int>& v) {
  while (v.size() > 1 && v.back() == 0)
    v.pop_back();
}

// SUB : reversed input and output vectors
vector<int> sub(const vector<int>& v1, const vector<int>& v2, int base){
  int borrow = 0, i = 0;
  vector<int> res;
  res.reserve(max(v1.size(), v2.size()));
  while(i < v1.size() || i < v2.size()){
    // extract digits
    int a = (i < v1.size()) ? v1[i] : 0;
    int b = (i < v2.size() ? v2[i] : 0);
    // get diff
    int diff = a - b - borrow;
     // append res and update borrow
    if(diff < 0){
      diff += base;
      borrow = 1;
    } else {
      borrow = 0;
    }
    res.push_back(diff);
    // next digit
    ++i;
  }
  trim(res);
  return res;
};

// INTEGER ADDITION SCHOOL METHOD : reversed input and output vectors
vector<int> add(const vector<int>& v1, const vector<int>& v2, int base){
  int carry = 0, i = 0;
  vector<int> res;
  res.reserve(max(v1.size(), v2.size()) + 1);
  while(i < v1.size() || i < v2.size()){
    // extract digits
    int a = (i < v1.size()) ? v1[i] : 0;
    int b = (i < v2.size() ? v2[i] : 0);
    // get sum
    int sum = a + b + carry;
    // update carry and append res
    carry = sum / base;
    sum = sum % base;
    res.push_back(sum);
    // next digit
    ++i;
  }
  if(carry) res.push_back(carry);
  trim(res);
  return res;
};

// KARATSUBA : reversed input and output vectors
vector<int> mult(const vector<int>& v1, const vector<int>& v2, int base){
  // base case
  if(v1.size() == 1 || v2.size() == 1){
    // ensure v1 is longer
    const vector<int>& s = (v1.size() == 1) ? v1 : v2;
    const vector<int>& l = (v1.size() == 1) ? v2 : v1;
    vector<int> res;
    int carry = 0;
    for(int i = 0; i < l.size(); ++i){
      int prod = l[i] * s[0] + carry;
      res.push_back(prod % base);
      carry = prod / base;
    }
    if(carry) res.push_back(carry);
    return res;
  }
  int m = min(v1.size(), v2.size()) / 2;
  // split vectors
  vector<int> l1(v1.begin(), v1.begin() + m);
  vector<int> h1(v1.begin() + m, v1.end());
  vector<int> l2(v2.begin(), v2.begin() + m);
  vector<int> h2(v2.begin() + m, v2.end());
  // recurse
  vector<int> a = mult(l1, l2, base);
  vector<int> c = mult(h1, h2, base);
  vector<int> b = mult(add(l1,h1,base), add(l2,h2,base), base);
  // mid term
  vector<int> mid = sub(sub(b,c,base),a,base);
  // Combine results : a + mid * base^m + c * base^(2*m)
  // build shifted mid
  vector<int> midShift(m, 0);
  midShift.insert(midShift.end(), mid.begin(), mid.end());
  // build shifted c
  vector<int> cShift(2*m, 0);
  cShift.insert(cShift.end(), c.begin(), c.end());
  // sum all
  vector<int> res = add(add(a, midShift, base), cShift, base);
  trim(res);
  return res;
}

int main(){
  vector<int> a = {3,4,5};
  vector<int> b = {1,2,9};
  vector<int> res = mult(a,b,10);
  trim(res);
  for(int n : res){
    cout << n;
  }
  cout << endl;
  return 0;
}