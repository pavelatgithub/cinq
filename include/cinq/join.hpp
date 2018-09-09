#pragma once

#include <iterator>
#include <cinq/enumerable.hpp>

namespace cinq {

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
    using difference_type = typename InputIterator1::difference_type;
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
