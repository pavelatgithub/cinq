#pragma once

#include <cinq/enumerable.hpp>

namespace cinq
{

/**
 *
 */
template <typename InputIterator, typename Condition>
class wrap_iter_cond
{
public:
    using iterator_type = InputIterator;
    using iterator_category = typename iterator_type::iterator_category;
    using value_type = typename iterator_type::value_type;
    using difference_type = typename iterator_type::difference_type;
    using pointer = typename iterator_type::pointer;
    using reference = typename iterator_type::reference;

public:
    wrap_iter_cond(iterator_type base, iterator_type end, Condition cond)
        : _it{std::move(base)},
          _end{std::move(end)},
          _cond{std::move(cond)}
    {}

    wrap_iter_cond& operator++()
    {
        do { ++_it; } while (_it != _end && !_cond(*_it));
        return *this;
    }

    wrap_iter_cond operator++(int)
    {
        wrap_iter_cond tmp(*this);
        operator++();
        return tmp;
    }

    reference operator*() const { return _it.operator*(); }
    pointer operator->() const { return _it.operator->(); }

    iterator_type base() const noexcept { return _it; }

private:
    iterator_type _it;
    iterator_type _end;
    Condition _cond;
};

template <typename I, typename C>
bool operator==(const wrap_iter_cond<I, C>& lhs,
                const wrap_iter_cond<I, C>& rhs)
{
   return lhs.base() == rhs.base();
}

template <typename I, typename C>
bool operator!=(const wrap_iter_cond<I, C>& lhs,
                const wrap_iter_cond<I, C>& rhs)
{
    return lhs.base() != rhs.base();
}

template <typename Predicate>
class where_closure
{
public:
    where_closure(Predicate pred)
        : pred(std::move(pred))
    {}

    Predicate predicate() const { return pred; }

private:
    Predicate pred;
};

template <typename Condition>
auto where(Condition&& cond) noexcept
{
    return where_closure<Condition>{std::forward<Condition>(cond)};
}

template <typename Iterator, typename Predicate>
auto operator%(const enumerable<Iterator>& range,
               const where_closure<Predicate>& pred)
{
    return enumerable<wrap_iter_cond<Iterator, Predicate>>{
        {range.begin(), range.end(), pred.predicate()},
        {range.end(), range.end(), pred.predicate()}};
}

}
