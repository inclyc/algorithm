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

    int flip;

    void mt() { size = fieldOr(ls, size, 0) + fieldOr(rs, size, 0) + 1; }
    void pd() {
      if (flip) {
        std::swap(ls, rs);
        flip = 0;
        if (ls)
          ls->flip ^= 1;
        if (rs)
          rs->flip ^= 1;
      }
    }
  };

  Node nodes[maxn];
  Node *root;
  Node *newnode(int v) {
    static int alloc = 0;
    nodes[alloc++] = {1, rnd(), v, nullptr, nullptr, 0};
    return nodes + alloc - 1;
  }

  ZipTree() { root = newnode(inf); }

  Node *merge(Node *l, Node *r) {
    if (!l)
      return r;
    if (!r)
      return l;
    l->pd();
    r->pd();
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
    x->pd();
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

  void rotate(int l, int r) {
    // rotate [l, r]
    auto [_, mid] = split(root, l - 1);
    auto [x, __] = split(mid, r - l + 1);
    x->flip = 1;
    root = merge(_, merge(x, __));
  }

  void dfs(Node *u) {
    if (!u)
      return;
    u->pd();
    dfs(u->ls);
    if (u->val != inf) {
      cout << u->val << " ";
    }
    dfs(u->rs);
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(0);
  int n, m;
  cin >> n >> m;
  ZipTree zip;
  for (int i = 1; i <= n; i++) {
    zip.insert(i);
  }
  for (int i = 0; i < m; i++) {
    int l, r;
    cin >> l >> r;
    zip.rotate(l, r);
  }
  zip.dfs(zip.root);
  cout << "\n";
}
