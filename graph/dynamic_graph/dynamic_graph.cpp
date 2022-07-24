// Author: lyc
// 2021/10/31
// 利用LCT维护生成树
#ifndef DYNAMIC_GRAPH_HPP
#define DYNAMIC_GRAPH_HPP

// Author: lyc
// 2021/10/31
// Link-Cut-Trees by Robert Endre Tarjan
// 为其他结构提供动态树支持。
// 在这个项目中，LCT被用于： 维护动态图中的生成树
#include <memory>
#ifndef LINK_CUT_TREE_HPP
#define LINK_CUT_TREE_HPP
#ifndef LIST_HPP
#define LIST_HPP
namespace DynamicGraph {
struct List {
  int *head;
  int *pre;
  int *next;
  void init(unsigned long);
  void free();
  void link(int, int);
  void cut(int, int);
  bool empty(int);
  int first(int);
};
} // namespace DynamicGraph

#endif // LIST_HPP

#include "unordered_set"
#include <algorithm>
#include <cstddef>

#define LCT_MAXN 100000 + 10
namespace DynamicGraph {
struct LinkCutTree {
  std::unordered_set<int> G[LCT_MAXN][2];
  int fa[LCT_MAXN];
  int ch[LCT_MAXN][2];
  int flp[LCT_MAXN];
  int sta[LCT_MAXN];
  int tp;
  int siz[LCT_MAXN];
  int sizv[LCT_MAXN];
  int tag[LCT_MAXN][2];

  List chv[2];

  void init(std::size_t);
  void free();

  void linkv(int, int);
  void cutv(int u, int v);
  void rev(int u);
  void pu(int u);
  void pd(int u);
  int sf(int u);
  bool isrt(int u);
  void rot(int u);
  void splay(int u);
  void access(int u);
  void makert(int u);
  void join(int u, int v);
  int findrt(int u);
  void link(int u, int v);
  void cut(int u, int v);
  int get(int u, int f);
  bool isconnected(int u, int v);
  void ins(int f, int u, int v);
  void del(int f, int u, int v);
};
} // namespace DynamicGraph

#endif // LINK_CUT_TREE_HPP

#include <cstddef>
#include <unordered_map>
namespace DynamicGraph {
typedef long long ll;
class Graph {

private:
  int N;
  int M;
  int blocks;
  ll mapNode(int x, int y);
  LinkCutTree lct[20];
  std::unordered_map<ll, int> LV;
  void insertAsTree(int lv, int u, int v);
  void insertAsGraph(int lv, int u, int v);
  void deleteTree(int lv, int u, int v);
  void deleteGraph(int lv, int u, int v);
  bool findReplace(int lv, int u, int v);

public:
  int getBlockSize(int u);
  int getBlock();
  Graph(int);
  ~Graph();
  int isConnected(int u, int v);
  void link(int u, int v);
  void cut(int u, int v);
};
} // namespace DynamicGraph

#endif // DYNAMIC_GRAPH_HPP

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <vector>

namespace DynamicGraph {

ll Graph::mapNode(int x, int y) { return static_cast<ll>(x) * M + y; }
void Graph::insertAsTree(int lv, int u, int v) {
  LV[mapNode(u, v)] = LV[mapNode(v, u)] = lv;
  lct[lv].ins(0, u, v);
}
void Graph::insertAsGraph(int lv, int u, int v) {
  LV[mapNode(u, v)] = LV[mapNode(v, u)] = lv;
  lct[lv].ins(1, u, v);
}
void Graph::deleteTree(int lv, int u, int v) { lct[lv].del(0, u, v); }
void Graph::deleteGraph(int lv, int u, int v) { lct[lv].del(1, u, v); }
bool Graph::findReplace(int lv, int u, int v) {
  lct[lv].access(u);
  lct[lv].access(v);
  if (lct[lv].siz[u] > lct[lv].siz[v])
    std::swap(u, v);
  int t = u, replacev = 0;
  while ((u = lct[lv].get(u, 0))) {
    std::unordered_set<int> &G = lct[lv].G[u][0];
    while (G.size()) {
      int nxt = *G.begin();
      deleteTree(lv, u, nxt);
      insertAsTree(lv + 1, u, nxt);
      lct[lv + 1].link(u, nxt);
    }
  }
  u = t;
  while ((u = lct[lv].get(u, 1))) {
    std::unordered_set<int> &G = lct[lv].G[u][1];
    while (G.size()) {
      int nxt = *G.begin();
      if (lct[lv].isconnected(u, nxt)) {
        deleteGraph(lv, u, nxt);
        insertAsGraph(lv + 1, u, nxt);
      } else {
        replacev = nxt;
        deleteGraph(lv, u, replacev);
        insertAsTree(lv, u, replacev);
        for (int i = 0; i <= lv; ++i)
          lct[i].link(u, replacev);
        break;
      }
    }
    if (replacev)
      return 1;
  }
  return 0;
}
int Graph::isConnected(int u, int v) { return lct[0].isconnected(u, v); }
int Graph::getBlockSize(int u) {
  lct[0].access(u);
  return lct[0].siz[u];
}
void Graph::link(int u, int v) {
  if (!isConnected(u, v)) {
    lct[0].link(u, v);
    insertAsTree(0, u, v);
    --blocks;
  } else
    insertAsGraph(0, u, v);
}
void Graph::cut(int u, int v) {
  int lv = LV[mapNode(u, v)];
  if (lct[lv].G[u][0].count(v)) {
    for (int i = 0; i <= lv; ++i)
      lct[i].cut(u, v);
    deleteTree(lv, u, v);
    ++blocks;
    for (int i = lv; i >= 0; --i)
      if (findReplace(i, u, v)) {
        --blocks;
        break;
      }
  } else
    deleteGraph(lv, u, v);
}
Graph::~Graph() {
  for (int i = 0; i < 20; ++i)
    lct[i].free();
}

Graph::Graph(int n) {
  for (std::size_t i = 0; i < 20; ++i)
    lct[i].init(static_cast<std::size_t>(n));
  LV.clear();
  blocks = n;
  N = n;
  M = 1000000000 + 7;
}

int Graph::getBlock() { return blocks; }
} // namespace DynamicGraph

#include <unordered_map>
#include <unordered_set>

namespace DynamicGraph {

#define l(u) ch[u][0]
#define r(u) ch[u][1]

void LinkCutTree::init(std::size_t n) {
  for (std::size_t i = 0; i < 2; ++i) {
    chv[i].init(n);
    for (std::size_t u = 1; u <= n; ++u)
      G[u][i].clear();
  }
  tp = 0;
  for (std::size_t u = 0; u <= n; ++u) {
    sizv[u] = 0;
    sta[u] = 0;
    fa[u] = l(u) = r(u) = tag[u][0] = tag[u][1] = flp[u] = sizv[u] = 0;
    siz[u] = 1;
  }
  siz[0] = 0;
}

void LinkCutTree::free() {
  chv[0].free();
  chv[1].free();
}

void LinkCutTree::linkv(int u, int v) {
  if (!u || !v)
    return;
  sizv[u] += siz[v];
  for (int i = 0; i < 2; ++i)
    if (tag[v][i])
      chv[i].link(u, v);
}
void LinkCutTree::cutv(int u, int v) {
  if (!u || !v)
    return;
  sizv[u] -= siz[v];
  for (int i = 0; i < 2; ++i)
    if (tag[v][i])
      chv[i].cut(u, v);
}
void LinkCutTree::rev(int u) {
  flp[u] ^= 1;
  std::swap(l(u), r(u));
}

void LinkCutTree::pu(int u) {
  if (!u)
    return;
  for (int i = 0; i < 2; ++i)
    tag[u][i] =
        tag[l(u)][i] | tag[r(u)][i] | !G[u][i].empty() | !chv[i].empty(u);
  siz[u] = siz[l(u)] + siz[r(u)] + 1 + sizv[u];
}

void LinkCutTree::pd(int u) {
  if (flp[u]) {
    rev(l(u));
    rev(r(u));
    flp[u] = 0;
  }
}

int LinkCutTree::sf(int u) { return u == r(fa[u]); }

bool LinkCutTree::isrt(int u) { return u != l(fa[u]) && u != r(fa[u]); }

void LinkCutTree::rot(int u) {
  int v = fa[u], f = sf(u);
  bool flag = isrt(v);
  if (!flag)
    ch[fa[v]][sf(v)] = u;
  else if (fa[v])
    cutv(fa[v], v);
  ch[v][f] = ch[u][f ^ 1];
  fa[ch[v][f]] = v;
  fa[u] = fa[v];
  ch[u][f ^ 1] = v;
  fa[v] = u;
  pu(v);
  if (flag) {
    pu(u);
    linkv(fa[u], u);
  }
}

void LinkCutTree::splay(int u) {
  sta[tp = 0] = u;
  for (int v = u; !isrt(v); v = fa[v])
    sta[++tp] = fa[v];
  for (; ~tp; pd(sta[tp--]))
    ;
  for (; !isrt(u); rot(u))
    if (!isrt(fa[u]) && sf(fa[u]) == sf(u))
      rot(fa[u]);
}

void LinkCutTree::access(int u) {
  int w = u;
  for (int v = 0; u; u = fa[v = u]) {
    splay(u);
    linkv(u, r(u));
    cutv(u, v);
    r(u) = v;
    pu(u);
  }

  splay(w);
}
void LinkCutTree::makert(int u) {
  access(u);
  rev(u);
}

void LinkCutTree::join(int u, int v) {
  makert(u);
  access(v);
}

int LinkCutTree::findrt(int u) {
  access(u);
  for (; l(u); pd(u), u = l(u))
    ;
  splay(u);
  return u;
}

void LinkCutTree::link(int u, int v) {
  makert(u);
  if (findrt(v) == u)
    return;
  fa[u] = v;
  linkv(v, u);
  pu(v);
  access(v);
}

void LinkCutTree::cut(int u, int v) {
  join(u, v);
  if (l(v) != u || r(u))
    return;
  fa[u] = l(v) = 0;
  pu(v);
}

int LinkCutTree::get(int u, int f) {
  access(u);
  if (!tag[u][f])
    return 0;
  while (G[u][f].empty()) {
    if (tag[l(u)][f])
      u = l(u);
    else if (tag[r(u)][f])
      u = r(u);
    else
      u = chv[f].first(u);
  }
  return u;
}
bool LinkCutTree::isconnected(int u, int v) { return findrt(u) == findrt(v); }
void LinkCutTree::ins(int f, int u, int v) {
  if (G[u][f].size() == 0)
    access(u);
  G[u][f].insert(v);
  pu(u);
  if (G[v][f].size() == 0)
    access(v);
  G[v][f].insert(u);
  pu(v);
}
void LinkCutTree::del(int f, int u, int v) {
  if (G[u][f].size() == 1)
    access(u);
  G[u][f].erase(v);
  pu(u);
  if (G[v][f].size() == 1)
    access(v);
  G[v][f].erase(u);
  pu(v);
}
} // namespace DynamicGraph

#include <cstddef>
namespace DynamicGraph {
void List::init(std::size_t n) {
  head = new int[n + 1]();
  pre = new int[n + 1]();
  next = new int[n + 1]();
  for (unsigned int i = 0; i <= n; ++i) {
    head[i] = pre[i] = next[i] = 0;
  }
}

void List::free() {
  delete[] head;
  delete[] pre;
  delete[] next;
}

void List::cut(int u, int v) {
  if (next[v] == v) {
    head[u] = 0;
  } else {
    if (head[u] == v) {
      head[u] = next[v];
    }
    v[next][pre] = v[pre];
    v[pre][next] = v[next];
  }
}

void List::link(int u, int v) {
  if (!head[u]) {
    head[u] = pre[v] = next[v] = v;
  } else {
    int x = head[u];
    int y = pre[x];
    pre[x] = next[y] = v;
    pre[v] = y;
    next[v] = x;
  }
}

bool List::empty(int u) { return head[u] == 0; }

int List::first(int u) { return head[u]; }
} // namespace DynamicGraph

#include <cctype>
#include <cstdio>

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

int main() {
  using DynamicGraph::Graph;
  using ReadOpt::rd;

  int n, m;
  rd(n);
  rd(m);
  auto dg = std::make_unique<Graph>(n);
  int lastans = 0;
  for (int i = 0; i < m; i++) {
    int op, u, v;
    rd(op);
    rd(u);
    rd(v);
    u ^= lastans;
    v ^= lastans;
    switch (op) {
    case 0:
      dg->link(u, v);
      break;
    case 1:
      dg->cut(u, v);
      break;
    case 2:
      bool isConn = dg->isConnected(u, v);
      printf("%c\n", isConn ? 'Y' : 'N');
      lastans = isConn ? u : v;
      break;
    }
  }
}
