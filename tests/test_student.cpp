#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include "cs225/PNG.h"
#include "dsets.h"
#include "maze.h"
#include "mazereader.h"

// You may write your own test cases in this file to test your code.
// Test cases in this file are not graded.

TEST_CASE("compression1", "[part1][student]") {
    DisjointSets ds(5);
    ds.setUnion(0, 1);
    ds.setUnion(2, 3);
    ds.setUnion(3, 4);
    REQUIRE(2 == ds.size(1));
    REQUIRE(3 == ds.size(3));
    ds.setUnion(0, 4);
    std::cout << ds.getValue(0) << std::endl;
    int root = ds.find(0);
    std::cout << ds.getValue(0) << std::endl;
}
