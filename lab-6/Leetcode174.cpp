#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
    int solve(vector<vector<int>> dungeon, int i, int j, int n, int m, vector<vector<int>> &solved) {
        if (i >= n || j >= m) return 1e9;
        if (i == n - 1 && j == m - 1) {
            if (dungeon[i][j] > 0) return 1;
            else return 1 - dungeon[i][j];
        }
        if (solved[i][j] != 1e5) return solved[i][j];
        int right = solve(dungeon, i, j + 1, n, m, solved);
        int bottom = solve(dungeon, i + 1, j, n, m, solved);

        int val = min(right, bottom) - dungeon[i][j];
        if (val > 0) return solved[i][j] = val;
        else return solved[i][j] = 1;
    }
public:
    int calculateMinimumHP(vector<vector<int>>& dungeon) {
        int m = dungeon[0].size();
        int n = dungeon.size();
        vector<vector<int>> dp(n, vector<int>(m, 1e5));
        return solve(dungeon, 0, 0, n, m, dp);
    }
};

int main() {
    Solution sol;

    // Example dungeon input
    vector<vector<int>> dungeon = {
        {-2, -3, 3},
        {-5, -10, 1},
        {10, 30, -5}
    };

    int result = sol.calculateMinimumHP(dungeon);
    cout << "Minimum HP required: " << result << endl;

    return 0;
}