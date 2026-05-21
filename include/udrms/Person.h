#pragma once

#include <QJsonObject>
#include <QString>

namespace udrms {

// Base identity record shared by people in the dormitory domain.
class Person {
public:
    Person() = default;
    Person(QString id, QString fullName);
    virtual ~Person() = default;

    QString id() const;
    QString fullName() const;

    void setFullName(const QString &fullName);

    virtual QJsonObject toJson() const;

protected:
    // Kept protected so derived loaders can rebuild objects from storage while
    // preserving the public invariant that IDs cannot be empty.
    void setId(const QString &id);

private:
    QString m_id;
    QString m_fullName;
};

} // namespace udrms
