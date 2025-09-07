#pragma once

#include <algorithm>
#include <unordered_set>
#include <queue>
#include <utility>
#include <string>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <cstdint>
#include <limits>


namespace pSTL {
    typedef uint32_t id_t;
    typedef double weight_t;
    template <typename T> class Node;
    template <typename T> using edge_t = std::vector<std::pair<Node<T>*, weight_t>>;

    template <typename T>
    class Node {
    public:
        // Constructors 
        Node(id_t _id) : m_val(), m_id(_id), m_edges(), m_isObstacle(false) {}
        Node(T _val, id_t _id) : m_val(_val), m_id(_id), m_edges(), m_isObstacle(false) {}
        Node(T _val, id_t _id, edge_t<T> _edges)
            : m_val(_val), m_id(_id), m_edges(std::move(_edges)), m_isObstacle(false) {
        }
        Node(T _val, id_t _id, edge_t<T> _edges, bool _flag)
            : m_val(_val), m_id(_id), m_edges(std::move(_edges)), m_isObstacle(_flag) {
        }
        ~Node() = default;

        // Setters 
        void setVal(T _val) {
            m_val = _val;
        }
        void setId(id_t _id) {
            m_id = _id;
        }
        void setObstacle(bool _isObstacle) {
            m_isObstacle = _isObstacle;
        }
        void addEdge(std::pair<Node<T>*, weight_t> _edge) {
            m_edges.push_back(_edge);
        }
        void removeEdge(id_t _toId) {
            auto it = std::remove_if(m_edges.begin(), m_edges.end(),
                [&_toId](const std::pair<Node<T>*, weight_t>& edge) {
                    return edge.first->getId() == _toId;
                });

            m_edges.erase(it, m_edges.end()); // Calling erase on an empty range is safe
        }

        // Getters 
        T getVal() const {
            return m_val;
        }
        id_t getId() const {
            return m_id;
        }
        bool isObstacle() const {
            return m_isObstacle;
        }
        edge_t<T>& getEdges() {
            return m_edges;
        }

    private:
        T m_val;
        id_t m_id;
        bool m_isObstacle;
        edge_t<T> m_edges;
    };
}
