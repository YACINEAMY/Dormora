#pragma once

#include <QString>

#include <stdexcept>

namespace udrms {

// User-facing domain rule violation. GUI and tests catch this instead of
// relying on lower-level Qt or STL exceptions.
class DomainError final : public std::runtime_error {
public:
    explicit DomainError(const QString &message)
        : std::runtime_error(message.toStdString())
    {
    }
};

} // namespace udrms
