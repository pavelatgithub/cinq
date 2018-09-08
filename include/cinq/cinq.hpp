#pragma once

#include <iterator>
#include <numeric>
#include <type_traits>
#include <vector>

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


/**
 * join_iterator is a single-pass input iterator that ...
 */
template <typename InputIterator1, typename InputIterator2, typename Filter>
class join_iterator
{
public:
    using iterator_category = std::input_iterator_tag;
    using value_type = std::pair<const typename InputIterator1::value_type&,
                                 const typename InputIterator2::value_type&>;
    //using difference_type = typename iterator_type::difference_type;
    using pointer = value_type*;
    using reference = value_type&;

public:
    join_iterator(InputIterator1 begin1, InputIterator1 end1,
                  InputIterator2 begin2, InputIterator2 end2,
                  Filter filter) noexcept
        : _range1{begin1, std::move(end1)},
          _range2{begin2, std::move(end2)},
          _it1{std::move(begin1)},
          _it2{std::move(begin2)},
          _filter{std::move(filter)}
    {}

    join_iterator& operator++()
    {
        for (; _it1 != _range1.end(); ++_it1)
        {
            for (++_it2; _it2 != _range2.end(); ++_it2)
            {
                if (_filter(*_it1, *_it2))
                    return *this;
            }
            _it2 = _range2.begin();
        }
        return *this;
    }

    join_iterator operator++(int)
    {
        join_iterator tmp(*this);
        operator++();
        return tmp;
    }

    value_type operator*() const noexcept
    {
        return value_type{ *_it1, *_it2 };
    }

    /**
     * pointer operator->() const;
     * Not supported.
    */

    bool equal(const join_iterator& rhs) const
    {
        return _it1 == rhs._it1 || _it2 == rhs._it2;
    }

private:
    enumerable<InputIterator1> _range1;
    enumerable<InputIterator2> _range2;
    InputIterator1 _it1;
    InputIterator2 _it2;
    Filter _filter;
};

template <typename It1, typename It2, typename Filter>
bool operator==(const join_iterator<It1, It2, Filter>& lhs,
                const join_iterator<It1, It2, Filter>& rhs)
{
    return lhs.equal(rhs);
}

template <typename It1, typename It2, typename Filter>
bool operator!=(const join_iterator<It1, It2, Filter>& lhs,
                const join_iterator<It1, It2, Filter>& rhs)
{
    return !lhs.equal(rhs);
}

template <typename Enumerable, typename Filter>
class join_on_closure
{
public:
    join_on_closure(const Enumerable& range, Filter filter)
        : _range{range},
          _filter{std::move(filter)}
    {}

    const Enumerable& range() const noexcept { return _range; }
    Filter filter() const noexcept { return _filter; }

private:
    const Enumerable& _range;
    Filter _filter;
};

template <typename Enumerable>
class join_closure
{
public:
    join_closure(const Enumerable& range) noexcept
        : _range{range}
    {}

    template <typename Filter>
    join_on_closure<Enumerable, Filter> on(Filter&& filter) const noexcept
    {
        return {_range, std::forward<Filter>(filter)};
    }

private:
    const Enumerable& _range;
};

template <typename Enumerable>
join_closure<Enumerable> join(const Enumerable& range) noexcept
{
    return {range};
}

template <typename EnumerableFrom, typename EnumerableJoinOn, typename Filter>
auto operator%(const EnumerableFrom& range,
               const join_on_closure<EnumerableJoinOn, Filter>& join_on)
{
    auto b1 = std::cbegin(range);
    auto b2 = std::cbegin(join_on.range());
    auto e1 = std::cend(range);
    auto e2 = std::cend(join_on.range());

    using iterator_type = join_iterator<decltype(b1), decltype(b2), Filter>;
    return enumerable<iterator_type>{
        { b1, e1, b2, e2, join_on.filter() },
        { e1, e1, e2, e2, join_on.filter() }};
}

}
