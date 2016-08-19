#pragma once

#include "utils_global.h"

#include <QtCore/qglobal.h>
#include <QtCore/QHash>

template<typename T> class UTILS_EXPORT Optional;
typedef Optional<void> Nothing;

template<typename T>
class UTILS_EXPORT Optional
{
private:
    typedef bool (Optional::*RestrictedBool)() const;
public:

    Q_DECL_CONSTEXPR inline Optional() : m_isNull(true), m_value() {}
    Q_DECL_CONSTEXPR inline Optional(const Nothing &) : m_isNull(true), m_value() {}
    Q_DECL_CONSTEXPR inline Optional(const T &value) : m_isNull(false), m_value(value) {}
    template <typename U>
    Q_DECL_CONSTEXPR inline Optional(const U &value) : m_isNull(false), m_value(value) {}
#ifdef Q_COMPILER_RVALUE_REFS
    Q_DECL_CONSTEXPR inline Optional(T &&value) : m_isNull(false), m_value(std::move(value)) {}
#endif

    template <typename U>
    Q_DECL_CONSTEXPR inline Optional(const Optional<U> &other) :
        m_isNull(other.isNull()),
        m_value(other.value())
    {}

    inline Optional &operator=(const T &value)
    {
        m_isNull = false;
        m_value = value;
        return *this;
    }

    template <typename U>
    inline Optional &operator=(const Optional<U> &other)
    {
        m_isNull = false;
        m_value = other.value();
        return *this;
    }

    inline Optional &operator=(const Nothing &)
    {
        m_isNull = true;
        m_value = T();
        return *this;
    }

    // compiler-generated copy/move ctors/assignment operators are ok!
    // compiler-generated dtor is ok!

    Q_DECL_CONSTEXPR inline bool isNull() const { return m_isNull; }

    inline T &value()
    {
        Q_ASSERT_X(!m_isNull, "Optional::value", "Optional is empty");
        return m_value;
    }

    inline const T &value() const
    {
        Q_ASSERT_X(!m_isNull, "Optional::value", "Optional is empty");
        return m_value;
    }

    template <typename U>
    inline T value(const U &defaultValue) const
    {
        return m_isNull ? defaultValue : m_value;
    }

#ifdef Q_COMPILER_RVALUE_REFS
//    template <typename U>
//    inline T value(U &&defaultValue) const;
#endif

    inline void swap(Optional &other)
    {
        qSwap(m_isNull, other.m_isNull);
        qSwap(m_value, other.m_value);
    }

    inline void reset() { m_isNull = true; m_value = T(); }

    inline T &operator*()
    {
        Q_ASSERT_X(!m_isNull, "Optional::operator*", "Optional is empty");
        return m_value;
    }

    inline const T &operator*() const
    {
        Q_ASSERT_X(!m_isNull, "Optional::operator*", "Optional is empty");
        return m_value;
    }

    inline T *operator->()
    {
        Q_ASSERT_X(!m_isNull, "Optional::operator->", "Optional is empty");
        return &m_value;
    }

    inline const T *operator->() const
    {
        Q_ASSERT_X(!m_isNull, "Optional::operator->", "Optional is empty");
        return &m_value;
    }

#ifndef Q_QDOC
    inline operator RestrictedBool() const { return m_isNull ? Q_NULLPTR : &Optional::isNull; }
#else
    explicit inline operator bool() const { return m_isNull; }
#endif

private:
    bool m_isNull;
    T m_value;
};

template <>
class Optional<void>
{
private:
    typedef bool (Optional<void>::*RestrictedBool)() const;
public:
    Q_DECL_CONSTEXPR inline bool isNull() const { return true; }

#ifndef Q_QDOC
    Q_DECL_CONSTEXPR inline operator RestrictedBool () const { return Q_NULLPTR; }
#else
    explicit inline operator bool() const { return false; }
#endif
};

template<typename T>
Q_DECL_CONSTEXPR inline bool operator==(const Optional<T> &lhs, const Optional<T> &rhs)
{
    return (rhs.isNull() && lhs.isNull())
            || (!rhs.isNull() && !lhs.isNull() && lhs.value() == rhs.value());
}

template<typename T>
Q_DECL_CONSTEXPR inline bool operator==(const Optional<T> &lhs, const T &rhs)
{
    return !lhs.isNull() && (lhs.value() == rhs);
}

template<typename T>
Q_DECL_CONSTEXPR inline bool operator==(const T &value, const Optional<T> &optional)
{
    return !optional.isNull() && (optional.value() == value);
}

template<typename T>
Q_DECL_CONSTEXPR inline bool operator!=(const Optional<T> &lhs, const Optional<T> &rhs)
{
    return !(lhs == rhs);
}

template<typename T>
Q_DECL_CONSTEXPR inline bool operator!=(const Optional<T> &lhs, const T &rhs)
{
    return !(lhs == rhs);
}

template<typename T>
Q_DECL_CONSTEXPR inline bool operator!=(const T &lhs, const Optional<T> &rhs)
{
    return !(rhs == lhs);
}

template<typename T>
Q_DECL_CONSTEXPR inline bool operator<(const Optional<T> &lhs, const Optional<T> &rhs)
{
    return (lhs.isNull() && !rhs.isNull())
            || (!rhs.isNull() && !lhs.isNull() && lhs.value() < rhs.value());
}

template<typename T>
Q_DECL_CONSTEXPR inline bool operator<(const Optional<T> &lhs, const T &rhs)
{
    return lhs.isNull() || (lhs.value() < rhs);
}

template<typename T>
Q_DECL_CONSTEXPR inline bool operator<(const T &lhs, const Optional<T> &rhs)
{
    return !rhs.isNull() && (lhs < rhs.value());
}

template<typename T>
Q_DECL_CONSTEXPR inline bool operator>(const Optional<T> &lhs, const Optional<T> &rhs)
{
    return rhs < lhs;
}

template<typename T>
Q_DECL_CONSTEXPR inline bool operator>(const Optional<T> &lhs, const T &rhs)
{
    return rhs < lhs;
}

template<typename T>
Q_DECL_CONSTEXPR inline bool operator>(const T &lhs, const Optional<T> &rhs)
{
    return rhs < lhs;
}

template<typename T>
Q_DECL_CONSTEXPR inline bool operator<=(const Optional<T> &lhs, const Optional<T> &rhs)
{
    return !(lhs > rhs);
}

template<typename T>
Q_DECL_CONSTEXPR inline bool operator<=(const Optional<T> &lhs, const T &rhs)
{
    return !(lhs > rhs);
}

template<typename T>
Q_DECL_CONSTEXPR inline bool operator<=(const T &lhs, const Optional<T> &rhs)
{
    return !(lhs > rhs);
}

template<typename T>
Q_DECL_CONSTEXPR inline bool operator>=(const Optional<T> &lhs, const Optional<T> &rhs)
{
    return !(lhs < rhs);
}

template<typename T>
Q_DECL_CONSTEXPR inline bool operator>=(const Optional<T> &lhs, const T &rhs)
{
    return !(lhs < rhs);
}

template<typename T>
Q_DECL_CONSTEXPR inline bool operator>=(const T &lhs, const Optional<T> &rhs)
{
    return !(lhs < rhs);
}

template <typename T>
Q_DECL_CONSTEXPR inline uint qHash(const Optional<T> &key, uint seed = 0)
{
    return key ? seed : ::qHash(T(*key), seed) ;
}
