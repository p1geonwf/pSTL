#include <iostream>
#include <vector>
#include <chrono>

#include "Node.h"
#include "Graph.h"

using namespace pSTL;

int main() {
    Graph<std::pair<double, double>> graph;
    
    const int GRID_SIZE = 1000;
    std::vector<pSTL::Node<std::pair<double, double>>*> nodes(GRID_SIZE * GRID_SIZE);

    id_t id = 0;
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; ++i) {
        nodes[i] = new pSTL::Node<std::pair<double, double>>({ i % GRID_SIZE, i / GRID_SIZE }, id++);
        graph.addNode(nodes[i]);
    }

    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            int nodeId = nodes[i * GRID_SIZE + j]->getId();

            if (j + 1 < GRID_SIZE) {
                int rightNodeId = nodes[i * GRID_SIZE + (j + 1)]->getId();
                graph.addEdgeUndirected(nodeId, rightNodeId, 1.0);
            }

            if (i + 1 < GRID_SIZE) {
                int bottomNodeId = nodes[(i + 1) * GRID_SIZE + j]->getId();
                graph.addEdgeUndirected(nodeId, bottomNodeId, 1.0);
            }

            if (i + 1 < GRID_SIZE && j + 1 < GRID_SIZE) {
                int bottomRightNodeId = nodes[(i + 1) * GRID_SIZE + (j + 1)]->getId();
                graph.addEdgeUndirected(nodeId, bottomRightNodeId, 1.0);
            }

            if (i + 1 < GRID_SIZE && j - 1 >= 0) {
                int bottomLeftNodeId = nodes[(i + 1) * GRID_SIZE + (j - 1)]->getId();
                graph.addEdgeUndirected(nodeId, bottomLeftNodeId, 1.0);
            }
        }
    }
    
    auto startTime = std::chrono::high_resolution_clock::now();
    /*
    graph.loadFromFile("test.txt");
    
    graph.DFS(0);
    std::cout << "\n";
    graph.BFS(0);
    std::cout << "\n";
    */
    std::unordered_map<id_t, weight_t> Dijkstra = graph.Dijkstra(0);
    std::vector<id_t> AStar = graph.AStar(0,999999);

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = endTime - startTime;
    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";

    bool breakPoint = true; // Set break point here to check the results of searches
}