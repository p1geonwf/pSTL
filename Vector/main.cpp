#include <iostream>
#include <cassert>

#include "Vector.h"

using namespace pSTL;

int main() {
    Vector<int> a;
    assert(a.size() == 0);
    assert(a.empty());

    // push_back and growth
    a.push_back(1);
    a.push_back(2);
    a.push_back(3);
    assert(a.size() == 3);
    assert(a[0] == 1 && a[1] == 2 && a[2] == 3);

    // insert_at in the middle
    a.insert_at(1, 99); // or insert_at(1, 99) depending on your signature
    assert(a.size() == 4);
    assert(a[0] == 1 && a[1] == 99 && a[2] == 2 && a[3] == 3);

    // front/back
    assert(a.front() == 1);
    assert(a.back() == 3);

    // reserve should not lose data
    std::size_t oldCap = a.capacity();
    a.reserve(oldCap + 10);
    assert(a.capacity() >= oldCap + 10);
    assert(a[0] == 1 && a[1] == 99 && a[2] == 2 && a[3] == 3);

    // find
    assert(a.find(99) == 1);
    assert(a.find(123456) == static_cast<std::size_t>(-1));

    // pop_back shrinks
    a.pop_back();
    assert(a.size() == 3);
    assert(a.back() == 2);

    // at() bounds exception
    bool threw = false;
    try { (void)a.at(999); }
    catch (const std::out_of_range&) { threw = true; }
    assert(threw && "at() should throw on OOB");

    // range-for iteration (begin/end)
    int sum = 0;
    for (int v : a) sum += v;
    assert(sum == (1 + 99 + 2));

    std::cout << "All runtime tests passed.\n";
    return 0;
}