#include "udrms/University.h"

#include "udrms/DomainError.h"

#include <QJsonArray>

#include <algorithm>

namespace udrms {

namespace {

QJsonArray requiredArray(const QJsonObject &object, const char *field)
{
    const QJsonValue value = object.value(field);
    if (!value.isArray()) {
        throw DomainError(QString("University JSON field '%1' must be an array.").arg(field));
    }
    return value.toArray();
}

QJsonObject requiredObjectEntry(const QJsonValue &value, const QString &context)
{
    if (!value.isObject()) {
        throw DomainError(context + " JSON array entries must be objects.");
    }
    return value.toObject();
}

} // namespace

void University::addStudent(const Student &student)
{
    if (m_students.contains(student.id())) {
        throw DomainError("Student already exists.");
    }
    m_students.insert(student.id(), student);
}

void University::removeStudent(const QString &studentId)
{
    Student &targetStudent = student(studentId);
    if (targetStudent.isAssigned()) {
        removeStudentFromRoom(studentId);
    }
    m_students.remove(studentId);
}

bool University::hasStudent(const QString &studentId) const
{
    return m_students.contains(studentId);
}

Student &University::student(const QString &studentId)
{
    auto it = m_students.find(studentId);
    if (it == m_students.end()) {
        throw DomainError("Student does not exist.");
    }
    return it.value();
}

const Student &University::student(const QString &studentId) const
{
    auto it = m_students.constFind(studentId);
    if (it == m_students.cend()) {
        throw DomainError("Student does not exist.");
    }
    return it.value();
}

QVector<Student> University::students() const
{
    QVector<Student> result(m_students.cbegin(), m_students.cend());
    std::sort(result.begin(), result.end(), [](const Student &left, const Student &right) {
        return left.id() < right.id();
    });
    return result;
}

void University::addDormitory(const Dormitory &dormitory)
{
    if (m_dormitories.contains(dormitory.id())) {
        throw DomainError("Dormitory already exists.");
    }
    m_dormitories.insert(dormitory.id(), dormitory);
}

void University::removeDormitory(const QString &dormitoryId)
{
    const Dormitory &targetDormitory = dormitory(dormitoryId);
    if (targetDormitory.totalOccupancy() > 0) {
        throw DomainError("Cannot remove a dormitory that still has residents.");
    }
    for (const Student &student : m_students) {
        if (student.dormitoryId().has_value() && student.dormitoryId().value() == dormitoryId) {
            throw DomainError("Cannot remove a dormitory referenced by assigned students.");
        }
    }
    m_dormitories.remove(dormitoryId);
}

bool University::hasDormitory(const QString &dormitoryId) const
{
    return m_dormitories.contains(dormitoryId);
}

Dormitory &University::dormitory(const QString &dormitoryId)
{
    auto it = m_dormitories.find(dormitoryId);
    if (it == m_dormitories.end()) {
        throw DomainError("Dormitory does not exist.");
    }
    return it.value();
}

const Dormitory &University::dormitory(const QString &dormitoryId) const
{
    auto it = m_dormitories.constFind(dormitoryId);
    if (it == m_dormitories.cend()) {
        throw DomainError("Dormitory does not exist.");
    }
    return it.value();
}

QVector<Dormitory> University::dormitories() const
{
    QVector<Dormitory> result(m_dormitories.cbegin(), m_dormitories.cend());
    std::sort(result.begin(), result.end(), [](const Dormitory &left, const Dormitory &right) {
        return left.id() < right.id();
    });
    return result;
}

void University::assignStudentToRoom(const QString &studentId, const QString &dormitoryId, int roomNumber)
{
    Student &targetStudent = student(studentId);
    Dormitory &targetDormitory = dormitory(dormitoryId);
    targetDormitory.assignStudent(targetStudent, roomNumber);
}

void University::removeStudentFromRoom(const QString &studentId)
{
    Student &targetStudent = student(studentId);
    if (!targetStudent.isAssigned()) {
        throw DomainError("Student is not assigned to a room.");
    }
    Dormitory &targetDormitory = dormitory(targetStudent.dormitoryId().value());
    targetDormitory.removeStudent(targetStudent);
}

void University::setRestaurantMenu(const QString &dormitoryId, const QDate &date, const DailyMenu &menu)
{
    dormitory(dormitoryId).restaurant().setMenu(date, menu);
}

std::optional<DailyMenu> University::restaurantMenuForStudent(const QString &studentId, const QDate &date) const
{
    const Student &targetStudent = student(studentId);
    if (!targetStudent.isAssigned()) {
        throw DomainError("Only assigned dormitory residents can view restaurant menus.");
    }
    return dormitory(targetStudent.dormitoryId().value()).restaurant().menuForDate(date);
}

void University::recordStudentMeal(const QString &studentId, const QDate &date)
{
    Student &targetStudent = student(studentId);
    if (!targetStudent.isAssigned()) {
        throw DomainError("Only assigned dormitory residents can access restaurant meals.");
    }
    dormitory(targetStudent.dormitoryId().value()).restaurant().recordMealServed(date);
}

void University::validateIntegrity() const
{
    QHash<QString, int> roomMembershipCount;

    for (const Dormitory &dormitory : m_dormitories) {
        if (dormitory.totalRoomCapacity() > dormitory.capacity()) {
            throw DomainError("Dormitory room capacity exceeds dormitory capacity.");
        }

        for (const Room &room : dormitory.rooms()) {
            if (room.occupancy() > room.capacity()) {
                throw DomainError("Room occupancy exceeds room capacity.");
            }

            for (const QString &studentId : room.studentIds()) {
                const Student &resident = student(studentId);
                if (!resident.isAssigned()) {
                    throw DomainError("Room contains a student without accommodation assignment.");
                }
                if (resident.dormitoryId().value() != dormitory.id() || resident.roomNumber().value() != room.number()) {
                    throw DomainError("Student accommodation assignment does not match room membership.");
                }
                roomMembershipCount[studentId] += 1;
            }
        }
    }

    for (const Student &student : m_students) {
        if (!student.isAssigned()) {
            if (roomMembershipCount.contains(student.id())) {
                throw DomainError("Unassigned student appears in a room.");
            }
            continue;
        }

        const Dormitory &assignedDormitory = dormitory(student.dormitoryId().value());
        const Room &assignedRoom = assignedDormitory.room(student.roomNumber().value());
        if (!assignedRoom.containsStudent(student.id())) {
            throw DomainError("Assigned student is missing from the assigned room.");
        }
        if (roomMembershipCount.value(student.id()) != 1) {
            throw DomainError("Student appears in more than one room.");
        }
    }
}

QJsonObject University::toJson() const
{
    QJsonArray studentArray;
    for (const Student &student : students()) {
        studentArray.append(student.toJson());
    }

    QJsonArray dormitoryArray;
    for (const Dormitory &dormitory : dormitories()) {
        dormitoryArray.append(dormitory.toJson());
    }

    return {
        {"students", studentArray},
        {"dormitories", dormitoryArray},
    };
}

University University::fromJson(const QJsonObject &object)
{
    University university;

    const QJsonArray dormitories = requiredArray(object, "dormitories");
    for (const QJsonValue &value : dormitories) {
        university.addDormitory(Dormitory::fromJson(requiredObjectEntry(value, "Dormitory")));
    }

    const QJsonArray students = requiredArray(object, "students");
    for (const QJsonValue &value : students) {
        university.addStudent(Student::fromJson(requiredObjectEntry(value, "Student")));
    }

    university.validateIntegrity();

    return university;
}

} // namespace udrms
