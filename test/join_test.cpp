#include <iostream>
#include <vector>
#include <catch2/catch.hpp>
#include "cinq/cinq.hpp"

TEST_CASE("Join test", "[join]")
{
    using namespace cinq;

    std::vector<int> v = {0, 1, 2, 3, 4};

    std::vector<std::pair<int, std::string>> v2 = {
            {0, "zero"}, {2, "two"}, {3, "three"}, {4, "four"}, {5, "five"}};

    auto result4 = from(v) %
                   join(v2).on([](auto i, auto j) { return i == j.first; });
    for (auto n : result4)
    {
        std::cout << n.first << ": " << n.second.first << ", "
                  << n.second.second << std::endl;
    }
}


