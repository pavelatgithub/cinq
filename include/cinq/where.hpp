#pragma once

#include <algorithm>
#include <iterator>
#include <cinq/enumerable.hpp>

namespace cinq
{

//
// Wrapper on any given iterator that adds conditional increment. This is
// compliant solely with InputIterator requirements, but not others.
//
// Note that although this provides difference_type, it does not provide a real
// difference (i.e. operator-), because resulting data is not yet known. This
// means that construction vector from this iterator is not going to work, as
// vector takes difference between two iterators to reserve capacity. But other
// non-contiguous containers, like list, work fine.
//
template <typename Iterator, typename Predicate>
class where_iterator
{
public:
    using iterator_type     = Iterator;
    using iterator_category = std::input_iterator_tag;
    using value_type        = typename iterator_type::value_type;
    using difference_type   = typename iterator_type::difference_type;
    using pointer           = typename iterator_type::pointer;
    using reference         = typename iterator_type::reference;

public:
    where_iterator(iterator_type base, iterator_type end, Predicate pred)
        : _it{ std::move(base) },
          _end{ std::move(end) },
          _pred{ std::move(pred) }
    {}

    where_iterator& operator++()
    {
        _it = std::find_if(++_it, _end, _pred);
        return *this;
    }

    where_iterator operator++(int)
    {
        where_iterator tmp(*this);
        operator++();
        return tmp;
    }

    reference operator*()  const { return _it.operator*(); }
    pointer   operator->() const { return _it.operator->(); }

    friend
    bool operator==(const where_iterator& lhs,
                    const where_iterator& rhs)
    {
        return lhs._it == rhs._it;
    }

    friend
    bool operator!=(const where_iterator& lhs,
                    const where_iterator& rhs)
    {
        return !(lhs == rhs);
    }

private:
    iterator_type _it;
    iterator_type _end;
    Predicate     _pred;
};

template <typename Predicate>
struct where_closure
{
    Predicate pred;
};

template <typename Predicate>
auto where(Predicate&& pred) noexcept
{
    return where_closure<Predicate>{ std::forward<Predicate>(pred) };
}

template <typename Iterator, typename Predicate>
auto operator%(const enumerable<Iterator>& range,
               const where_closure<Predicate>& closure)
{
    return enumerable<where_iterator<Iterator, Predicate>>{
            { range.begin(), range.end(), closure.pred },
            { range.end(), range.end(), closure.pred }};
}

// TODO: decide if we need to support where without from.

template <typename Enumerable, typename Predicate>
auto operator%(const Enumerable& range,
               const where_closure<Predicate>& closure)
{
    auto begin = std::cbegin(range);
    auto end = std::cend(range);

    return enumerable<where_iterator<decltype(begin), Predicate>>{
            { begin, end, closure.pred },
            { end, end, closure.pred }};
}

}
