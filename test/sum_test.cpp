#include <vector>
#include <catch2/catch.hpp>
#include "cinq/cinq.hpp"

TEST_CASE("Sum test", "[sum]")
{
    using namespace cinq;

    std::vector<int> v = {0, 1, 2, 3, 4};
    auto result = from(v) %
                  where([](auto i) { return i % 2 == 0; }) %
                  sum();

    REQUIRE(result == 6);

    auto result3 = from(v.cbegin(), v.cend()) %
                   where([](auto i) { return i % 2 == 0; }) %
                   sum_lazy();

    REQUIRE(result3.query() == 6);
}

