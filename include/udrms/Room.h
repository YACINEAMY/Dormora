#pragma once

#include <QJsonObject>
#include <QSet>
#include <QString>
#include <QVector>

namespace udrms {

// Physical room with a capacity limit and resident membership set.
class Room final {
public:
    Room() = default;
    Room(int number, int capacity);

    int number() const;
    int capacity() const;
    int occupancy() const;
    bool isFull() const;
    bool isEmpty() const;
    bool containsStudent(const QString &studentId) const;
    // Returned in sorted order so UI, persistence, and tests stay stable.
    QVector<QString> studentIds() const;

    void addStudent(const QString &studentId);
    void removeStudent(const QString &studentId);

    QJsonObject toJson() const;
    static Room fromJson(const QJsonObject &object);

private:
    int m_number = 0;
    int m_capacity = 0;
    QSet<QString> m_studentIds;
};

} // namespace udrms
