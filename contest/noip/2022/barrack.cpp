#include <bits/stdc++.h>

using std::cin;
using std::cout;
using std::stack;
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
  explicit operator int() { return x; }
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

auto tarjan(int n, const Graph &G) {
  int nDfs = 0, nBcc = 0;
  vector<int> dfn(n + 1);
  vector<int> low(n + 1);
  vector<int> mp(n + 1);
  stack<int> st;
  std::function<void(int, int)> dfs = [&](int u, int fa) {
    dfn[u] = ++nDfs;
    low[u] = dfn[u];
    st.push(u);
    for (auto v : G[u]) {
      if (v == fa)
        continue;
      if (!dfn[v]) {
        dfs(v, u);
        low[u] = std::min(low[u], low[v]);
      } else {
        low[u] = std::min(dfn[v], low[u]);
      }
    }

    if (dfn[u] == low[u]) {
      ++nBcc;
      int x;
      do {
        x = st.top();
        st.pop();
        mp[x] = nBcc;
      } while (x != u);
    }
  };

  dfs(1, -1);
  return mp;
}

auto rebuildGraph(int n, const vector<int> &mp, const Graph &oldG)
    -> std::tuple<int, Graph, vector<int>, vector<int>> {
  Graph newG(n + 1);
  vector<int> nW(n + 1), eW(n + 1);

  for (int u = 1; u <= n; u++) {
    auto mU = mp[u];
    nW[mU]++;
    for (auto v : oldG[u]) {
      auto mV = mp[v];
      if (mU != mV)
        newG[mU].push_back(mV);
      if (mU == mV && u < v)
        eW[mU]++;
    }
  }
  return {n, newG, nW, eW};
}

auto treeDp(int n, const Graph &G, const vector<int> &nW,
            const vector<int> &eW) {
  vector<Z> D(n + 1), E(n + 1), F(n + 1);
  vector<bool> vis(n + 1);
  std::function<void(int)> dfs = [&](int u) {
    vis[u] = true;
    Z V = power(Z(2), eW[u]);
    Z W = power(Z(2), nW[u] + eW[u]) - V;
    E[u] = W;
    F[u] = V;
    D[u] = W;
    for (auto v : G[u]) {
      if (vis[v])
        continue;
      dfs(v);
      D[u] = 2 * D[v] * F[u] + 2 * D[u] * F[v] + E[u] * E[v];
      E[u] = (E[v] + 2 * F[v]) * E[u] + F[u] * E[v];
      F[u] *= 2 * F[v];
    }
  };

  dfs(1);
  return D[1];
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0);
  int n, m;
  cin >> n >> m;
  Graph G(n + 1);
  for (int i = 1; i <= n; i++)
    G[i].clear();
  for (int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    G[u].push_back(v);
    G[v].push_back(u);
  }
  auto s = tarjan(n, G);
  auto [newN, nG, nW, eW] = rebuildGraph(n, s, G);
  cout << treeDp(newN, nG, nW, eW) << std::endl;
  return 0;
}
