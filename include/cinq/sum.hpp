#pragma once

#include <numeric>
#include <cinq/enumerable.hpp>

namespace cinq
{

// Immediate sum calculation.

class sum_tag {};

inline auto sum() noexcept
{
    return sum_tag{};
}

template <typename Range>
auto operator%(const Range& range, const sum_tag&)
{
    // TODO: support arrays?
    return std::accumulate(std::cbegin(range), std::cend(range),
            typename Range::value_type{});
}

// Lazy sum calculation
template <typename Range>
class summer
{
public:
    using range_type = Range;
    using value_type = typename range_type::value_type;

public:
    summer(const range_type& range)
        : _range{range}
    {}

    value_type query() const
    {
        return std::accumulate(
                std::cbegin(_range), std::cend(_range), value_type{});
    }

private:
    const range_type& _range;
};

class summer_tag {};

inline auto sum_lazy() noexcept
{
    return summer_tag{};
}

template <typename Range>
auto operator%(const Range& range, const summer_tag&)
{
    return summer<Range>{range};
}
}
