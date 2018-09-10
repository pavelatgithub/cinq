#include <vector>
#include <catch2/catch.hpp>
#include "cinq/cinq.hpp"

TEST_CASE("Empty input", "[sum]")
{
    REQUIRE(std::vector<int>{} % cinq::sum()() == 0);
}

TEST_CASE("Raw array", "[sum]")
{
    int v[] = {0, 1, 2, 3, 4};
    REQUIRE(v % cinq::sum()() == 10);
}

TEST_CASE("Simple sum query", "[sum]")
{
    using namespace cinq;

    std::vector<int> v = {0, 1, 2, 3, 4};

    SECTION("Immediate sum calculation")
    {
        auto result = from(v) %
                      where([](auto i) { return i % 2 == 0; }) %
                      sum()();
        REQUIRE(result == 6);
    }
    SECTION("Lazy sum calculation")
    {
        auto result = from(v.cbegin(), v.cend()) %
                      where([](auto i) { return i % 2 == 0; }) %
                      sum();
        REQUIRE(result() == 6);
    }
}
