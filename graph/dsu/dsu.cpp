#include <cctype>
#include <cstddef>
#include <cstdio>
#include <memory>

typedef unsigned long long ull;

namespace ReadOpt {

inline char nc() {
  static char buf[100000], *p1, *p2;
  return p1 == p2 && (static_cast<void>(p2 = (p1 = buf) +
                                             fread(buf, 1, 100000, stdin)),
                      p1 == p2)
             ? EOF
             : *p1++;
}
template <class T> inline void rd(T &x) {
  x = 0;
  int f = 1;
  char ch = nc();
  for (; !isdigit(ch); ch = nc())
    if (ch == '-')
      f = -1;
  for (; isdigit(ch); ch = nc())
    x = x * 10 - 48 + ch;
  x *= f;
}

} // namespace ReadOpt

namespace DSU {
class DSU {
private:
  std::unique_ptr<int[]> fa;
  int findFa(int);

public:
  explicit DSU(int n);
  void link(int u, int v) { fa[static_cast<size_t>(findFa(u))] = findFa(v); }
  bool isConnected(int u, int v) { return findFa(u) == findFa(v); }
};
} // namespace DSU

namespace DSU {
DSU::DSU(int n) {
  fa = std::make_unique<int[]>(static_cast<size_t>(n));
  for (int i = 0; i <= n; i++) {
    fa[static_cast<size_t>(i)] = i;
  }
}

int DSU::findFa(int u) {
  int &f = fa[static_cast<size_t>(u)];
  if (f == u) {
    return f;
  } else {
    f = findFa(f);
    return f;
  }
}
} // namespace DSU

int main() {
  freopen("test.in", "r", stdin);
  int n, m;
  ull ans = 0;
  const ull mod = 998244353ll;
  ReadOpt::rd(n);
  ReadOpt::rd(m);
  auto dsu = std::make_unique<DSU::DSU>(n);
  for (int i = 0; i < m; i++) {
    int op, u, v;
    ReadOpt::rd(op);
    ReadOpt::rd(u);
    ReadOpt::rd(v);
    switch (op) {
    case 0:
      dsu->link(u, v);
      break;
    case 1:
      ull x = static_cast<ull>(dsu->isConnected(u, v));
      ans = ans << 1 | x;
      ans %= mod;
      break;
    }
  }
  printf("%llu\n", ans);
  return 0;
}
