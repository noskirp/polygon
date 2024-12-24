#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <math.h>

using namespace std;

class Graph
int main() {
    string filename = "spb_graph.txt";

    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        cerr << "error opening file" << filename << endl;
        return 1; 
    }

    vector<vector<double>> matrix;
    string line;
    while (getline(inputFile, line, '\n')) {
        vector<double> row;
        stringstream ss(line);
        string cell;

        while (getline(ss, cell, ',')) {
            try {
                double value = stod(cell); 
                row.push_back(value);
            } catch (const invalid_argument& e) {
                cerr << "Error: Invalid input in file. Expected a number." << endl;
                inputFile.close();
                return 1; 
            } catch (const out_of_range& e) {
                cerr << "Error: Number in file is out of range." << endl;
                inputFile.close();
                return 1;
            }
        }
        matrix.push_back(row);
    }
    inputFile.close();


    return 0;
}

struct Node {
    double lon, lat;  // Долгота и широта узла
    std::vector<std::pair<Node*, double>> nodes; // Список соседей узла и расстояний до них
};

struct Graph {
    std::vector<Node*> nodes; // Список всех узлов в графе

    Node* find_closest_node(double lat, double lon) {
        // Функция для поиска в графе узла, который ближе всего находится к указанной точке
        double min_distance = 99999; // Инициализация минимального расстояния
        Node* node_found = nullptr; // Указатель на найденный узел

        for (auto node : nodes) {
            double distance = std::sqrt(std::pow(node->lat - lat, 2) + std::pow(node->lon - lon, 2)); // Расчет расстояния
            if (distance < min_distance) { // Если найденное расстояние меньше минимального
                node_found = node; // Обновляем найденный узел
                min_distance = distance; // Обновляем минимальное расстояние
            }
        }

        return node_found; // Возвращаем найденный узел
    }
};

