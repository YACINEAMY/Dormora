#pragma once

#include "udrms/Restaurant.h"
#include "udrms/Room.h"
#include "udrms/Student.h"

#include <QHash>
#include <QJsonObject>
#include <QString>
#include <QVector>

namespace udrms {

// Dormitory aggregate. It owns rooms and one restaurant, while University owns
// the cross-dormitory student registry.
class Dormitory final {
public:
    Dormitory() = default;
    Dormitory(QString id, QString name, int capacity, Restaurant restaurant);

    QString id() const;
    QString name() const;
    int capacity() const;
    int totalRoomCapacity() const;
    int totalOccupancy() const;
    bool hasRoom(int roomNumber) const;

    Restaurant &restaurant();
    const Restaurant &restaurant() const;

    void addRoom(const Room &room);
    void removeRoom(int roomNumber);
    Room &room(int roomNumber);
    const Room &room(int roomNumber) const;
    QVector<Room> rooms() const;
    QVector<Room> availableRooms() const;
    QVector<Room> occupiedRooms() const;

    // Updates both the room membership and the supplied Student assignment.
    void assignStudent(Student &student, int roomNumber);
    void removeStudent(Student &student);

    QJsonObject toJson() const;
    static Dormitory fromJson(const QJsonObject &object);

private:
    QString m_id;
    QString m_name;
    int m_capacity = 0;
    QHash<int, Room> m_rooms;
    Restaurant m_restaurant;
};

} // namespace udrms
