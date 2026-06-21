#include "udrms/Person.h"

#include "udrms/DomainError.h"

namespace udrms {

Person::Person(QString id, QString fullName)
    : m_id(std::move(id))
    , m_fullName(std::move(fullName))
{
    if (m_id.trimmed().isEmpty()) {
        throw DomainError("Person ID cannot be empty.");
    }
    if (m_fullName.trimmed().isEmpty()) {
        throw DomainError("Full name cannot be empty.");
    }
}

QString Person::id() const
{
    return m_id;
}

QString Person::fullName() const
{
    return m_fullName;
}

void Person::setFullName(const QString &fullName)
{
    if (fullName.trimmed().isEmpty()) {
        throw DomainError("Full name cannot be empty.");
    }
    m_fullName = fullName;
}

QJsonObject Person::toJson() const
{
    return {
        {"id", m_id},
        {"fullName", m_fullName},
    };
}

void Person::setId(const QString &id)
{
    if (id.trimmed().isEmpty()) {
        throw DomainError("Person ID cannot be empty.");
    }
    m_id = id;
}

} // namespace udrms
