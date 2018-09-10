#pragma once

#include <iterator>
#include <numeric>
#include <cinq/enumerable.hpp>

namespace cinq
{

namespace detail
{
    template <typename InputIterator>
    auto sum_range(InputIterator begin, InputIterator end)
    {
        return std::accumulate(begin, end,
                typename InputIterator::value_type{});
    }

    template <typename T>
    auto sum_range(T* begin, T* end)
    {
        return std::accumulate(begin, end, T{});
    }
}

class sum_tag {};

class summer_tag
{
public:
    sum_tag operator()() const noexcept { return sum_tag{}; }
};

constexpr
summer_tag sum() noexcept
{
    return {};
}

template <typename Enumerable>
auto operator%(const Enumerable& range, const sum_tag&)
{
    return detail::sum_range(std::cbegin(range), std::cend(range));
}

template <typename Enumerable>
auto operator%(const Enumerable& range, const summer_tag&)
{
    return [begin = std::cbegin(range),
            end = std::cend(range)]
    {
        return detail::sum_range(begin, end);
    };
}

}
