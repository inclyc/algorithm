#include <bits/stdc++.h>

using std::cin;
using std::cout;
using std::vector;

typedef vector<vector<int>> Graph;

constexpr int P = 1e9 + 7;

int norm(int x) {
  if (x < 0)
    x += P;
  if (x >= P)
    x -= P;
  return x;
}

template <class T, class U> T constexpr power(T a, U b) {
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
  explicit operator int() { return x; }
  Z operator-() const { return Z(norm(P - x)); }
  Z inv() const {
    assert(x != 0);
    return power(*this, P - 2);
  }

  Z pow(int pow) { return power(*this, pow); }

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

Z tarjan(int n, const Graph &G) {
  Z ans = 0;
  vector<int> dfn(n + 1), low(n + 1);
  vector<Z> f(n + 1);
  int nDFS = 0;
  std::function<void(int, int)> dfs = [&](int u, int parent) {
    f[u] = 1;
    Z tmp = 0;
    dfn[u] = low[u] = ++nDFS;
    for (auto v : G[u]) {
      if (v == parent)
        continue;
      if (!dfn[v]) {
        dfs(v, u);
        low[u] = std::min(low[u], low[v]);
        if (low[v] > dfn[u])
          f[v] /= 2;
        f[u] *= f[v] + 1;
        tmp += f[v];
      } else {
        low[u] = std::min(low[u], dfn[v]);
      }
    }
    f[u] += f[u] - 1;
    ans += f[u] - tmp;
  };
  dfs(1, -1);
  return ans;
}

int main() {
  std::ios::sync_with_stdio(false);
  cin.tie(0);
  int n, m;
  cin >> n >> m;
  Graph G(n + 1);
  for (int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    G[u].push_back(v);
    G[v].push_back(u);
  }
  cout << tarjan(n, G) * Z(2).pow(m) << '\n';
  return 0;
}
