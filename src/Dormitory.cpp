#include "udrms/Dormitory.h"

#include "udrms/DomainError.h"

#include <QJsonArray>

#include <algorithm>

namespace udrms {

Dormitory::Dormitory(QString id, QString name, int capacity, Restaurant restaurant)
    : m_id(std::move(id))
    , m_name(std::move(name))
    , m_capacity(capacity)
    , m_restaurant(std::move(restaurant))
{
    if (m_id.trimmed().isEmpty()) {
        throw DomainError("Dormitory ID cannot be empty.");
    }
    if (m_name.trimmed().isEmpty()) {
        throw DomainError("Dormitory name cannot be empty.");
    }
    if (m_capacity <= 0) {
        throw DomainError("Dormitory capacity must be positive.");
    }
}

QString Dormitory::id() const
{
    return m_id;
}

QString Dormitory::name() const
{
    return m_name;
}

int Dormitory::capacity() const
{
    return m_capacity;
}

int Dormitory::totalRoomCapacity() const
{
    int total = 0;
    for (const Room &room : m_rooms) {
        total += room.capacity();
    }
    return total;
}

int Dormitory::totalOccupancy() const
{
    int total = 0;
    for (const Room &room : m_rooms) {
        total += room.occupancy();
    }
    return total;
}

bool Dormitory::hasRoom(int roomNumber) const
{
    return m_rooms.contains(roomNumber);
}

Restaurant &Dormitory::restaurant()
{
    return m_restaurant;
}

const Restaurant &Dormitory::restaurant() const
{
    return m_restaurant;
}

void Dormitory::addRoom(const Room &room)
{
    if (m_rooms.contains(room.number())) {
        throw DomainError("Room already exists in this dormitory.");
    }
    if (totalRoomCapacity() + room.capacity() > m_capacity) {
        throw DomainError("Room capacity exceeds dormitory capacity.");
    }
    m_rooms.insert(room.number(), room);
}

void Dormitory::removeRoom(int roomNumber)
{
    Room &targetRoom = room(roomNumber);
    if (!targetRoom.isEmpty()) {
        throw DomainError("Cannot remove an occupied room.");
    }
    m_rooms.remove(roomNumber);
}

Room &Dormitory::room(int roomNumber)
{
    auto it = m_rooms.find(roomNumber);
    if (it == m_rooms.end()) {
        throw DomainError("Room does not exist in this dormitory.");
    }
    return it.value();
}

const Room &Dormitory::room(int roomNumber) const
{
    auto it = m_rooms.constFind(roomNumber);
    if (it == m_rooms.cend()) {
        throw DomainError("Room does not exist in this dormitory.");
    }
    return it.value();
}

QVector<Room> Dormitory::rooms() const
{
    QVector<Room> result(m_rooms.cbegin(), m_rooms.cend());
    std::sort(result.begin(), result.end(), [](const Room &left, const Room &right) {
        return left.number() < right.number();
    });
    return result;
}

QVector<Room> Dormitory::availableRooms() const
{
    QVector<Room> result;
    for (const Room &room : rooms()) {
        if (!room.isFull()) {
            result.append(room);
        }
    }
    return result;
}

QVector<Room> Dormitory::occupiedRooms() const
{
    QVector<Room> result;
    for (const Room &room : rooms()) {
        if (!room.isEmpty()) {
            result.append(room);
        }
    }
    return result;
}

void Dormitory::assignStudent(Student &student, int roomNumber)
{
    if (student.isAssigned()) {
        throw DomainError("Student is already assigned to a room.");
    }
    Room &targetRoom = room(roomNumber);
    targetRoom.addStudent(student.id());
    student.assignToRoom(m_id, roomNumber);
}

void Dormitory::removeStudent(Student &student)
{
    if (!student.isAssigned() || student.dormitoryId().value() != m_id) {
        throw DomainError("Student is not assigned to this dormitory.");
    }
    Room &assignedRoom = room(student.roomNumber().value());
    assignedRoom.removeStudent(student.id());
    student.clearAssignment();
}

QJsonObject Dormitory::toJson() const
{
    QJsonArray roomArray;
    for (const Room &room : rooms()) {
        roomArray.append(room.toJson());
    }

    return {
        {"id", m_id},
        {"name", m_name},
        {"capacity", m_capacity},
        {"rooms", roomArray},
        {"restaurant", m_restaurant.toJson()},
    };
}

Dormitory Dormitory::fromJson(const QJsonObject &object)
{
    Dormitory dormitory(
        object.value("id").toString(),
        object.value("name").toString(),
        object.value("capacity").toInt(),
        Restaurant::fromJson(object.value("restaurant").toObject()));

    const QJsonArray rooms = object.value("rooms").toArray();
    for (const QJsonValue &value : rooms) {
        dormitory.addRoom(Room::fromJson(value.toObject()));
    }

    return dormitory;
}

} // namespace udrms
