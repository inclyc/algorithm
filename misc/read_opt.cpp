#include <cctype>
#include <cstdio>

namespace ReadOpt {

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

} // namespace ReadOpt