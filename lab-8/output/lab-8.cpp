#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <fstream>
#include <queue>
#include <limits>
#include <stack>
#include <chrono>
#include <functional>
#include <Windows.h>
#include <math.h>
#include <locale>

struct Edge {
    std::string coordinates; // "lon,lat"
    double weight;           // Вес ребра
};

class Node {
public:
    std::string coordinates; // "lon,lat"
    std::vector<Edge> edges;

    Node(const std::string& coords) : coordinates(coords) {}
};

class Graph {
public:
    std::unordered_map<std::string, Node*> nodes;

    ~Graph() {
        for (auto& pair : nodes) {
            delete pair.second; // Освобождение памяти
        }
    }

    void addNode(const std::string& coords) {
        if (nodes.find(coords) == nodes.end()) {
            nodes[coords] = new Node(coords);
        }
    }

    void addEdge(const std::string& from, const std::string& to, double weight) {
        if (nodes.find(from) != nodes.end() && nodes.find(to) != nodes.end()) {
            nodes[from]->edges.push_back({ to, weight });
        }
        else {
            std::cerr << "Error: Node " << from << " or Node " << to << " does not exist.\n";
        }
    }
    std::vector<std::string> convert_coords(const std::string& str) {
        std::stringstream ss(str);
        std::string item;
        std::vector<std::string> items;

        while (std::getline(ss, item, ',')) {
            items.push_back(item);
        }
        return items; 
    }

    std::string find_closest_node(std::string& input_coords) {
        double min_distance = std::numeric_limits<double>::infinity();
        std::string closest_node = "";
        std::vector<std::string> input_coords_sv = convert_coords(input_coords);
        double lat = std::stod(input_coords_sv[0]);
        double lon = std::stod(input_coords_sv[1]);

        for (const auto& pair : nodes) {
            const std::string& str = pair.first;
            std::vector<std::string> node_coords = convert_coords(str);
            if (node_coords.size() != 2) continue; 

            double node_lat = std::stod(node_coords[1]);
            double node_lon = std::stod(node_coords[0]);

            double distance = std::sqrt(std::pow(node_lat - lat, 2) + std::pow(node_lon - lon, 2));

            if (distance < min_distance) {
                min_distance = distance;
                closest_node = std::to_string(node_lat) + ",";
                closest_node = closest_node + std::to_string(node_lon);
            }
        }

        return closest_node; 
    }

    double dfs(const std::string& start, const std::string& end, std::vector<std::string>& shortestPath) {
        if (nodes.find(start) == nodes.end() || nodes.find(end) == nodes.end()) {
            std::cerr << "Error: One or both nodes not found for DFS.\n";
            return std::numeric_limits<double>::infinity();
        }

        std::unordered_map<std::string, bool> visited;
        std::vector<std::string> currentPath;
        double minWeight = std::numeric_limits<double>::infinity();

        std::stack<std::pair<std::string, std::pair<double, std::vector<std::string>>>> stack;
        stack.push({ start, {0, {}} });

        while (!stack.empty()) {
            auto current = stack.top();
            stack.pop();

            std::string currentNode = current.first;
            double currentWeight = current.second.first;
            currentPath = current.second.second;
            currentPath.push_back(currentNode);
            if (currentNode == end) {
                if (currentWeight < minWeight) {
                    minWeight = currentWeight;
                    shortestPath = currentPath;
                }
                continue;
            }

            for (const Edge& edge : nodes[currentNode]->edges) {
                if (!visited[edge.coordinates]) {
                    visited[edge.coordinates] = true;
                    stack.push({ edge.coordinates, {currentWeight + edge.weight, currentPath} });
                }
            }
        }

        return minWeight;
    }

    double bfs(const std::string& start, const std::string& end, std::vector<std::string>& shortestPath) {
        std::unordered_map<std::string, bool> visited;
        std::unordered_map<std::string, std::string> parent;
        std::unordered_map<std::string, double> weights;
        std::queue<std::string> queue;

        for (const auto& pair : nodes) {
            visited[pair.first] = false;
            weights[pair.first] = 0;
        }

        queue.push(start);
        visited[start] = true;
        parent[start] = "";

        while (!queue.empty()) {
            std::string current = queue.front();
            queue.pop();

            for (const Edge& edge : nodes[current]->edges) {
                if (!visited[edge.coordinates]) {
                    visited[edge.coordinates] = true;
                    parent[edge.coordinates] = current;
                    weights[edge.coordinates] = weights[current] + edge.weight;
                    queue.push(edge.coordinates);

                    if (edge.coordinates == end) {
                        std::vector<std::string> path;
                        for (std::string at = end; at != ""; at = parent[at]) {
                            path.push_back(at);
                        }
                        std::reverse(path.begin(), path.end());
                        shortestPath = path;
                        return weights[end];
                    }
                }
            }
        }

        return std::numeric_limits<double>::infinity();
    }

    double dijkstra(const std::string& start, const std::string& end, std::vector<std::string>& shortestPath) {
        if (nodes.find(start) == nodes.end() || nodes.find(end) == nodes.end()) {
            std::cerr << "Error: One or both nodes not found.\n";
            return std::numeric_limits<double>::infinity();
        }

        std::unordered_map<std::string, double> distances;
        std::unordered_map<std::string, std::string> previous;
        std::priority_queue<std::pair<double, std::string>, std::vector<std::pair<double, std::string>>, std::greater<std::pair<double, std::string>>> minHeap;

        for (const auto& pair : nodes) {
            distances[pair.first] = std::numeric_limits<double>::infinity();
            previous[pair.first] = "";
        }

        distances[start] = 0;
        minHeap.push({ 0, start });

        while (!minHeap.empty()) {
            std::pair<double, std::string> topElement = minHeap.top();
            double currentDistance = topElement.first;
            std::string currentNode = topElement.second;
            minHeap.pop();

            if (currentNode == end) {
                break;
            }

            for (const Edge& edge : nodes[currentNode]->edges) {
                double newDistance = currentDistance + edge.weight;

                if (newDistance < distances[edge.coordinates]) {
                    distances[edge.coordinates] = newDistance;
                    previous[edge.coordinates] = currentNode;
                    minHeap.push({ newDistance, edge.coordinates });
                }
            }
        }

        std::vector<std::string> path;
        for (std::string at = end; at != ""; at = previous[at]) {
            path.push_back(at);
        }
        std::reverse(path.begin(), path.end());

        if (!path.empty() && path.front() == start) {
            shortestPath = path;
            return distances[end];
        }

        return std::numeric_limits<double>::infinity();
    }

    double aStar(const std::string& start, const std::string& end, std::vector<std::string>& shortestPath) {
        if (nodes.find(start) == nodes.end() || nodes.find(end) == nodes.end()) {
            std::cerr << "Error: One or both nodes not found.\n";
            return std::numeric_limits<double>::infinity();
        }

        std::unordered_map<std::string, double> gCosts;
        std::unordered_map<std::string, double> fCosts;
        std::unordered_map<std::string, std::string> cameFrom;
        auto cmp = [](const std::pair<double, std::string>& a, const std::pair<double, std::string>& b) {
            return a.first > b.first;
            };
        std::priority_queue<std::pair<double, std::string>, std::vector<std::pair<double, std::string>>, decltype(cmp)> openSet(cmp);

        gCosts[start] = 0;
        fCosts[start] = heuristic(start, end);
        openSet.push({ fCosts[start], start });

        while (!openSet.empty()) {
            std::string currentNode = openSet.top().second;
            openSet.pop();

            if (currentNode == end) {
                break;
            }

            for (const Edge& edge : nodes[currentNode]->edges) {
                double tentativeGCost = gCosts[currentNode] + edge.weight;

                if (gCosts.find(edge.coordinates) == gCosts.end() || tentativeGCost < gCosts[edge.coordinates]) {
                    cameFrom[edge.coordinates] = currentNode;
                    gCosts[edge.coordinates] = tentativeGCost;
                    fCosts[edge.coordinates] = tentativeGCost + heuristic(edge.coordinates, end);
                    openSet.push({ fCosts[edge.coordinates], edge.coordinates });
                }
            }
        }

        std::vector<std::string> path;
        for (std::string at = end; at != ""; at = cameFrom[at]) {
            path.push_back(at);
        }
        std::reverse(path.begin(), path.end());

        if (!path.empty() && path.front() == start) {
            shortestPath = path;
            return gCosts[end];
        }

        return std::numeric_limits<double>::infinity();
    }

    double heuristic(const std::string& a, const std::string& b) {
        return 0;
    }
};

void parseGraphInput(std::istream& inputStream, Graph& graph) {
    std::string line;
    while (std::getline(inputStream, line)) {
        std::size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {
            std::string coordinates = line.substr(0, colonPos);
            graph.addNode(coordinates);

            std::string edgesStr = line.substr(colonPos + 1);
            std::istringstream edgesStream(edgesStr);
            std::string edgeInfo;

            while (std::getline(edgesStream, edgeInfo, ';')) {
                if (edgeInfo.empty()) continue;

                std::size_t weightPos = edgeInfo.find_last_of(',');

                if (weightPos != std::string::npos) {
                    std::string toCoordinates = edgeInfo.substr(0, weightPos);
                    std::string weightStr = edgeInfo.substr(weightPos + 1);

                    try {
                        double weight = std::stod(weightStr);
                        graph.addNode(toCoordinates);
                        graph.addEdge(coordinates, toCoordinates, weight);
                    }
                    catch (const std::exception& e) {
                        std::cerr << "Error parsing edge: " << edgeInfo << " - " << e.what() << "\n";
                    }
                }
                else {
                    std::cerr << "Error: Malformed edge information: " << edgeInfo << "\n";
                }
            }
        }
    }
}
bool check(std::ifstream& file, const std::string& toFind) {
    std::string line;
    while (std::getline(file, line)) {
        if (line.find(toFind) != std::string::npos) {
            return false;
        }
    }
    return true; 
}
int main() {
    Graph graph;
    std::string filename = "C:/Users/d-bys/OneDrive/Desktop/itmo/ITMO/ALGOS/polygon/lab-8/output/spb_graph.txt";
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error reading file " << filename << "\n";
        return 1;
    }
    parseGraphInput(file, graph);

    std::string start, end;
    std::cout << "Input starting point (lon,lat): ";
    std::getline(std::cin, start);
    std::cout << "Input ending point (lon,lat): ";
    std::getline(std::cin, end);
   
    

    std::vector<std::string> dfsPath; // Хранит лучший найденный путь через DFS
    auto start_time_dfs = std::chrono::high_resolution_clock::now();
    double dfsDistance = graph.dfs(start, end, dfsPath);
    auto end_time_dfs = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> dfsDuration = end_time_dfs - start_time_dfs;

    if (dfsDistance == std::numeric_limits<double>::infinity()) {
        std::cout << "No existing way from " << start << " to " << end << ".\n";
    }
    else {
        std::cout << "Shortest distance from " << start << " to " << end << " is " << dfsDistance
            << ". Time to execute: " << dfsDuration.count() << " secs.\n";
    }

    std::vector<std::string> bfsPath; // Хранит лучший найденный путь через BFS
    auto start_time_bfs = std::chrono::high_resolution_clock::now();
    double bfsDistance = graph.bfs(start, end, bfsPath);
    auto end_time_bfs = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> bfsDuration = end_time_bfs - start_time_bfs;

    if (bfsDistance == std::numeric_limits<double>::infinity()) {
        std::cout << "No existing way from " << start << " to " << end << ".\n";
    }
    else {
        std::cout << "Shortest distance from " << start << " to " << end << " is " << bfsDistance
            << ". Time to execute: " << bfsDuration.count() << " secs.\n";
    }

    std::vector<std::string> dijkstraPath; // Хранит лучший найденный путь через Дейкстру
    auto start_time_dj = std::chrono::high_resolution_clock::now();
    double dijkstraDistance = graph.dijkstra(start, end, dijkstraPath);
    auto end_time_dj = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> dijkstraDuration = end_time_dj - start_time_dj;

    if (dijkstraDistance == std::numeric_limits<double>::infinity()) {
        std::cout << "No existing way from " << start << " to " << end << ".\n";
    }
    else {
        std::cout << "Shortest distance from " << start << " to " << end << " is " << dijkstraDistance
            << ". Time to execute: " << dijkstraDuration.count() << " secs.\n";
    }

    std::vector<std::string> aStarPath; // Хранит лучший найденный путь через A*
    auto start_time_astar = std::chrono::high_resolution_clock::now();
    double aStarDistance = graph.aStar(start, end, aStarPath);
    auto end_time_astar = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> aStarDuration = end_time_astar - start_time_astar;

    if (aStarDistance == std::numeric_limits<double>::infinity()) {
        std::cout << "No existing way from " << start << " to " << end << ".\n";
    }
    else {
        std::cout << "Shortest distance from " << start << " to " << end << " is " << aStarDistance
            << ". Time to execute: " << aStarDuration.count() << " secs.\n";
    }

    // Освобождение памяти
    for (const auto& pair : graph.nodes) {
        delete pair.second;
    }
    return 0;
}