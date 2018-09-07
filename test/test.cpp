#include <iostream>
#include <vector>
#include "cinq/cinq.hpp"

int main(int argc, char* argv[])
{
    using namespace cinq;

    std::vector<int> v = {0, 1, 2, 3, 4};

    //auto result = from(v)
    //              where([](int i) { return i % 2 == 0; })
    //              sum();
    for (auto n : from(v))
    {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    auto result = from(v) %
                  where([](auto i) { return i % 2 == 0; });

    for (auto n : result)
    {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    auto result2 = from(v) %
                   where([](auto i) { return i % 2 == 0; }) %
                   sum();

    std::cout << result2 << std::endl;

    auto result3 = from(v.cbegin(), v.cend()) %
                   where([](auto i) { return i % 2 == 0; }) %
                   sum_lazy();

    std::cout << result3.query() << std::endl;

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
