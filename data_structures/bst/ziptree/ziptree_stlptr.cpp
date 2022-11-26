#include <bits/stdc++.h>

using std::cin;
using std::cout;
using std::pair;

#define fieldOr(ptr, field, fallback) ((ptr) ? ((ptr)->field) : fallback)

const int inf = 0x3f3f3f3f;
const int maxn = 1e5 + 8;

uint32_t seed = 1;
uint32_t rnd() {
  seed ^= seed << 13;
  seed ^= seed >> 17;
  seed ^= seed << 5;
  return seed;
}

struct ZipTree {

  struct Node {
    uint32_t size;
    uint32_t weight;
    int val;
    Node *ls;
    Node *rs;

    void mt() { size = fieldOr(ls, size, 0) + fieldOr(rs, size, 0) + 1; }
  };

  Node nodes[maxn];
  Node *root;
  Node *newnode(int v) {
    static int alloc = 0;
    nodes[alloc++] = {1, rnd(), v, nullptr, nullptr};
    return nodes + alloc - 1;
  }

  ZipTree() { root = newnode(inf); }

  Node *merge(Node *l, Node *r) {
    if (!l)
      return r;
    if (!r)
      return l;
    if (l->weight < r->weight) {
      l->rs = merge(l->rs, r);
      l->mt();
      return l;
    } else {
      r->ls = merge(l, r->ls);
      r->mt();
      return r;
    }
  }

  pair<Node *, Node *> split(Node *x, uint32_t k) {
    assert(x);
    if (!k)
      return {nullptr, x};
    if (k == x->size)
      return {x, nullptr};
    if (k <= fieldOr(x->ls, size, 0)) {
      auto [l, r] = split(x->ls, k);
      x->ls = r;
      x->mt();
      return {l, x};
    } else {
      auto [l, r] = split(x->rs, k - fieldOr(x->ls, size, 0) - 1);
      x->rs = l;
      x->mt();
      return {x, r};
    }
  }

  uint32_t rank(Node *x, int w) {
    if (!x)
      return 0;
    if (w <= x->val)
      return rank(x->ls, w);
    else
      return rank(x->rs, w) + fieldOr(x->ls, size, 0) + 1;
  }

  void insert(int v) {
    int rk = rank(root, v);
    auto [l, r] = split(root, rk);
    auto n = newnode(v);
    root = merge(merge(l, n), r);
  }

  auto find(int d) {
    auto [l, r] = split(root, d);
    auto [_, x] = split(l, d - 1);
    auto ans = fieldOr(x, val, inf);
    root = merge(merge(_, x), r);
    return ans;
  }

  auto del(int v) {
    auto rk = rank(root, v) + 1;
    auto [l, r] = split(root, rk);
    auto [lx, _] = split(l, rk - 1);
    root = merge(lx, r);
  }

  auto pre(int v) { return find(rank(root, v)); }
  auto suc(int v) { return find(rank(root, v + 1) + 1); }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0);
  int n;
  cin >> n;
  ZipTree zip;
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
