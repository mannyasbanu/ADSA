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

// GREATER : reversed input vectors
bool isGreater(const vector<int>& v1, const vector<int>& v2){
  if(v1.size() != v2.size()) return v1.size() > v2.size();
  if(v1.size() == 0) return false;
  for(int i = v1.size() - 1; i >= 0; --i){
    if(v1[i] != v2[i]) return v1[i] > v2[i];
  }
  return false;
}

// EQUAL : reversed input vectors
bool isEqual(const vector<int>& v1, const vector<int>& v2){
  if(v1.size() != v2.size()) return false;
  for(int i = 0; i < v1.size(); ++i){
    if(v1[i] != v2[i]) return false;
  }
  return true;
}

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

// SCHOOL DIVISION : reversed input and output vectors
vector<int> div(const vector<int>& v1, const vector<int>& v2, int base){
  // handle division by zero
  if(v2 == vector<int>({0})) return {0};
  // early exit
  if(isGreater(v2,v1)) return {0};
  vector<int> res;
  vector<int> dividend = {};
  // traverse from rightmost digit
  for(int i = v1.size() - 1; i >= 0; --i){
    // bring down digit
    dividend.insert(dividend.begin(), v1[i]);
    trim(dividend);
    // count times divisor in dividend
    vector<int> count = {0};
    vector<int> sum = {0};
    vector<int> preSum = {0};
    while (isGreater(dividend, sum) || isEqual(dividend, sum)) {
      preSum = sum;
      sum = add(sum, v2, base);
      if(isGreater(dividend, sum) || isEqual(dividend, sum)){
        count = add(count, {1}, base);
      }
    }
    // get remainder
    dividend = sub(dividend, preSum, base);
    // add quotient to result
    res.insert(res.begin(), count[0]);
  }
  trim(res);
  return res;
}

#include <sstream>
int main(){
  // read input
  string num1, num2;
  int base;
  cin >> num1 >> num2 >> base;
  // convert to reversed vector of digits
  vector<int> v1, v2;
  for(int i = num1.size() - 1; i >= 0; --i){
    v1.push_back(num1[i] - '0');
  }
  for(int i = num2.size() - 1; i >= 0; --i){
    v2.push_back(num2[i] - '0');
  }
  // perform operations
  vector<int> sum = add(v1, v2, base);
  vector<int> prod = mult(v1, v2, base);
  vector<int> quot = div(v1, v2, base);
  // trim zeroes
  trim(sum);
  trim(prod);
  trim(quot);
  // print results
  for(int i = sum.size() - 1; i >= 0; --i){
    cout << sum[i];
  }
  cout << " ";
  for(int i = prod.size() - 1; i >= 0; --i){
    cout << prod[i];
  }
  cout << " ";
  for(int i = quot.size() - 1; i >= 0; --i){
    cout << quot[i];
  }
  return 0;
}