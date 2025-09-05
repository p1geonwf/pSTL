#pragma once

#include <iostream>
#include <ostream>
#include <utility>
#include <stdexcept>
#include <functional>

template <typename T> 
class Node {
public:
	// Unified constructor for handling ambiguous parameters
	template<typename U>
	Node(U&& val, Node* left = nullptr, Node* right = nullptr)
		: m_val(std::forward<U>(val)), m_left(left), m_right(right) {
	}

	// Nodes will shadow copy each other
	// No ownership taking so defaults will work 
	virtual ~Node() = default;
	Node(const Node&) = default;
	Node(Node&&) = default;
	Node& operator=(const Node&) = default;
	Node& operator=(Node&&) = default;
	
	T m_val;
	Node* m_left;
	Node* m_right;
};

template <typename T>
class BST {
public:
	BST() : m_head(nullptr) {}
	BST(const BST& other) : m_head(clone(other.m_head)) {}
	BST(BST&& other) noexcept : m_head(other.m_head) {
		other.m_head = nullptr;
	}
	~BST() {
		makeEmpty();
	}
	
	BST& operator=(const BST& other) {
		if (this != &other) {
			makeEmpty();
			m_head = clone(other.m_head);
		}
		return *this;
	}
	BST& operator=(BST&& other) noexcept {
		if (this != &other) {
			makeEmpty();
			m_head = other.m_head;
			other.m_head = nullptr;
		}
		return *this;
	}

	Node<T>* getHead() const {
		return m_head;
	}
	Node<T>*& getHeadRef() {
		return m_head;
	}

	const T& findMin() const {
		return findMin(getHead());
	}
	const T& findMax() const {
		return findMax(getHead());
	}

	bool contains(const T& val) const {
		return contains(getHead(), val);
	}
	bool isEmpty() const {
		return (getHead() == nullptr) ? true : false;
	}

	void printTree(std::ostream& out = std::cout) const {
		printTree(getHead(), out);
		out << std::endl;
	}

	void makeEmpty() {
		return makeEmpty(getHeadRef());
	}

	void insert(const T& val) {
		insert(val, getHeadRef());
	}
	void insert(T&& val) {
		insert(std::forward<T>(val), getHeadRef());
	}
	void remove(const T& val) {
		return remove(val, getHeadRef());
	}

private:
	Node<T>* m_head;

	const T& findMin(Node<T>* curr) const {
		if (curr == nullptr) {
			throw std::runtime_error("BST is empty");
		}
		while (curr->m_left != nullptr) {
			curr = curr->m_left;
		}
		return curr->m_val;
	}
	const T& findMax(Node<T>* curr) const {
		if (curr == nullptr) {
			throw std::runtime_error("BST is empty");
		}
		while (curr->m_right != nullptr) {
			curr = curr->m_right;
		}
		return curr->m_val;
	}

	bool contains(Node<T>* node, const T& val) const {
		while (node != nullptr) {
			if (node->m_val == val) {
				return true;
			}
			else if (val > node->m_val) {
				node = node->m_right;
			}
			else {
				node = node->m_left;
			}
		}
		return false;
	}	
	
	void printTree(Node<T>* node, std::ostream& out) const {
		if (node) {
			printTree(node->m_left, out);
			out << node->m_val << " ";
			printTree(node->m_right, out);
		}
	}

	void makeEmpty(Node<T>*& curr) {
		while (curr != nullptr) {
			makeEmpty(curr->m_left);
			makeEmpty(curr->m_right);
			delete curr;
			curr = nullptr;
		}
	}
	void insert(const T& val, Node<T>*& node) {
		if (node == nullptr) {
			node = new Node<T>(val, nullptr, nullptr);
		}
		else if (node->m_val > val) {
			insert(val, node->m_left);
		}
		else if (node->m_val < val) {
			insert(val, node->m_right);
		}
		else {
			// duplicate
		}
	}
	void insert(T&& val, Node<T>*& node) {
		if (node == nullptr) {
			node = new Node<T>(std::move(val), nullptr, nullptr);
		}
		else if (node->m_val > val) {
			insert(std::forward<T>(val), node->m_left);
		}
		else if (node->m_val < val) {
			insert(std::forward<T>(val), node->m_right);
		}
		else {
			// duplicate
		}
	}
	void remove(const T& val, Node<T>*& node) {
		if (node == nullptr)
			return;

		if (val < node->m_val) {
			remove(val, node->m_left);
		}
		else if (node->m_val < val) {
			remove(val, node->m_right);
		}
		else if (node->m_left != nullptr && node->m_right != nullptr) {
			node->m_val = findMin(node->m_right);
			remove(node->m_val, node->m_right);
		}
		else {
			Node<T>* oldNode = node;
			node = (node->m_left != nullptr) ? node->m_left : node->m_right;
			delete oldNode;
		}
	}
	Node<T>* clone(Node<T>* node) const {
		if (node == nullptr) {
			return nullptr;
		}
		else {
			return new Node<T>{ node->m_val, clone(node->m_left), clone(node->m_right) };
		}
	}
};