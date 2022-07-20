#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#define debug(...) fprintf(stderr, __VA_ARGS__)
const int maxn = 1e5 + 10;
inline char nc() {
  static char buf[100000], *p1, *p2;
  return p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, 100000, stdin), p1 == p2)
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
typedef unsigned uint;

uint seed = 1;
uint rnd() {
  seed ^= seed << 13;
  seed ^= seed >> 17;
  seed ^= seed << 5;
  return seed;
}

struct ZipTree {
  uint ww[maxn];
  int ls[maxn], rs[maxn], size[maxn], val[maxn], alc, root;
  inline void newnode(int &x, int v) {
    x = ++alc;
    ww[alc] = rnd();
    val[alc] = v;
    size[alc] = 1;
  }
  inline void mt(int x) {
    if (!x)
      return;
    size[x] = ls[x][size] + rs[x][size] + 1;
  }
  void merge(int &x, int l, int r) {
    if (!l || !r)
      x = l ^ r;
    else if (ww[l] < ww[r])
      x = l, merge(rs[x], rs[x], r), mt(x);
    else
      x = r, merge(ls[x], l, ls[x]), mt(x);
  }
  void split(int x, int &l, int &r, int k) {
    if (!k)
      l = 0, r = x;
    else if (k == x[size])
      l = x, r = 0;
    else if (k <= ls[x][size])
      r = x, split(ls[x], l, ls[x], k), mt(x);
    else
      l = x, split(rs[x], rs[x], r, k - ls[x][size] - 1), mt(x);
  }
  int rank(int x, int w) {
    if (!x)
      return 0;
    if (w <= val[x])
      return rank(ls[x], w);
    return rank(rs[x], w) + ls[x][size] + 1;
  }
  inline void insert(int v) {
    int x, y, tmp = 0, rk = rank(root, v);
    split(root, x, y, rk);
    newnode(tmp, v);
    merge(x, x, tmp);
    merge(root, x, y);
  }
  inline int find(int d) {
    int x, y, z, ans;
    split(root, x, y, d);
    split(x, z, x, d - 1);
    ans = val[x];
    merge(x, z, x);
    merge(root, x, y);
    return ans;
  }
  inline void del(int v) {
    int x, y, tmp, rk = rank(root, v) + 1;
    split(root, x, y, rk);
    split(x, x, tmp, rk - 1);
    merge(root, x, y);
  }
  inline int pre(int v) {
    int x, y, z, ans, rk = rank(root, v);
    split(root, x, y, rk);
    split(x, z, x, rk - 1);
    ans = val[x];
    merge(x, z, x);
    merge(root, x, y);
    return ans;
  }
  inline int suc(int v) {
    int x, y, z, ans, rk = rank(root, v + 1);
    split(root, x, y, rk + 1);
    split(x, z, x, rk);
    ans = val[x];
    merge(x, z, x);
    merge(root, x, y);
    return ans;
  }
} zip;
const int inf = 0x3f3f3f3f;

int main() {
  srand((unsigned long long)(new char));
  int n;
  rd(n);
  zip.ww[0] = zip.val[0] = inf;
  for (int i = 1, op, x; i <= n; i++) {
    rd(op);
    rd(x);
    if (op == 1)
      zip.insert(x);
    else if (op == 2)
      zip.del(x);
    else if (op == 3)
      printf("%d\n", zip.rank(zip.root, x) + 1);
    else if (op == 4)
      printf("%d\n", zip.find(x));
    else if (op == 5)
      printf("%d\n", zip.pre(x));
    else
      printf("%d\n", zip.suc(x));
  }
}