#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;

void read_csv(const std::string& filename, std::vector<int>& intArray) { // Функция чтения чисел из csv в массив
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string value;

        while (std::getline(ss, value, ',')) { 
            try {
                int number = std::stoi(value);
                intArray.push_back(number); 
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid number: " << value << std::endl;
            } catch (const std::out_of_range& e) {
                std::cerr << "Number out of range: " << value << std::endl;
            }
        }
    }
    file.close();
}

void find_combinations(const vector<int>& arr, vector<int>& current, int start, vector<vector<int>>& result){
    if (current.size() == 8){
        int sum = 0;
        for (int arr_index : current){
            sum += arr[arr_index];
        }
        if (sum == 0) result.push_back(current);
        return;
    }
    for (int i = start; i <= arr.size(); i++) {
        current.push_back(i);
        find_combinations(arr, current, i+1, result);
        current.pop_back();
    }
}

vector<vector<int>> find_subarrays(const vector<int>& arr) {
    vector<vector<int>> result;
    vector<int> current;
    find_combinations(arr, current, 0, result);
    return result; 
}

int main() {
    std::vector<std::string> data_samples = {"8", "10", "15", "20", "20"};
    std::vector<float> corr_median_time;
    std::cout << data_samples.size() << std::endl;
    for(int i=0; i < data_samples.size(); i++){
        float sum_time = 0;
        int res_size = 0;
        for (int j = 1; j <= 10; j ++) {
        std::vector<int> arr;        
        std::string tmp_file = data_samples[i] + "_" + std::to_string(j) + ".csv";


        read_csv(tmp_file, arr);

        int n = arr.size();

        auto start = std::chrono::high_resolution_clock::now(); // Фиксируем время старта    

        vector<vector<int>> result = find_subarrays(arr);

        auto end = std::chrono::high_resolution_clock::now(); // Фиксируем время окончания
        
        std::chrono::duration<double> duration = end - start;
        std::cout << duration.count() << std::endl; // Выводим время работы
        if (result.size() != 0){
            sum_time += duration.count();
            res_size++;
        }
        
    }
    corr_median_time.push_back(sum_time/res_size);
    }
    for (auto now : corr_median_time) {
        cout << "," << now << " ";
    }
  return 0;
    }


