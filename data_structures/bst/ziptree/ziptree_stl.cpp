#include <bits/stdc++.h>

using std::cin;
using std::cout;
using std::vector;

uint32_t seed = 1;
uint32_t rnd() {
  seed ^= seed << 13;
  seed ^= seed >> 17;
  seed ^= seed << 5;
  return seed;
}

struct ZipTree {

  struct Node {
    uint32_t weight;
    uint32_t l, r;
    uint32_t size;
    int val;
  };

  vector<Node> nodes;
  uint32_t root;
  ZipTree() { root = 0; }

  uint32_t &ls(uint32_t x) { return nodes[x].l; }
  uint32_t &rs(uint32_t x) { return nodes[x].r; }
  uint32_t &sz(uint32_t x) { return nodes[x].size; }
  uint32_t &ww(uint32_t x) { return nodes[x].weight; }
  int &val(uint32_t x) { return nodes[x].val; }

  void newnode(uint32_t &x, int v) {
    x = nodes.size();
    nodes.push_back({rnd(), 0, 0, 1, v});
  }

  void mt(uint32_t x) {
    auto &n = nodes[x];
    n.size = nodes[n.r].size + nodes[n.l].size + 1;
  }

  void merge(uint32_t &x, uint32_t l, uint32_t r) {
    if (!l || !r)
      x = l ^ r;
    else if (ww(l) < ww(r)) {
      x = l;
      merge(rs(x), rs(x), r);
      mt(x);
    } else {
      x = r;
      merge(ls(x), l, ls(x));
      mt(x);
    }
  }

  void split(uint32_t x, uint32_t &l, uint32_t &r, uint32_t k) {
    if (!k)
      l = 0, r = x;
    else if (k == sz(x)) {
      l = x;
      r = 0;
    } else if (k <= sz(ls(x))) {
      r = x;
      split(ls(x), l, ls(x), k);
      mt(x);
    } else {
      l = x;
      split(rs(x), rs(x), r, k - sz(ls(x)) - 1);
      mt(x);
    }
  }

  uint32_t rank(uint32_t x, int w) {
    if (!x)
      return 0;
    if (w <= val(x))
      return rank(ls(x), w);
    else
      return rank(rs(x), w) + sz(ls(x)) + 1;
  }

  uint32_t find(uint32_t d) {
    uint32_t x = 0, y = 0, z = 0;
    split(root, x, y, d);
    split(x, z, x, d - 1);
    uint32_t ans = val(x);
    merge(x, z, x);
    merge(root, x, y);
    return ans;
  }

  void del(uint32_t v) {
    uint32_t x, y, tmp, rk = rank(root, v) + 1;
    split(root, x, y, rk);
    split(x, x, tmp, rk - 1);
    merge(root, x, y);
  }

  int pre(int v) {
    uint32_t x, y, z, rk = rank(root, v);
    split(root, x, y, rk);
    split(x, z, x, rk - 1);
    int ans = val(x);
    merge(x, z, x);
    merge(root, x, y);
    return ans;
  }

  int suc(int v) {
    uint32_t x, y, z, rk = rank(root, v + 1);
    split(root, x, y, rk + 1);
    split(x, z, x, rk);
    int ans = val(x);
    merge(x, z, x);
    merge(root, x, y);
    return ans;
  }

  void insert(int v) {
    uint32_t x, y, tmp = 0, rk = rank(root, v);
    split(root, x, y, rk);
    newnode(tmp, v);
    merge(x, x, tmp);
    merge(root, x, y);
  }
};

const int inf = 0x3f3f3f3f;

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0);
  int n;
  cin >> n;
  ZipTree zip;
  zip.nodes.push_back({inf, 0, 0, 0, inf});
  for (int i = 1, op, x; i <= n; i++) {
    cin >> op >> x;
    if (op == 1)
      zip.insert(x);
    else if (op == 2)
      zip.del(x);
    else if (op == 3)
      cout << zip.rank(zip.root, x) + 1 << '\n';
    else if (op == 4)
      cout << zip.find(x) << '\n';
    else if (op == 5)
      cout << zip.pre(x) << '\n';
    else
      cout << zip.suc(x) << '\n';
  }
  return 0;
}
