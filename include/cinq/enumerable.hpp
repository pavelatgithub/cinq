#pragma once

namespace cinq {
/**
 * A wrapper for anything enumerable based on the given iterator type.
 */
template <typename InputIterator>
class enumerable
{
public:
    using iterator = InputIterator;
    using value_type = typename iterator::value_type;

public:
    enumerable(iterator begin, iterator end) noexcept
        : _begin{std::move(begin)},
          _end{std::move(end)}
    {}

    iterator begin() noexcept { return _begin; }
    iterator begin() const noexcept { return _begin; }
    iterator end() noexcept { return _end; }
    iterator end() const noexcept { return _end; }

    iterator cbegin() const noexcept { return _begin; }
    iterator cend() const noexcept { return _end; }

private:
    iterator _begin;
    iterator _end;
};

}
