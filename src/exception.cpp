#include "exception.h"

ExceptionFile::ExceptionFile(QString const& error) : error(error)
{
}

ExceptionReplacing::ExceptionReplacing(QString const& error) : error(error)
{
}

ExceptionMask::ExceptionMask(
        TypeError const& type, QString const& mask, QString const& expected)
    : type(type), mask(mask), expected(expected)
{
}
