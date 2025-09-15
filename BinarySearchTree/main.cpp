#include <iostream>
#include <cassert>
#include <sstream>

#include "BinarySearchTree.h"

using namespace pSTL;

void testInsertionAndContains() {
	BST<int> bst;
	bst.insert(10);
	bst.insert(5);
	bst.insert(15);
	bst.insert(2);
	bst.insert(7);
	bst.insert(12);
	bst.insert(20);

	assert(bst.contains(10));
	assert(bst.contains(5));
	assert(bst.contains(15));
	assert(bst.contains(2));
	assert(bst.contains(7));
	assert(bst.contains(12));
	assert(bst.contains(20));
	assert(!bst.contains(100));
}

void testFindMinMax() {
	BST<int> bst;
	bst.insert(8);
	bst.insert(3);
	bst.insert(10);
	bst.insert(1);
	bst.insert(6);
	bst.insert(14);

	assert(bst.findMin() == 1);
	assert(bst.findMax() == 14);
}

void testDuplicateInsertion() {
	BST<int> bst;
	bst.insert(5);
	bst.insert(5); // duplicate
	assert(bst.contains(5));
	// Can't directly assert that duplicates aren't inserted,
	// but if insert does nothing, structure remains correct
}

void testIsEmpty() {
	BST<int> bst;
	assert(bst.isEmpty());
	bst.insert(42);
	assert(!bst.isEmpty());
}

void testPrintTree() {
	BST<int> bst;
	bst.insert(3);
	bst.insert(1);
	bst.insert(4);
	bst.insert(2);

	std::ostringstream out;
	bst.printTree(out);
	assert(out.str() == "1 2 3 4 \n" || out.str() == "1 2 3 4 \n"); // basic sanity
}

void testRemoveLeaf() {
	BST<int> bst;
	bst.insert(10);
	bst.insert(5);
	bst.insert(15);
	bst.remove(5);
	assert(!bst.contains(5));
}

void testRemoveNodeWithOneChild() {
	BST<int> bst;
	bst.insert(10);
	bst.insert(5);
	bst.insert(3);
	bst.remove(5);
	assert(!bst.contains(5));
	assert(bst.contains(3));
}

void testRemoveNodeWithTwoChildren() {
	BST<int> bst;
	bst.insert(10);
	bst.insert(5);
	bst.insert(15);
	bst.insert(12);
	bst.insert(17);
	bst.remove(15);
	assert(!bst.contains(15));
	assert(bst.contains(12));
	assert(bst.contains(17));
}

void testCopyConstructor() {
	BST<int> original;
	original.insert(1);
	original.insert(2);
	original.insert(3);

	BST<int> copy = original;
	assert(copy.contains(1));
	assert(copy.contains(2));
	assert(copy.contains(3));
}

void testMoveConstructor() {
	BST<int> original;
	original.insert(1);
	original.insert(2);
	original.insert(3);

	BST<int> moved = std::move(original);
	assert(moved.contains(1));
	assert(moved.contains(2));
	assert(moved.contains(3));
	assert(original.isEmpty());
}

void testCopyAssignment() {
	BST<int> a, b;
	a.insert(7);
	a.insert(3);
	a.insert(9);

	b = a;
	assert(b.contains(3));
	assert(b.contains(7));
	assert(b.contains(9));
}

void testMoveAssignment() {
	BST<int> a, b;
	a.insert(8);
	a.insert(4);
	a.insert(12);

	b = std::move(a);
	assert(b.contains(4));
	assert(b.contains(8));
	assert(b.contains(12));
	assert(a.isEmpty());
}

int main() {
	testInsertionAndContains();
	testFindMinMax();
	testDuplicateInsertion();
	testIsEmpty();
	testPrintTree();
	testRemoveLeaf();
	testRemoveNodeWithOneChild();
	testRemoveNodeWithTwoChildren();
	testCopyConstructor();
	testMoveConstructor();
	testCopyAssignment();
	testMoveAssignment();

	std::cout << "All tests passed successfully.\n";
	return 0;
}
