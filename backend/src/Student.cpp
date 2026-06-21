#include "udrms/Student.h"

#include "udrms/DomainError.h"

namespace udrms {

namespace {

QString requiredString(const QJsonObject &object, const char *field)
{
    const QJsonValue value = object.value(field);
    if (!value.isString()) {
        throw DomainError(QString("Student JSON field '%1' must be a string.").arg(field));
    }
    return value.toString();
}

int requiredInt(const QJsonObject &object, const char *field)
{
    const QJsonValue value = object.value(field);
    if (!value.isDouble()) {
        throw DomainError(QString("Student JSON field '%1' must be a number.").arg(field));
    }
    return value.toInt();
}

} // namespace

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
        requiredString(object, "id"),
        requiredString(object, "fullName"),
        requiredInt(object, "academicYear"));

    const bool hasDormitoryId = object.contains("dormitoryId");
    const bool hasRoomNumber = object.contains("roomNumber");
    if (hasDormitoryId != hasRoomNumber) {
        throw DomainError("Student JSON assignment must include both dormitoryId and roomNumber.");
    }
    if (hasDormitoryId) {
        student.assignToRoom(requiredString(object, "dormitoryId"), requiredInt(object, "roomNumber"));
    }

    return student;
}

} // namespace udrms
