#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

#include "HashMap.h"

using namespace pSTL;

int main() {
    // ===== Test operator[] and basic insertion =====
    {
        HashMap<int, std::string> hm;
        assert(hm.size() == 0);
        assert(hm.empty());

        hm[1] = "one";
        hm[2] = "two";
        hm[3] = "three";

        assert(hm.size() == 3);
        assert(hm[1] == "one");
        assert(hm[2] == "two");
        assert(hm[3] == "three");

        std::string& val = hm[4];
        assert(val == "");
        val = "four";
        assert(hm[4] == "four");
    }

    // ===== Test at() =====
    {
        HashMap<int, std::string> hm;
        hm.insert(10, "ten");
        hm.insert(20, "twenty");

        assert(hm.at(10) == "ten");
        assert(hm.at(20) == "twenty");

        bool exceptionThrown = false;
        try {
            hm.at(30);
        }
        catch (const std::out_of_range&) {
            exceptionThrown = true;
        }
        assert(exceptionThrown);
    }

    // ===== Test find() =====
    {
        HashMap<int, std::string> hm;
        hm.insert(100, "hundred");
        hm.insert(200, "two hundred");

        auto nodePtr = hm.find(100);
        assert(nodePtr != nullptr);
        assert(nodePtr->getValue() == "hundred");

        assert(hm.find(300) == nullptr);
    }

    // ===== Test insert() =====
    {
        HashMap<int, std::string> hm;
        hm.insert(1, "one");
        hm.insert(1, "ONE");
        assert(hm[1] == "ONE");
    }

    // ===== Test erase() =====
    {
        HashMap<int, std::string> hm;
        hm.insert(1, "one");
        hm.insert(2, "two");
        hm.insert(3, "three");
        assert(hm.size() == 3);

        bool erased = hm.erase(2);
        assert(erased);
        assert(hm.size() == 2);
        assert(hm.find(2) == nullptr);

        erased = hm.erase(2);
        assert(!erased);
    }

    // ===== Test copy constructor =====
    {
        HashMap<int, std::string> hm;
        hm.insert(1, "one");
        hm.insert(2, "two");
        hm.insert(3, "three");

        HashMap<int, std::string> hmCopy = hm;
        assert(hmCopy.size() == hm.size());
        assert(hmCopy[1] == "one");
        assert(hmCopy[2] == "two");
        assert(hmCopy[3] == "three");

        hmCopy.insert(1, "ONE");
        assert(hmCopy[1] == "ONE");
        assert(hm[1] == "one");
    }

    // ===== Test copy assignment operator =====
    {
        HashMap<int, std::string> hm1;
        hm1.insert(10, "ten");
        hm1.insert(20, "twenty");

        HashMap<int, std::string> hm2;
        hm2.insert(30, "thirty");

        hm2 = hm1;
        assert(hm2.size() == hm1.size());
        assert(hm2[10] == "ten");
        assert(hm2[20] == "twenty");

        hm2 = hm2;
        assert(hm2.size() == hm1.size());
    }

    // ===== Test move constructor =====
    {
        HashMap<int, std::string> hm;
        hm.insert(1, "one");
        hm.insert(2, "two");

        HashMap<int, std::string> hmMoved = std::move(hm);
        assert(hmMoved.size() == 2);
        assert(hmMoved[1] == "one");
        assert(hmMoved[2] == "two");
    }

    // ===== Test move assignment operator =====
    {
        HashMap<int, std::string> hm1;
        hm1.insert(5, "five");
        hm1.insert(6, "six");

        HashMap<int, std::string> hm2;
        hm2.insert(7, "seven");

        hm2 = std::move(hm1);
        assert(hm2.size() == 2);
        assert(hm2[5] == "five");
        assert(hm2[6] == "six");
    }

    // ===== Test reserve() and grow() =====
    {
        HashMap<int, std::string> hm;
        for (int i = 0; i < 20; i++) {
            hm.insert(i, "num" + std::to_string(i));
        }
        size_t originalSize = hm.size();
        hm.reserve(100);
        for (int i = 0; i < 20; i++) {
            assert(hm[i] == "num" + std::to_string(i));
        }

        hm.grow();
        for (int i = 0; i < 20; i++) {
            assert(hm[i] == "num" + std::to_string(i));
        }
        assert(hm.size() == originalSize);
    }

    std::cout << "All tests passed successfully.\n";
}