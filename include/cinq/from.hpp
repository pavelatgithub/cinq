#pragma once

#include <cinq/enumerable.hpp>

namespace cinq
{

template <typename Container>
auto from(const Container& container) noexcept
{
    auto begin = std::cbegin(container);
    return enumerable<decltype(begin)>{begin, std::cend(container)};
}

template <typename InputIterator>
auto from(InputIterator begin, InputIterator end) noexcept
{
    return enumerable<InputIterator>{begin, end};
}

}
