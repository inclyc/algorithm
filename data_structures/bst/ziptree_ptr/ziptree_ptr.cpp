#include <cctype>
#include <cstdio>

#define NOT_NULL_OR(ptr, field, fallback) ((ptr) ? ((ptr)->field) : fallback)

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

namespace XorShift {
typedef unsigned int uint;
uint seed = 3;
uint rnd() {
  seed ^= seed << 13;
  seed ^= seed >> 17;
  seed ^= seed << 5;
  return seed;
}
} // namespace XorShift

namespace ZipTree {
typedef unsigned int uint;
struct Node {
  uint weight;
  Node *l, *r;
  int size, val;

  void mt() { size = NOT_NULL_OR(l, size, 0) + NOT_NULL_OR(r, size, 0) + 1; }
};

Node *root;
const int inf = 0x3f3f3f3f;
const int maxn = 1e5 + 10;

Node nodes[maxn];

Node *alloc(int v) {
  static Node *end = nodes;
  end->weight = XorShift::rnd();
  end->val = v;
  end->size = 1;
  return end++;
}

void merge(Node *&x, Node *l, Node *r) {
  if (!l || !r) {
    x = l ? l : r;
  } else if (l->weight < r->weight) {
    x = l;
    merge(x->r, x->r, r);
    x->mt();
  } else {
    x = r;
    merge(x->l, l, x->l);
    x->mt();
  }
}

void split(Node *x, Node *&l, Node *&r, int k) {
  if (!k) {
    l = 0;
    r = x;
  } else if (k == x->size) {
    l = x;
    r = 0;
  } else if (k <= NOT_NULL_OR(x->l, size, 0)) {
    r = x;
    split(x->l, l, x->l, k);
    x->mt();
  } else {
    l = x;
    split(x->r, x->r, r, k - NOT_NULL_OR(x->l, size, 0) - 1);
    x->mt();
  }
}

int rank(Node *x, int w) {
  if (!x)
    return 0;
  if (w <= x->val)
    return rank(x->l, w);
  return rank(x->r, w) + NOT_NULL_OR(x->l, size, 0) + 1;
}

void insert(int v) {
  int rk = rank(root, v);
  Node *x, *y, *t;
  split(root, x, y, rk);
  t = alloc(v);
  merge(x, x, t);
  merge(root, x, y);
}

int find(int d) {
  Node *x, *y, *z;
  split(root, x, y, d);
  split(x, z, x, d - 1);
  int ans = NOT_NULL_OR(x, val, inf);
  merge(x, z, x);
  merge(root, x, y);
  return ans;
}

void del(int v) {
  Node *x, *y, *t;
  int rk = rank(root, v) + 1;
  split(root, x, y, rk);
  split(x, x, t, rk - 1);
  merge(root, x, y);
}

int pre(int v) {
  Node *x, *y, *z;
  int ans, rk = rank(root, v);
  split(root, x, y, rk);
  split(x, z, x, rk - 1);
  ans = NOT_NULL_OR(x, val, inf);
  merge(x, z, x);
  merge(root, x, y);
  return ans;
}

int suc(int v) {
  Node *x, *y, *z;
  int ans, rk = rank(root, v + 1);
  split(root, x, y, rk + 1);
  split(x, z, x, rk);
  ans = NOT_NULL_OR(x, val, inf);
  merge(x, z, x);
  merge(root, x, y);
  return ans;
}

} // namespace ZipTree

int main() {
  int n;
  rd(n);
  for (int i = 1, op, x; i <= n; i++) {
    rd(op);
    rd(x);
    if (op == 1)
      ZipTree::insert(x);
    else if (op == 2)
      ZipTree::del(x);
    else if (op == 3)
      printf("%d\n", ZipTree::rank(ZipTree::root, x) + 1);
    else if (op == 4)
      printf("%d\n", ZipTree::find(x));
    else if (op == 5)
      printf("%d\n", ZipTree::pre(x));
    else
      printf("%d\n", ZipTree::suc(x));
  }
}