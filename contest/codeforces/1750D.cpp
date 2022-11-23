#include <bits/stdc++.h>

constexpr auto P = 998244353;

int norm(int x) {
  if (x < 0)
    x += P;
  if (x >= P)
    x -= P;
  return x;
}

template <class T, class U> T power(T a, U b) {
  T res = 1;
  for (; b; b /= 2, a *= a)
    if (b % 2)
      res *= a;
  return res;
}

struct Z {
  int x;
  Z(int x = 0) : x(norm(x)) {}
  Z(int64_t x) : x(norm(x % P)) {}
  int val() const { return x; }
  Z operator-() const { return Z(norm(P - x)); }
  Z inv() const {
    assert(x != 0);
    return power(*this, P - 2);
  }
  Z &operator*=(const Z &rhs) {
    x = int64_t(x) * rhs.x % P;
    return *this;
  }
  Z &operator+=(const Z &rhs) {
    x = norm(x + rhs.x);
    return *this;
  }
  Z &operator-=(const Z &rhs) {
    x = norm(x - rhs.x);
    return *this;
  }
  Z &operator/=(const Z &rhs) { return *this *= rhs.inv(); }
  friend Z operator*(const Z &lhs, const Z &rhs) {
    Z res = lhs;
    res *= rhs;
    return res;
  }
  friend Z operator+(const Z &lhs, const Z &rhs) {
    Z res = lhs;
    res += rhs;
    return res;
  }
  friend Z operator-(const Z &lhs, const Z &rhs) {
    Z res = lhs;
    res -= rhs;
    return res;
  }
  friend Z operator/(const Z &lhs, const Z &rhs) {
    Z res = lhs;
    res /= rhs;
    return res;
  }
  friend std::istream &operator>>(std::istream &is, Z &a) {
    int64_t v;
    is >> v;
    a = Z(v);
    return is;
  }
  friend std::ostream &operator<<(std::ostream &os, const Z &a) {
    return os << a.val();
  }
};

std::vector<std::array<int, 2>> divisors(int n) {
  std::vector<int> p;
  for (int i = 2; i * i <= n; i++) {
    if (n % i == 0) {
      p.push_back(i);
      while (n % i == 0) {
        n /= i;
      }
    }
  }
  if (n > 1) {
    p.push_back(n);
  }

  n = p.size();
  std::vector<std::array<int, 2>> a(1 << n);
  a[0] = {1, 1};
  for (int i = 1; i < (1 << n); i++) {
    int j = __builtin_ctz(i);
    auto [x, y] = a[i ^ (1 << j)];
    a[i] = {x * p[j], -y};
  }
  return a;
}

void solve() {
  int n, m;
  std::cin >> n >> m;

  std::vector<int> a(n);
  for (int i = 0; i < n; i++) {
    std::cin >> a[i];
  }
  for (int i = 1; i < n; i++) {
    if (a[i - 1] % a[i]) {
      std::cout << 0 << "\n";
      return;
    }
  }
  Z ans = 1;
  for (int i = 1; i < n; i++) {
    if (a[i] == a[i - 1]) {
      ans *= m / a[i];
    } else {
      Z res = 0;
      for (auto [x, u] : divisors(a[i - 1] / a[i])) {
        res += u * (m / a[i] / x);
      }
      ans *= res;
    }
  }
  std::cout << ans << "\n";
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int t;
  std::cin >> t;

  while (t--) {
    solve();
  }

  return 0;
}
