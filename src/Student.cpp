#include "udrms/Student.h"

#include "udrms/DomainError.h"

namespace udrms {

Student::Student(QString id, QString fullName, int academicYear)
    : Person(std::move(id), std::move(fullName))
    , m_academicYear(academicYear)
{
    if (m_academicYear <= 0) {
        throw DomainError("Academic year must be positive.");
    }
}

int Student::academicYear() const
{
    return m_academicYear;
}

std::optional<QString> Student::dormitoryId() const
{
    return m_dormitoryId;
}

std::optional<int> Student::roomNumber() const
{
    return m_roomNumber;
}

bool Student::isAssigned() const
{
    return m_dormitoryId.has_value() && m_roomNumber.has_value();
}

void Student::setAcademicYear(int academicYear)
{
    if (academicYear <= 0) {
        throw DomainError("Academic year must be positive.");
    }
    m_academicYear = academicYear;
}

void Student::assignToRoom(const QString &dormitoryId, int roomNumber)
{
    if (dormitoryId.trimmed().isEmpty()) {
        throw DomainError("Dormitory ID cannot be empty.");
    }
    if (roomNumber <= 0) {
        throw DomainError("Room number must be positive.");
    }
    m_dormitoryId = dormitoryId;
    m_roomNumber = roomNumber;
}

void Student::clearAssignment()
{
    m_dormitoryId.reset();
    m_roomNumber.reset();
}

QJsonObject Student::toJson() const
{
    QJsonObject object = Person::toJson();
    object["academicYear"] = m_academicYear;
    if (m_dormitoryId.has_value()) {
        object["dormitoryId"] = *m_dormitoryId;
    }
    if (m_roomNumber.has_value()) {
        object["roomNumber"] = *m_roomNumber;
    }
    return object;
}

Student Student::fromJson(const QJsonObject &object)
{
    Student student(
        object.value("id").toString(),
        object.value("fullName").toString(),
        object.value("academicYear").toInt());

    if (object.contains("dormitoryId") && object.contains("roomNumber")) {
        student.assignToRoom(object.value("dormitoryId").toString(), object.value("roomNumber").toInt());
    }

    return student;
}

} // namespace udrms
