#include "exception.h"

/*!
    \class Exception
    Base class for ImageTool's exceptions
*/

/*!
    Constructs Exception.
*/
Exception::Exception() = default;

/*!
    Destroys the object.
*/
Exception::~Exception() Q_DECL_NOEXCEPT = default;

/*!
    \reimp
*/
const char *Exception::what() const Q_DECL_NOEXCEPT
{
    return "Exception";
}

/*!
    \class ExitException
    This exception is used in command line parsers to terminate the program.
    We don't rely on exit(0) call because stack is not unwinded and destructors are not called,
    so we throw this exception, and catch it in main().
*/

/*!
    Constructs ExitException with return code \a code.
*/
ExitException::ExitException(int code) :
    _code(code)
{
}

/*!
    Destroys the object.
*/
ExitException::~ExitException() Q_DECL_NOEXCEPT = default;

/*!
    \fn int ExitException::code() const Q_DECL_NOEXCEPT;
    Returns the code that should be return from the program.
*/

/*!
    \reimp
*/
const char *ExitException::what() const Q_DECL_NOEXCEPT
{
    return "ExitException";
}

/*!
    \class RuntimeError
    Exception with localized string describing occured error.
*/

/*!
    Constructs RuntimeError with the message \a message.
*/
RuntimeError::RuntimeError(const QString &message) :
    _message(message)
{
}

/*!
    Move-constructs RuntimeError with the message \a message.
*/
RuntimeError::RuntimeError(QString &&message) Q_DECL_NOEXCEPT:
    _message(std::move(message))
{
}

/*!
    Destroys the object.
*/
RuntimeError::~RuntimeError() Q_DECL_NOEXCEPT = default;

/*!
    Returns the message describing the error.
*/
const QString &RuntimeError::message() const Q_DECL_NOEXCEPT
{
    return _message;
}

/*!
    \reimp
*/
const char *RuntimeError::what() const Q_DECL_NOEXCEPT
{
    return "RuntimeError";
}
