/*
    Copyright (c) 2016 Martin Buck
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation the rights to
    use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
    and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
    DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
    OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef FLAGGEDT_H
#define FLAGGEDT_H

namespace flaggedT {

//------------------------------------------------------------------------------

template <typename T>
class FlaggedTBase {
protected:
    FlaggedTBase() = delete;

    FlaggedTBase(FlaggedTBase const&) = default;
    FlaggedTBase(FlaggedTBase&&) = default;
    FlaggedTBase& operator=(FlaggedTBase const&) = default;
    FlaggedTBase& operator=(FlaggedTBase&&) = default;

    T data;

    FlaggedTBase(T const& in)
        : data(in)
    {
    }

    FlaggedTBase(T&& in)
        : data(std::move(in))
    {
    }

public:
    T const& get() const
    {
        return data;
    }

    operator T const& () const
    {
        return data;
    }

    operator T&& ()
    {
        return std::move(data);
    }
};

//------------------------------------------------------------------------------

template <typename T>
class NonNull : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    NonNull() = delete;
    NonNull(nullptr_t) = delete;

    NonNull(NonNull const&) = default;
    NonNull(NonNull&&) = default;
    NonNull& operator=(NonNull const&) = default;
    NonNull& operator=(NonNull&&) = default;

    ///THROWS
    NonNull(T&& in)
        : base(std::move(in))
    {
        if (nullptr == base::data)
            throw std::logic_error("Can't pass nullptr to constructor of NonNull");
    }
};

//------------------------------------------------------------------------------

template <typename T>
class Sorted : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    Sorted() = delete;

    Sorted(Sorted const&) = default;
    Sorted(Sorted&&) = default;
    Sorted& operator=(Sorted const&) = default;
    Sorted& operator=(Sorted&&) = default;

    Sorted(T&& in)
        : base(std::move(in))
    {
        std::sort(std::begin(base::data), std::end(base::data));
    }
};

//------------------------------------------------------------------------------

template <typename T>
class Shuffled : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    Shuffled() = delete;

    Shuffled(Shuffled const&) = default;
    Shuffled(Shuffled&&) = default;
    Shuffled& operator=(Shuffled const&) = default;
    Shuffled& operator=(Shuffled&&) = default;

    Shuffled(T&& in)
        : base(std::move(in))
    {
        std::shuffle(std::begin(base::data), std::end(base::data));
    }
};

//------------------------------------------------------------------------------

template <typename T>
class Unique : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    Unique() = delete;

    Unique(Unique const&) = default;
    Unique(Unique&&) = default;
    Unique& operator=(Unique const&) = default;
    Unique& operator=(Unique&&) = default;

    Unique(T&& in)
        : base(std::move(in))
    {
        make_data_unique();
    }

private:
    void make_data_unique()
    {
        if (base::data.empty())
            return;
        base::data.erase(std::unique(std::begin(base::data), std::end(base::data)), std::end(base::data));
    }
};

//------------------------------------------------------------------------------

template <typename T>
class UniqueAndSorted : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    UniqueAndSorted() = delete;

    UniqueAndSorted(UniqueAndSorted const&) = default;
    UniqueAndSorted(UniqueAndSorted&&) = default;
    UniqueAndSorted& operator=(UniqueAndSorted const&) = default;
    UniqueAndSorted& operator=(UniqueAndSorted&&) = default;

    UniqueAndSorted(T&& in)
        : base(std::move(in))
    {
        std::sort(std::begin(base::data), std::end(base::data));
        make_data_unique();
    }

    UniqueAndSorted(Unique<T>&& in)
        : base(std::move(in.data))
    {
        std::sort(std::begin(base::data), std::end(base::data));
    }

    UniqueAndSorted(Sorted<T>&& in)
        : base(std::move(in.data))
    {
        make_data_unique();
    }

private:
    void make_data_unique() ///@todo duplicate definition
    {
        if (base::data.empty())
            return;
        base::data.erase(std::unique(std::begin(base::data), std::end(base::data)), std::end(base::data));
    }
};

//------------------------------------------------------------------------------

template <typename T>
class NonZero : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    NonZero() = delete;

    NonZero(NonZero const&) = default;
    NonZero(NonZero&&) = default;
    NonZero& operator=(NonZero const&) = default;
    NonZero& operator=(NonZero&&) = default;

    ///THROWS
    NonZero(T&& in)
        : base(std::move(in))
    {
        if (0 == base::data)
            throw std::logic_error("Can't pass 0 to constructor of NonZero");
    }
};

//------------------------------------------------------------------------------

template <typename T>
class Positive : public NonZero<T> {
    using base = NonZero<T>;

public:
    Positive() = delete;

    Positive(Positive const&) = default;
    Positive(Positive&&) = default;
    Positive& operator=(Positive const&) = default;
    Positive& operator=(Positive&&) = default;

    ///THROWS
    Positive(T&& in)
        : base(std::move(in))
    {
        if (base::data <= 0)
            throw std::logic_error("Can't pass <= 0 to constructor of Positive");
    }
};

template <typename T>
class NonPositive : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    NonPositive() = delete;

    NonPositive(NonPositive const&) = default;
    NonPositive(NonPositive&&) = default;
    NonPositive& operator=(NonPositive const&) = default;
    NonPositive& operator=(NonPositive&&) = default;

    ///THROWS
    NonPositive(T&& in)
        : base(std::move(in))
    {
        if (base::data > 0)
            throw std::logic_error("Can't pass > 0 to constructor of NonPositive");
    }
};

//------------------------------------------------------------------------------

template <typename T>
class Negative : public NonZero<T> {
    using base = NonZero<T>;

public:
    Negative() = delete;

    Negative(Negative const&) = default;
    Negative(Negative&&) = default;
    Negative& operator=(Negative const&) = default;
    Negative& operator=(Negative&&) = default;

    ///THROWS
    Negative(T&& in)
        : base(std::move(in))
    {
        if (base::data >= 0)
            throw std::logic_error("Can't pass >= 0 to constructor of Negative");
    }
};

template <typename T>
class NonNegative : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    NonNegative() = delete;

    NonNegative(NonNegative const&) = default;
    NonNegative(NonNegative&&) = default;
    NonNegative& operator=(NonNegative const&) = default;
    NonNegative& operator=(NonNegative&&) = default;

    ///THROWS
    NonNegative(T&& in)
        : base(std::move(in))
    {
        if (base::data < 0)
            throw std::logic_error("Can't pass < 0 to constructor of NonNegative");
    }
};

//------------------------------------------------------------------------------

template <typename T, T MAX>
class CeiledInclusive : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    CeiledInclusive() = delete;

    CeiledInclusive(CeiledInclusive const&) = default;
    CeiledInclusive(CeiledInclusive&&) = default;
    CeiledInclusive& operator=(CeiledInclusive const&) = default;
    CeiledInclusive& operator=(CeiledInclusive&&) = default;

    ///THROWS
    CeiledInclusive(T&& in)
        : base(std::move(in))
    {
        if (base::data > MAX)
            throw std::logic_error("Passed value to constructor of CeiledInclusive is too big");
    }
};

template <typename T, T MAX>
class CeiledExclusive : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    CeiledExclusive() = delete;

    CeiledExclusive(CeiledExclusive const&) = default;
    CeiledExclusive(CeiledExclusive&&) = default;
    CeiledExclusive& operator=(CeiledExclusive const&) = default;
    CeiledExclusive& operator=(CeiledExclusive&&) = default;

    ///THROWS
    CeiledExclusive(T&& in)
        : base(std::move(in))
    {
        if (base::data >= MAX)
            throw std::logic_error("Passed value to constructor of CeiledExclusive is too big");
    }
};

//------------------------------------------------------------------------------

template <typename T, T MIN>
class FlooredInclusive : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    FlooredInclusive() = delete;

    FlooredInclusive(FlooredInclusive const&) = default;
    FlooredInclusive(FlooredInclusive&&) = default;
    FlooredInclusive& operator=(FlooredInclusive const&) = default;
    FlooredInclusive& operator=(FlooredInclusive&&) = default;

    ///THROWS
    FlooredInclusive(T&& in)
        : base(std::move(in))
    {
        if (base::data < MIN)
            throw std::logic_error("Passed value to constructor of FlooredInclusive is too small");
    }
};

template <typename T, T MIN>
class FlooredExclusive : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    FlooredExclusive() = delete;

    FlooredExclusive(FlooredExclusive const&) = default;
    FlooredExclusive(FlooredExclusive&&) = default;
    FlooredExclusive& operator=(FlooredExclusive const&) = default;
    FlooredExclusive& operator=(FlooredExclusive&&) = default;

    ///THROWS
    FlooredExclusive(T&& in)
        : base(std::move(in))
    {
        if (base::data <= MIN)
            throw std::logic_error("Passed value to constructor of FlooredExclusive is too small");
    }
};

//------------------------------------------------------------------------------

template <typename T, T MIN, T MAX>
class Bounded : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    Bounded() = delete;

    Bounded(Bounded const&) = default;
    Bounded(Bounded&&) = default;
    Bounded& operator=(Bounded const&) = default;
    Bounded& operator=(Bounded&&) = default;

    ///THROWS
    Bounded(T&& in)
        : base(std::move(in))
    {
        if (base::data < MIN)
            throw std::logic_error("Passed value to constructor of Bounded is too small");
        if (base::data > MAX)
            throw std::logic_error("Passed value to constructor of Bounded is too big");
    }
};

//------------------------------------------------------------------------------

template <typename T>
class NonEmpty : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    NonEmpty() = delete;

    NonEmpty(NonEmpty const&) = default;
    NonEmpty(NonEmpty&&) = default;
    NonEmpty& operator=(NonEmpty const&) = default;
    NonEmpty& operator=(NonEmpty&&) = default;

    ///THROWS
    NonEmpty(T&& in)
        : base(std::move(in))
    {
        if (base::data.empty())
            throw std::logic_error("Can't pass empty container to constructor of NonEmpty");
    }
};

//------------------------------------------------------------------------------

template <typename T, std::size_t SIZE>
class MoreThan : public NonEmpty<T>
{
    using base = NonEmpty<T>;

public:
    MoreThan() = delete;

    MoreThan(MoreThan const&) = default;
    MoreThan(MoreThan&&) = default;
    MoreThan& operator=(MoreThan const&) = default;
    MoreThan& operator=(MoreThan&&) = default;

    ///THROWS
    MoreThan(T&& in)
        : base(std::move(in))
    {
        if (base::data.size() <= SIZE)
            throw std::logic_error("Passed too small container to constructor of MoreThan");
    }
};

//------------------------------------------------------------------------------

template <typename T, std::size_t SIZE>
class LessThan : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    LessThan() = delete;

    LessThan(LessThan const&) = default;
    LessThan(LessThan&&) = default;
    LessThan& operator=(LessThan const&) = default;
    LessThan& operator=(LessThan&&) = default;

    ///THROWS
    LessThan(T&& in)
        : base(std::move(in))
    {
        if (base::data.size() >= SIZE)
            throw std::logic_error("Passed too big container to constructor of LessThan");
    }
};

//------------------------------------------------------------------------------

template <typename T, std::size_t SIZE>
class FixedSized : public FlaggedTBase<T> {
    using base = FlaggedTBase<T>;

public:
    FixedSized() = delete;

    FixedSized(FixedSized const&) = default;
    FixedSized(FixedSized&&) = default;
    FixedSized& operator=(FixedSized const&) = default;
    FixedSized& operator=(FixedSized&&) = default;

    ///THROWS
    FixedSized(T&& in)
        : base(std::move(in))
    {
        if (base::data.size() != SIZE)
            throw std::logic_error("Passed container with wrong size to constructor of FixedSized");
    }
};
}

#endif // FLAGGEDT_H