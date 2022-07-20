#include <cstddef>
#include <cstdio>

namespace Splay {

struct Node {
  // childs of this node
  Node *ch[2];

  // father
  Node *fa;

  // metadata
  int val, size, weight;

  int rel() { return fa->ch[1] == this; }

  void maintain() {
    size = (ch[0] ? ch[0]->size : 0) + (ch[1] ? ch[1]->size : 0) + weight;
  }

  void rotate() {
    int o = rel();
    auto f = fa, c = ch[o ^ 1];
    fa = f->fa;
    if (fa) {
      int fo = f->rel();
      fa->ch[fo] = this;
    }

    f->ch[o] = c;
    if (c)
      c->fa = f;
    f->fa = this;
    f->maintain();
    ch[o ^ 1] = f;

    this->maintain();
  }

  void splay(Node *target) {
    for (; fa && fa != target; rotate())
      if (fa->fa && fa->fa != target) {
        if (rel() == fa->rel())
          fa->rotate();
        else
          rotate();
      }
  }

  // greatest lesser than v
  Node *prev(int v) {
    auto p = this, ans = this;
    for (; p;) {
      if (p->val < v) {
        ans = p;
        p = p->ch[1];
      } else {
        p = p->ch[0];
      }
    }
    return ans;
  }

  // least greater than v
  Node *suc(int v) {
    auto p = this, ans = this;
    for (; p;) {
      if (p->val > v) {
        ans = p;
        p = p->ch[0];
      } else
        p = p->ch[1];
    }
    return ans;
  }
};

const int inf = 0x3f3f3f3f;
const int maxn = 1e5 + 10;

Node nodes[maxn];

Node *root;

Node *alloc(int val) {
  static Node *end = nodes;
  end->val = val;
  end->weight = end->size = 1;
  end->fa = end->ch[1] = end->ch[0] = nullptr;
  return end++;
}

void splay(Node *node, Node *target = nullptr) {
  node->splay(target);
  if (!target) {
    root = node;
  }
}

Node *&ipoint(int val) {
  auto prev = root->prev(val), suc = root->suc(val);
  splay(prev);
  splay(suc, prev);
  return suc->ch[0];
}

Node *insert(int val) {
  auto p = ipoint(val);
  if (p) {
    p->weight++;
  } else {
    auto node = alloc(val);
    p = node;
    p->fa = root->ch[1];
  }
  splay(p);
  return p;
}

int rank_of(int val) {
  auto p = ipoint(val);
  splay(p);
  return p->ch[0]->size;
}

void del(int val) {
  auto p = ipoint(val);
  if (p->weight > 1) {
    p->weight--;
    Splay::splay(p);
  } else {
    p->fa->ch[0] = nullptr;
    p->fa->maintain();
    root->maintain();
  }
}

int kth(int k) {
  // rank = [|left|, |left| + weight)
  auto p = root;
  int ans = p->val;
  for (; p;) {
    int lsize = (p->ch[0] ? p->ch[0]->size : 0);
    if (lsize <= k && k < lsize + p->weight) {
      return p->val;
    }
    if (k < lsize) {
      p = p->ch[0];
    } else {
      k -= lsize + p->weight;
      p = p->ch[1];
    }
  }
  return ans;
}

int prev(int val) {
  auto prev = root->prev(val);
  return prev->val;
}

int suc(int val) {
  auto suc = root->suc(val);
  return suc->val;
}

void init() {
  auto infNode = alloc(inf);
  auto ninfNode = alloc(-inf);
  root = infNode;
  infNode->ch[0] = ninfNode;
  ninfNode->fa = infNode;
  infNode->maintain();
}

} // namespace Splay

int main() {
  int cases;
  Splay::init();
  scanf("%d", &cases);
  for (; cases--;) {
    int opt, x;
    scanf("%d%d", &opt, &x);
    switch (opt) {
    case 1:
      Splay::insert(x);
      break;
    case 2:
      Splay::del(x);
      break;
    case 3:
      printf("%d\n", Splay::rank_of(x));
      break;
    case 4:
      printf("%d\n", Splay::kth(x));
      break;
    case 5:
      printf("%d\n", Splay::prev(x));
      break;
    case 6:
      printf("%d\n", Splay::suc(x));
      break;
    }
  }
}