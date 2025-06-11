// 匹配
#include <bits/stdc++.h>
const int N = 80 + 5, INF = 0x3f3f3f3f;
std::vector<std::pair<int, int>> v, ans;
bool visx[N], visy[N];
int n, w[N][N], match[N], slack[N], lx[N], ly[N], max;

void input()
{
    std::cin >> n;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
            std::cin >> w[i][j];
    }
}
bool dfs(int u)
{
    visx[u] = 1;
    for (int v = 1; v <= n; v++)
    {
        if (w[u][v] == lx[u] + ly[v]) // 在相等子图中
        {
            if (visy[v])
                continue;
            visy[v] = 1;
            if (!match[v] || dfs(match[v]))
            {
                match[v] = u;
                return 1;
            }
        }
        else // 在 T' 中
            slack[v] = std::min(slack[v], lx[u] + ly[v] - w[u][v]);
    }
    return 0;
}
int KM() // x: 不考虑的男生，y: 不考虑的女生（-1 为忽略）
{
    memset(match, 0, sizeof(match));
    // 初始化左部顶标
    for (int i = 1; i <= n; i++)
    {
        lx[i] = -INF;
        for (int j = 1; j <= n; j++)
            lx[i] = std::max(lx[i], w[i][j]);
    }

    // 为每个左部节点寻找匹配
    for (int i = 1; i <= n; i++)
    {
        std::fill(slack + 1, slack + n + 1, INF);

        while (1)
        {
            memset(visx, 0, sizeof(visx));
            memset(visy, 0, sizeof(visy));

            if (dfs(i))
                break; // 找到增广路

            // 计算最小调整量 a
            int a = INF;
            for (int j = 1; j <= n; j++)
            {
                if (!visy[j])
                    a = std::min(a, slack[j]);
            }

            // 调整顶标
            for (int j = 1; j <= n; j++)
            {
                if (visx[j]) // S
                    lx[j] -= a;
                if (visy[j]) // T
                    ly[j] += a;
                else // T' 维护 slack 数组(都减少 a)
                    slack[j] -= a;
            }
        }
    }
    int res = 0;
    for (int i = 1; i <= n; i++)
        res += w[match[i]][i];
    return res;
}

void solve()
{
    max = KM();
    for (int i = 1; i <= n; i++)
        v.push_back({match[i], i}); // 预处理可能的答案
    std::sort(v.begin(), v.end());
    for (auto i : v)
    {
        int tmp = w[i.first][i.second];
        w[i.first][i.second] = -INF;
        int res = KM();
        if (res != max)
            ans.push_back(i);
        w[i.first][i.second] = tmp;
    }
    std::cout << max << '\n';
    for (auto i : ans)
        std::cout << i.first << ' ' << i.second << '\n';
}
int main()
{
    input();
    solve();
}