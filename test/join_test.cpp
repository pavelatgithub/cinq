#include <iostream>
#include <list>
#include <vector>
#include <catch2/catch.hpp>
#include "cinq/cinq.hpp"

TEST_CASE("Join test", "[join]")
{
    using namespace cinq;

    std::vector<int> v = {0, 1, 2, 3, 4};

    std::vector<std::pair<int, std::string>> v2 = {
            {0, "zero"}, {2, "two"}, {3, "three"}, {5, "five"}};

    auto result = from(v) %
                  join(v2).on([](auto i, auto j) { return i == j.first; });

    std::list<std::pair<int, std::pair<int, std::string>>> res(
            result.begin(), result.end());

    std::list<std::pair<int, std::pair<int, std::string>>> exp{
        {0, {0, "zero"}}, {2, {2, "two"}}, {3, {3, "three"}}};

    REQUIRE(res == exp);
}


