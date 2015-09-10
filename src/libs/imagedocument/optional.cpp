#include "optional.h"

/*!
    \class Optional

    The Optional class manages an optional contained value, i.e. a value that
    semantically may not be present.

    A common use case for optional is the return value of a function that may
    fail. As opposed to other approaches, such as QPair<T, bool>, optional is
    more readable, as the intent is expressed explicitly.

    The value is guaranteed to be allocated within the
    optional object itself, i.e. no dynamic memory allocation ever takes place.
    Thus, an optional object models an object, not a pointer, even though the
    operator*() and operator->() are defined.

    Optinal can be in 2 states - empty or not.
    The optional object is not empty on the following conditions:
      - The object is initialized with a value of type T
      - The object is assigned an non-empty optional.
    The object is empty on the following conditions:
      - The object is default-initialized.
      - The object is initialized with a value of NullOptional or an empty
        optional object.
      - The object is assigned a value of NullOptional or an empty optional.
*/

/*!
    \fn Optional<T>::Optional()

    Constructs an empty Optional object.
*/

/*!
    \fn Optional<T>::Optional(const Optional &other)

    Constructs a new Optional object that is a copy of the given \a other
    object.
*/

/*!
    \fn Optional<T>::Optional(const Optional<void> &other)

    Constructs a new Optional object that is a copy of the given \a other
    object.

    This constructor is typically used to implicitly create empty optional from
    NullOptional.
*/

/*!
    \fn Optional<T>::Optional(const T &value)

    Constructs an non-empty Optional object and from the \a value.
*/

/*!
    \fn Optional<T>::Optional(const Optional<U> &other)

    Constructs a new Optional object that is a copy of the given \a other
    object.

    This constructor tries to implicitly convert value from U type to T.
*/

/*!
    \fn Optional<T> &Optional<T>::operator=(const T &value)

    Assigns contents to the \a value and makes this object non-empty.
*/

/*!
    \fn Optional<T> &Optional<T>::operator=(const Optional<T> &other)

    Makes a copy of the given \a other object and assigns it to this Optional.
*/

/*!
    \fn Optional<T> &Optional<T>::operator=(Optional<T> &&other)

    Move-assigns \a other to this Optional instance.
*/

/*!
    \fn bool Optional<T>::isEmpty() const

    Returns true is this object is empty.
*/

/*!
    \fn T &Optional<T>::value()

    Returns the reference to the contained value.

    \note This function asserts if this object is empty.
*/

/*!
    \fn const T &Optional<T>::value() const

    Returns the const reference to the contained value.

    \note This function asserts if this object is empty.
*/

/*!
    \fn const T &Optional<T>::value(const U &defaultValue) const

    Returns the contained value if object is not empty or \a defaultValue
    otherwise.
*/

/*!
    \fn void Optional<T>::swap(Optional<T> &other);

    Swaps this Optional object with the \a other.
*/

/*!
    \fn void Optional<T>::reset();

    Makes this Optional object empty.
*/

/*!
    \fn T &Optional<T>::operator*()

    Returns a reference to the contained value.

    \note This function asserts if this object is empty.
*/

/*!
    \fn const T &Optional<T>::operator*() const

    Returns a const reference to the contained value.

    \note This function asserts if this object is empty.
*/

/*!
    \fn T *Optional<T>::operator->()

    Returns a pointer to the contained value.

    \note This function asserts if this object is empty.
*/

/*!
    \fn const T *Optional<T>::operator->() const

    Returns a const pointer to the contained value.

    \note This function asserts if this object is empty.
*/

/*!
    \fn Optional<T>::operator bool() const

    Returns true if this Optional object is not empty.
*/

/*!
    \fn bool operator==(const Optional<T> &left, const Optional<T> &right)

    \relates Optional

    Returns true if the \a left Optional object is equal to the \a right
    object.
*/

/*!
    \fn bool operator<(const Optional<T> &left, const Optional<T> &right)

    \relates Optional

    Returns true if the \a left Optional object is less than the \a right
    object.

    \note Empty Optional is less than non-empty Optional.
*/

/*!
    \fn bool operator>(const Optional<T> &left, const Optional<T> &right)

    \relates Optional

    Returns true if the \a left Optional object is greater than the \a right
    object.

    \note Non-empty Optional is greater than an empty Optional.
*/

/*!
    \fn bool operator==(const Optional<T> &optional, const T &value)

    \relates Optional

    Returns true if the \a optional contains the \a value.
*/

/*!
    \fn bool operator==(const T &value, const Optional<T> &optional)

    \relates Optional

    Returns true if the \a optional contains the \a value.
*/

/*!
    \fn bool operator<(const Optional<T> &optional, const T &value)

    \relates Optional

    Returns true if the \a optional's value is less than the \a value.

    \note Empty Optional is always less than any value.
*/

/*!
    \fn bool operator>(const Optional<T> &optional, const T &value)

    \relates Optional

    Returns true if the \a optional's value is greater than the \a value.

    \note Any value is always greater than an empty Optional.
*/


/*!
    \fn bool operator<(const T &value, const Optional<T> &optional)

    \relates Optional

    Returns true if the \a value is less than the \a optional's value.

    \note Empty Optional is always less than any value.
*/

/*!
    \fn bool operator>(const T &value, const Optional<T> &optional)

    \relates Optional

    Returns true if the \a value is greater than the \a optional's value.

    \note Any value is always greater than an empty Optional.
*/

/*!
    \class Nothing

    This is a helper class that simplifies constructing on an empty Optionals.
*/
