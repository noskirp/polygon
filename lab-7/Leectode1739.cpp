#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
using namespace std;

class Solution {
public:
    int minimumBoxes(int totalCubes) {
        int totalFullFloors = 0, layerCount = 1;

        while (totalFullFloors + layerCount * (layerCount + 1) / 2 <= totalCubes) {
            totalFullFloors += layerCount * (layerCount + 1) / 2;
            ++layerCount;
        }
        --layerCount;
        int bottomCubes = layerCount * (layerCount + 1) / 2;
        
      
        int addLayer = 1;
        while (totalFullFloors < totalCubes) {
            bottomCubes++;
            totalFullFloors += addLayer;
            ++addLayer;
        }
        return bottomCubes;
    }
};
Solution obj;

int main(){
    cout << obj.minimumBoxes(15) << " ";
}
