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
Exception::~Exception() noexcept = default;

/*!
    \reimp
*/
const char *Exception::what() const noexcept
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
ExitException::~ExitException() noexcept = default;

/*!
    \fn int ExitException::code() const noexcept;
    Returns the code that should be return from the program.
*/

/*!
    \reimp
*/
const char *ExitException::what() const noexcept
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
RuntimeError::RuntimeError(QString &&message) noexcept:
    _message(std::move(message))
{
}

/*!
    Destroys the object.
*/
RuntimeError::~RuntimeError() noexcept = default;

/*!
    Returns the message describing the error.
*/
const QString &RuntimeError::message() const noexcept
{
    return _message;
}

/*!
    \reimp
*/
const char *RuntimeError::what() const noexcept
{
    return "RuntimeError";
}
