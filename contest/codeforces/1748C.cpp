#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <stdint.h>
#include <vector>

using namespace std;

const int maxn = 2e5 + 10;

int64_t a[maxn];

int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);
  cout.tie(0);

  int T;
  cin >> T;

  for (; T--;) {
    int n;
    int ans = 0;
    int maxfreq = 0;
    bool found_wc = false;
    map<int64_t, int> freq;
    cin >> n;
    for (int i = 1; i <= n; i++) {
      cin >> a[i];
      // 0 下标
      if (a[i] == 0) {
        if (found_wc)
          ans += maxfreq;
        else
          ans += freq[0];

        found_wc = true;
        maxfreq = 0;
        freq.clear();
      }

      // 前缀和
      a[i] += a[i - 1];
      maxfreq = max(maxfreq, ++freq[a[i]]);
    }
    if (found_wc)
      ans += maxfreq;
    else
      ans += freq[0];
    cout << ans << '\n';
  }
  return 0;
}
