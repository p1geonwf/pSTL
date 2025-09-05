#pragma once

#include "Node.h"

// Custom Hash for std::pair<double, double>
struct pair_hash {
    size_t operator()(const std::pair<double, double>& pair) const noexcept {
        size_t h1 = std::hash<double>{}(pair.first);
        size_t h2 = std::hash<double>{}(pair.second);

        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};

template <typename T>
class Graph {
public:
    // Constructors/Destructors (Delete other constructors)
    ~Graph() {
        for (auto& pair : m_nodes) {
            delete pair.second;
        }
        m_nodes.clear();
        m_lookupMap.clear();
    }

    // Checkers
    bool doesNodeExist(id_t id) const {
        return m_nodes.find(id) != m_nodes.end();
    }

    bool isValidEdge(id_t _fromId, id_t _toId) const {
        return doesNodeExist(_fromId) && doesNodeExist(_toId);
    }

    bool isValidWeight(id_t _fromId, id_t _toId) const {
        if (isValidEdge(_fromId, _toId)) {
            auto fromNodeIt = m_nodes.find(_fromId);
            if (fromNodeIt != m_nodes.end()) {
                Node<T>* fromNode = fromNodeIt->second;
                for (const auto& edge : fromNode->getEdges()) {
                    if (edge.first->getId() == _toId) {
                        return edge.second >= 0;
                    }
                }
            }
        }
        return false;
    }

    bool isGraphValid() const {
        std::unordered_set<id_t> idSet;

        for (const auto& pair : m_nodes) {
            Node<T>* node = pair.second;
            if (!idSet.insert(node->getId()).second) {
                std::cerr << "Duplicate ID detected: " << node->getId() << std::endl;
                return false;
            }
        }

        for (const auto& pair : m_nodes) {
            Node<T>* node = pair.second;
            for (const auto& e : node->getEdges()) {
                Node<T>* edge = e.first;
                weight_t weight = e.second;

                if (doesNodeExist(edge->getId())) {
                    std::cerr << " (ID: " << node->getId() << ") references a non-existent Node"
                        << "(ID: " << edge->getId() << ")!" << std::endl;
                    return false;
                }

                if (!isValidWeight(node->getId(), edge->getId())) {
                    std::cerr << "Invalid weight on edge from Node (ID: " << node->getId()
                        << ") to Node (ID: " << edge->getId() << ")!" << std::endl;
                    return false;
                }
            }
        }

        return true;
    }

    // Setters
    void addNode(Node<T>* _node) {
        if (!doesNodeExist(_node->getId())) {
            m_nodes[_node->getId()] = _node;
            m_lookupMap[_node->getVal()] = _node->getId();
        }
        else {
            std::cerr << "Node already exists in the graph!" << std::endl;
        }
    }

    void addEdgeDirected(id_t _from, id_t _to, weight_t _weight) {
        if (isValidEdge(_from, _to)) {
            m_nodes[_from]->addEdge({ m_nodes[_to], _weight });
        }
        else {
            std::cerr << "Invalid edge: One or both nodes not found!" << std::endl;
        }
    }

    void addEdgeUndirected(id_t _from, id_t _to, weight_t _weight) {
        if (isValidEdge(_from, _to) && !m_nodes[_from]->isObstacle() && !m_nodes[_to]->isObstacle()) {
            m_nodes[_from]->addEdge({ m_nodes[_to], _weight });
            m_nodes[_to]->addEdge({ m_nodes[_from], _weight });
        }
        else {
            std::cerr << "Invalid edge or obstacle detected!" << std::endl;
        }
    }

    void removeNode(id_t _nodeId) {
        auto it = m_nodes.find(_nodeId);
        m_lookupMap.erase(m_lookupMap.find(m_nodes[_nodeId]->getVal()));

        if (it != m_nodes.end()) {
            Node<T>* nodeToRemove = it->second;

            for (auto& [id, node] : m_nodes) {
                node->removeEdge(nodeToRemove->getId());
            }

            delete nodeToRemove;
            m_nodes.erase(it);

        }
        else {
            std::cerr << "Node with ID " << _nodeId << " not found!" << std::endl;
        }
    }

    void removeEdgeDirected(id_t _fromId, id_t _toId) {
        auto it = m_nodes.find(_fromId);
        if (it != m_nodes.end()) {
            Node<T>* fromNode = it->second;
            if (findEdge(_fromId, _toId)) {
                fromNode->removeEdge(_toId);
            }
        }
    }

    void removeEdgeUndirected(id_t _fromId, id_t _toId) {
        removeEdgeDirected(_fromId, _toId);
        removeEdgeDirected(_toId, _fromId);
    }

    void setWeightDirected(id_t _fromId, id_t _toId, weight_t weight) {
        auto fromIt = m_nodes.find(_fromId);
        auto toIt = m_nodes.find(_toId);

        if (fromIt != m_nodes.end() && toIt != m_nodes.end()) {
            Node<T>* fromNode = fromIt->second;
            Node<T>* toNode = toIt->second;

            bool edgeFound = false;
            for (auto& edge : fromNode->getEdges()) {
                if (edge.first->getId() == _toId) {
                    edge.second = weight;
                    edgeFound = true;
                    break;
                }
            }
            if (!edgeFound) {
                std::cerr << "Edge from " << _fromId << " to " << _toId << " not found!" << std::endl;
            }
        }
        else {
            std::cerr << "Invalid edge: One or both nodes not found!" << std::endl;
        }
    }

    void setWeightUndirected(id_t _fromId, id_t _toId, weight_t weight) {
        setWeightDirected(_fromId, _toId, weight);
        setWeightDirected(_toId, _fromId, weight);
    }

    // Getters
    weight_t getWeight(id_t _fromId, id_t _toId) const {
        auto fromIt = m_nodes.find(_fromId);
        if (fromIt != m_nodes.end()) {
            Node<T>* fromNode = fromIt->second;

            const edge_t<T>& edges = fromNode->getEdges();
            for (const auto& edge : edges) {
                if (edge.first->getId() == _toId) {
                    return edge.second;
                }
            }
        }

        std::cerr << "Edge from " << _fromId << " to " << _toId << " not found!" << std::endl;
        return -1.0;
    }

    bool findEdge(id_t _fromId, id_t _toId) const {
        auto fromIt = m_nodes.find(_fromId);
        if (fromIt != m_nodes.end()) {
            const edge_t<T>& edges = fromIt->second->getEdges();
            for (const auto& edge : edges) {
                if (edge.first->getId() == _toId) {
                    return true;
                }
            }
        }
        return false;
    }

    // DFS/BFS

    void DFS(id_t _startId) {
        auto startIt = m_nodes.find(_startId);
        if (startIt != m_nodes.end()) {
            std::unordered_set<id_t> visited;
            DFSHelper(startIt->second, visited);
        }
        else {
            std::cerr << "Node with ID " << _startId << " not found!" << std::endl;
        }
    }

    void DFSHelper(Node<T>* _node, std::unordered_set<id_t>& _visited) {
        if (_node == nullptr || _visited.count(_node->getId()))
            return;

        _visited.insert(_node->getId());
        std::cout << _node->getVal() << ' ';

        for (auto& edge : _node->getEdges()) {
            DFSHelper(edge.first, _visited);
        }
    }

    void BFS(id_t _startId) {
        auto startIt = m_nodes.find(_startId);
        if (startIt == m_nodes.end()) {
            std::cerr << "Node with ID " << _startId << " not found!" << std::endl;
            return;
        }

        std::queue<id_t> q;
        std::unordered_set<id_t> visited;

        q.push(_startId);
        visited.insert(_startId);

        while (!q.empty()) {
            id_t currentId = q.front();
            q.pop();

            Node<T>* node = m_nodes[currentId];
            std::cout << node->getVal() << ' ';

            for (auto& edge : node->getEdges()) {
                id_t edgeId = edge.first->getId();
                if (!visited.count(edgeId)) {
                    q.push(edgeId);
                    visited.insert(edgeId);
                }
            }
        }
    }

    // Dijkstra
    std::unordered_map<id_t, weight_t> Dijkstra(id_t _startNodeId) {
        auto startId = m_nodes.find(_startNodeId);
        if (startId == m_nodes.end()) {
            std::cerr << "Node with ID " << _startNodeId << " not found!" << std::endl;
            return {};
        }
        Node<T>* _startNode = startId->second;

        std::unordered_map<id_t, bool> visited;
        std::unordered_map<id_t, weight_t> distance;

        for (const auto& nodePair : m_nodes) {
            distance[nodePair.first] = std::numeric_limits<weight_t>::infinity();
        }
        distance[_startNode->getId()] = 0.0;

        auto compare = [&distance](Node<T>* left, Node<T>* right) {
            return distance[left->getId()] > distance[right->getId()];
        };

        std::priority_queue<Node<T>*, std::vector<Node<T>*>, decltype(compare)> pq(compare); // Priority queue (min-heap)
        pq.push(_startNode);

        while (!pq.empty()) {
            Node<T>* node = pq.top();
            pq.pop();

            if (visited[node->getId()]) {
                continue;
            }
            visited[node->getId()] = true;

            for (const auto& edge : node->getEdges()) {
                Node<T>* neighbor = edge.first;
                weight_t edgeWeight = edge.second;
                weight_t newDist = distance[node->getId()] + edgeWeight;

                if (newDist < distance[neighbor->getId()]) {
                    distance[neighbor->getId()] = newDist;
                    pq.push(neighbor);
                }
            }
        }

        return distance;
    }

    // A* Implementation
    std::vector<id_t> AStar(id_t _startId, id_t _endId) {
        std::unordered_map<id_t, weight_t> gScore;
        std::unordered_map<id_t, weight_t> fScore;
        std::unordered_map<id_t, id_t> cameFrom;
        std::priority_queue<
            std::pair<weight_t, id_t>,
            std::vector<std::pair<weight_t, id_t>>,
            std::greater<std::pair<weight_t, id_t>>
        > openSet;

        for (const auto& pair : m_nodes) {
            gScore[pair.first] = std::numeric_limits<weight_t>::infinity();
        }
        gScore[_startId] = 0.0;

        openSet.push({ 0.0, _startId });
        fScore[_startId] = heuristic(_startId, _endId);

        while (!openSet.empty()) {
            id_t current = openSet.top().second;
            openSet.pop();

            if (current == _endId) {
                return reconstructPath(cameFrom, current);
            }

            for (auto& edge : m_nodes[current]->getEdges()) {
                id_t neighborId = edge.first->getId();
                weight_t weight = edge.second;

                if (m_nodes[neighborId]->isObstacle()) {
                    continue; // Skip obstacles
                }

                weight_t tentativeGScore = gScore[current] + weight;

                if (tentativeGScore < gScore[neighborId]) {
                    cameFrom[neighborId] = current;
                    gScore[neighborId] = tentativeGScore;
                    fScore[neighborId] = gScore[neighborId] + heuristic(neighborId, _endId);
                    openSet.push({ fScore[neighborId], neighborId });
                }
            }
        }

        return {};
    }

    weight_t heuristic(id_t _startId, id_t _endId) {
        Node<T>* start = m_nodes[_startId];
        int x1 = start->getVal().first;
        int y1 = start->getVal().second;

        Node<T>* end = m_nodes[_endId];
        int x2 = end->getVal().first;
        int y2 = end->getVal().second;

        return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
    }

    std::vector<id_t> reconstructPath(std::unordered_map<id_t, id_t>& _cameFrom, id_t _currentId) {
        std::vector<id_t> path;
        path.push_back(_currentId);

        while (_cameFrom.find(_currentId) != _cameFrom.end()) {
            _currentId = _cameFrom[_currentId];
            path.push_back(_currentId);
        }

        std::reverse(path.begin(), path.end());
        return path;
    }

    // Save/Load

    void saveToFile(const std::string& _filename) const {
        std::ofstream outFile(_filename);
        if (!outFile.is_open()) {
            std::cerr << "Failed to open file for writing." << std::endl;
            return;
        }

        for (const auto& nodePair : m_nodes) {
            Node<T>* node = nodePair.second;
            outFile << "ID: " << static_cast<int>(node->getId())
                << " Val: " << node->getVal()
                << " Edges: ";

            for (const auto& edge : node->getEdges()) {
                outFile << "(" << edge.first->getId() << ", " << edge.first->getVal() << ", " << edge.second << ") ";
            }
            outFile << "\n";
        }

        outFile.close();
        std::cout << "Graph saved to file: " << _filename << std::endl;
    }

    void loadFromFile(const std::string& _filename) {
        std::ifstream inFile(_filename);
        if (!inFile.is_open()) {
            std::cerr << "Failed to open file for reading." << std::endl;
            return;
        }

        std::string line;
        while (std::getline(inFile, line)) {
            std::stringstream ss(line);
            std::string label;

            id_t nodeId;
            ss >> label >> nodeId;

            T nodeValue;
            ss >> label >> nodeValue;
            Node<T>* currentNode;

            if (m_nodes.find(nodeId) == m_nodes.end()) {
                currentNode = new Node<T>(nodeValue, nodeId);
                m_nodes[nodeId] = currentNode;
            }
            else {
                currentNode = m_nodes[nodeId];
            }

            char ignore;
            T edgeValue;
            id_t edgeId;
            weight_t edgeWeight;

            ss >> label;
            while (ss >> ignore >> edgeId >> ignore >> edgeValue >> ignore >> edgeWeight >> ignore) {
                Node<T>* edgeNode;
                if (m_nodes.find(edgeId) == m_nodes.end()) {
                    edgeNode = new Node<T>(edgeValue, edgeId);
                    m_nodes[edgeId] = edgeNode;
                }
                else {
                    edgeNode = m_nodes[edgeId];
                }

                currentNode->addEdge({ edgeNode, edgeWeight });
            }
        }

        inFile.close();
        std::cout << "Graph loaded from file: " << _filename << std::endl;
    }

private:
    std::unordered_map<id_t, Node<T>*> m_nodes;
    std::unordered_map<std::pair<int, int>, id_t, pair_hash> m_lookupMap;
};

/********************************** Operator Overloads ****************************************/

std::ostream& operator<<(std::ostream& os, const std::pair<double, double>& p) {
    os << "{" << p.first << ", " << p.second << "}";
    return os;
}

std::istream& operator>>(std::istream& is, std::pair<double, double>& p) {
    char ignore;
    is >> ignore >> p.first >> ignore >> p.second >> ignore;
    return is;
}