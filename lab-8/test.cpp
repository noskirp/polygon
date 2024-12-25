#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

int main() {
    string filename = "spb_graph.txt";

    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        cerr << "error opening" << filename << endl;
        return 1; 
    }

    vector<vector<double>> matrix;
    string line;

    // Read each line until the end of the file
    while (getline(inputFile, line, '\n')) {
        vector<double> row;
        stringstream ss(line);
        string cell;

        // Read each number within the row, separated by commas
        while (getline(ss, cell, ',')) {
            try {
                double value = stod(cell); // Convert string to double
                row.push_back(value);
            } catch (const invalid_argument& e) {
                cerr << "Error: Invalid input in file. Expected a number." << endl;
                inputFile.close();
                return 1; // Indicate an error
            } catch (const out_of_range& e) {
                cerr << "Error: Number in file is out of range." << endl;
                inputFile.close();
                return 1; // Indicate an error
            }
        }
        matrix.push_back(row);
    }

    inputFile.close();

    // Print the matrix (optional)
    cout << "Matrix:" << endl;
    for (const auto& row : matrix) {
        for (double val : row) {
            cout << val << " ";
        }
        cout << endl;
    }

    return 0;
}