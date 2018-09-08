#include <list>
#include <vector>
#include <catch2/catch.hpp>
#include "cinq/cinq.hpp"

TEST_CASE("Simple from/where test", "[where]")
{
    using namespace cinq;

    std::vector<int> v = {0, 1, 2, 3, 4};
    auto result = from(v) %
                  where([](auto i) { return i % 2 == 0; });

    std::list<int> v2(result.begin(), result.end());
    REQUIRE(v2 == std::list<int>{0, 2, 4});
}
