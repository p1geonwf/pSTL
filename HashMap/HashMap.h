#pragma once

#include <vector>
#include <stdexcept>
#include <functional>
#include <initializer_list>

template <typename key_t, typename value_t>
class Node {
public:
    Node(const key_t& key, const value_t& value)
        : m_key(key), m_val(value), m_next(nullptr) {
    }
    Node(const key_t& key, const value_t& value, Node* next)
        : m_key(key), m_val(value), m_next(next) {
    }

    const key_t& getKey() const {
        return m_key;
    }
    value_t& getValue() {
        return m_val;
    }
    const value_t& getValue() const {
        return m_val;
    }
    Node* getNext() const {
        return m_next;
    }
    Node*& nextRef() {
        return m_next;
    }

    void setNext(Node* next) {
        m_next = next;
    }

private:
    key_t m_key;
    value_t m_val;
    Node* m_next;
}; 

template <typename key_t, typename value_t>
class HashMap {
public:
    using node_t = Node<key_t, value_t>;

    HashMap(size_t capacity = 10)
        : m_capacity(capacity), m_size(0) {
        m_map.resize(m_capacity, nullptr);
    }

    HashMap(std::initializer_list<std::pair<key_t, value_t>> initList)
        : HashMap(initList.size() * 2) {
        for (const auto& p : initList) {
            insert(p.first, p.second);
        }
    }

    ~HashMap() {
        for (auto head : m_map) {
            while (head) {
                node_t* temp = head;
                head = head->getNext();
                delete temp;
            }
        }
    }

    HashMap(const HashMap& other)
        : m_capacity(other.m_capacity), m_size(other.m_size), m_map(other.m_capacity, nullptr) {
        for (size_t i = 0; i < m_capacity; i++) {
            node_t* currOther = other.m_map[i];
            node_t** currThis = &m_map[i]; // pointer to the bucket in the new map
            while (currOther) {
                *currThis = new node_t(currOther->getKey(), currOther->getValue());
                currOther = currOther->getNext();
                currThis = &((*currThis)->nextRef());
            }
        }
    }

    HashMap(HashMap&& other)
        : m_capacity(other.m_capacity), m_size(other.m_size), m_map(std::move(other.m_map)) {
        other.m_capacity = 0;
        other.m_size = 0;
    }

    HashMap& operator=(const HashMap& other) {
        if (this == &other)
            return *this;

        for (auto head : m_map) {
            while (head) {
                node_t* temp = head;
                head = head->getNext();
                delete temp;
            }
        }

        m_capacity = other.m_capacity;
        m_size = other.m_size;
        m_map.assign(m_capacity, nullptr);

        for (size_t i = 0; i < m_capacity; ++i) {
            node_t* currOther = other.m_map[i];
            node_t** currThis = &m_map[i];
            while (currOther) {
                *currThis = new node_t(currOther->getKey(), currOther->getValue());
                currOther = currOther->getNext();
                currThis = &((*currThis)->nextRef());
            }
        }
        return *this;
    }

    HashMap& operator=(HashMap&& other) {
        if (this == &other)
            return *this;

        for (auto head : m_map) {
            while (head) {
                node_t* temp = head;
                head = head->getNext();
                delete temp;
            }
        }

        m_capacity = other.m_capacity;
        m_size = other.m_size;
        m_map = std::move(other.m_map);

        other.m_capacity = 0;
        other.m_size = 0;

        return *this;
    }

    value_t& operator[](const key_t& key) {
        size_t index = std::hash<key_t>{}(key) % m_capacity;
        node_t* curr = m_map[index];

        while (curr) {
            if (curr->getKey() == key)
                return curr->getValue();
            curr = curr->getNext();
        }

        node_t* newNode = new node_t(key, value_t{});
        newNode->setNext(m_map[index]);
        m_map[index] = newNode;
        m_size++;
        return newNode->getValue();
    }

    value_t& at(const key_t& key) {
        size_t index = std::hash<key_t>{}(key) % m_capacity;
        node_t* curr = m_map[index];
        while (curr) {
            if (curr->getKey() == key)
                return curr->getValue();
            curr = curr->getNext();
        }
        throw std::out_of_range("Key not found");
    }

    node_t* find(const key_t& key) {
        size_t index = std::hash<key_t>{}(key) % m_capacity;
        node_t* curr = m_map[index];
        while (curr) {
            if (curr->getKey() == key)
                return curr;
            curr = curr->getNext();
        }
        return nullptr;
    }

    size_t size() const {
        return m_size;
    }

    bool empty() const {
        return m_size == 0;
    }

    void insert(const key_t& key, const value_t& value) {
        size_t index = std::hash<key_t>{}(key) % m_capacity;
        node_t* curr = m_map[index];

        while (curr) {
            if (curr->getKey() == key) {
                curr->getValue() = value;
                return;
            }
            curr = curr->getNext();
        }

        node_t* newNode = new node_t(key, value);
        newNode->setNext(m_map[index]);
        m_map[index] = newNode;
        m_size++;
    }

    bool erase(const key_t& key) {
        size_t index = std::hash<key_t>{}(key) % m_capacity;
        node_t* curr = m_map[index];
        node_t* prev = nullptr;

        while (curr) {
            if (curr->getKey() == key) {
                if (prev) {
                    prev->setNext(curr->getNext());
                }
                else {
                    m_map[index] = curr->getNext();
                }
                delete curr;
                m_size--;
                return true;
            }
            prev = curr;
            curr = curr->getNext();
        }
        return false;
    }

    void reserve(size_t newCapacity) {
        if (newCapacity <= m_capacity)
            return;

        std::vector<node_t*> newMap(newCapacity, nullptr);
        for (size_t i = 0; i < m_capacity; ++i) {
            node_t* current = m_map[i];
            while (current) {
                node_t* next = current->getNext();
                size_t newIndex = std::hash<key_t>{}(current->getKey()) % newCapacity;
                current->setNext(newMap[newIndex]);
                newMap[newIndex] = current;
                current = next;
            }
        }
        m_map = std::move(newMap);
        m_capacity = newCapacity;
    }

    void grow() {
        reserve(m_capacity * 2);
    }
private:
    size_t m_capacity;
    size_t m_size;
    std::vector<node_t*> m_map; 
    /* 
    * Vector of linked list heads
    * Each element is a pointer to a head of a separate linked list
    * Not the most optimal solution for closed addressing hash maps
    * Better soltuion would be to use 2 arrays only (cache locality)
    */
};