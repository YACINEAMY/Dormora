#include "udrms/Room.h"

#include "udrms/DomainError.h"

#include <QJsonArray>

#include <algorithm>

namespace udrms {

Room::Room(int number, int capacity)
    : m_number(number)
    , m_capacity(capacity)
{
    if (m_number <= 0) {
        throw DomainError("Room number must be positive.");
    }
    if (m_capacity <= 0) {
        throw DomainError("Room capacity must be positive.");
    }
}

int Room::number() const
{
    return m_number;
}

int Room::capacity() const
{
    return m_capacity;
}

int Room::occupancy() const
{
    return m_studentIds.size();
}

bool Room::isFull() const
{
    return occupancy() >= m_capacity;
}

bool Room::isEmpty() const
{
    return m_studentIds.isEmpty();
}

bool Room::containsStudent(const QString &studentId) const
{
    return m_studentIds.contains(studentId);
}

QVector<QString> Room::studentIds() const
{
    QVector<QString> ids(m_studentIds.cbegin(), m_studentIds.cend());
    std::sort(ids.begin(), ids.end());
    return ids;
}

void Room::addStudent(const QString &studentId)
{
    if (studentId.trimmed().isEmpty()) {
        throw DomainError("Student ID cannot be empty.");
    }
    if (m_studentIds.contains(studentId)) {
        throw DomainError("Student is already assigned to this room.");
    }
    if (isFull()) {
        throw DomainError("Room is already full.");
    }
    m_studentIds.insert(studentId);
}

void Room::removeStudent(const QString &studentId)
{
    if (!m_studentIds.remove(studentId)) {
        throw DomainError("Student is not assigned to this room.");
    }
}

QJsonObject Room::toJson() const
{
    QJsonArray students;
    for (const QString &studentId : studentIds()) {
        students.append(studentId);
    }

    return {
        {"number", m_number},
        {"capacity", m_capacity},
        {"students", students},
    };
}

Room Room::fromJson(const QJsonObject &object)
{
    Room room(object.value("number").toInt(), object.value("capacity").toInt());
    const QJsonArray students = object.value("students").toArray();
    for (const QJsonValue &value : students) {
        room.addStudent(value.toString());
    }
    return room;
}

} // namespace udrms
