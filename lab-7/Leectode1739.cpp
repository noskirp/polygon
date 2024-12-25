#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
using namespace std;

class Solution {
public:
    int minimumBoxes(int totalCuboids) {
        int totalFullFloors = 0, layerCount = 1;

        while (totalFullFloors + layerCount * (layerCount + 1) / 2 <= totalCuboids) {
            totalFullFloors += layerCount * (layerCount + 1) / 2;
            ++layerCount;
        }
        --layerCount;
        int minBoxes = layerCount * (layerCount + 1) / 2;
      
        layerCount = 1;
        while (totalFullFloors < totalCuboids) {
            minBoxes++;
            totalFullFloors += layerCount;
            ++layerCount;
        }
        return minBoxes;
    }
};


int main() {
    Solution sol;

    int totalCuboids = 4;

    int result = sol.minimumBoxes(totalCuboids);
    cout << "Minimum HP required: " << result << endl;

    return 0;
}