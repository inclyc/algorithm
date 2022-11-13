// 初始集合 S = \phi, 开一个 D[n][n] 数组
// u 和 v 都在集合 S 中时，D[u][v] 为 S 子图最短路
// 否则，D[u][v] 表示直接相连的边权
// 考虑如何扩大集合 S，每次加入一个元素 k
// 状态转移： D[i][j] = min (D[i][j], D[i][k] + D[k][j])

#include <algorithm>

const int MAXN = 100;

int D[MAXN][MAXN];

void Floyd(int n) {
  // D[n][n] 初始化为邻接矩阵
  for (int k = 1; k <= n; k++)
    for (int i = 1; i <= n; i++)
      for (int j = 1; j <= n; j++)
        D[i][j] = std::min(D[i][j], D[i][k] + D[k][j]);
}
